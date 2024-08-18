package interpreter

import (
  "pongo/lexer"
  "pongo/parser"
  "pongo/evaluator"
  "pongo/object"
)

func Interpret(source string) {
  l := lexer.New(source)
  p := parser.New(l)
  env := object.NewEnvironment()

  program := p.ParseProgram()

  evaluator.Eval(program, env)
}