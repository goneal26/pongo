package object

// methods for storing variable/label bindings

func NewEnvironment() *Environment {
  s := make(map[string]Object)
  return &Environment{store: s}
}

type Environment struct {
  store map[string]Object
}

// get an existing binding, ok is false if not found
func (e *Environment) Get(name string) (Object, bool) {
  obj, ok := e.store[name]
  return obj, ok
}

// set an existing binding, ok is false if not found/cannot set
func (e *Environment) Set(name string, val Object) bool {
  obj, found := e.store[name]
  if found && obj.Type() != LBL_OBJ { // cannot reassign labels!
    e.store[name] = val
    return true
  }

  return false
}

// create a new binding, ok is false if binding already exists
func (e *Environment) Add(name string, val Object) bool {
  _, found := e.store[name]
  if !found {
    e.store[name] = val
    return true
  }

  return false
}

// remove variable binding, returns ok -> false if not found/cannot remove
func (e *Environment) Remove(name string) bool {
  obj, found := e.store[name]
  if found && obj.Type() != LBL_OBJ { // cannot delete labels!
    delete(e.store, name)
    return true
  }

  return false
}

// remove all variable bindings except for labels
func (e *Environment) RemoveAll() {
  for name, val := range e.store {
    if val.Type() != LBL_OBJ {
      delete(e.store, name)
    }
  }
}