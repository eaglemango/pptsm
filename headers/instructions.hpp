/*!
    @file
    @brief PPTSM Assembly language instructions
*/
INSTRUCTION(END, 0, {
    active = false;
})

INSTRUCTION(PUSH, 1, {
    stack.push(machine.code[++curr_cell]);
})

INSTRUCTION(PUSHR, 101, {
    stack.push(GetRegisterData(machine.code[++curr_cell]));
})

INSTRUCTION(POP, 2, {
    stack.pop();
})

INSTRUCTION(TOP, 3, {
    UpdateRegister(machine.code[++curr_cell], stack.top());
})

INSTRUCTION(ADD, 4, {
    UpdateRegister(REG_T, stack.top());
    stack.pop();

    UpdateRegister(REG_T, T + stack.top());
    stack.pop();

    stack.push(T);
})

INSTRUCTION(SUB, 5, {
    UpdateRegister(REG_T, stack.top());
    stack.pop();

    UpdateRegister(REG_T, T - stack.top());
    stack.pop();

    stack.push(T);
})

INSTRUCTION(MUL, 6, {
    UpdateRegister(REG_T, stack.top());
    stack.pop();

    UpdateRegister(REG_T, static_cast<int>((T / PPTSM_PRECISION) * (stack.top() / PPTSM_PRECISION) * PPTSM_PRECISION));
    stack.pop();

    stack.push(T);
})

INSTRUCTION(DIV, 7, {
    UpdateRegister(REG_T, stack.top());
    stack.pop();

    UpdateRegister(REG_T, static_cast<int>(1.0 * T / stack.top() * PPTSM_PRECISION));
    stack.pop();

    stack.push(T);
})

INSTRUCTION(J, 8, {
    ++curr_cell;
    curr_cell = machine.code[curr_cell] - 1;
})

#define JMP(OPERATION) \
    ++curr_cell; \
    UpdateRegister(REG_T, stack.top()); \
    stack.pop(); \
    UpdateRegister(REG_T, static_cast<int>(T OPERATION stack.top())); \
    stack.pop(); \
    if (T) { \
        curr_cell = machine.code[curr_cell] - 1; \
    }    

INSTRUCTION(JA, 9, {
    JMP(>)
})

INSTRUCTION(JAE, 10, {
    JMP(>=)
})

INSTRUCTION(JB, 11, {
    JMP(<)
})

INSTRUCTION(JBE, 12, {
    JMP(<=)
})

INSTRUCTION(JE, 13, {
    JMP(==)
})

INSTRUCTION(JNE, 14, {
    JMP(!=)
})

#undef JMP

INSTRUCTION(GET, 15, {
    float temp = 0.0;
    printf("GET: ");
    scanf("%f", &temp);

    stack.push(static_cast<int>(temp * PPTSM_PRECISION));
})

INSTRUCTION(PUT, 16, {
    printf("PUT: %.3f\n", stack.top() / PPTSM_PRECISION);

    stack.pop();
})

INSTRUCTION(CALL, 17, {
    ++curr_cell;

    stack.push(curr_cell + 1);

    curr_cell = machine.code[curr_cell] - 1;
})

INSTRUCTION(RET, 18, {
    curr_cell = stack.top() - 1;
    stack.pop();
})

INSTRUCTION(SQRT, 19, {
    UpdateRegister(REG_T, stack.top());
    stack.pop();

    UpdateRegister(REG_T, static_cast<int>(sqrt(T / PPTSM_PRECISION) * PPTSM_PRECISION));

    stack.push(T);
})

INSTRUCTION(MEOW, 20, {
    printf("Meow!\n");
})
