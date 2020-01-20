package logger

import "log"

type LoggerService interface {
	GetStdOut() *log.Logger
	StdOut(fmt string, opts ...interface{})
	StdErr(fmt string, opts ...interface{})
	StdWarn(fmt string, opts ...interface{})
	StdErrFatal(fmt string, opts ...interface{})
}
