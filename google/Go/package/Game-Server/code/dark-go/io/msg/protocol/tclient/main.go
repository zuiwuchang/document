package main

import (
	"dark-go/io/msg"
	"dark-go/io/msg/protocol"
	"log"
	"net"
	"time"
)

func main() {
	//test_once()

	ch := make(chan int)
	sum := 0

	count := 2000
	for i := 0; i < count; i++ {
		go test_long(i, ch)
	}
	for {
		sum += <-ch
		if sum >= count {
			break
		}
	}
}
func test_long(i int, ch chan int) {
	c, e := net.Dial("tcp", "127.0.0.1:1102")
	if e != nil {
		log.Fatalln("%v connect : %v", i, e)
		log.Printf("%v connect : %v", i, e)
		return
	}
	defer c.Close()

	//消息 解析器
	reader := msg.Reader{}

	//協議路由器
	router := protocol.NewDefaultRouter()
	//註冊路由
	router.Register(&TextHandler2{})

	buf := make([]byte, 1024)
	for {
		n, e := c.Read(buf)
		if e != nil {
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
	ch <- 1
}
func test_once() {
	now := time.Now()
	c, e := net.Dial("tcp", "127.0.0.1:1102")
	if e != nil {
		log.Fatalln("connect :", e)
		return
	}
	defer c.Close()

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

	dur := time.Now().Sub(now)
	log.Println(dur)
}
