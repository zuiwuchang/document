package msg

import (
	"testing"
)

func TestWriter(t *testing.T) {
	w := Writer{}
	id := uint32(1)
	{
		str1 := "this is msg test\n"
		str2 := "Cerberus it's an idea\n"
		w.Write([]byte(str1))
		w.Write([]byte(str2))
		m, e := w.CreateMsg(id)
		if e != nil {
			t.Fatal(e)
		}

		if body, e := m.GetBody(); e != nil {
			t.Fatal(e)
		} else if string(body) != str1+str2 {
			t.Fatal("bad GetBody")
		}

		if header, e := m.GetHeader(); e != nil {
			t.Fatal(e)
		} else if header.Id != id ||
			header.Size != uint32(len(str1)+len(str2)) ||
			header.Flag != HEADER_FLAG {
			t.Fatal("bad GetHeader")
		}
	}

	id = 2
	{
		str1 := "i love kate beckinsale\n"
		w.Write([]byte(str1))
		m, e := w.CreateMsg(id)
		if e != nil {
			t.Fatal(e)
		}

		if body, e := m.GetBody(); e != nil {
			t.Fatal(e)
		} else if string(body) != str1 {
			t.Fatal("bad GetBody")
		}

		if header, e := m.GetHeader(); e != nil {
			t.Fatal(e)
		} else if header.Id != id ||
			header.Size != uint32(len(str1)) ||
			header.Flag != HEADER_FLAG {
			t.Fatal("bad GetHeader")
		}
	}
}
func TestReader(t *testing.T) {
	w := Writer{}
	str1 := "kate beckinsale is so sexy\n"
	str2 := "i love her\n"
	w.Write([]byte(str1))
	w.Write([]byte(str2))
	id := uint32(1)
	m, _ := w.CreateMsg(id)

	b := m.GetData()
	r := Reader{}
	r.Write(b[:len(b)/2])
	r.Write(b[len(b)/2:])
	m = r.GetMsg()
	if m == nil {
		t.Fatal("bad GetMsg")

		if body, e := m.GetBody(); e != nil {
			t.Fatal(e)
		} else if string(body) != str1+str2 {
			t.Fatal("bad GetBody")
		}

		if header, e := m.GetHeader(); e != nil {
			t.Fatal(e)
		} else if header.Id != id ||
			header.Size != uint32(len(str1)+len(str2)) ||
			header.Flag != HEADER_FLAG {
			t.Fatal("bad GetHeader")
		}
	}
	m = r.GetMsg()
	if m != nil {
		t.Fatal("bad GetMsg")
	}

}
