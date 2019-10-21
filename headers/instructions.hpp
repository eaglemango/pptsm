INSTRUCTION(END, end, 0, {
    active = false;
})

INSTRUCTION(PUSH, push, 1, {
    stack.Push(ParseArgument(machine.code[++curr_cell]));
})

INSTRUCTION(POP, pop, 2, {
    stack.Pop();
})

INSTRUCTION(TOP, top, 3, {
    UpdateRegister(machine.code[++curr_cell], stack.Top());
})

INSTRUCTION(ADD, add, 4, {
    UpdateRegister(REG_T, stack.Top());
    stack.Pop();

    UpdateRegister(REG_T, T + stack.Top());
    stack.Pop();

    stack.Push(T);
})

INSTRUCTION(SUB, sub, 5, {
    UpdateRegister(REG_T, stack.Top());
    stack.Pop();

    UpdateRegister(REG_T, T - stack.Top());
    stack.Pop();

    stack.Push(T);
})

INSTRUCTION(MUL, mul, 6, {
    UpdateRegister(REG_T, stack.Top());
    stack.Pop();

    UpdateRegister(REG_T, static_cast<int>((T / 1000.0) * (stack.Top() / 1000.0) * 1000));
    stack.Pop();

    stack.Push(T);
})

INSTRUCTION(DIV, div, 7, {
    UpdateRegister(REG_T, stack.Top());
    stack.Pop();

    UpdateRegister(REG_T, static_cast<int>(1.0 * T / stack.Top() * 1000));
    stack.Pop();

    stack.Push(T);
})

INSTRUCTION(J, j, 8, {
    ++curr_cell;
    curr_cell = machine.code[curr_cell] - 1;
})

INSTRUCTION(JA, ja, 9, {
    ++curr_cell;

    UpdateRegister(REG_T, stack.Top());
    stack.Pop();

    UpdateRegister(REG_T, static_cast<int>(T > stack.Top()));
    stack.Pop();

    if (T) {
        curr_cell = machine.code[curr_cell] - 1;
    }
})

INSTRUCTION(JAE, jae, 10, {
    ++curr_cell;

    UpdateRegister(REG_T, stack.Top());
    stack.Pop();

    UpdateRegister(REG_T, static_cast<int>(T >= stack.Top()));
    stack.Pop();

    if (T) {
        curr_cell = machine.code[curr_cell] - 1;
    }
})

INSTRUCTION(JB, jb, 11, {
    ++curr_cell;

    UpdateRegister(REG_T, stack.Top());
    stack.Pop();

    UpdateRegister(REG_T, static_cast<int>(T < stack.Top()));
    stack.Pop();

    if (T) {
        curr_cell = machine.code[curr_cell] - 1;
    }
})

INSTRUCTION(JBE, jbe, 12, {
    ++curr_cell;

    UpdateRegister(REG_T, stack.Top());
    stack.Pop();

    UpdateRegister(REG_T, static_cast<int>(T <= stack.Top()));
    stack.Pop();

    if (T) {
        curr_cell = machine.code[curr_cell] - 1;
    }
})

INSTRUCTION(JE, je, 13, {
    ++curr_cell;

    UpdateRegister(REG_T, stack.Top());
    stack.Pop();

    UpdateRegister(REG_T, static_cast<int>(T == stack.Top()));
    stack.Pop();

    if (T) {
        curr_cell = machine.code[curr_cell] - 1;
    }
})

INSTRUCTION(JNE, jne, 14, {
    ++curr_cell;

    UpdateRegister(REG_T, stack.Top());
    stack.Pop();

    UpdateRegister(REG_T, static_cast<int>(T != stack.Top()));
    stack.Pop();

    if (T) {
        curr_cell = machine.code[curr_cell] - 1;
    }
})

INSTRUCTION(GET, get, 15, {
    float temp = 0.0;
    printf("GET: ");
    scanf("%f", &temp);

    stack.Push(static_cast<int>(temp * 1000));
})

INSTRUCTION(PUT, put, 16, {
    printf("PUT: %.3f\n", stack.Top() / 1000.0);

    stack.Pop();
})

INSTRUCTION(CALL, call, 17, {
    ++curr_cell;

    stack.Push(curr_cell + 1);

    curr_cell = machine.code[curr_cell] - 1;
})

INSTRUCTION(RET, ret, 18, {
    curr_cell = stack.Top() - 1;
    stack.Pop();
})

INSTRUCTION(SQRT, sqrt, 19, {
    UpdateRegister(REG_T, stack.Top());
    stack.Pop();

    UpdateRegister(REG_T, static_cast<int>(sqrt(T / 1000.0) * 1000));

    stack.Push(T);
})
