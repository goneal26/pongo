package ast

import (
  "pongo/token"
  "bytes"
  "strconv" // for debug purposes (IntegerLiterals)
)


/*
AST datatypes for parsing
*/

type Node interface {
  TokenLiteral() string
  String() string
}

type Statement interface {
  Node
  statementNode()
}

type Expression interface {
  Node
  expressionNode()
}

type Program struct {
  Statements []Statement
  // Labels []LblStatement
}

// returns first statement string in program
func (p *Program) TokenLiteral() string {
  if len(p.Statements) > 0 {
    return p.Statements[0].TokenLiteral()
  } else {
    return ""
  }
}

// returns string for every statement in program
func (p *Program) String() string {
  var out bytes.Buffer

  for _, s := range p.Statements {
    out.WriteString(s.String())
  }

  return out.String()
}

type AssignStatement struct { // identifier (@ index?) = expression;
  Token token.Token // token.ASSIGN
  Name *Identifier
  HasAt bool // tracks if index is null (marked with presence of "@")
  Index Expression
  Value Expression
}

func (as *AssignStatement) statementNode() {}
func (as *AssignStatement) TokenLiteral() string { return as.Token.Literal }
func (as *AssignStatement) String() string {
  var out bytes.Buffer

  out.WriteString(as.Name.String())
  if as.HasAt {
    out.WriteString(" @ " + as.Index.String())
  }
  out.WriteString(" " + as.TokenLiteral() + " " + as.Value.String())

  out.WriteString(";")

  return out.String()
}


type ShortStatement struct { // short identifier (= expression);
  Token token.Token // token.SHORT
  Name *Identifier
  Value Expression // optional/nullable expression 
}

func (ss *ShortStatement) statementNode() {}
func (ss *ShortStatement) TokenLiteral() string { return ss.Token.Literal }
func (ss *ShortStatement) String() string {
  var out bytes.Buffer

  out.WriteString(ss.TokenLiteral() + " ")
  out.WriteString(ss.Name.String())

  if ss.Value != nil {
    out.WriteString(" = ")
    out.WriteString(ss.Value.String())
  }

  out.WriteString(";")

  return out.String()
}

type BuffStatement struct {
  Token token.Token // token.BUFF
  Name *Identifier
  Value Expression
}

func (bs *BuffStatement) statementNode() {}
func (bs *BuffStatement) TokenLiteral() string { return bs.Token.Literal }
func (bs *BuffStatement) String() string {
  var out bytes.Buffer

  out.WriteString(bs.TokenLiteral() + " ")
  out.WriteString(bs.Name.String())
  out.WriteString(" = ")

  if bs.Value != nil {
    out.WriteString(bs.Value.String())
  }

  out.WriteString(";")

  return out.String()
}


type LblStatement struct { // lbl MAIN;
  Token token.Token // token.LBL
  Name *Identifier
  Value Expression // optional/nullable
  // ^ value can only be int/hex literal according to parse function!
}

func (ls *LblStatement) statementNode() {}
func (ls *LblStatement) TokenLiteral() string { return ls.Token.Literal }
func (ls *LblStatement) String() string {
  var out bytes.Buffer

  out.WriteString(ls.TokenLiteral() + " ")
  out.WriteString(ls.Name.String())

  if ls.Value != nil {
    out.WriteString(" = ")
    out.WriteString(ls.Value.String())
  }

  out.WriteString(";")

  return out.String()
}

type PrintStatement struct {
  Token token.Token // token.PRINT
  Value Expression
  Message string // nullable, either prints expression or string literal
}

func (ps *PrintStatement) statementNode() {}
func (ps *PrintStatement) TokenLiteral() string { return ps.Token.Literal }
func (ps *PrintStatement) String() string {
  var out bytes.Buffer

  out.WriteString(ps.TokenLiteral() + " ")
  if ps.Message == "" {
    out.WriteString(ps.Value.String())
  } else {
    out.WriteString("\"" + ps.Message + "\"")
  }

  out.WriteString(";")

  return out.String()
}

type PrintlnStatement struct {
  Token token.Token // token.PRINTLN
  Value Expression
  Message string // both the expression and string can be nullable
}

func (pls *PrintlnStatement) statementNode() {}
func (pls *PrintlnStatement) TokenLiteral() string { return pls.Token.Literal }
func (pls *PrintlnStatement) String() string {
  var out bytes.Buffer

  out.WriteString(pls.TokenLiteral())
  if pls.Message != "" { // no string literal
    out.WriteString(" \"" + pls.Message + "\"")
  }
  if pls.Value != nil {
    out.WriteString(" " + pls.Value.String())
  }

  out.WriteString(";")

  return out.String()
}

type IfgotoStatement struct {
  Token token.Token // token.IF
  Condition Expression
  Destination *Identifier
}

func (igs *IfgotoStatement) statementNode() {}
func (igs *IfgotoStatement) TokenLiteral() string { return igs.Token.Literal }
func (igs *IfgotoStatement) String() string {
  var out bytes.Buffer
  
  out.WriteString(igs.TokenLiteral() + " ")
  out.WriteString(igs.Condition.String() + " ")
  out.WriteString("goto " + igs.Destination.String())

  out.WriteString(";")

  return out.String()
}

type GotoStatement struct {
  Token token.Token // token.GOTO
  Destination *Identifier // label name
}

func (gs *GotoStatement) statementNode() {}
func (gs *GotoStatement) TokenLiteral() string { return gs.Token.Literal }
func (gs *GotoStatement) String() string {
  var out bytes.Buffer

  out.WriteString(gs.TokenLiteral() + " ")
  out.WriteString(gs.Destination.String())

  out.WriteString(";")

  return out.String()
}

type SmashStatement struct {
  Token token.Token // token.SMASH
  Name *Identifier // var name
}

func (sms *SmashStatement) statementNode() {}
func (sms *SmashStatement) TokenLiteral() string { return sms.Token.Literal }
func (sms *SmashStatement) String() string {
  var out bytes.Buffer

  out.WriteString(sms.TokenLiteral() + " ")
  out.WriteString(sms.Name.String())

  out.WriteString(";")

  return out.String()
}

type ClockStatement struct {
  Token token.Token // token.CLOCK
}

func (cs *ClockStatement) statementNode() {}
func (cs *ClockStatement) TokenLiteral() string { return cs.Token.Literal }
func (cs *ClockStatement) String() string {
  var out bytes.Buffer

  out.WriteString(cs.TokenLiteral())

  out.WriteString(";")

  return out.String()
}

type ExitStatement struct {
  Token token.Token // token.EXIT
}

func (es *ExitStatement) statementNode() {}
func (es *ExitStatement) TokenLiteral() string { return es.Token.Literal }
func (es *ExitStatement) String() string {
  var out bytes.Buffer

  out.WriteString(es.TokenLiteral())

  out.WriteString(";")

  return out.String()
}

type SmashallStatement struct {
  Token token.Token // token.SMASHALL
}

func (sa *SmashallStatement) statementNode() {}
func (sa *SmashallStatement) TokenLiteral() string { return sa.Token.Literal }
func (sa *SmashallStatement) String() string {
  var out bytes.Buffer

  out.WriteString(sa.TokenLiteral())

  out.WriteString(";")

  return out.String()
}

type Identifier struct {
  Token token.Token // the token.IDENT token
  Value string
}

func (i *Identifier) expressionNode() {}
func (i *Identifier) TokenLiteral() string { return i.Token.Literal }
func (i *Identifier) String() string { return i.Value }


type IntegerLiteral struct {
  Token token.Token
  Value int16 // pongo uses shorts
}

func (il *IntegerLiteral) expressionNode() {}
func (il *IntegerLiteral) TokenLiteral() string { return il.Token.Literal }
func (il *IntegerLiteral) String() string {
 // return il.Token.Literal
 // printing int16 value as string for debugging:
 t := strconv.Itoa(int(il.Value))
 return t
}

type Boolean struct {
  Token token.Token
  Value bool 
  // pongo bools get stored as bools during parsing, but converted to
  // int16s during evaluation
}

func (b *Boolean) expressionNode() {}
func (b *Boolean) TokenLiteral() string { return b.Token.Literal }
func (b *Boolean) String() string { return b.Token.Literal }

// support for the rand keyword- has the same precedence as an int literal,
// upon usage is evaluated to a random 16-bit int, seeded at the start of the
// program
type RandKeyword struct {
  Token token.Token
}

func (r *RandKeyword) expressionNode() {}
func (r *RandKeyword) TokenLiteral() string { return r.Token.Literal }
func (r *RandKeyword) String() string { return r.Token.Literal }

// similar to rand, upon usage is evaluated to a 16-bit int that the user inputs
type InputKeyword struct {
  Token token.Token
}

func (ip *InputKeyword) expressionNode() {}
func (ip *InputKeyword) TokenLiteral() string { return ip.Token.Literal }
func (ip *InputKeyword) String() string { return ip.Token.Literal }

type PrefixExpression struct {
  Token token.Token // prefix token (!, -, +)
  Operator string
  Right Expression
}

func (pe *PrefixExpression) expressionNode() {}
func (pe *PrefixExpression) TokenLiteral() string { return pe.Token.Literal }
func (pe *PrefixExpression) String() string {
  var out bytes.Buffer

  out.WriteString("(")
  out.WriteString(pe.Operator)
  out.WriteString(pe.Right.String())
  out.WriteString(")")

  return out.String()
}

type InfixExpression struct {
  Token token.Token // binary operator token
  Left Expression
  Operator string
  Right Expression
}

func (oe *InfixExpression) expressionNode() {}
func (oe *InfixExpression) TokenLiteral() string { return oe.Token.Literal }
func (oe *InfixExpression) String() string {
  var out bytes.Buffer

  out.WriteString("(")
  out.WriteString(oe.Left.String())
  out.WriteString(" " + oe.Operator + " ")
  out.WriteString(oe.Right.String())
  out.WriteString(")")

  return out.String()
}