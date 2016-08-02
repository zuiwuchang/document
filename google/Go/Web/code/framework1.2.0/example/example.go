package example

import (
	"dark/session"
	"dark/web"
	"fmt"
	"net/http"
	"strconv"
)

func Initialization() {
	instance := web.Instance()

	h := Hellow{}
	instance.RegisterController("hellow", h)

	t := Test{}
	instance.RegisterController("test", t)

	s := Session{}
	instance.RegisterController("session", s)

}

type Hellow struct {
}

func (h Hellow) ServeHTTP(rw http.ResponseWriter, req *http.Request, s *session.Session) {
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

func (t Test) ServeHTTP(rw http.ResponseWriter, req *http.Request, s *session.Session) {
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

type Session struct {
}

func (h Session) ServeHTTP(rw http.ResponseWriter, req *http.Request, s *session.Session) {
	var i int = 0
	id := s.Id
	if v, ok := s.GetValue("number"); ok {
		if i, ok = v.(int); ok {
			i++
		}
		s.SetValue("number", i)
	} else {
		s.SetValue("number", i)
	}

	html := "<!DOCTYPE html>" +
		"<html>" +
		"<head>" +
		"	<title></title>" +
		"</head>" +
		"<body>" +
		"session = " + id +
		"<br>val = " + strconv.Itoa(i) +
		"</body>" +
		"</html>"
	fmt.Fprintln(rw, html)
}
