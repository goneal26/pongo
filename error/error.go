package error

import (
  "os"
  "fmt"
)

// for errors that don't terminate the program (eg overflow)
func Warning(line int, format string, a ...interface{}) {
  msg := fmt.Sprintf(format, a...)
  fmt.Printf("Warning on line %d: %s.\n", line, msg)
}

// for errors that terminate the program (eg syntax)
func Fatal(line int, format string, a ...interface{}) {
  msg := fmt.Sprintf(format, a...)
  fmt.Printf("Error on line %d: %s.\n", line, msg)
  os.Exit(1)
}

// for debug messages
func Debug(format string, a ...interface{}) {
  msg := fmt.Sprintf(format, a...)
  fmt.Printf("DEBUG: %s.\n", msg)
}