package web

import (
	"dark/session"
	"net/http"
)

func init() {
	Instance()
}

type Handler interface {
	ServeHTTP(http.ResponseWriter, *http.Request, *session.Session)
}

var instance *singleRouter

type singleRouter struct {
	staticRounter map[string]int

	names map[string]Handler

	controller map[string]Handler
}

func Instance() *singleRouter {
	if instance == nil {
		instance = &singleRouter{}
		instance.staticRounter = make(map[string]int)
		instance.names = make(map[string]Handler)
		instance.controller = make(map[string]Handler)
	}
	return instance
}

func (s *singleRouter) AddStaticRounter(url string) {
	s.staticRounter[url] = 0
}
func (s *singleRouter) StaticRounter() map[string]int {
	return s.staticRounter
}
func (s *singleRouter) RegisterController(name string, h Handler) {
	s.names[name] = h
}

func (s *singleRouter) AddController(url, name string) {
	s.controller[url] = s.names[name]
}
func (s *singleRouter) Controller() map[string]Handler {
	return s.controller
}
