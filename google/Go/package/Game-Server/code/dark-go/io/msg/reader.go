package msg

import (
	"bytes"
	"encoding/binary"
)

//消息 解析器
type Reader struct {
	buffer bytes.Buffer
}

//向 解析器 寫入 數據
func (r *Reader) Write(bytes []byte) {
	r.buffer.Write(bytes)
}

//從 解析器中 獲取 消息
func (r *Reader) GetMsg() *Msg {
	size := r.buffer.Len()
	if size < HEADER_SIZE {
		return nil
	}

	data := r.buffer.Bytes()
	buf := &bytes.Buffer{}
	buf.Write(data[:HEADER_SIZE])
	header := &Header{}
	binary.Read(buf, binary.LittleEndian, header)
	sum := header.Size + HEADER_SIZE
	if size < int(sum) {
		return nil
	}
	data = make([]byte, sum)
	r.buffer.Read(data)
	msg := &Msg{data: data}
	return msg
}
