package extreme

import (
	"net/http"
)

//保存名字字符串 和 控制器 關聯
var names map[string]http.Handler

//保存名字字符串 和 監聽器 關係
var listeners map[string]Listener

func init() {
	names = make(map[string]http.Handler, 16)
	listeners = make(map[string]Listener, 2)
}

//將一個名字字符串 和 struct 控制器 關聯
func SetController(name string, handler http.Handler) {
	names[name] = handler
}

//將一個名字字符串 和 func 控制器 關聯
func SetControllerFunc(name string, handler func(http.ResponseWriter, *http.Request)) {
	names[name] = http.HandlerFunc(handler)
}

//查詢 一個 名字 是否和 控制器 關聯
func GetController(name string) (h http.Handler, ok bool) {
	h, ok = names[name]
	return
}

//刪除  名字 字符串 和 控制器 之間的關聯(如果 存在的話)
func DelController(name string) {
	delete(names, name)
}

//將一個名字字符串 和 struct 監聽器 關聯
func SetListener(name string, handler Listener) {
	listeners[name] = handler
}

//將一個名字字符串 和 func 監聽器 關聯
func SetListenerFunc(name string, handler func(http.ResponseWriter, *http.Request) bool) {
	listeners[name] = ListenerFunc(handler)
}

//查詢 一個 名字 是否和 監聽器 關聯
func GetListener(name string) (h Listener, ok bool) {
	h, ok = listeners[name]
	return
}

//刪除  名字 字符串 和 監聽器 之間的關聯(如果 存在的話)
func DelListener(name string) {
	delete(listeners, name)
}
