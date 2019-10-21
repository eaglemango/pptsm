#define INSTRUCTION(NAME,SYNONYM,CODE,ACTION) INSTR_##NAME = CODE,

enum Instructions {
    #include "instructions.hpp"
};

#undef INSTRUCTION

#define REGISTER(NAME,SYNONYM,CODE) REG_##NAME = CODE,

enum Registers {
    #include "registers.hpp"
};

#undef REGISTER
