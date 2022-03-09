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
#include <vector>

#include "code.hpp"
#include "config.hpp"
#include "constants.hpp"

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

    std::vector<char*> labels;
    std::vector<int> labels_addresses;

    std::vector<char*> data_labels;
    std::vector<int> data_labels_addresses;

    int offset;
};

Assembler::Assembler() {
    source = Code<char>();
    machine = Code<int>();

    cells_count = 0;
    offset = 0;
}

Assembler::~Assembler() {
    for (auto label : labels) {
        free(label);
    }

    for (auto label : data_labels) {
        free(label);
    }
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

    int write_count = fwrite(machine.code, sizeof(int), PPTSM_PROCESS_MEMORY_SIZE, machine_file);
    assert(write_count == PPTSM_PROCESS_MEMORY_SIZE);

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

    bool is_data_segment = false;
    char* token = strtok(source_copy, DELIMITERS);
    while (token) {
        int token_length = strlen(token);

        if (token[0] == '.') {
            is_data_segment = true;
        }
        
        if (token[token_length - 1] == ':') {
            token[token_length - 1] = '\0';

            if (is_data_segment) {
                for (int i = 0; i < data_labels.size(); ++i) {
                    if (!strcmp(token, data_labels[i])) {
                        printf("Error: double data label");
                    }
                }

                char *token_copy = strdup(token);

                data_labels.push_back(token_copy);
                data_labels_addresses.push_back(cells_count);

                cells_count += 256;

            } else {
                for (int i = 0; i < labels.size(); ++i) {
                    if (!strcmp(token, labels[i])) {
                        printf("Error: double label");
                    }
                }

                char *token_copy = strdup(token);

                labels.push_back(token_copy);
                labels_addresses.push_back(cells_count);
            }

        } else if (!is_data_segment) {
            ++cells_count;
        }

        token = strtok(nullptr, DELIMITERS);
    }

    int labels_offset = PPTSM_PROCESS_MEMORY_SIZE - cells_count; // +-1
    assert(labels_offset >= 0);
    assert(labels_offset < PPTSM_PROCESS_MEMORY_SIZE);

    for (size_t i = 0; i < labels_addresses.size(); ++i) {
        labels_addresses[i] += labels_offset;
    }

    for (size_t i = 0; i < data_labels_addresses.size(); ++i) {
        data_labels_addresses[i] += labels_offset;
    }

    offset = labels_offset;

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
    machine.code = (int*) calloc(PPTSM_PROCESS_MEMORY_SIZE, sizeof(int));

    int curr_cell = offset;
    machine.code[0] = offset;

    bool is_data_segment = false;

    char* token = strtok(source.code, DELIMITERS);
    while (token) {
        int token_length = strlen(token);

        if (token[0] == '.') {
            is_data_segment = true;
        }

        if (!is_data_segment && token[token_length - 1] != ':') {
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

                for (int i = 0; i < labels.size(); ++i) {
                    if (!strcmp(token, labels[i])) {
                        machine.code[curr_cell] = labels_addresses[i];
                        is_parsed = true;
                        break;
                    }
                }

                for (int i = 0; i < data_labels.size(); ++i) {
                    if (!strcmp(token, data_labels[i])) {
                        machine.code[curr_cell] = data_labels_addresses[i];
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
        } else if (is_data_segment) {
            if (token[token_length - 1] == ':') {
                // flex

                char* dst_ptr = (char*)(machine.code + curr_cell);
                memset((void*)(machine.code + curr_cell), 0, 256 * 4);
                curr_cell += 256;

                token = strtok(nullptr, DELIMITERS);
                token_length = strlen(token);
                while (true) {
                    if (token[0] == '"') {
                        ++token;
                        --token_length;
                    }
                    memcpy(dst_ptr, token, token_length);
                    dst_ptr[token_length] = ' ';

                    dst_ptr += token_length + 1;

                    if (token[token_length - 1] == '"') {
                        dst_ptr[-2] = '\0';
                        break;
                    }

                    token = strtok(nullptr, DELIMITERS);
                    token_length = strlen(token);
                }
            }
        }

        token = strtok(nullptr, DELIMITERS);
    }
}


int main(int argc, char** argv) {
    Assembler assembler;

    printf("Loading source code...\n");
    assembler.LoadCode(argv[1]);
    printf("Done!\n\n");

    printf("Assembling...\n");
    assembler.Assemble();
    printf("Done!\n");

    if (argc < 3) {
        assembler.SaveCode(PPTSM_ASM_OUTPUT);
    } else {
        assembler.SaveCode(argv[2]);
    }

    return 0;
}
