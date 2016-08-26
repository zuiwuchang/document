//自動 將 協議 指令 路由相應處理器
package protocol

import (
	"dark-go/io/msg"
	"net"
)

const (
	PROTOCOL_HASH_SIZE = 32
)

//生產 協議 數據
type Writer interface {
	//返回 協議 唯一標識 32字節 的 字符串
	Hash() string
	//寫入 數據流
	Write(b []byte)
	//用 已輸入數據流 生產一個協議消息
	CreateMsg(id uint32) (*msg.Msg, error)
}

//協議 路由器
type Router interface {
	//分發 消息 到路由
	//如沒沒有 匹配路由 返回 false
	Transmit(c net.Conn, m *msg.Msg) (bool, error)

	//註冊 路由
	Register(h Handler)

	//註銷 路由
	UnRegister(hash string)
}

//協議 處理器
type Handler interface {
	//返回 協議 唯一標識 32字節 的 字符串
	Hash() string

	//處理一條 協議消息
	Deal(c net.Conn, id uint32, b []byte)
}
