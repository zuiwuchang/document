package main

import (
	"dark-go/io/msg"
	"dark-go/io/msg/protocol"
	"fmt"
	"log"
	"net"
)

func main() {
	addr := ":1102"
	s, e := net.Listen("tcp", addr)
	if e != nil {
		log.Fatalln("Listen :", e)
		return
	}
	log.Println("Listen ", addr)
	for {
		c, e := s.Accept()
		if e != nil {
			log.Println("Accept :", e)
			continue
		}
		go doRead(c)
	}
}
func doRead(c net.Conn) {
	defer c.Close()
	fmt.Println(c.RemoteAddr(), "in")

	//發送 初始消息
	{
		w := protocol.TextWriter{}
		w.Write([]byte("welcome to cerberus server"))
		m, _ := w.CreateMsg(GetId())
		c.Write(m.GetData())

		w.Write([]byte("cerberus it's an idea"))
		m, _ = w.CreateMsg(GetId())
		c.Write(m.GetData())
	}

	//消息 解析器
	reader := msg.Reader{}

	//協議路由器
	router := protocol.NewDefaultRouter()
	//註冊路由
	router.Register(&TextHandler{})

	buf := make([]byte, 1024)
	for {
		n, e := c.Read(buf)
		if e != nil {
			fmt.Println(c.RemoteAddr(), "out")
			break
		}

		reader.Write(buf[:n])
		for {
			m := reader.GetMsg()
			if m == nil {
				break
			}
			router.Transmit(c, m)
		}
	}
}
