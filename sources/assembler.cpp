/*!
    @file
    @brief Assembler for assembly language
*/
#include <cassert>
#include <cctype>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "code.hpp"
#include "config.hpp"
#include "constants.hpp"
#include "dynamic_buffer.hpp"

/*!
    @brief Assembler

    Generates machine code for CPU Emulator
*/
class Assembler {
public:
    Assembler();
    ~Assembler();

    void LoadCode(const char* file_path);
    void SaveCode(const char* file_path);

    void Assemble();

private:
    void PreprocessCode();
    void TranslateCode();

    Code<char> source;
    Code<int> machine;

    int cells_count;

    DynamicBuffer<char*> labels;
    DynamicBuffer<int> labels_addresses;
};

Assembler::Assembler() {
    source = Code<char>();
    machine = Code<int>();

    cells_count = 0;

    labels = DynamicBuffer<char*>(1);
    labels_addresses = DynamicBuffer<int>(1);
}

Assembler::~Assembler() {
    labels.CleanContents();
}

/*!
    @brief Load assembly code from file
    @param[in] file_path Path to file
    @param[out] . No return value
*/
void Assembler::LoadCode(const char* file_path) {
    FILE* source_file = fopen(file_path, "r");
    assert(source_file);

    fseek(source_file, 0, SEEK_END);
    source.size = ftell(source_file);
    fseek(source_file, 0, SEEK_SET);

    source.code = (char*) calloc(source.size, sizeof(char));
    assert(source.code);

    int read_count = fread(source.code, sizeof(char), source.size, source_file);
    assert(read_count == source.size);

    int close_result = fclose(source_file);
    assert(close_result != EOF);
}

/*!
    @brief Saves machine code to file
    @param[in] file_path Path to file
    @param[out] . No return value
*/
void Assembler::SaveCode(const char* file_path) {
    FILE* machine_file = fopen(file_path, "wb");
    assert(machine_file);

    int write_count = fwrite(machine.code, sizeof(int), cells_count, machine_file);
    assert(write_count == cells_count);

    int close_result = fclose(machine_file);
    assert(close_result != EOF);
}

/*!
    @brief Starts two-stage process of compilation
    @param[in] . No input value
    @param[out] . No return value
*/
void Assembler::Assemble() {
    PreprocessCode();
    TranslateCode();
}

/*!
    @brief Preprocesses lables
    @param[in] . No input value
    @param[out] . No return value
    @warning Uses thread-unsafe strtok function
*/
void Assembler::PreprocessCode() {
    char* source_copy = strdup(source.code);

    char* token = strtok(source_copy, DELIMITERS);
    while (token) {
        int token_length = strlen(token);
        
        if (token[token_length - 1] == ':') {
            token[token_length - 1] = '\0';
             for (int i = 0; i < labels.GetCurrSize(); ++i) {
                if (!strcmp(token, labels[i])) {
                    printf("Error: double label");
                }
            }

            char* token_copy = strdup(token);

            labels.Add(token_copy);
            labels_addresses.Add(cells_count);

        } else {
            ++cells_count;
        }

        token = strtok(nullptr, DELIMITERS);
    }

    assert(source_copy);
    free(source_copy);
    source_copy = nullptr;
}

/*!
    @brief Translates assembly code to machine code
    @param[in] . No input value
    @param[out] . No return value
    @warning Uses thread-unsafe strtok function
*/
void Assembler::TranslateCode() {
    machine.code = (int*) calloc(cells_count, sizeof(int));

    int curr_cell = 0;

    char* token = strtok(source.code, DELIMITERS);
    while (token) {
        int token_length = strlen(token);

        if (token[token_length - 1] != ':') {
            #define INSTRUCTION(NAME, CODE, ACTION) \
                else if (!strcasecmp(token, #NAME)) { \
                    machine.code[curr_cell] = INSTR_##NAME; \
                }

            bool asm_is_bad = false;

            if (asm_is_bad) {

            }

            #include "instructions.hpp"

            else {
                bool is_parsed = false;

                for (int i = 0; i < labels.GetCurrSize(); ++i) {
                    if (!strcmp(token, labels[i])) {
                        machine.code[curr_cell] = labels_addresses[i];
                        is_parsed = true;
                        break;
                    }
                }

                if (!is_parsed) {
                    #define REGISTER(NAME, CODE) \
                        else if (!strcasecmp(token, #NAME)) { \
                            machine.code[curr_cell] = REG_##NAME; \
                            if (machine.code[curr_cell - 1] == INSTR_PUSH) { \
                                machine.code[curr_cell - 1] = INSTR_PUSHR; \
                            } \
                        }

                    bool is_asm_bad = false;

                    if (is_asm_bad) {

                    }

                    #include "registers.hpp"

                    else {
                        machine.code[curr_cell] = static_cast<int>(atof(token) * 1000);
                    }

                    #undef REGISTER
                }
            }

            #undef INSTRUCTION

            ++curr_cell;
        }

        token = strtok(nullptr, DELIMITERS);
    }
}


int main(int argc, char** argv) {
    Assembler assembler;

    assembler.LoadCode(argv[1]);

    assembler.Assemble();

    if (argc < 3) {
        assembler.SaveCode(PPTSM_ASM_OUTPUT);
    } else {
        assembler.SaveCode(argv[2]);
    }

    return 0;
}
