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
    ERROR,
    EXIT
};

struct Context {
    enum State current_state;
    int number;
    int result;
    char buffer[64];
};

void state_init(struct Context *ctx) {
    printf("Calculator started. Commands: 'add X', 'sub X', 'mul X', 'div X', 'exit'\n");
    ctx->current_state = READ_COMMAND;
}

void state_read_command(struct Context *ctx) {
    printf("> ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        ctx->current_state = EXIT;
        return;
    }
    
    size_t len = strlen(ctx->buffer);
    if (len > 0 && ctx->buffer[len-1] == '\n') {
        ctx->buffer[len-1] = '\0';
    }
    
    if (strcmp(ctx->buffer, "exit") == 0) {
        ctx->current_state = EXIT;
    } else if (strncmp(ctx->buffer, "add ", 4) == 0 ||
               strncmp(ctx->buffer, "sub ", 4) == 0 ||
               strncmp(ctx->buffer, "mul ", 4) == 0 ||
               strncmp(ctx->buffer, "div ", 4) == 0) {
        ctx->current_state = PROCESS_NUMBER;
    } else {
        printf("Invalid command\n");
        ctx->current_state = READ_COMMAND;
    }
}

void state_process_number(struct Context *ctx) {
    char *num_str = ctx->buffer + 4;
    if (*num_str == '\0') {
        printf("Missing number\n");
        ctx->current_state = READ_COMMAND;
        return;
    }
    
    for (char *p = num_str; *p != '\0'; p++) {
        if (!isdigit(*p) && *p != '-') {
            printf("Invalid number format\n");
            ctx->current_state = READ_COMMAND;
            return;
        }
    }
    
    char *endptr;
    long num = strtol(num_str, &endptr, 10);
    if (*endptr != '\0') {
        printf("Invalid number format\n");
        ctx->current_state = READ_COMMAND;
        return;
    }
    if (num < -1000000 || num > 1000000) {
        printf("Number out of range\n");
        ctx->current_state = READ_COMMAND;
        return;
    }
    
    ctx->number = (int)num;
    
    if (strncmp(ctx->buffer, "add ", 4) == 0) {
        if ((ctx->result > 0 && num > INT_MAX - ctx->result) ||
            (ctx->result < 0 && num < INT_MIN - ctx->result)) {
            printf("Addition overflow\n");
            ctx->current_state = READ_COMMAND;
            return;
        }
        ctx->result += ctx->number;
    } else if (strncmp(ctx->buffer, "sub ", 4) == 0) {
        if ((ctx->result > 0 && -num < INT_MIN + ctx->result) ||
            (ctx->result < 0 && -num > INT_MAX + ctx->result)) {
            printf("Subtraction overflow\n");
            ctx->current_state = READ_COMMAND;
            return;
        }
        ctx->result -= ctx->number;
    } else if (strncmp(ctx->buffer, "mul ", 4) == 0) {
        if (ctx->result != 0) {
            long long product = (long long)ctx->result * (long long)ctx->number;
            if (product > INT_MAX || product < INT_MIN) {
                printf("Multiplication overflow\n");
                ctx->current_state = READ_COMMAND;
                return;
            }
        }
        ctx->result *= ctx->number;
    } else if (strncmp(ctx->buffer, "div ", 4) == 0) {
        if (ctx->number == 0) {
            printf("Division by zero\n");
            ctx->current_state = READ_COMMAND;
            return;
        }
        if (ctx->result == INT_MIN && ctx->number == -1) {
            printf("Division overflow\n");
            ctx->current_state = READ_COMMAND;
            return;
        }
        ctx->result /= ctx->number;
    }
    
    ctx->current_state = DISPLAY_RESULT;
}

void state_display_result(struct Context *ctx) {
    printf("Result: %d\n", ctx->result);
    ctx->current_state = READ_COMMAND;
}

int main(void) {
    struct Context ctx;
    ctx.current_state = INIT;
    ctx.result = 0;
    ctx.number = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    while (ctx.current_state != EXIT) {
        switch (ctx.current_state) {
            case INIT:
                state_init(&ctx);
                break;
            case READ_COMMAND:
                state_read_command(&ctx);
                break;
            case PROCESS_NUMBER:
                state_process_number(&ctx);
                break;
            case DISPLAY_RESULT:
                state_display_result(&ctx);
                break;
            case ERROR:
                printf("Error occurred\n");
                ctx.current_state = READ_COMMAND;
                break;
            case EXIT:
                break;
        }
    }
    
    printf("Goodbye