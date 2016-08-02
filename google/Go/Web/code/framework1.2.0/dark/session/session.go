package session

import (
	"crypto/rand"
	"encoding/base64"
	"fmt"
	"io"
	"net/http"
	"sync"
	"time"
)

func init() {
	Instance()
}

var instance *singleSessionManage

type singleSessionManage struct {
	mutex sync.RWMutex

	sessions map[string]*Session
}

func Instance() *singleSessionManage {
	if instance == nil {
		instance = &singleSessionManage{}
		instance.sessions = make(map[string]*Session)
	}
	return instance
}

//返回全局唯一的 session id
func (s *singleSessionManage) sessionId() (id string, e error) {
	b := make([]byte, 32)
	for {
		if _, e = io.ReadFull(rand.Reader, b); e != nil {
			return
		}
		id = base64.URLEncoding.EncodeToString(b)

		if _, ok := s.sessions[id]; !ok {
			return
		}
	}
}

func (s *singleSessionManage) CreateSession(rw http.ResponseWriter, req *http.Request) (session *Session, e error) {
	s.mutex.Lock()
	defer s.mutex.Unlock()
	var id string
	//已經存在刪除之
	if val, e := req.Cookie("sessionid"); e != nil && val != nil {
		id := val.Value
		if _, ok := s.sessions[id]; ok {
			delete(s.sessions, id)
		}
	}

	//返回id
	id, e = s.sessionId()
	if e != nil {
		return
	}

	//設置cookie
	cookie := http.Cookie{Name: "sessionid", Value: id}
	http.SetCookie(rw, &cookie)

	//創建服務器數據
	s.sessions[id] = &Session{Time: time.Now(), Id: id, data: make(map[string]interface{})}
	session = s.sessions[id]
	return
}

func (s *singleSessionManage) GetSession(rw http.ResponseWriter, req *http.Request) (session *Session, e error) {
	s.mutex.RLock()
	defer s.mutex.RUnlock()
	var val *http.Cookie
	if val, e = req.Cookie("sessionid"); e == nil {
		if val == nil {
			e = fmt.Errorf("no found session")
			return
		}
		id := val.Value
		if s, ok := s.sessions[id]; ok {
			session = s
		} else {
			e = fmt.Errorf("no found session")
		}
	}
	return
}
func (s *singleSessionManage) DestotySession(rw http.ResponseWriter, req *http.Request) {
	if val, e := req.Cookie("sessionid"); e != nil && val != nil {
		s.mutex.Lock()
		defer s.mutex.Unlock()
		id := val.Value
		if _, ok := s.sessions[id]; ok {
			delete(s.sessions, id)
		}
	}
}

func (s *singleSessionManage) CleanSession(maxlife /*second*/ int64) {
	s.mutex.Lock()
	defer s.mutex.Unlock()

	unix := time.Now().Unix()

	for id, session := range s.sessions {
		pre := session.Time.Unix()
		//fmt.Println(unix, pre, maxlife)
		pre += maxlife
		if pre < unix {
			delete(s.sessions, id)
		}
	}
}

type Session struct {
	Time  time.Time
	Mutex sync.Mutex

	data map[string]interface{}
	Id   string
}

func (s *Session) SetValue(key string, val interface{}) {
	s.data[key] = val
}
func (s *Session) GetValue(key string) (val interface{}, ok bool) {
	val, ok = s.data[key]
	return
}
func (s *Session) DeleteValue(key string) {
	delete(s.data, key)
}
