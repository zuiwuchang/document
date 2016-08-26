package main

import (
	"sync"
)

var g_id uint32
var g_mutex sync.Mutex

func GetId() uint32 {
	g_mutex.Lock()
	defer g_mutex.Unlock()
	g_id++

	return g_id
}
