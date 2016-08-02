// 在 net.http 之上 封裝的一個 極端簡單的 http 服務器框架
package extreme

import (
	"fmt"
	"net/http"
	"os"
	"regexp"
	"strings"
)

//請求方式
const (
	ALL  = "ALL"
	GET  = "GET"
	POST = "POST"
)

func init() {
	handler = Handler{}
	handler.Indexs = make([]string, 0, 2)
	handler.Controllers = make([]Controller, 0, 16)
	handler.ServeFile = make([]*regexp.Regexp, 0, 16)
	handler.ListenerBefore = make([]ListenerHandler, 0, 2)
	handler.ListenerEnd = make([]ListenerHandler, 0, 2)
}

//路由表實例
var handler Handler

//開啓http 服務
func ListenAndServe(addr string) (err error) {
	err = http.ListenAndServe(addr, handler)
	return
}

//路由表
type Handler struct {
	//首頁
	Indexs []string

	//控制器
	Controllers []Controller
	//靜態文件
	ServeFile []*regexp.Regexp

	//LISTENER_BEFORE 監聽器
	ListenerBefore []ListenerHandler

	//LISTENER_END 監聽器
	ListenerEnd []ListenerHandler
}

//實現 http.Handler 接口 在此分發路由
func (h Handler) ServeHTTP(rw http.ResponseWriter, req *http.Request) {
	//LISTENER_BEFORE 監聽器
	for _, listener := range h.ListenerBefore {
		if listener.Regex.MatchString(req.URL.Path) &&
			listener.Handler.ServeHTTP(rw, req) {
			return
		}
	}
	//LISTENER_END 監聽器
	defer func() {
		for _, listener := range h.ListenerEnd {
			if listener.Regex.MatchString(req.URL.Path) &&
				listener.Handler.ServeHTTP(rw, req) {
				return
			}
		}
	}()

	//訪問首頁
	if req.URL.Path == "/" {
		for _, index := range h.Indexs {
			f, e := os.Open(index)
			if e == nil {
				f.Close()
				http.ServeFile(rw, req, index)
				return
			}
		}
		http.NotFound(rw, req)
		return
	}

	//解析靜態路由
	for _, s := range h.ServeFile {
		if s.MatchString(req.URL.Path) {
			http.ServeFile(rw, req, req.URL.Path[1:])
			return
		}
	}

	//解析路由
	for _, c := range h.Controllers {
		if !c.Regex.MatchString(req.URL.Path) {
			continue
		}
		if c.Method != ALL && c.Method != req.Method {
			continue
		}
		//解析 form 參數
		req.ParseForm()
		if len(c.Params) > 0 {
			e := resolveRouteParam(c.Params, req)
			if e != nil {
				return
			}
		}
		c.Handler.ServeHTTP(rw, req)
		return
	}

	//404
	http.NotFound(rw, req)
	fmt.Println("not found")
}

//爲路由參數 解析參數
func resolveRouteParam(names []string, req *http.Request) (e error) {
	path := req.URL.Path
	parts := strings.Split(path, "/")
	max := len(parts)
	count := len(names)
	if count > max {
		e = fmt.Errorf("無法正確解析參數 %v", path)
		return
	}
	for i := 0; i < count; i++ {
		req.Form.Set(names[i], parts[max-count+i])
	}

	return
}

//路由信息
type Controller struct {
	//請求方法
	Method string

	//匹配規則
	Regex *regexp.Regexp

	//綁定參數
	Params []string

	//控制器
	Handler http.Handler
}

//註冊靜態文件服務
func RegisterServeFile(route string) (err error) {
	var regex *regexp.Regexp
	regex, err = regexp.Compile(route)
	if err == nil {
		handler.ServeFile = append(handler.ServeFile, regex)
	}
	return
}

//將一個 實現了 http.Handler 的型別 註冊爲路由
func RegisterController(method string, route string, handle http.Handler) (err error) {
	switch method {
	case ALL:
	case GET:
	case POST:
	default:
		err = fmt.Errorf("不支持的 method %v", method)
		return

	}
	//分解 路由 參數 正則
	parts := strings.Split(route, "/")
	params := make([]string, 0, 5)
	for i, part := range parts {
		if strings.HasPrefix(part, ":") {
			//參數去掉 :
			part = part[1:]
			//請求不能以/開始
			expr := "([^/]+)"
			if index := strings.Index(part, "("); index != -1 {
				expr = part[index:]
				part = part[:index]
			}
			params = append(params, part)
			parts[i] = expr
		}
	}
	route = strings.Join(parts, "/")

	//參數正則
	regex, err := regexp.Compile(route)
	if err != nil {
		return
	}

	handler.Controllers = append(handler.Controllers,
		Controller{Method: method,
			Regex:   regex,
			Params:  params,
			Handler: handle})
	return
}

//將一個 函數 註冊爲 路由
func RegisterControllerFunc(method string, route string, handler func(http.ResponseWriter, *http.Request)) (err error) {
	err = RegisterController(method, route, http.HandlerFunc(handler))
	return
}

//通過名字 字符串 註冊路由
func RegisterControllerByName(method string, route, controller string) (err error) {
	//獲取控制器
	handle, ok := GetController(controller)
	if !ok {
		err = fmt.Errorf("不存在此控制器 %v", controller)
		return
	}
	err = RegisterController(method, route, handle)
	return
}

//監聽器類型
const (
	//在請求到達時
	LISTENER_BEFORE = "BEFORE"

	//在請求處理完畢 準備返回時
	LISTENER_END = "END"
)

//監聽器 接口
type Listener interface {
	//返回true 代表阻止 後續處理 結束request
	ServeHTTP(rw http.ResponseWriter, req *http.Request) bool
}

//監聽器 信息
type ListenerHandler struct {
	//處理程序
	Handler Listener

	//匹配規則
	Regex *regexp.Regexp
}

//監聽器 func 型別定義
type ListenerFunc func(http.ResponseWriter, *http.Request) bool

//型別轉換 到Listener
func (l ListenerFunc) ServeHTTP(rw http.ResponseWriter, req *http.Request) bool {
	return l(rw, req)
}

//將一個 函數 註冊爲 監聽器
// (監聽器 路由不支持參數匹配 route 將作爲一個正則表達式 匹配路徑)
func RegisterListenerFunc(ltype, route string, listener func(http.ResponseWriter, *http.Request) bool) (err error) {
	err = RegisterListener(ltype, route, ListenerFunc(listener))

	return
}

//將一個 實現了 Listener接口的 型別  註冊爲監聽器
// (監聽器 路由不支持參數匹配 route 將作爲一個正則表達式 匹配路徑)
func RegisterListener(ltype, route string, listener Listener) (err error) {
	var regex *regexp.Regexp
	regex, err = regexp.Compile(route)
	if err == nil {
		switch ltype {
		case LISTENER_BEFORE:
			handler.ListenerBefore = append(handler.ListenerBefore, ListenerHandler{listener, regex})
		case LISTENER_END:
			handler.ListenerEnd = append(handler.ListenerEnd, ListenerHandler{listener, regex})
		default:
			err = fmt.Errorf("不存在此類型的監聽器 %v", ltype)
		}
	}
	return
}

//通過名字字符串 註冊監聽器
func RegisterListenerByName(ltype, route, name string) (err error) {
	listener, ok := GetListener(name)
	if !ok {
		err = fmt.Errorf("不存在此監聽器 %v", name)
		return
	}
	err = RegisterListener(ltype, route, listener)
	return
}

// 註冊首頁 文件
func RegisterIndexPage(page string) {
	handler.Indexs = append(handler.Indexs, page)
}
