package main

import (
	"dark-go/io/msg/protocol"
	"fmt"
	"net"
	"time"
)

type TextHandler struct {
	protocol.TextHandler
}

func (t *TextHandler) Deal(c net.Conn, id uint32, b []byte) {
	str := string(b)
	fmt.Printf("recv (%v [%v]) %v\n", c.RemoteAddr(), id, str)

	if str == "cerberus it's an idea" {
		w := protocol.TextWriter{}
		w.Write([]byte("i'm a soldier"))
		m, _ := w.CreateMsg(GetId())
		c.Write(m.GetData())

		w.Write([]byte("i want join cerberus"))
		m, _ = w.CreateMsg(GetId())
		c.Write(m.GetData())
	} else if str == "you are cerberus soldier now" {
		c.Close()
	}
}

type TextHandler2 struct {
	protocol.TextHandler
}

func (t *TextHandler2) Deal(c net.Conn, id uint32, b []byte) {
	str := string(b)
	fmt.Printf("recv (%v [%v]) %v\n", c.RemoteAddr(), id, str)

	if str == "cerberus it's an idea" {
		w := protocol.TextWriter{}
		w.Write([]byte("i'm a soldier"))
		m, _ := w.CreateMsg(GetId())
		c.Write(m.GetData())

		w.Write([]byte("i want join cerberus"))
		m, _ = w.CreateMsg(GetId())
		c.Write(m.GetData())
	} else if str == "you are cerberus soldier now" {
		time.Sleep(100)

		w := protocol.TextWriter{}
		w.Write([]byte("i'm a soldier"))
		m, _ := w.CreateMsg(GetId())
		c.Write(m.GetData())

		w.Write([]byte("i want join cerberus"))
		m, _ = w.CreateMsg(GetId())
		c.Write(m.GetData())
	}
}
