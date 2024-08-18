package lexer

import (
  "pongo/token"
  "pongo/error"
)

type Lexer struct {
  input string
  position int // current position in input (points to current char)
  readPosition int // current reading pos in input (after current char)
  ch byte // current char
  currentLineNumber int// increment for every newline
}

// global vars for tracking comments and quotations for string literals
var seenComment bool = false // reset upon seeing newline
var seenQuote bool = false // reset upon seeing closing quote

func New(input string) *Lexer {
  l := &Lexer{input: input, currentLineNumber: 1}
  l.readChar()
  return l
}

func (l *Lexer) readChar() {
  if l.readPosition >= len(l.input) {
    l.ch = 0
  } else {
    l.ch = l.input[l.readPosition]

  }

  if l.ch == '#' {
    seenComment = true
  }

  if l.ch == '\n' {
    l.currentLineNumber = l.currentLineNumber + 1
  }

  if l.ch == '\n' || l.ch == 0 {
    // added 0 so that pressing enter in repl resets the flag,
    // otherwise infinite loop in skipWhitespace()
    seenComment = false
  }

  l.position = l.readPosition
  l.readPosition += 1
}

func (l *Lexer) NextToken() token.Token {
  var tok token.Token

  l.skipWhitespace()

  switch l.ch {
    case '"':
      l.readChar() // consume first quote
      literal, hadError := l.readStringLiteral()
      if hadError {
        tok = token.Token{Type: token.ERROR, Literal: "Unterminated String"}
        error.Fatal(l.currentLineNumber, "unterminated string literal")
      } else {
        tok = token.Token{Type: token.STRINGLIT, Literal: literal}
      }
    case '=':
      if l.peekChar() == '=' {
        ch := l.ch
        l.readChar()
        tok = token.Token{Type: token.EQ, Literal: string(ch) + string(l.ch)}
      } else {
        tok = newToken(token.ASSIGN, l.ch)
      }
    case '!':
      if l.peekChar() == '=' {
        ch := l.ch
        l.readChar()
        tok = token.Token{Type: token.NOT_EQ, Literal: string(ch) + string(l.ch)}
      } else {
        tok = newToken(token.BANG, l.ch)
      }
    case '>':
      if l.peekChar() == '=' {
        ch := l.ch
        l.readChar()
        tok = token.Token{Type: token.GE, Literal: string(ch) + string(l.ch)}
      } else {
        tok = newToken(token.GT, l.ch)
      }
    case '<':
      if l.peekChar() == '=' {
        ch := l.ch
        l.readChar()
        tok = token.Token{Type: token.LE, Literal: string(ch) + string(l.ch)}
      } else {
        tok = newToken(token.LT, l.ch)
      }
    case ';':
      tok = newToken(token.SEMICOLON, l.ch)
    case ',':
      tok = newToken(token.COMMA, l.ch)
    case '(':
      tok = newToken(token.LPAREN, l.ch)
    case ')':
      tok = newToken(token.RPAREN, l.ch)
    case '+':
      tok = newToken(token.PLUS, l.ch)
    case '*':
      tok = newToken(token.TIMES, l.ch)
    case '/':
      tok = newToken(token.DIVIDE, l.ch)
    case '-':
      tok = newToken(token.MINUS, l.ch)
    case '&':
      tok = newToken(token.AND, l.ch)
    case '@':
      tok = newToken(token.AT, l.ch)
    case '%':
      tok = newToken(token.MODULO, l.ch)
    case '|':
      tok = newToken(token.OR, l.ch)
    case 0:
      tok.Literal = ""
      tok.Type = token.EOF
    default:
      if isLetter(l.ch) {
        tok.Literal = l.readIdentifier()
        tok.Type = token.LookupIdent(tok.Literal)
        tok.LineNumber = l.currentLineNumber // update line number
        return tok
      } else if isDigit(l.ch) {
        if l.ch == '0' && (l.peekChar() == 'x' || l.peekChar() == 'X') { 
          // is hex literal
          l.readChar()
          l.readChar() // consume twice, for the 0 and x
          tok.Type = token.HEX
          tok.Literal = l.readHex()
        } else {
          tok.Type = token.DECIMAL // is decimal literal
          tok.Literal = l.readNumber()
        }
        tok.LineNumber = l.currentLineNumber // update line number
        return tok
      } else {
        tok = newToken(token.UNKNOWN, l.ch)
        error.Fatal(l.currentLineNumber, "unrecognized token '%s'", tok.Literal)
      }
  }

  tok.LineNumber = l.currentLineNumber // update line number

  l.readChar()
  return tok
}

func (l *Lexer) peekChar() byte {
  if l.readPosition >= len(l.input) {
    return 0
  } else {
    return l.input[l.readPosition]
  }
}

func (l *Lexer) skipWhitespace() { // also skips comments
  for l.ch == ' ' || l.ch == '\t' || l.ch == '\n' || l.ch == '\r' || seenComment {
    l.readChar()
  }
}

func (l *Lexer) readStringLiteral() (string, bool) {
  position := l.position
  l.readChar()
  for l.ch != '"' { // iterate until closing quote
    l.readChar()
    if l.ch == '\n' {
      return "", true
    }
  }
  return l.input[position:l.position], false
}

func (l *Lexer) readIdentifier() string {
  position := l.position
  for isLetter(l.ch) {
    l.readChar()
  }
  return l.input[position:l.position]
}

func (l *Lexer) readNumber() string {
  position := l.position
  for isDigit(l.ch) {
    l.readChar()
  }
  return l.input[position:l.position]
}

func (l *Lexer) readHex() string {
  position := l.position
  for isHexDigit(l.ch) {
    l.readChar()
  }

  return l.input[position:l.position]
}

func isDigit(ch byte) bool {
  return '0' <= ch && ch <= '9'
}

func isHexDigit(ch byte) bool {
  return isDigit(ch) || ('a' <= ch && ch <= 'f') || ('A' <= ch && ch <= 'F')
}

func isLetter(ch byte) bool {
  return 'a' <= ch && ch <= 'z' || 'A' <= ch && ch <= 'Z' || ch == '_'
}

func newToken(tokenType token.TokenType, ch byte) token.Token {
  return token.Token{Type: tokenType, Literal: string(ch)}
}