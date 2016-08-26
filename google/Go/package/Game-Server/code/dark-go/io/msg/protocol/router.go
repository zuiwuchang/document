package protocol

import (
	"dark-go/io/msg"
	"fmt"
	"net"
)

//默認 路由器
type routerDefault struct {
	handlers map[string]Handler
}

func NewDefaultRouter() Router {
	router := &routerDefault{}
	router.handlers = make(map[string]Handler)
	return router
}
func (r *routerDefault) Register(h Handler) {
	r.handlers[h.Hash()] = h
}
func (r *routerDefault) UnRegister(hash string) {
	delete(r.handlers, hash)
}
func (r *routerDefault) Transmit(c net.Conn, m *msg.Msg) (bool, error) {
	header, e := m.GetHeader()
	if e != nil {
		return false, e
	}
	if header.Size < PROTOCOL_HASH_SIZE {
		return false, fmt.Errorf("protocol format error")
	}
	body, e := m.GetBody()
	if e != nil {
		return false, e
	}
	hash := string(body[:PROTOCOL_HASH_SIZE])
	if handler, ok := r.handlers[hash]; ok {
		handler.Deal(c, header.Id, body[PROTOCOL_HASH_SIZE:])
		return true, nil
	}
	return false, nil
}
