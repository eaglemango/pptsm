/*!
    @file
    @brief PPTSM Assembly language instructions
*/
INSTRUCTION(END, 0, {
    active = false;
})

INSTRUCTION(PUSH, 1, {
    stack.Push(machine.code[++curr_cell]);
})

INSTRUCTION(PUSHR, 101, {
    stack.Push(GetRegisterData(machine.code[++curr_cell]));
})

INSTRUCTION(POP, 2, {
    stack.Pop();
})

INSTRUCTION(TOP, 3, {
    UpdateRegister(machine.code[++curr_cell], stack.Top());
})

INSTRUCTION(ADD, 4, {
    UpdateRegister(REG_T, stack.Top());
    stack.Pop();

    UpdateRegister(REG_T, T + stack.Top());
    stack.Pop();

    stack.Push(T);
})

INSTRUCTION(SUB, 5, {
    UpdateRegister(REG_T, stack.Top());
    stack.Pop();

    UpdateRegister(REG_T, T - stack.Top());
    stack.Pop();

    stack.Push(T);
})

INSTRUCTION(MUL, 6, {
    UpdateRegister(REG_T, stack.Top());
    stack.Pop();

    UpdateRegister(REG_T, static_cast<int>((T / PPTSM_PRECISION) * (stack.Top() / PPTSM_PRECISION) * PPTSM_PRECISION));
    stack.Pop();

    stack.Push(T);
})

INSTRUCTION(DIV, 7, {
    UpdateRegister(REG_T, stack.Top());
    stack.Pop();

    UpdateRegister(REG_T, static_cast<int>(1.0 * T / stack.Top() * PPTSM_PRECISION));
    stack.Pop();

    stack.Push(T);
})

INSTRUCTION(J, 8, {
    ++curr_cell;
    curr_cell = machine.code[curr_cell] - 1;
})

#define JMP(OPERATION) \
    ++curr_cell; \
    UpdateRegister(REG_T, stack.Top()); \
    stack.Pop(); \
    UpdateRegister(REG_T, static_cast<int>(T OPERATION stack.Top())); \
    stack.Pop(); \
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

    stack.Push(static_cast<int>(temp * PPTSM_PRECISION));
})

INSTRUCTION(PUT, 16, {
    printf("PUT: %.3f\n", stack.Top() / PPTSM_PRECISION);

    stack.Pop();
})

INSTRUCTION(CALL, 17, {
    ++curr_cell;

    stack.Push(curr_cell + 1);

    curr_cell = machine.code[curr_cell] - 1;
})

INSTRUCTION(RET, 18, {
    curr_cell = stack.Top() - 1;
    stack.Pop();
})

INSTRUCTION(SQRT, 19, {
    UpdateRegister(REG_T, stack.Top());
    stack.Pop();

    UpdateRegister(REG_T, static_cast<int>(sqrt(T / PPTSM_PRECISION) * PPTSM_PRECISION));

    stack.Push(T);
})

INSTRUCTION(MEOW, 20, {
    printf("Meow!\n");
})
