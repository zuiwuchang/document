package main

import (
	"cerberus/http/extreme"
	clog "cerberus/http/log"
	"cerberus/http/session"
	"encoding/xml"
	"fmt"
	"io/ioutil"
	"log"
	"new/controller"
	"os"
	"time"
)

const (
	CONFIG_FILE = "private/cnf.xml"
)

func main() {
	//讀取配置
	cnf, err := initConfig()
	if err != nil {
		fmt.Println(err)
		return
	}
	//初始化日誌
	initLog(cnf)

	//註冊路由
	registerController(cnf)

	//初始化session
	initSession(cnf)

	//用戶初始化
	if err := controller.Initialize(); err != nil {
		fmt.Printf("用戶初始化失敗 %v", err)
		return
	}

	//開啓服務器
	addr := fmt.Sprintf(":%v", cnf.Server.Port)
	clog.Infof("work at %v", addr)
	if err := extreme.ListenAndServe(addr); err != nil {
		fmt.Println(err)
	}
}

//初始化 路由設置
func registerController(cnf *Config) {
	//註冊 首頁
	for _, s := range cnf.Router.Index {
		extreme.RegisterIndexPage(s)
	}

	//註冊 靜態服務
	/*extreme.RegisterServeFile("/css/")
	extreme.RegisterServeFile("/js/")
	extreme.RegisterServeFile("/img/")*/
	for _, s := range cnf.Router.Static {
		e := extreme.RegisterServeFile(s)
		if e != nil {
			clog.Error(e)
		}
	}

	//註冊路由
	/*//註冊一般路由
	extreme.RegisterControllerByName(extreme.GET, "/hellow", "hellow word")
	//指定 參數 正則 路由
	extreme.RegisterControllerByName(extreme.GET, "/param/:id([0-9]+)/:name([a-z]+)", "param")*/
	for _, h := range cnf.Router.Handler {
		e := extreme.RegisterControllerByName(h.Method, h.Url, h.Name)
		if e != nil {
			clog.Error(e)
		}
	}

	//註冊監聽器
	//extreme.RegisterListenerByName(extreme.LISTENER_BEFORE, "/", "mylistener")
	for _, h := range cnf.Router.Listener {
		e := extreme.RegisterListenerByName(h.Method, h.Url, h.Name)
		if e != nil {
			clog.Error(e)
		}
	}
}

//初始化session
func initSession(cnf *Config) {
	go cleanSession(cnf.Server.Interval, cnf.Server.Maxlife)
}

//定時清理 無效session
func cleanSession(minute time.Duration, secondLive int64) {
	for {
		time.Sleep(minute * time.Minute)
		session.CleanSession(secondLive)
	}
}

//服務器配置文檔
type Config struct {
	XMLName xml.Name `xml:"cnf"`
	Version string   `xml:"version,attr"`
	Server  Server   `xml:"server"`
	Router  Router   `xml:"router"`
}

//服務器配置
type Server struct {
	Port     uint32        `xml:"port"`
	Log      int           `xml:"log"`
	File     LogFile       `xml:"logfile"`
	Loglv    int           `xml:"loglv"`
	Maxlife  int64         `xml:"maxlive"`
	Interval time.Duration `xml:"interval"`
}

//日誌文件配置
type LogFile struct {
	Log       int    `xml:"log"`
	Directory string `xml:"directory"`
	Size      int64  `xml:"maxsize"`
	Count     int    `xml:"maxcount"`
}

//路由配置
type Router struct {
	Index    []string     `xml:"index>url"`
	Static   []string     `xml:"static>url"`
	Handler  []Controller `xml:"controller>note"`
	Listener []Controller `xml:"listener>note"`
}

//路由信息
type Controller struct {
	Url    string `xml:"url"`
	Name   string `xml:"name"`
	Method string `xml:"method"`
}

//初始化配置信息
func initConfig() (cnf *Config, err error) {
	//讀取配置
	file, e := os.Open(CONFIG_FILE)
	if e != nil {
		err = fmt.Errorf("can't open %v %v\n", CONFIG_FILE, e)
		return
	}
	defer file.Close()
	data, e := ioutil.ReadAll(file)
	if e != nil {
		err = e
		return
	}
	cnf = &Config{}
	e = xml.Unmarshal(data, cnf)
	if e != nil {
		err = fmt.Errorf("%v format error %v\n", CONFIG_FILE, e)
		return
	}
	return
}

//初始化日誌
func initLog(cnf *Config) {
	clog.SetGrade(cnf.Server.Loglv)
	if cnf.Server.Log == 1 {
		logger := log.New(os.Stdout, "", log.LstdFlags)
		clog.AddSink(logger, "stdout")
	}
	if cnf.Server.File.Log == 1 {
		file := clog.NewFile(cnf.Server.File.Directory, cnf.Server.File.Size, cnf.Server.File.Count)
		logger := log.New(file, "", log.LstdFlags)
		clog.AddSink(logger, "file")
	}
}
