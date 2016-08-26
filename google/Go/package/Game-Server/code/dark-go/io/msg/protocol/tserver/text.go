package main

import (
	"dark-go/io/msg/protocol"
	"fmt"
	"net"
)

type TextHandler struct {
	protocol.TextHandler
}

func (t *TextHandler) Deal(c net.Conn, id uint32, b []byte) {
	str := string(b)
	fmt.Printf("recv (%v [%v]) %v\n", c.RemoteAddr(), id, str)
	if str == "i want join cerberus" {
		w := protocol.TextWriter{}
		w.Write([]byte("you are cerberus soldier now"))
		m, _ := w.CreateMsg(GetId())
		c.Write(m.GetData())
	}
}
