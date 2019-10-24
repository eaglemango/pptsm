#include <cassert>
#include <cctype>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "code.hpp"
#include "constants.hpp"
#include "dynamic_buffer.hpp"


class Assembler {
public:
    Assembler();
    ~Assembler();

    void LoadCode(char* file_path);
    void SaveCode(char* file_path);

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

void Assembler::LoadCode(char* file_path) {
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

void Assembler::SaveCode(char* file_path) {
    FILE* machine_file = fopen(file_path, "wb");
    assert(machine_file);

    int write_count = fwrite(machine.code, sizeof(int), cells_count, machine_file);
    assert(write_count == cells_count);

    int close_result = fclose(machine_file);
    assert(close_result != EOF);
}

void Assembler::Assemble() {
    PreprocessCode();
    TranslateCode();
}

// Map labels to addresses
void Assembler::PreprocessCode() {
    char* source_copy = strdup(source.code);

    char* token = strtok(source_copy, DELIMITERS);
    while (token) {
        int token_length = strlen(token);
        
        // Check only labels
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

void Assembler::TranslateCode() {
    machine.code = (int*) calloc(cells_count, sizeof(int));

    int curr_cell = 0;

    char* token = strtok(source.code, DELIMITERS);
    while (token) {
        int token_length = strlen(token);

        // Skip labels
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
        assembler.SaveCode("pptsm.exe");
    } else {
        assembler.SaveCode(argv[2]);
    }

    return 0;
}
