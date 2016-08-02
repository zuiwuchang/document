//字符串處理工具
package strutil

import (
	"bytes"
	"fmt"
)

type JsonMap map[string]interface{}
type JsonSlice []interface{}

//將 go map 轉爲json字符串
func ToJsonString(json JsonMap) string {
	bs := bytes.NewBuffer(nil)
	writeJsonMap(bs, json)
	return bs.String()
}
func writeJsonValue(bs *bytes.Buffer, v interface{}) {
	switch v.(type) {
	case map[string]interface{}:
		writeJsonMap(bs, v.(map[string]interface{}))
	case JsonMap:
		writeJsonMap(bs, v.(JsonMap))
	case []interface{}:
		writeJsonSlice(bs, v.([]interface{}))
	case JsonSlice:
		writeJsonSlice(bs, v.(JsonSlice))
	case error:
		fmt.Fprintf(bs, "\"%v\"", v)
	case string:
		fmt.Fprintf(bs, "\"%v\"", v)
	default:
		fmt.Fprintf(bs, "%v", v)
	}
}
func writeJsonMap(bs *bytes.Buffer, json map[string]interface{}) {
	bs.WriteString("{")
	once := true
	for k, v := range json {
		if once {
			once = false
		} else {
			bs.WriteString(",")
		}
		fmt.Fprintf(bs, "\"%v\":", k)

		writeJsonValue(bs, v)
	}
	bs.WriteString("}")
}
func writeJsonSlice(bs *bytes.Buffer, vs []interface{}) {
	once := true
	bs.WriteString("[")
	for _, v := range vs {
		if once {
			once = false
		} else {
			bs.WriteString(",")
		}
		writeJsonValue(bs, v)
	}
	bs.WriteString("]")
}

//創建一個 符合json 的新的 map
func NewJsonMap() JsonMap {
	return make(JsonMap)
}

//創建一個 符合json 的新的 slice
func NewJsonSlice(capacity int) JsonSlice {
	return make(JsonSlice, 0, capacity)
}
