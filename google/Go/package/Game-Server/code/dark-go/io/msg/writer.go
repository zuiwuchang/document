//帶消息 邊界的 數據流 生成 解析
package msg

import (
	"bytes"
	"encoding/binary"
)

type buffer []byte

//消息 生成器
type Writer struct {
	buffer bytes.Buffer
}

//傳入 消息 數據流
func (w *Writer) Write(b []byte) {
	if b == nil || len(b) == 0 {
		return
	}

	w.buffer.Write(b)
}

//將 傳入的 數據流 生產一個 消息
func (w *Writer) CreateMsg(id uint32) (*Msg, error) {
	header := &Header{Flag: HEADER_FLAG, Id: id, Size: uint32(w.buffer.Len())}

	buf := &bytes.Buffer{}
	e := binary.Write(buf, binary.LittleEndian, header)
	if e != nil {
		return nil, e
	}
	data := make([]byte, HEADER_SIZE+header.Size)
	_, e = buf.Read(data)
	if e != nil {
		return nil, e
	}

	_, e = w.buffer.Read(data[HEADER_SIZE:])
	if e != nil {
		return nil, e
	}

	msg := &Msg{data: data}
	return msg, nil
}
