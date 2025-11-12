//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

enum State {
    INIT,
    READ_COMMAND,
    PROCESS_NUMBER,
    DISPLAY_RESULT,
    ERROR_STATE,
    EXIT_STATE
};

struct Context {
    enum State current_state;
    int number;
    int result;
    char command[32];
};

void state_init(struct Context *ctx) {
    printf("Calculator started. Commands: 'add', 'sub', 'mul', 'div', 'exit'\n");
    ctx->current_state = READ_COMMAND;
    ctx->number = 0;
    ctx->result = 0;
    memset(ctx->command, 0, sizeof(ctx->command));
}

void state_read_command(struct Context *ctx) {
    printf("Enter command: ");
    if (fgets(ctx->command, sizeof(ctx->command), stdin) == NULL) {
        ctx->current_state = ERROR_STATE;
        return;
    }
    
    size_t len = strlen(ctx->command);
    if (len > 0 && ctx->command[len - 1] == '\n') {
        ctx->command[len - 1] = '\0';
    }
    
    if (strcmp(ctx->command, "exit") == 0) {
        ctx->current_state = EXIT_STATE;
    } else if (strcmp(ctx->command, "add") == 0 || 
               strcmp(ctx->command, "sub") == 0 ||
               strcmp(ctx->command, "mul") == 0 ||
               strcmp(ctx->command, "div") == 0) {
        ctx->current_state = PROCESS_NUMBER;
    } else {
        printf("Invalid command. Try again.\n");
        ctx->current_state = READ_COMMAND;
    }
}

void state_process_number(struct Context *ctx) {
    char input[32];
    printf("Enter number: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        ctx->current_state = ERROR_STATE;
        return;
    }
    
    char *endptr;
    long num = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') {
        printf("Invalid number format.\n");
        ctx->current_state = READ_COMMAND;
        return;
    }
    
    if (num < -1000000 || num > 1000000) {
        printf("Number out of range (-1000000 to 1000000).\n");
        ctx->current_state = READ_COMMAND;
        return;
    }
    
    ctx->number = (int)num;
    
    if (strcmp(ctx->command, "add") == 0) {
        if ((ctx->result > 0 && ctx->number > INT_MAX - ctx->result) ||
            (ctx->result < 0 && ctx->number < INT_MIN - ctx->result)) {
            printf("Addition would overflow.\n");
            ctx->current_state = READ_COMMAND;
            return;
        }
        ctx->result += ctx->number;
    } else if (strcmp(ctx->command, "sub") == 0) {
        if ((ctx->number > 0 && ctx->result < INT_MIN + ctx->number) ||
            (ctx->number < 0 && ctx->result > INT_MAX + ctx->number)) {
            printf("Subtraction would overflow.\n");
            ctx->current_state = READ_COMMAND;
            return;
        }
        ctx->result -= ctx->number;
    } else if (strcmp(ctx->command, "mul") == 0) {
        if (ctx->result > 0 && ctx->number > 0 && ctx->result > INT_MAX / ctx->number) {
            printf("Multiplication would overflow.\n");
            ctx->current_state = READ_COMMAND;
            return;
        }
        if (ctx->result > 0 && ctx->number < 0 && ctx->number < INT_MIN / ctx->result) {
            printf("Multiplication would underflow.\n");
            ctx->current_state = READ_COMMAND;
            return;
        }
        if (ctx->result < 0 && ctx->number > 0 && ctx->result < INT_MIN / ctx->number) {
            printf("Multiplication would underflow.\n");
            ctx->current_state = READ_COMMAND;
            return;
        }
        if (ctx->result < 0 && ctx->number < 0 && ctx->result < INT_MAX / ctx->number) {
            printf("Multiplication would overflow.\n");
            ctx->current_state = READ_COMMAND;
            return;
        }
        ctx->result *= ctx->number;
    } else if (strcmp(ctx->command, "div") == 0) {
        if (ctx->number == 0) {
            printf("Division by zero.\n");
            ctx->current_state = READ_COMMAND;
            return;
        }
        if (ctx->result == INT_MIN && ctx->number == -1) {
            printf("Division would overflow.\n");
            ctx->current_state = READ_COMMAND;
            return;
        }
        ctx->result /= ctx->number;
    }
    
    ctx->current_state = DISPLAY_RESULT;
}

void state_display_result(struct Context *ctx) {
    printf("Current result: %d\n", ctx->result);
    ctx->current_state = READ_COMMAND;
}

void state_error(struct Context *ctx) {
    printf("An error occurred. Resetting calculator.\n");
    state_init(ctx);
}

int main(void) {
    struct Context ctx;
    state_init(&ctx);