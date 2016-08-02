package controller

import (
	"cerberus/http/extreme"
	"cerberus/http/log"
	"cerberus/http/session"
	"fmt"
	"net/http"
)

//爲定義的控制器 註冊
func init() {
	extreme.SetControllerFunc("hellow word", HellowFunc)
	extreme.SetController("param", ParamTest{})

	extreme.SetListenerFunc("mylistener", MyListener)
}

//服務器初始話 完畢 再次 添加自定義初始化 數據
//返回 錯誤 將停止服務器
func Initialize() (err error) {
	fmt.Println("controller")
	return
}

//控制器 只需要實現 http.Handler 接口即可
//此處演示了經典的 hellow word
func HellowFunc(rw http.ResponseWriter, req *http.Request) {
	fmt.Fprint(rw, "hellow word")
}

type ParamTest struct {
}

//演示了如何使用 參數匹配
func (h ParamTest) ServeHTTP(rw http.ResponseWriter, req *http.Request) {
	fmt.Fprintf(rw, "id=%v name=%v", req.FormValue("id"), req.FormValue("name"))
}

//演示了監聽器 和 session
//監聽器 基本同 控制器 只是要多返回一個 bool  true 代表 後續程序不再 處理此請求
func MyListener(rw http.ResponseWriter, req *http.Request) bool {
	s, e := session.GetAndCreateSession(rw, req)
	if e != nil {
		log.Error("MyListener", e)
	} else {
		number := 0
		v, ok := s.GetValue("number")
		if ok {
			number = v.(int)
			number++
		}
		s.SetValue("number", number)
		fmt.Println("session ", number)
	}
	return false
}
