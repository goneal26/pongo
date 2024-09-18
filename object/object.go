package object
// for implementing types

import (
  "fmt"
  "bytes"
)

type ObjectType string

type Object interface {
  Type() ObjectType
  Inspect() string
}

// all type names:
const (
  SHORT_OBJ = "SHORT"
  BUFF_OBJ = "BUFF"
  LBL_OBJ = "LBL"
)


// short datatype
type Short struct {
  Value int16
}
func (s *Short) Inspect() string { return fmt.Sprintf("%d", s.Value) }
func (s *Short) Type() ObjectType { return SHORT_OBJ }

// lbl datatype (basically a const short)
type Lbl struct {
  Value int16
}
func (l *Lbl) Inspect() string { return fmt.Sprintf("%d", l.Value) }
func (l *Lbl) Type() ObjectType { return LBL_OBJ }

type Buff struct {
  Value []Object 
  Size int16
}
func (b *Buff) Inspect() string { 
  var out bytes.Buffer

  out.WriteString("[")

  for i, v := range b.Value {
    out.WriteString(fmt.Sprintf("%s", v.Inspect()))
    
    if i < len(b.Value) - 1 {
      out.WriteString(" ")
    }
  }

  out.WriteString("]")

  return out.String()
}
func (b *Buff) Type() ObjectType { return BUFF_OBJ }
