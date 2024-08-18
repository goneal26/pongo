package token

const (
	UNKNOWN = "UNKNOWN" // for unrecognized tokens
	ERROR = "ERROR" // returned if lexer encounters error parsing another token
	EOF = "EOF"

	// identifiers and literals
	IDENT = "IDENT"
	DECIMAL = "DECIMAL" // base-10 number literal
	HEX = "HEX" // base-16 number literal
	STRINGLIT = "STRINGLIT" // string literal
	
	// operators
	ASSIGN = "="
	PLUS = "+"
	MINUS = "-"
	TIMES = "*"
	DIVIDE = "/"
	MODULO = "%"
	BANG = "!"
	AND = "&"
	OR = "|"
  AT = "@" // like a "get" operator

  // comparison operators
	LT = "<"
	GT = ">"
	LE = "<="
	GE = ">="
	EQ = "=="
	NOT_EQ = "!="

	// delims
	SEMICOLON = ";"
	COMMA = ","
	LPAREN = "("
	RPAREN = ")"

	// keywords
	SHORT = "SHORT"
	SMASH = "SMASH"
	SMASHALL = "SMASHALL"
	GOTO = "GOTO"
	// GOBACK = "GOBACK"
	IF = "IF"
	EXIT = "EXIT"
	PRINT = "PRINT"
	PRINTLN = "PRINTLN"
	CLOCK = "CLOCK"
	INPUT = "INPUT"
	TRUE = "TRUE"
	FALSE = "FALSE"
	RAND = "RAND"
	LBL = "LBL"
  BUFF = "BUFF"
  SIZEOF = "SIZEOF"
  // OP = "OP"
  // NOTE: would I rather do pure-functions over ops?
)

type TokenType string

type Token struct {
	Type TokenType
	Literal string
	LineNumber int // store line number in token for error messages
}

var keywords = map[string]TokenType {
  "short": SHORT,
  "smash": SMASH,
  "smashall": SMASHALL,
  "goto": GOTO,
  "if": IF,
  // "goback": GOBACK,
  "exit": EXIT,
  "print": PRINT,
  "println": PRINTLN,
  "clock": CLOCK,
  "input": INPUT,
  "true": TRUE,
  "false": FALSE,
  "rand": RAND,
  "lbl": LBL,
  "buff": BUFF,
  "sizeof": SIZEOF,
  // "op": OP,
}

func LookupIdent(ident string) TokenType {
  if tok, ok := keywords[ident]; ok {
    return tok
  }
  return IDENT
}