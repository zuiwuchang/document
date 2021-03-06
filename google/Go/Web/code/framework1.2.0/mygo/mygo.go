package main

import (
	dlog "dark/log"
	"dark/session"
	"dark/web"
	"encoding/xml"
	"example"
	"io/ioutil"
	"log"
	"net/http"
	"os"
	"strconv"
	"strings"
	"time"
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
	Port  uint32  `xml:"port"`
	Log   int     `xml:"log"`
	File  LogFile `xml:"logfile"`
	Loglv int     `xml:"loglv"`
}
type LogFile struct {
	Log       int    `xml:"log"`
	Directory string `xml:"directory"`
	Size      int64  `xml:"maxsize"`
	Count     int    `xml:"maxcount"`
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
	go SessionDestory(30)

	//讀取配置
	file, e := os.Open("cnf.xml")
	if e != nil {
		dlog.Errorf("can't open cnf.xml\n%v", e)
		return
	}
	defer file.Close()
	data, e := ioutil.ReadAll(file)
	if e != nil {
		dlog.Error(e)
		return
	}
	cnf := Config{}
	e = xml.Unmarshal(data, &cnf)
	if e != nil {
		dlog.Errorf("cnf format error\n%v", e)
		return
	}
	//初始化日誌
	dlog.SetGrade(cnf.Server.Loglv)
	if cnf.Server.Log == 1 {
		logger := log.New(os.Stdout, "", log.LstdFlags)
		dlog.AddSink(logger, "stdout")
	}
	if cnf.Server.File.Log == 1 {
		file := dlog.NewFile(cnf.Server.File.Directory, cnf.Server.File.Size, cnf.Server.File.Count)
		logger := log.New(file, "", log.LstdFlags)
		dlog.AddSink(logger, "file")
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
	dlog.Info("work at", addr)
	if e := http.ListenAndServe(addr, nil); e != nil {
		dlog.Fatal(e)
	}

}
func RouterHandler(rw http.ResponseWriter, req *http.Request) {
	s, e := LockSession(rw, req)
	if e == nil {
		defer s.Mutex.Unlock()
	} else {
		dlog.Debug("create session error" + req.URL.Path)
		return
	}
	path := req.URL.Path
	//html
	if path == "/" {
		http.ServeFile(rw, req, "index.html")
		return
	}

	if strings.HasSuffix(path, ".html") {
		http.ServeFile(rw, req, path[1:])
		return
	}

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
			val.ServeHTTP(rw, req, s)
			return
		}
	}

	//404
	http.NotFound(rw, req)
}
func LockSession(rw http.ResponseWriter, req *http.Request) (s *session.Session, e error) {
	manage := session.Instance()

	s, e = manage.GetSession(rw, req)
	if e != nil {
		s, e = manage.CreateSession(rw, req)
		if e == nil {
			dlog.Debug("new session")
		}
	}

	if e == nil {
		s.Mutex.Lock()
		s.Time = time.Now()
	}
	return
}

func SessionDestory(maxlife int64) {
	for {
		time.Sleep(time.Minute * 10)
		dlog.Debug("SessionDestory")

		session.Instance().CleanSession(maxlife)
	}
}
