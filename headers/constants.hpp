const char DELIMITERS[] = " \n\r\t";

#define INSTRUCTION(NAME, CODE, ACTION) INSTR_##NAME = CODE,

enum Instructions {
    #include "instructions.hpp"
};

#undef INSTRUCTION

#define REGISTER(NAME, CODE) REG_##NAME = CODE,

enum Registers {
    #include "registers.hpp"
};

#undef REGISTER
