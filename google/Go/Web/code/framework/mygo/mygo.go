package main

import (
	"dark/web"
	"encoding/xml"
	"example"
	"fmt"
	"io/ioutil"
	"net/http"
	"os"
	"strconv"
	"strings"
)

func init() {
	example.Initialization()
}

type Config struct {
	XMLName xml.Name `xml:"cnf"`
	Version string   `xml:"version,attr"`
	Server  Server   `xml:"server"`
	Router  Router   `xml:"router"`
}
type Server struct {
	Port uint32 `xml:"port"`
}
type Router struct {
	Static     []string     `xml:"static>note"`
	Controller []Controller `xml:"controller>note"`
}
type Controller struct {
	Url  string `xml:"url"`
	Name string `xml:"name"`
}

func main() {

	instance := web.Instance()

	//讀取配置
	file, e := os.Open("cnf.xml")
	if e != nil {
		fmt.Println("can't open cnf.xml\n%v", e)
		return
	}
	defer file.Close()
	data, e := ioutil.ReadAll(file)
	if e != nil {
		fmt.Println(e)
		return
	}
	cnf := Config{}
	e = xml.Unmarshal(data, &cnf)
	if e != nil {
		fmt.Println("cnf format error\n%v", e)
		return
	}

	//註冊靜態文件服務
	for _, v := range cnf.Router.Static {
		instance.AddStaticRounter(v)
	}

	//註冊控制器
	for _, v := range cnf.Router.Controller {
		instance.AddController(v.Url, v.Name)
	}

	//註冊路由
	http.HandleFunc("/", RouterHandler)

	//監聽
	addr := ":" + strconv.FormatUint(uint64(cnf.Server.Port), 10)
	fmt.Println(addr)
	fmt.Println("work at %s", addr)
	if e := http.ListenAndServe(addr, nil); e != nil {
		fmt.Println(e)
	}

}
func RouterHandler(rw http.ResponseWriter, req *http.Request) {
	path := req.URL.Path

	instance := web.Instance()
	//靜態路由
	for key, _ := range instance.StaticRounter() {
		if strings.HasPrefix(path, key) {
			http.ServeFile(rw, req, path[1:])
			return
		}
	}

	//路由控制器
	for key, val := range instance.Controller() {
		if strings.HasPrefix(path, key) {
			val.ServeHTTP(rw, req)
			return
		}
	}

	//404
	http.NotFound(rw, req)
}
