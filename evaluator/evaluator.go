package evaluator

import (
  "pongo/ast"
  "pongo/object"
  "pongo/error"
  "fmt"
  "time"
  "math/rand"
)

var (
  TRUE = &object.Short{Value: -1}
  FALSE = &object.Short{Value: 0}
)

var pc int = 0 // program counter (really it's a "statement counter")

func Eval(node ast.Node, env *object.Environment, start time.Time) object.Object {
  switch node := node.(type) {
    
    // for evaluating individual statements
    case *ast.BuffStatement:
      var length object.Object
      length = Eval(node.Value, env, start) // get length of new buff

      if length.(*object.Short).Value < 1 { // invalid length
        error.Fatal(node.Token.LineNumber, 
          "Cannot create buff of length %d (is less than 1)", 
          length.(*object.Short).Value)
      }

      var newbuff object.Object
      newbuff = &object.Buff{Size: length.(*object.Short).Value}

      for i := 0; i < int(length.(*object.Short).Value); i++ {
        newbuff.(*object.Buff).Elements = append(newbuff.(*object.Buff).Elements, 0)
      }

      if !env.Add(node.Name.Value, newbuff) {
        error.Fatal(node.Token.LineNumber, "variable %s already declared",
          node.Name.Value)
      }
      return nil

    case *ast.ExitStatement:
      return nil

    case *ast.PrintlnStatement:
      if node.Message != "" {
        fmt.Println(node.Message)
        return nil
      }
      if node.Value != nil {
        var val object.Object
        val = Eval(node.Value, env, start)
        fmt.Println(val.Inspect())
        return nil
      }
      fmt.Println()
      return nil

    case *ast.PrintStatement:
      if node.Message != "" {
        fmt.Print(node.Message)
        return nil
      }
      if node.Value != nil {
        var val object.Object
        val = Eval(node.Value, env, start)
        fmt.Print(val.Inspect())
        return nil
      }
      return nil

    case *ast.SmashStatement:
      if !env.Remove(node.Name.Value) {
        error.Fatal(node.Token.LineNumber, 
          "cannot smash variable %s (not found/is a label)", node.Name.Value)
      }
      return nil

    case *ast.SmashallStatement:
      env.RemoveAll()
      return nil

    case *ast.ClockStatement:
      t := time.Now()
      fmt.Printf("%f\n", t.Sub(start).Seconds())
      return nil

    case *ast.IfgotoStatement:
      condition := Eval(node.Condition, env, start)
      if condition == TRUE || condition.(*object.Short).Value == -1 {
        val, ok := env.Get(node.Destination.Value)
        if !ok {
          error.Fatal(node.Token.LineNumber, "variable %s not found", 
            node.Destination.Value)
        }
        if val.Type() != object.LBL_OBJ {
          error.Fatal(node.Token.LineNumber, 
            "IFGOTO destination \"%s\" is not a label (instead is type %s)", 
            node.Destination.Value, val.Type())
        }
        if val.(*object.Lbl).Value < 0 {
          error.Fatal(node.Token.LineNumber, 
            "IFGOTO destination \"%s\" value %d is not positive", 
            node.Destination.Value, val.(*object.Lbl).Value)
        } else {
          pc = int(val.(*object.Lbl).Value)
        }
      }
      return nil

    case *ast.GotoStatement:
      val, ok := env.Get(node.Destination.Value)
      if !ok {
        error.Fatal(node.Token.LineNumber, "variable %s not found", 
          node.Destination.Value)
      }
      if val.Type() != object.LBL_OBJ {
        error.Fatal(node.Token.LineNumber, 
          "GOTO destination \"%s\" is not a label (instead is type %s)", 
          node.Destination.Value, val.Type())
      }
      if val.(*object.Lbl).Value < 0 {
        error.Fatal(node.Token.LineNumber, 
          "GOTO destination \"%s\" value %d is not positive", 
          node.Destination.Value, val.(*object.Lbl).Value)
      } else {
        pc = int(val.(*object.Lbl).Value)
      }
      return nil

    case *ast.LblStatement:
      var val object.Object
      if node.Value != nil {
        temp := Eval(node.Value, env, start)
        if temp.(*object.Short).Value < 0 {
          error.Fatal(node.Token.LineNumber, "cannot assign negative value to label (must be within 0 or 32767)")
        }
        val = &object.Lbl{Value: temp.(*object.Short).Value}
      } else {
        val = &object.Lbl{Value: int16(pc)}
      }
      if !env.Add(node.Name.Value, val) {
        error.Fatal(node.Token.LineNumber, "cannot reassign label %s", node.Name.Value)
      }
      return nil

    case *ast.ShortStatement:
      var val object.Object
      if node.Value != nil {
        val = Eval(node.Value, env, start)
      } else {
        val = FALSE
      }
      if !env.Add(node.Name.Value, val) {
        error.Fatal(node.Token.LineNumber, "variable %s already declared", node.Name.Value)
      }
      return nil

    case *ast.AssignStatement:
      if node.HasAt {
        // see what type the name is
        name, ok := env.Get(node.Name.Value)
        if !ok {
          error.Fatal(node.Token.LineNumber, "variable %s not found", node.Value)
        }

        if name.Type() == object.LBL_OBJ { // can't do this for labels!
          error.Fatal(node.Token.LineNumber, "variable %s is a label and is immutable", node.Name.Value)
        }

        // get the index now
        index := Eval(node.Index, env, start)

        if name.Type() == object.BUFF_OBJ {
          // out of bounds error
          if index.(*object.Short).Value < 0 || index.(*object.Short).Value >= name.(*object.Buff).Size {
            error.Fatal(node.Token.LineNumber, "index %d is out of bounds for BUFF %s", 
              index.(*object.Short).Value, node.Name.Value)
          }

          val := Eval(node.Value, env, start)
          name.(*object.Buff).Elements[index.(*object.Short).Value] = val.(*object.Short).Value
          return nil
        }

        if name.Type() == object.SHORT_OBJ {
          // out of bounds error
          if index.(*object.Short).Value < 0 || index.(*object.Short).Value > 15 {
            error.Fatal(node.Token.LineNumber, 
              "index %d is out of bounds for variable %s", 
              index.(*object.Short).Value, node.Name.Value)
          }

          // bit-flipping
          val := Eval(node.Value, env, start)
          var num int16
          if val.(*object.Short).Value == -1 {
            num = setFlag(name.(*object.Short).Value, true, index.(*object.Short).Value)
          } else if val.(*object.Short).Value == 0 {
            num = setFlag(name.(*object.Short).Value, false, index.(*object.Short).Value)
          } else { // can't flip a bit to something other than true or false
            error.Fatal(node.Token.LineNumber, "cannot assign non-boolean value with '@' expression")
          }

          if !env.Set(node.Name.Value, &object.Short{Value: num}) {
            error.Fatal(node.Token.LineNumber, "variable %s not found", node.Name.Value)
          }
          return nil
        }

      } else {
        val := Eval(node.Value, env, start)

        // buff resizing
        variable, ok := env.Get(node.Name.Value)
        if !ok {
          error.Fatal(node.Token.LineNumber, "variable %s not found", node.Name.Value)
        }

        if variable.Type() == object.BUFF_OBJ {
          error.Fatal(node.Token.LineNumber, "cannot resize buff object")
        }

        if !env.Set(node.Name.Value, val) {
          error.Fatal(node.Token.LineNumber, "%s is a label and cannot be reassigned", node.Name.Value)
        }
        return nil
      }
      

    // for evaluating units of expressions
    case *ast.Identifier:
      val, ok := env.Get(node.Value)
      if !ok {
        error.Fatal(node.Token.LineNumber, "variable %s not found", node.Value)
      }
      // if it's a lbl, cast it to a short here and return for expression eval
      if val.Type() == object.LBL_OBJ {
        return &object.Short{Value: val.(*object.Lbl).Value}
      }
      return val

    case *ast.RandKeyword:
      randomValue := int16(rand.Intn(65536))
      return &object.Short{Value: randomValue}

    case *ast.IntegerLiteral:
      return &object.Short{Value: node.Value}

    case *ast.InputKeyword:
      var i int16
      n, err := fmt.Scan(&i)
      if n > 1 || err != nil {
        error.Fatal(node.Token.LineNumber, "Invalid input")
      }
      return &object.Short{Value: i}

    case *ast.Boolean:
      if node.Value == true {
        return TRUE
      } else {
        return FALSE
      }

    case *ast.PrefixExpression:
      right := Eval(node.Right, env, start)
      return evalPrefixExpression(node.Operator, right)

    case *ast.InfixExpression:
      left := Eval(node.Left, env, start)
      right := Eval(node.Right, env, start)
      result, ok := evalInfixExpression(node.Operator, left, right)
      if !ok {
        error.Fatal(node.Token.LineNumber, 
          "Expression eval error (either out-of-bounds @ expression or invalid types)")
      }
      return result

    // for evaluating the entire program
    case *ast.Program:
      return evalStatements(node.Statements, env, start)
  }

  return nil
}

func evalInfixExpression(operator string, left object.Object, 
  right object.Object) (object.Object, bool) {

  switch {
    case left.Type() == object.SHORT_OBJ && right.Type() == object.SHORT_OBJ:
      return evalShortInfixExpression(operator, left, right)
    case left.Type() == object.BUFF_OBJ && operator == "@":
      return evalBuffAtExpression(left, right)
    default: 
      return nil, false
  }
}

func evalBuffAtExpression(left object.Object, right object.Object) (object.Object, bool) {
  if right.(*object.Short).Value < 0 || right.(*object.Short).Value >= left.(*object.Buff).Size {
    return nil, false // out of bounds
  }

  return &object.Short{Value: left.(*object.Buff).Elements[right.(*object.Short).Value]}, true
}

func evalShortInfixExpression(operator string, left object.Object, right object.Object) (object.Object, bool) {
  leftVal := left.(*object.Short).Value
  rightVal := right.(*object.Short).Value // just assume shorts for now

  switch operator {
    case "+":
      return &object.Short{Value: leftVal + rightVal}, true
    case "-":
      return &object.Short{Value: leftVal - rightVal}, true
    case "*":
      return &object.Short{Value: leftVal * rightVal}, true
    case "/":
      return &object.Short{Value: leftVal / rightVal}, true
    case "%":
      return &object.Short{Value: leftVal % rightVal}, true
    case ">":
      if leftVal > rightVal {
        return TRUE, true
      } else {
        return FALSE, true
      }
    case "<":
      if leftVal < rightVal {
        return TRUE, true
      } else {
        return FALSE, true
      }
    case ">=":
      if leftVal >= rightVal {
        return TRUE, true
      } else {
        return FALSE, true
      }
    case "<=":
      if leftVal <= rightVal {
        return TRUE, true
      } else {
        return FALSE, true
      }
    case "==":
      if leftVal == rightVal {
        return TRUE, true
      } else {
        return FALSE, true
      }
    case "!=":
      if leftVal != rightVal {
        return TRUE, true
      } else {
        return FALSE, true
      }
    case "&":
      return &object.Short{Value: leftVal & rightVal}, true
    case "|":
      return &object.Short{Value: leftVal | rightVal}, true
    case "@":
      if rightVal < 0 || rightVal > 15 {
        return nil, false // out of bounds error
      } else {
        if getFlag(leftVal, rightVal) {
          return TRUE, true
        } else {
          return FALSE, true
        }
      }
    default:
      return nil, false // ? not entirely sure when this would occur
  }
} // TODO some messy boolean error handling here, could use a refactor

// true if num's binary digit at index is 1, else 0
// 0 is the rightmost bit, 15 is the leftmost bit
func getFlag(num int16, index int16) bool {
  if ((num & (1 << index)) != 0) { // bit-shift fuckery (I LOVE BINARY!!!)
    return true
  } else {
    return false
  }
}

// will set num's binary digit at index to 1 if flag is true, else 0
func setFlag(num int16, flag bool, index int16) int16 {
  if flag {
    return num | (1 << index)
  } else { // even more bit-shift fuckery
    return num & (^(1 << index))
  }
}

func evalPrefixExpression(operator string, right object.Object) object.Object {
  switch operator {
    case "!":
      // return bitwise not of right, using unary xor
      return &object.Short{Value: ^(right.(*object.Short).Value)}
    case "-":
      return &object.Short{Value: -(right.(*object.Short).Value)}
    case "+":
      n := right.(*object.Short).Value
      if n >= 0 {
        return &object.Short{Value: n}
      } else { // because apparently Go's unary plus doesn't actually make a 
        // negative number positive
        return &object.Short{Value: 0 - n}
      }
    case "sizeof":
      if right.Type() == object.SHORT_OBJ || right.Type() == object.LBL_OBJ {
        return &object.Short{Value: 16}
      } else { // is a buff
        return &object.Short{Value: right.(*object.Buff).Size}
      }
    default:
      return nil
  }
}

func evalStatements(stmts []ast.Statement, env *object.Environment, start time.Time) object.Object { // TODO
  var result object.Object

  // eval all labels first!
  for i, statement := range stmts {
    pc = i
    if statement.TokenLiteral() == "lbl" {
      result = Eval(statement, env, start) // don't need the return here
    } else {
      continue
    }
  }

  pc = 0 // reset pc
  rand.Seed(time.Now().UnixNano()) // seed the rng

  // now eval everything that isn't a label
  for j := 0; j < len(stmts); {
    // fmt.Printf("pc = %d\n", pc)

    if pc >= len(stmts) {
      break
    }

    statement := stmts[pc]

    if statement.TokenLiteral() == "lbl" { // skip labels on second iteration
      pc++
      continue
    }

    result = Eval(statement, env, start)
    pc++

    // if encountered exit statement, exit program
    if statement.TokenLiteral() == "exit" {
      return nil
    }

    j = pc
  }

  return result
}