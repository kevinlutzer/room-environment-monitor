package config

import (
	"log"
	"os"
)

type LoggerService interface {
	StdOut(fmt string, opts ...interface{})
	StdErr(fmt string, opts ...interface{})
	StdWarn(fmt string, opts ...interface{})
	GetStdOut() *log.Logger
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
	l.stdOut.Printf(fmt, opts...)
}

func (l *logger) StdWarn(fmt string, opts ...interface{}) {
	l.stdOut.Printf(fmt, opts...)
}
