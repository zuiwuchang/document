//文本 傳輸 協議
package protocol

import (
	"dark-go/io/msg"
)

type TextWriter struct {
	writer msg.Writer
	flag   bool
}

//返回 協議 唯一標識 32字節 的 字符串
func (t *TextWriter) Hash() string {
	//md5 cerberus dark go msg protocol text
	return "7053fb816d7d350c7029539b9a3c14af"
}

//寫入 數據流
func (t *TextWriter) Write(b []byte) {
	if !t.flag {
		t.writer.Write([]byte(t.Hash()))
		t.flag = true
	}
	t.writer.Write(b)
}

//用 已輸入數據流 生產一個協議消息
func (t *TextWriter) CreateMsg(id uint32) (*msg.Msg, error) {
	m, e := t.writer.CreateMsg(id)
	if e == nil {
		t.flag = false
	}
	return m, e
}

type TextHandler struct {
}

//返回 協議 唯一標識 32字節 的 字符串
func (t *TextHandler) Hash() string {
	//md5 cerberus dark go msg protocol text
	return "7053fb816d7d350c7029539b9a3c14af"
}
