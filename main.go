package main

import (
  "fmt"
  "strings"
  "os"
  "time"
  "pongo/lexer"
  "pongo/parser"
  "pongo/evaluator"
  "pongo/object"
  "pongo/shell"
)

func main() {
  // verify correct num of command args
  argLength := len(os.Args)
  if argLength != 2 {
    /*
    fmt.Println("Error: Incorrect number of command arguments.")
    fmt.Println("Usage: pongo [filename]")
    os.Exit(1)
    */
    // testing shell/repl here for now, TODO proper shell init
    shell.Start(os.Stdin, os.Stdout)
  }

  // verify file extension is .pgo
  var filename string
  filename = os.Args[1]
  if !strings.HasSuffix(filename, ".pgo") {
    fmt.Println("Error: Unrecognized file type.")
    fmt.Println("Pongo only recognizes '.pgo' files.")
    os.Exit(1)
  }

  // now read the file
  b, err := os.ReadFile(filename)
  if err != nil {
    fmt.Println("Error: file not found.") // TODO better message
    os.Exit(1)
  }

  code := string(b)

  Interpret(code)
}

func Interpret(source string) {
  start := time.Now()
  l := lexer.New(source)
  p := parser.New(l)
  env := object.NewEnvironment()

  program := p.ParseProgram()

  evaluator.Eval(program, env, start)
}