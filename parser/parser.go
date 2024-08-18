package parser

import (
  "pongo/ast"
  "pongo/lexer"
  "pongo/token"
  "pongo/error"
  "strconv"
)

type (
  prefixParseFn func() ast.Expression
  infixParseFn func(ast.Expression) ast.Expression
)

const (
  _ int = iota
  LOWEST
  OR // bitwise or
  AND // bitwise and
  EQUALS // ==
  LESSGREATER // > or <
  SUM // +
  PRODUCT // *
  PREFIX // -X or !X or sizeof X
  CALL // add(x, y)
  AT // buff @ 3
)

var precedences = map[token.TokenType]int {

  token.EQ: EQUALS,
  token.NOT_EQ: EQUALS,
  token.LT: LESSGREATER,
  token.GT: LESSGREATER,
  token.LE: LESSGREATER,
  token.GE: LESSGREATER,
  token.PLUS: SUM,
  token.MINUS: SUM,
  token.TIMES: PRODUCT,
  token.DIVIDE: PRODUCT,
  token.MODULO: PRODUCT,
  token.AT: AT,
  token.AND: AND,
  token.OR: OR,
}

func (p *Parser) peekPrecedence() int {
  if p, ok := precedences[p.peekToken.Type]; ok {
    return p
  }

  return LOWEST
}

func (p *Parser) curPrecendence() int {
  if p, ok := precedences[p.curToken.Type]; ok {
    return p
  }

  return LOWEST
}

type Parser struct {
  l *lexer.Lexer
  errors []string
  curToken token.Token
  peekToken token.Token

  prefixParseFns map[token.TokenType]prefixParseFn
  infixParseFns map[token.TokenType]infixParseFn
}

func New(l *lexer.Lexer) *Parser {
  p := &Parser{l: l}

  // read 2 tokens to set curToken and peekToken
  p.nextToken()
  p.nextToken()

  p.prefixParseFns = make(map[token.TokenType]prefixParseFn)
  p.registerPrefix(token.IDENT, p.parseIdentifier)
  p.registerPrefix(token.DECIMAL, p.parseIntegerLiteral)
  p.registerPrefix(token.HEX, p.parseHexLiteral)
  p.registerPrefix(token.BANG, p.parsePrefixExpression)
  p.registerPrefix(token.MINUS, p.parsePrefixExpression)
  p.registerPrefix(token.PLUS, p.parsePrefixExpression)
  p.registerPrefix(token.SIZEOF, p.parsePrefixExpression)
  p.registerPrefix(token.TRUE, p.parseBoolean)
  p.registerPrefix(token.FALSE, p.parseBoolean)
  p.registerPrefix(token.RAND, p.parseRandom)
  p.registerPrefix(token.INPUT, p.parseInput)
  p.registerPrefix(token.LPAREN, p.parseGroupedExpression)

  p.infixParseFns = make(map[token.TokenType]infixParseFn)
  p.registerInfix(token.PLUS, p.parseInfixExpression)
  p.registerInfix(token.MINUS, p.parseInfixExpression)
  p.registerInfix(token.TIMES, p.parseInfixExpression)
  p.registerInfix(token.DIVIDE, p.parseInfixExpression)
  p.registerInfix(token.MODULO, p.parseInfixExpression)
  p.registerInfix(token.EQ, p.parseInfixExpression)
  p.registerInfix(token.NOT_EQ, p.parseInfixExpression)
  p.registerInfix(token.LT, p.parseInfixExpression)
  p.registerInfix(token.GT, p.parseInfixExpression)
  p.registerInfix(token.LE, p.parseInfixExpression)
  p.registerInfix(token.GE, p.parseInfixExpression)
  p.registerInfix(token.AND, p.parseInfixExpression)
  p.registerInfix(token.OR, p.parseInfixExpression)
  p.registerInfix(token.AT, p.parseInfixExpression)

  return p
}

func (p *Parser) peekError(t token.TokenType) {
  error.Fatal(p.peekToken.LineNumber, 
    "parser expected next token to be %s, got %s instead", t, p.peekToken.Type)
}

func (p *Parser) registerPrefix(tokenType token.TokenType, fn prefixParseFn) {
  p.prefixParseFns[tokenType] = fn
}

func (p *Parser) registerInfix(tokenType token.TokenType, fn infixParseFn) {
  p.infixParseFns[tokenType] = fn
}

func (p *Parser) nextToken() {
  p.curToken = p.peekToken
  p.peekToken = p.l.NextToken()
}

func (p *Parser) curTokenIs(t token.TokenType) bool {
  return p.curToken.Type == t
}

func (p *Parser) peekTokenIs(t token.TokenType) bool {
  return p.peekToken.Type == t
}

func (p *Parser) expectPeek(t token.TokenType) bool {
  if p.peekTokenIs(t) {
    p.nextToken()
    return true
  } else {
    p.peekError(t)
    return false
  }
}

func (p *Parser) ParseProgram() *ast.Program {
  program := &ast.Program{}

  program.Statements = []ast.Statement{}

  for p.curToken.Type != token.EOF {
    stmt := p.parseStatement()
    if stmt != nil {
      program.Statements = append(program.Statements, stmt)
    }
    p.nextToken()
  }

  return program
}

func (p *Parser) parseStatement() ast.Statement {
  switch p.curToken.Type {
    case token.SHORT:
      return p.parseShortStatement()
    case token.BUFF:
      return p.parseBuffStatement()
    case token.LBL:
      return p.parseLabelStatement()
    case token.GOTO:
      return p.parseGotoStatement()
    case token.SMASH:
      return p.parseSmashStatement()
    case token.CLOCK:
      return p.parseClockStatement()
    case token.EXIT:
      return p.parseExitStatement()
    case token.SMASHALL:
      return p.parseSmashallStatement()
    case token.PRINT:
      return p.parsePrintStatement()
    case token.PRINTLN:
      return p.parsePrintlnStatement()
    case token.IF:
      return p.parseIfgotoStatement()
    case token.IDENT:
      return p.parseAssignStatement()
    default:
      error.Fatal(p.curToken.LineNumber, 
        "unrecognized start of program statement \"%s\"", p.curToken.Literal)
      return nil
  }
}

func (p *Parser) parseIfgotoStatement() *ast.IfgotoStatement {
  stmt := &ast.IfgotoStatement{Token: p.curToken}

  p.nextToken()

  stmt.Condition = p.parseExpression(LOWEST)

  if !p.expectPeek(token.GOTO) {
    return nil
  }

  if !p.expectPeek(token.IDENT) {
    return nil
  }

  stmt.Destination = &ast.Identifier{Token: p.curToken, Value: p.curToken.Literal}

  if p.peekTokenIs(token.SEMICOLON) {
    p.nextToken()
  } else {
    error.Fatal(p.curToken.LineNumber, "missing semicolon")
  }

  return stmt
}

func (p *Parser) parseAssignStatement() *ast.AssignStatement {
  stmt := &ast.AssignStatement{}

  stmt.Name = &ast.Identifier{Token: p.curToken, Value: p.curToken.Literal}

  if p.peekTokenIs(token.AT) {
    stmt.HasAt = true
    p.nextToken()
    p.nextToken()
    stmt.Index = p.parseExpression(LOWEST)
  } else {
    stmt.HasAt = false
  }

  if !p.expectPeek(token.ASSIGN) {
    return nil
  }

  stmt.Token = p.curToken

  p.nextToken()

  stmt.Value = p.parseExpression(LOWEST)

  if p.peekTokenIs(token.SEMICOLON) {
    p.nextToken()
  } else {
    error.Fatal(p.curToken.LineNumber, "missing semicolon")
  }

  return stmt
}

func (p *Parser) parseShortStatement() *ast.ShortStatement {
  stmt := &ast.ShortStatement{Token: p.curToken}

  if !p.expectPeek(token.IDENT) {
    return nil
  }

  stmt.Name = &ast.Identifier{Token: p.curToken, Value: p.curToken.Literal}

  // making optional assign in short statement
  if p.peekTokenIs(token.ASSIGN) { // "short x = expr"
    p.nextToken()
    p.nextToken() // weird to consume two tokens
    stmt.Value = p.parseExpression(LOWEST)
  }

  if p.peekTokenIs(token.SEMICOLON) {
    p.nextToken()
  } else {
    error.Fatal(p.curToken.LineNumber, "missing semicolon")
  }

  return stmt
}

func (p *Parser) parseBuffStatement() *ast.BuffStatement {
  stmt := &ast.BuffStatement{Token: p.curToken}

  if !p.expectPeek(token.IDENT) {
    return nil
  }

  stmt.Name = &ast.Identifier{Token: p.curToken, Value: p.curToken.Literal}

  // buff statements require a buffer size (i.e. "buff name = expression;")
  // and the "= expression" is required
  if !p.expectPeek(token.ASSIGN) {
    return nil
  }

  p.nextToken()

  stmt.Value = p.parseExpression(LOWEST)

  if p.peekTokenIs(token.SEMICOLON) {
    p.nextToken()
  } else {
    error.Fatal(p.curToken.LineNumber, "missing semicolon")
  }

  return stmt
}

func (p *Parser) parseLabelStatement() *ast.LblStatement {
  stmt := &ast.LblStatement{Token: p.curToken}

  if !p.expectPeek(token.IDENT) {
    return nil
  }

  stmt.Name = &ast.Identifier{Token: p.curToken, Value: p.curToken.Literal}

  if p.peekTokenIs(token.ASSIGN) { // lbl NAME = int;
    p.nextToken()
    p.nextToken()

    // lbls can only be int or hex literals
    if p.curTokenIs(token.DECIMAL) {
      stmt.Value = p.parseIntegerLiteral()
    } else if p.curTokenIs(token.HEX) {
      stmt.Value = p.parseHexLiteral()
    } else { // invalid lbl assignment
      p.peekError(token.DECIMAL)
      return nil
    }
  }

  if p.peekTokenIs(token.SEMICOLON) {
    p.nextToken()
  } else {
    error.Fatal(p.curToken.LineNumber, "missing semicolon")
  }

  return stmt
}

func (p *Parser) parseGotoStatement() *ast.GotoStatement {
  stmt := &ast.GotoStatement{Token: p.curToken}

  if !p.expectPeek(token.IDENT) {
    return nil
  }

  stmt.Destination = &ast.Identifier{Token: p.curToken, Value: p.curToken.Literal}

  if p.peekTokenIs(token.SEMICOLON) {
    p.nextToken()
  } else {
    error.Fatal(p.curToken.LineNumber, "missing semicolon")
  }

  return stmt
}

func (p *Parser) parseSmashStatement() *ast.SmashStatement {
  stmt := &ast.SmashStatement{Token: p.curToken}

  if !p.expectPeek(token.IDENT) {
    return nil
  }

  stmt.Name = &ast.Identifier{Token: p.curToken, Value: p.curToken.Literal}

  if p.peekTokenIs(token.SEMICOLON) {
    p.nextToken()
  } else {
    error.Fatal(p.curToken.LineNumber, "missing semicolon")
  }

  return stmt
}

func (p *Parser) parsePrintStatement() *ast.PrintStatement {
  stmt := &ast.PrintStatement{Token: p.curToken}

  p.nextToken()

  if p.curTokenIs(token.STRINGLIT) {
    stmt.Message = p.curToken.Literal
  } else {
    stmt.Value = p.parseExpression(LOWEST)
  }

  if stmt.Message == "" && stmt.Value == nil { // missing argument err
    return nil
  }

  if p.peekTokenIs(token.SEMICOLON) {
    p.nextToken()
  } else {
    error.Fatal(p.curToken.LineNumber, "missing semicolon")
  }

  return stmt
}

func (p *Parser) parsePrintlnStatement() *ast.PrintlnStatement {
  stmt := &ast.PrintlnStatement{Token: p.curToken}

  // p.nextToken()

  if p.peekTokenIs(token.STRINGLIT) { // print string literal
    p.nextToken()
    stmt.Message = p.curToken.Literal
  } else if p.peekTokenIs(token.SEMICOLON) { // end of statement, print newline
    p.nextToken()
    // p.nextToken()
    return stmt
  } else {
    p.nextToken()
    stmt.Value = p.parseExpression(LOWEST)
  }

  if p.peekTokenIs(token.SEMICOLON) {
    p.nextToken()
  } else {
    error.Fatal(p.curToken.LineNumber, "missing semicolon")
  }

  return stmt
}

func (p *Parser) parseClockStatement() *ast.ClockStatement {
  stmt := &ast.ClockStatement{Token: p.curToken}

  // p.nextToken()

  if p.peekTokenIs(token.SEMICOLON) {
    p.nextToken()
  } else {
    error.Fatal(p.curToken.LineNumber, "missing semicolon")
  }

  return stmt
}

func (p *Parser) parseExitStatement() *ast.ExitStatement {
  stmt := &ast.ExitStatement{Token: p.curToken}

  // p.nextToken()

  if p.peekTokenIs(token.SEMICOLON) {
    p.nextToken()
  } else {
    error.Fatal(p.curToken.LineNumber, "missing semicolon")
  }

  return stmt
}

func (p *Parser) parseSmashallStatement() *ast.SmashallStatement {
  stmt := &ast.SmashallStatement{Token: p.curToken}

  // p.nextToken()

  if p.peekTokenIs(token.SEMICOLON) {
    p.nextToken()
  } else {
    error.Fatal(p.curToken.LineNumber, "missing semicolon")
  }

  return stmt
}

// EXPRESSION PARSING BELOW:

func (p *Parser) parseGroupedExpression() ast.Expression {
  p.nextToken()

  exp := p.parseExpression(LOWEST)

  if !p.expectPeek(token.RPAREN) {
    return nil
  }

  return exp
}

func (p *Parser) parseExpression(precedence int) ast.Expression {
  prefix := p.prefixParseFns[p.curToken.Type]
  if prefix == nil {
    p.noPrefixParseError(p.curToken.Type)
    return nil
  }
  leftExp := prefix()

  for !p.peekTokenIs(token.SEMICOLON) && precedence < p.peekPrecedence() {
    infix := p.infixParseFns[p.peekToken.Type]
    if infix == nil {
      return leftExp
    }

    p.nextToken()

    leftExp = infix(leftExp)
  }

  return leftExp
}

func (p *Parser) parseIdentifier() ast.Expression {
  return &ast.Identifier{Token: p.curToken, Value: p.curToken.Literal}
}

func (p *Parser) parseIntegerLiteral() ast.Expression {
  lit := &ast.IntegerLiteral{Token: p.curToken}

  value64, err := strconv.ParseInt(p.curToken.Literal, 0, 16)
  if err != nil {
    error.Fatal(p.curToken.LineNumber, "could not parse %q as 16-bit integer", p.curToken.Literal)
    return nil
  }
  lit.Value = int16(value64)
  
  return lit
}

func (p *Parser) parseHexLiteral() ast.Expression {
  lit := &ast.IntegerLiteral{Token: p.curToken}

  value64, err := strconv.ParseInt(p.curToken.Literal, 16, 32)
  // the weird conversions bypass ParseInt's range, so that you can
  // input "0xffff" and get -1 as an int16
  if err != nil || value64 > 65535 {
    // the size check for value64 prevents overflows that could result
    // from bypassing ParseInt's range
    error.Fatal(p.curToken.LineNumber, "could not parse %q as 16-bit integer", p.curToken.Literal)
    return nil
  }
  lit.Value = int16(value64)
  
  return lit
}

func (p *Parser) parseRandom() ast.Expression {
  return &ast.RandKeyword{Token: p.curToken}
}

func (p *Parser) parseInput() ast.Expression {
  return &ast.InputKeyword{Token: p.curToken}
}

func (p *Parser) parseBoolean() ast.Expression {
  return &ast.Boolean{Token: p.curToken, Value: p.curTokenIs(token.TRUE)}
}

func (p *Parser) noPrefixParseError(t token.TokenType) {
  error.Fatal(p.curToken.LineNumber, "no prefix parse function for %s found", t)
}

func (p *Parser) parsePrefixExpression() ast.Expression {
  expression := &ast.PrefixExpression{
    Token: p.curToken,
    Operator: p.curToken.Literal,
  }

  p.nextToken()

  expression.Right = p.parseExpression(PREFIX)

  return expression
}

func (p *Parser) parseInfixExpression(left ast.Expression) ast.Expression {
  expression := &ast.InfixExpression{
    Token: p.curToken,
    Operator: p.curToken.Literal,
    Left: left,
  }

  precedence := p.curPrecendence()
  p.nextToken()
  expression.Right = p.parseExpression(precedence)

  return expression
}



