// 在 log 之上 封裝的一個 日誌庫 實現了 日誌論轉 輸出等級 多輸出槽
package log

import (
	"fmt"
	"log"
	"os"
	"regexp"
	"runtime"
	"sort"
	"strconv"
	"strings"
	"sync"
	"time"
)

var singleLog *LoggerManage

func init() {
	singleLog = NewManage(INFO)
	singleLog.SetFlag(SHOW_GRADE | SHOW_FILE)
}

//記錄 TRACE 等級 日誌
func Tracef(format string, a ...interface{}) {
	singleLog.printf(TRACE, " [TRACE]", format, a...)
}

//記錄 DEBUG 等級 日誌
func Debugf(format string, a ...interface{}) {
	singleLog.printf(DEBUG, " [DEBUG]", format, a...)
}

//記錄 INFO 等級 日誌
func Infof(format string, a ...interface{}) {
	singleLog.printf(INFO, " [INFO]", format, a...)
}

//記錄 WARNING 等級 日誌
func Waringf(format string, a ...interface{}) {
	singleLog.printf(WARNING, " [WARNING]", format, a...)
}

//記錄 ERROR 等級 日誌
func Errorf(format string, a ...interface{}) {
	singleLog.printf(ERROR, " [ERROR]", format, a...)
}

//記錄 FATAL 等級 日誌
func Fatalf(format string, a ...interface{}) {
	singleLog.printf(FATAL, " [FATAL]", format, a...)
}

//記錄 TRACE 等級 日誌
func Trace(a ...interface{}) {
	singleLog.print(TRACE, " [TRACE]", a...)
}

//記錄 DEBUG 等級 日誌
func Debug(a ...interface{}) {
	singleLog.print(DEBUG, " [DEBUG]", a...)
}

//記錄 INFO 等級 日誌
func Info(a ...interface{}) {
	singleLog.print(INFO, " [INFO]", a...)
}

//記錄 WARNING 等級 日誌
func Waring(a ...interface{}) {
	singleLog.print(WARNING, " [WARNING]", a...)
}

//記錄 ERROR 等級 日誌
func Error(a ...interface{}) {
	singleLog.print(ERROR, " [ERROR]", a...)
}

//記錄 FATAL 等級 日誌
func Fatal(a ...interface{}) {
	singleLog.print(FATAL, " [FATAL]", a...)
}

//設置記錄等級
func SetGrade(grade int) {
	singleLog.grade = grade
}

//返回記錄等級
func GetGrade() int {
	return singleLog.grade
}

//設置 額外顯示信息
func SetFlag(flag int) {
	singleLog.flag = flag
}

//返回 額外顯示信息
func GetFlag() int {
	return singleLog.flag
}

//增加一個 日誌輸出位置 到 日誌管理器
//name 爲自定義的一個 名字 以區別 多個 輸出
func AddSink(w *log.Logger, name string) {
	singleLog.AddSink(w, name)
}

//日誌等級
const (
	TRACE   = 0
	DEBUG   = 1
	INFO    = 2
	WARNING = 3
	ERROR   = 4
	FATAL   = 5
)

//日誌 額外信息
const (
	SHOW_GRADE = 0x1
	SHOW_FILE  = 0x2
)

//創建一個 新的日誌管理器
//grade 是要記錄的日誌等級
func NewManage(grade int) (l *LoggerManage) {
	l = &LoggerManage{}
	l.grade = grade
	l.loggers = make(map[string]*log.Logger)
	return
}

//日誌管理器
type LoggerManage struct {
	mutex   sync.Mutex
	loggers map[string]*log.Logger

	flag  int
	grade int
}

//設置記錄等級
func (l *LoggerManage) SetGrade(grade int) {
	l.grade = grade
}

//返回記錄等級
func (l *LoggerManage) GetGrade() int {
	return l.grade
}

//設置 額外顯示信息
func (l *LoggerManage) SetFlag(flag int) {
	l.flag = flag
}

//返回 額外顯示信息
func (l *LoggerManage) GetFlag() int {
	return l.flag
}

//增加一個 日誌輸出位置 到 日誌管理器
//name 爲自定義的一個 名字 以區別 多個 輸出
func (l *LoggerManage) AddSink(w *log.Logger, name string) {
	l.mutex.Lock()
	defer l.mutex.Unlock()

	l.loggers[name] = w
}

//記錄 TRACE 等級 日誌
func (l *LoggerManage) Tracef(format string, a ...interface{}) {
	l.printf(TRACE, " [TRACE]", format, a...)
}

//記錄 DEBUG 等級 日誌
func (l *LoggerManage) Debugf(format string, a ...interface{}) {
	l.printf(DEBUG, " [DEBUG]", format, a...)
}

//記錄 INFO 等級 日誌
func (l *LoggerManage) Infof(format string, a ...interface{}) {
	l.printf(INFO, " [INFO]", format, a...)
}

//記錄 WARNING 等級 日誌
func (l *LoggerManage) Waringf(format string, a ...interface{}) {
	l.printf(WARNING, " [WARNING]", format, a...)
}

//記錄 ERROR 等級 日誌
func (l *LoggerManage) Errorf(format string, a ...interface{}) {
	l.printf(ERROR, " [ERROR]", format, a...)
}

//記錄 FATAL 等級 日誌
func (l *LoggerManage) Fatalf(format string, a ...interface{}) {
	l.printf(FATAL, " [FATAL]", format, a...)
}

//記錄 TRACE 等級 日誌
func (l *LoggerManage) Trace(a ...interface{}) {
	l.print(TRACE, " [TRACE]", a...)
}

//記錄 DEBUG 等級 日誌
func (l *LoggerManage) Debug(a ...interface{}) {
	l.print(DEBUG, " [DEBUG]", a...)
}

//記錄 INFO 等級 日誌
func (l *LoggerManage) Info(a ...interface{}) {
	l.print(INFO, " [INFO]", a...)
}

//記錄 WARNING 等級 日誌
func (l *LoggerManage) Waring(a ...interface{}) {
	l.print(WARNING, " [WARNING]", a...)
}

//記錄 ERROR 等級 日誌
func (l *LoggerManage) Error(a ...interface{}) {
	l.print(ERROR, " [ERROR]", a...)
}

//記錄 FATAL 等級 日誌
func (l *LoggerManage) Fatal(a ...interface{}) {
	l.print(FATAL, " [FATAL]", a...)
}
func (l *LoggerManage) print(lv int, flag string, a ...interface{}) {
	l.mutex.Lock()
	defer l.mutex.Unlock()
	if l.grade > lv {
		return
	}

	str := fmt.Sprint(a...)
	if SHOW_FILE&l.flag == SHOW_FILE {
		str = codeWhere(3) + str
	}

	if SHOW_GRADE&l.flag == SHOW_GRADE {
		str += flag
	}

	for _, logger := range l.loggers {
		logger.Print(str)
	}
}
func (l *LoggerManage) printf(lv int, flag, format string, a ...interface{}) {
	l.mutex.Lock()
	defer l.mutex.Unlock()
	if l.grade > lv {
		return
	}

	str := fmt.Sprintf(format, a...)
	if SHOW_FILE&l.flag == SHOW_FILE {
		str = codeWhere(3) + str
	}

	if SHOW_GRADE&l.flag == SHOW_GRADE {
		str += flag
	}

	for _, logger := range l.loggers {
		logger.Print(str)
	}
}

func codeWhere(skip int) string {
	pc, file, line, ok := runtime.Caller(skip)
	if ok {
		fn := runtime.FuncForPC(pc)
		return fmt.Sprintf("%v %v %v ", fn.Name(), file, line)
	}
	return ""
}

//創建 新日誌論轉文件 size爲單個文件大小 count爲 最大文件數量
func NewFile(directory string, size int64, count int) *LoopFile {
	return &LoopFile{Directory: directory,
		MaxFileSize:  size,
		MaxFileCount: count}
}

//實現 輪換的 日誌輸出
type LoopFile struct {
	Directory    string
	MaxFileSize  int64
	MaxFileCount int

	file *os.File
}

func (l *LoopFile) Write(p []byte) (n int, e error) {
	if l.file == nil {
		l.file, e = l.openFile()
		if e != nil {
			return
		}
	}
	n, e = l.file.Write(p)
	info, _ := l.file.Stat()
	if info.Size() >= l.MaxFileSize {
		l.file.Close()
		l.file = nil
	}
	return
}

type mystrings []string

func (a mystrings) Len() int {
	return len(a)
}
func (a mystrings) Less(i, j int) bool {
	left_begin := strings.Index(a[i], ".")
	right_begin := strings.Index(a[j], ".")

	left := a[i][0 : left_begin+1]
	right := a[j][0 : right_begin+1]
	if left == right {
		e := strings.LastIndex(a[i], ".")
		nl, _ := strconv.Atoi(a[i][left_begin+1 : e])

		e = strings.LastIndex(a[j], ".")
		nr, _ := strconv.Atoi(a[j][right_begin+1 : e])
		return nl < nr
	}

	return left < right
}
func (a mystrings) Swap(i, j int) {
	a[i], a[j] = a[j], a[i]
}

func (l *LoopFile) openFile() (f *os.File, e error) {
	d, err := os.Open(l.Directory)
	if err != nil {
		e = err
		return
	}
	infos, err := d.Readdir(0)
	if err != nil {
		e = err
		return
	}
	logfiles := make([]string, 0)
	for _, info := range infos {
		b, _ := regexp.MatchString("^[0-9]{1,4}-[0-9]{1,2}-[0-9]{1,2}\\.[0-9]+\\.log", info.Name())
		if !info.Mode().IsDir() && b {
			logfiles = append(logfiles, info.Name())
		}
	}
	sort.Sort(mystrings(logfiles))
	now := time.Now()
	name := fmt.Sprintf("%04d-%02d-%d.", now.Year(), now.Month(), now.Day())
	max := 0
	for _, info := range logfiles {
		if strings.HasPrefix(info, name) {
			tmp := info[len(name):]
			n := strings.Index(tmp, ".")
			if n != -1 {
				tmp = tmp[0:n]
			}
			n, e = strconv.Atoi(tmp)
			if e == nil {
				if n >= max {
					max = n + 1
				}
			}
		}
	}

	//創建新日誌文件
	name += strconv.Itoa(max) + ".log"
	directory := l.Directory
	if !strings.HasSuffix(l.Directory, "/") &&
		!strings.HasSuffix(l.Directory, "\\") {

		directory += "/"
	}
	f, e = os.Create(directory + name)
	if e == nil {
		count := len(logfiles) + 1
		if count > l.MaxFileCount {
			count -= l.MaxFileCount
			for i, d := range logfiles {
				if i >= count {
					break
				}
				os.Remove(directory + d)
			}
		}
	}
	return
}

func (l *LoopFile) Close() (e error) {
	if l.file != nil {
		e = l.file.Close()
	}
	return
}
