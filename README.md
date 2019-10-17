# PPTSM - "Push Pop Top" Stack Machine

## What is it?

PPTSM is a simple stack machine. It has its own assembly language for writing programs for following usage.

## How does it work?

PPTSM consists of three main parts:

* Assembly language -- a tool for writing human-recognizable programs for stack machine
* Assembler -- a program, which translates your code to stack machine code
* Emulator -- a program, that launches assembled program on stack machine

These parts are complemented by additional parts:

* Disassembler -- a program, which translates stack machine code to assembly language
* Optimizer -- a subprogram for assembler, that can remove odd instructions to code work faster

## Assembly language

Assembly language is an easy way to write programs for stack machine.

All instructions are divided to several categories:

*By complexity*

* 0-level -- instructions don't use another instructions in implementation
* 1-level -- instructions use at least one 0-level instruction
...
* n-level -- instructions use at least one (n-1)-level instructions

The low-level types of instructions are preferred, because high-level instructions are designed for general situations, when your problem can be solved easier.

*By machine usage*

* Stack-related -- instructions that use stack in some purpose
* IO -- instructions that use interaction with users' input and output devices
* Quiet -- instructions don't use stack and IO

*By operator type*

Some instructions can be considered as operators.

* Arithmetic -- instructions that behave like arithmetic operators
* Logic -- instructions that behave like logic operators
* Data -- instructions that operate with data

*Examples*

* push -- 0-level stack-related data instruction
* add -- 1-level stack-related arithmetic instruction
