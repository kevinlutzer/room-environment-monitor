package logger

import (
	"log"
	"os"
)

type LoggerService interface {
	GetStdOut() *log.Logger
	StdOut(fmt string, opts ...interface{})
	StdErr(fmt string, opts ...interface{})
	StdWarn(fmt string, opts ...interface{})
	StdErrFatal(fmt string, opts ...interface{})
}

type logger struct {
	stdOut  *log.Logger
	stdErr  *log.Logger
	stdWarn *log.Logger
}

func NewLogger() LoggerService {
	return &logger{
		stdOut:  log.New(os.Stdout, "INFO: ", log.LstdFlags),
		stdErr:  log.New(os.Stderr, "ERROR: ", log.LstdFlags),
		stdWarn: log.New(os.Stdout, "WARNING: ", log.LstdFlags),
	}
}

func (l *logger) GetStdOut() *log.Logger {
	return l.stdOut
}

func (l *logger) StdOut(fmt string, opts ...interface{}) {
	l.stdOut.Printf(fmt, opts...)
}

func (l *logger) StdErr(fmt string, opts ...interface{}) {
	l.stdErr.Printf(fmt, opts...)
}

func (l *logger) StdWarn(fmt string, opts ...interface{}) {
	l.stdWarn.Printf(fmt, opts...)
}

func (l *logger) StdErrFatal(fmt string, opts ...interface{}) {
	l.stdErr.Fatalf(fmt, opts...)
}
