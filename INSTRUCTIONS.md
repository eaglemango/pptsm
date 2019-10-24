# Assembly language instructions list

Here you can find all PPTSM Assembly instructions with description.

## Data Instructions

`PUSH x` - pushes `x` to stack (`x` is number or register)

`PUSHR r` - pushes register's data to stack

`POP` - pops data from stack

`TOP r` - writes data from top of stack to register

## Arithmetic Instructions

`ADD` - pops two values from stack and then pushes their sum

`SUB` - pops two values from stack and then pushes their sub

`MUL` - pops two values from stack and then pushes their mul

`DIV` - pops two values from stack and then pushes their div

`SQRT` - pops value from stack and then pushes its square root

## Jump Instructions

`J l` - jumps to label

`JA l` - pops two values from stack and then jumps to label if the first was above

`JAE l` - pops two values from stack and then jumps to label if the first was above or equal

`JB l` - pops two values from stack and then jumps to label if the first was below

`JBE l` - pops two values from stack and then jumps to label if the first was below or equal

`JE l` - pops two values from stack and then jumps to label if they were equal

`JNE l` - pops two values from stack and then jumps to label they weren't equal

`CALL l` - pushes return link to stack and then jumps to label

`RET` - pops value from stack and interprets it as return link (jumps to it)

## IO Instructions

`GET` - reads data from `stdin` and pushes it to stack

`PUT` - pops value from stack and writes it to `stdout`

## Others

`END` - stops CPU Emulator work

`MEOW` - writes cute things to `stdout`
