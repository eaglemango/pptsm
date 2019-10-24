INSTRUCTION(END, 0, {
    active = false;
})

INSTRUCTION(PUSH, 1, {
    stack.Push(ParseArgument(machine.code[++curr_cell]));
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

    UpdateRegister(REG_T, static_cast<int>((T / 1000.0) * (stack.Top() / 1000.0) * 1000));
    stack.Pop();

    stack.Push(T);
})

INSTRUCTION(DIV, 7, {
    UpdateRegister(REG_T, stack.Top());
    stack.Pop();

    UpdateRegister(REG_T, static_cast<int>(1.0 * T / stack.Top() * 1000));
    stack.Pop();

    stack.Push(T);
})

INSTRUCTION(J, 8, {
    ++curr_cell;
    curr_cell = machine.code[curr_cell] - 1;
})

INSTRUCTION(JA, 9, {
    ++curr_cell;

    UpdateRegister(REG_T, stack.Top());
    stack.Pop();

    UpdateRegister(REG_T, static_cast<int>(T > stack.Top()));
    stack.Pop();

    if (T) {
        curr_cell = machine.code[curr_cell] - 1;
    }
})

INSTRUCTION(JAE, 10, {
    ++curr_cell;

    UpdateRegister(REG_T, stack.Top());
    stack.Pop();

    UpdateRegister(REG_T, static_cast<int>(T >= stack.Top()));
    stack.Pop();

    if (T) {
        curr_cell = machine.code[curr_cell] - 1;
    }
})

INSTRUCTION(JB, 11, {
    ++curr_cell;

    UpdateRegister(REG_T, stack.Top());
    stack.Pop();

    UpdateRegister(REG_T, static_cast<int>(T < stack.Top()));
    stack.Pop();

    if (T) {
        curr_cell = machine.code[curr_cell] - 1;
    }
})

INSTRUCTION(JBE, 12, {
    ++curr_cell;

    UpdateRegister(REG_T, stack.Top());
    stack.Pop();

    UpdateRegister(REG_T, static_cast<int>(T <= stack.Top()));
    stack.Pop();

    if (T) {
        curr_cell = machine.code[curr_cell] - 1;
    }
})

INSTRUCTION(JE, 13, {
    ++curr_cell;

    UpdateRegister(REG_T, stack.Top());
    stack.Pop();

    UpdateRegister(REG_T, static_cast<int>(T == stack.Top()));
    stack.Pop();

    if (T) {
        curr_cell = machine.code[curr_cell] - 1;
    }
})

INSTRUCTION(JNE, 14, {
    ++curr_cell;

    UpdateRegister(REG_T, stack.Top());
    stack.Pop();

    UpdateRegister(REG_T, static_cast<int>(T != stack.Top()));
    stack.Pop();

    if (T) {
        curr_cell = machine.code[curr_cell] - 1;
    }
})

INSTRUCTION(GET, 15, {
    float temp = 0.0;
    printf("GET: ");
    scanf("%f", &temp);

    stack.Push(static_cast<int>(temp * 1000));
})

INSTRUCTION(PUT, 16, {
    printf("PUT: %.3f\n", stack.Top() / 1000.0);

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

    UpdateRegister(REG_T, static_cast<int>(sqrt(T / 1000.0) * 1000));

    stack.Push(T);
})

INSTRUCTION(MEOW, 20, {
    printf("Meow!\n");
})
