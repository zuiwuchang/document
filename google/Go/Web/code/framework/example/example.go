package example

import (
	"dark/web"
	"fmt"
	"net/http"
)

func Initialization() {
	instance := web.Instance()

	h := Hellow{}
	instance.RegisterController("hellow", h)

	t := Test{}
	instance.RegisterController("test", t)

}

type Hellow struct {
}

func (h Hellow) ServeHTTP(rw http.ResponseWriter, req *http.Request) {
	html := "<!DOCTYPE html>" +
		"<html>" +
		"<head>" +
		"	<title></title>" +
		"</head>" +
		"<body>" +
		"hellow word" +
		"<img src='/images/a.jpg'>" +
		"</body>" +
		"</html>"
	fmt.Fprintln(rw, html)
}

type Test struct {
}

func (t Test) ServeHTTP(rw http.ResponseWriter, req *http.Request) {
	html := "<!DOCTYPE html>" +
		"<html>" +
		"<head>" +
		"	<title></title>" +
		"</head>" +
		"<body>" +
		"this is a test" +
		"<img src='/images/a.jpg'>" +
		"</body>" +
		"</html>"
	fmt.Fprintln(rw, html)
}
