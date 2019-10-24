#include <cassert>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "code.hpp"
#include "config.hpp"
#include "constants.hpp"
#include "stack.hpp"

class CPU {
public:
    CPU();
    ~CPU();

    void LoadBinary(const char* file_path);

    void Execute();

private:
    #define REGISTER(NAME, CODE) \
        int NAME = 0;

    #include "registers.hpp"

    #undef REGISTER

    Code<int> machine;
    Stack<int> stack = Stack<int>(PPTSM_STACK_SIZE);

    int GetRegisterData(int register_code);
    void UpdateRegister(int register_code, int value);
};

int CPU::GetRegisterData(int register_code) {
    #define REGISTER(NAME, CODE) \
        case CODE: \
            return NAME;

    switch (register_code) {
        #include "registers.hpp"

        default:
            return -1;
    }

    #undef REGISTER
}

void CPU::UpdateRegister(int register_code, int value) {
    #define REGISTER(NAME, CODE) \
       case CODE: \
           NAME = value; \
           break;

    switch (register_code) {
        #include "registers.hpp"

        default:
            break;
    }

    #undef REGISTER
}

CPU::CPU() {

}

CPU::~CPU() {
    machine.~Code();
}

void CPU::LoadBinary(const char* file_path) {
    FILE* machine_file = fopen(file_path, "rb");
    assert(machine_file);

    fseek(machine_file, 0, SEEK_END);
    machine.size = ftell(machine_file);
    fseek(machine_file, 0, SEEK_SET);

    machine.code = (int*) calloc(machine.size, sizeof(int));

    assert(machine.code);
    int read_count = fread(machine.code, sizeof(int), machine.size / sizeof(int), machine_file);
    assert(read_count == machine.size / sizeof(int));

    int close_result = fclose(machine_file);
    assert(close_result != EOF);
}

void CPU::Execute() {
    int curr_cell = 0;

    bool active = true;
    while (active && curr_cell < machine.size / sizeof(int)) {
        #define INSTRUCTION(NAME, CODE, ACTION) \
            case CODE: \
                ACTION \
                break;

        switch (machine.code[curr_cell]) {
            #include "instructions.hpp"
        }

        ++curr_cell;
    }

    #undef INSTRUCTION
}

int main(int argc, char** argv) {
    CPU cpu;

    cpu.LoadBinary(argv[1]);

    cpu.Execute();
}
