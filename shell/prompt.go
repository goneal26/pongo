package shell

import (
  "bufio"
  "fmt"
  "io"
)

const WHITESPACE = " \t" // whitespace cutset for shell commands
const PREFIX = ":" // shell commands start with this

func Start(in io.Reader, out io.Writer) {
  scanner := bufio.NewScanner(in)
  sh := initShell() // start the shell

  for {
    fmt.Printf(sh.prompt(), sh.lineNumber);

    scanned := scanner.Scan()
    if !scanned {
      return
    }

    line := scanner.Text()

    // check if it's pongo or a shell command
    sh.handle(line)

    fmt.Printf("Entered: <%s>\n", line)
  }
}

