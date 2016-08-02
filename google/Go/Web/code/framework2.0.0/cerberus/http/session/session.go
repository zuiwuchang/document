//實現了http的 session功能
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

const (
	CERBERUSE_SESSION_ID_NAME = "cerberus_session_id"
)

//session實例
var instance *singleSessionManage

//session型別
type singleSessionManage struct {
	mutex    sync.RWMutex
	sessions map[string]*Session
}

//返回session單件
func Instance() *singleSessionManage {
	if instance == nil {
		instance = &singleSessionManage{}
		instance.sessions = make(map[string]*Session)
	}
	return instance
}

//通過 session id 返回次session
func GetSessionById(id string) (se *Session, ok bool) {
	se, ok = instance.sessions[id]
	return
}

//通過 session id 返回次session
func (s *singleSessionManage) GetSessionById(id string) (se *Session, ok bool) {
	s.mutex.Lock()
	defer s.mutex.Unlock()
	se, ok = s.sessions[id]
	return
}

//返回全局唯一的 session id
func (s *singleSessionManage) sessionId() (id string, err error) {
	b := make([]byte, 32)
	for {
		if _, err = io.ReadFull(rand.Reader, b); err != nil {
			return
		}
		id = base64.URLEncoding.EncodeToString(b)

		if _, ok := s.sessions[id]; !ok {
			return
		}
	}
}

//返回session 如果不存在 創建之
func GetAndCreateSession(rw http.ResponseWriter, req *http.Request) (session *Session, err error) {
	session, err = instance.GetAndCreateSession(rw, req)
	return
}

//返回session 如果不存在 創建之
func (s *singleSessionManage) GetAndCreateSession(rw http.ResponseWriter, req *http.Request) (session *Session, err error) {
	s.mutex.RLock()
	defer s.mutex.RUnlock()

	session, err = s.getSession(req)
	if err != nil {
		//不存在 創建之
		session, err = s.createSession(rw, req)
	}
	return
}

//創建 session
func (s *singleSessionManage) createSession(rw http.ResponseWriter, req *http.Request) (session *Session, err error) {
	var id string
	//已經存在刪除之
	if val, e := req.Cookie(CERBERUSE_SESSION_ID_NAME); e != nil && val != nil {
		id := val.Value
		if _, ok := s.sessions[id]; ok {
			delete(s.sessions, id)
		}
	}

	//返回id
	id, err = s.sessionId()
	if err != nil {
		return
	}

	//設置cookie
	cookie := http.Cookie{Name: CERBERUSE_SESSION_ID_NAME, Value: id}
	http.SetCookie(rw, &cookie)

	//創建服務器數據
	s.sessions[id] = &Session{Time: time.Now(), Id: id, data: make(map[string]interface{})}
	session = s.sessions[id]
	return
}

//創建 session
func CreateSession(rw http.ResponseWriter, req *http.Request) (session *Session, err error) {
	session, err = instance.CreateSession(rw, req)
	return
}

//創建 session
func (s *singleSessionManage) CreateSession(rw http.ResponseWriter, req *http.Request) (session *Session, err error) {
	s.mutex.Lock()
	defer s.mutex.Unlock()
	session, err = s.createSession(rw, req)
	return
}

//返回session
func (s *singleSessionManage) getSession(req *http.Request) (session *Session, err error) {
	var val *http.Cookie
	if val, err = req.Cookie(CERBERUSE_SESSION_ID_NAME); err == nil {
		if val == nil {
			err = fmt.Errorf("no found session")
			return
		}
		id := val.Value
		if s, ok := s.sessions[id]; ok {
			session = s
		} else {
			err = fmt.Errorf("no found session")
		}
	}
	return
}

//返回session
func GetSession(req *http.Request) (session *Session, err error) {
	session, err = instance.GetSession(req)
	return
}

//返回session
func (s *singleSessionManage) GetSession(req *http.Request) (session *Session, err error) {
	s.mutex.RLock()
	defer s.mutex.RUnlock()
	session, err = s.getSession(req)
	return
}

//銷燬session
func DestotySession(req *http.Request) {
	instance.DestotySession(req)
}

//銷燬session
func (s *singleSessionManage) DestotySession(req *http.Request) {
	if val, e := req.Cookie(CERBERUSE_SESSION_ID_NAME); e != nil && val != nil {
		s.mutex.Lock()
		defer s.mutex.Unlock()
		id := val.Value
		if _, ok := s.sessions[id]; ok {
			delete(s.sessions, id)
		}
	}
}

//刪除無效session
func CleanSession(maxlife /*second*/ int64) {
	instance.CleanSession(maxlife)
}

//刪除無效session
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
	mutex sync.Mutex

	data map[string]interface{}
	Id   string
}

func (s *Session) SetValue(key string, val interface{}) {
	s.mutex.Lock()
	defer s.mutex.Unlock()
	s.data[key] = val
}
func (s *Session) GetValue(key string) (val interface{}, ok bool) {
	s.mutex.Lock()
	defer s.mutex.Unlock()
	val, ok = s.data[key]
	return
}
func (s *Session) DeleteValue(key string) {
	s.mutex.Lock()
	defer s.mutex.Unlock()
	delete(s.data, key)
}
