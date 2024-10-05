package shell

import (
  "fmt"
  "strings"
)

type shell struct {
  source string
  lineNumber uint // current line the shell is editing
  lastLine uint // if you were to append a line, it'd be at this number
  appendMode bool // if true, appending lines to end of src
}

func isCommand(src string) bool {
  return strings.HasPrefix(strings.Trim(src, WHITESPACE), PREFIX)
}

func initShell() *shell {
  return &shell{lineNumber: 0, lastLine: 1, appendMode: true}
}

func (s *shell) handle(cmd string) {
  if isCommand(cmd) {
    fmt.Printf("COMMAND <%s>\n", cmd)
  } else {
    s.lineNumber++
  }
}

func (s shell) prompt() string {
  if s.appendMode {
    return " %d} "
  } else {
    return " %d| "
  }
}