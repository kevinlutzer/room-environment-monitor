package config

import (
	"log"
	"os"
)

type Logger struct {
	StdOut  *log.Logger
	StdErr  *log.Logger
	StdWarn *log.Logger
}

func NewLogger() *Logger {
	return &Logger{
		StdOut:  log.New(os.Stdout, "INFO: ", log.LstdFlags),
		StdErr:  log.New(os.Stderr, "ERROR: ", log.LstdFlags),
		StdWarn: log.New(os.Stdout, "WARNING: ", log.LstdFlags),
	}
}
