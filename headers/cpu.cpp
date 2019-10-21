#include "stack.hpp"

#include <iostream>
#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <climits>

#include "code.hpp"
#include "constants.hpp"
#include <cmath>

class CPU {
public:
    CPU();
    ~CPU();

    void LoadBinary(char* file_path);

    void Execute();

    void ShowStack();

private:
    #define REGISTER(NAME,SYNONYM,CODE) \
        int NAME = 0;

    #include "registers.hpp"

    #undef REGISTER

    Code<int> machine;
    Stack<int> stack = Stack<int>(100);

    int ParseArgument(int argument);
    void UpdateRegister(int register_code, int value);
};

void CPU::ShowStack() {
    while (!stack.IsEmpty()) {
        std::cout << stack.Top() << ' ';
        stack.Pop();
    }
}

int CPU::ParseArgument(int argument) {
    #define REGISTER(NAME,SYNONYM,CODE) \
        case INT32_MAX - CODE: \
            return NAME;

    switch (argument) {
        #include "registers.hpp"

        default:
            return argument;
    }

    #undef REGISTER
}

void CPU::UpdateRegister(int register_code, int value) {
    #define REGISTER(NAME,SYNONYM,CODE) \
       case INT32_MAX - CODE: \
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

void CPU::LoadBinary(char* file_path) {
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
        // std::cout << curr_cell;
        #define INSTRUCTION(NAME,SYNONYM,CODE,ACTION) \
            case CODE: \
                std::cout << #NAME << ' '; \
                ACTION \
                break;

        switch (machine.code[curr_cell]) {
            #include "instructions.hpp"
        }

        ++curr_cell;
    }

    #undef INSTRUCTION
}

int main() {
    CPU cpu;

    cpu.LoadBinary("pptsm");

    cpu.Execute();

    cpu.ShowStack();
}
