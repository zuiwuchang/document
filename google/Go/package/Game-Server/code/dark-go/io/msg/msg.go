//帶消息 邊界的 數據流 生成 解析
package msg

import (
	"bytes"
	"encoding/binary"
	"fmt"
)

const (
	HEADER_FLAG = 0x0000044E
	HEADER_SIZE = 12
)

type Header struct {
	Flag uint32
	Id   uint32
	Size uint32
}
type Msg struct {
	data []byte
}

func (m *Msg) GetHeader() (*Header, error) {
	if len(m.data) < HEADER_SIZE {
		return nil, fmt.Errorf("msg format error")
	}

	buf := &bytes.Buffer{}
	buf.Write(m.data[0:HEADER_SIZE])
	header := &Header{}
	e := binary.Read(buf, binary.LittleEndian, header)
	if e != nil {
		return nil, e
	}
	return header, nil
}
func (m *Msg) GetBody() ([]byte, error) {
	if len(m.data) < HEADER_SIZE {
		return nil, fmt.Errorf("msg format error")
	}
	return m.data[HEADER_SIZE:], nil
}
func (m *Msg) GetData() []byte {
	return m.data
}
