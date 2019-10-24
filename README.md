# PPTSM - "Push Pop Top" Stack Machine

## What is it?

PPTSM is a simple stack machine. It has its own assembly language for writing programs for following usage.

## How does it work?

PPTSM consists of three main parts:

* Assembly language -- a tool for writing human-recognizable programs for stack machine
* Assembler -- a program, which translates your code to stack machine code
* CPU Emulator -- a program, that launches assembled program on stack machine

These parts are complemented by additional parts:

* Disassembler -- a program, which translates stack machine code to assembly language

## Documentation

You can get beautiful documentation using `Doxygen`:

```
$ doxygen Doxyfile
```

You'll find it in `doxymentation` folder.

## Requirements

PPTSM requires [Stack](https://github.com/eaglemango/stack) for proper work. You can use your own instead (don't forget to make the similar interface or use adapter).
