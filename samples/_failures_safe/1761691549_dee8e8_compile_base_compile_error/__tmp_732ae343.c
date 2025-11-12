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
    ctx->number = 0;
    ctx->result = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

void state_read_command(struct Context *ctx) {
    printf("Enter command: ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    size_t len = strlen(ctx->buffer);
    if (len > 0 && ctx->buffer[len - 1] == '\n') {
        ctx->buffer[len - 1] = '\0';
    }
    
    if (strcmp(ctx->buffer, "exit") == 0) {
        ctx->current_state = EXIT;
    } else if (strncmp(ctx->buffer, "add ", 4) == 0 ||
               strncmp(ctx->buffer, "sub ", 4) == 0 ||
               strncmp(ctx->buffer, "mul ", 4) == 0 ||
               strncmp(ctx->buffer, "div ", 4) == 0) {
        ctx->current_state = PROCESS_NUMBER;
    } else {
        ctx->current_state = ERROR;
    }
}

void state_process_number(struct Context *ctx) {
    char *num_str = ctx->buffer + 4;
    if (*num_str == '\0') {
        ctx->current_state = ERROR;
        return;
    }
    
    for (char *p = num_str; *p; p++) {
        if (!isdigit(*p) && *p != '-' && *p != '+') {
            ctx->current_state = ERROR;
            return;
        }
    }
    
    char *endptr;
    long num = strtol(num_str, &endptr, 10);
    if (endptr == num_str || *endptr != '\0') {
        ctx->current_state = ERROR;
        return;
    }
    if (num < -1000000 || num > 1000000 || num < INT_MIN || num > INT_MAX) {
        ctx->current_state = ERROR;
        return;
    }
    
    ctx->number = (int)num;
    ctx->current_state = DISPLAY_RESULT;
}

void state_display_result(struct Context *ctx) {
    char operation = ctx->buffer[0];
    int prev_result = ctx->result;
    
    switch (operation) {
        case 'a':
            if ((ctx->number > 0 && ctx->result > INT_MAX - ctx->number) ||
                (ctx->number < 0 && ctx->result < INT_MIN - ctx->number)) {
                ctx->current_state = ERROR;
                return;
            }
            ctx->result += ctx->number;
            break;
        case 's':
            if ((ctx->number < 0 && ctx->result > INT_MAX + ctx->number) ||
                (ctx->number > 0 && ctx->result < INT_MIN + ctx->number)) {
                ctx->current_state = ERROR;
                return;
            }
            ctx->result -= ctx->number;
            break;
        case 'm':
            if (ctx->number == 0) {
                ctx->result = 0;
                break;
            }
            if (ctx->result > 0) {
                if (ctx->number > 0) {
                    if (ctx->result > INT_MAX / ctx->number) {
                        ctx->current_state = ERROR;
                        return;
                    }
                } else {
                    if (ctx->number < INT_MIN / ctx->result) {
                        ctx->current_state = ERROR;
                        return;
                    }
                }
            } else if (ctx->result < 0) {
                if (ctx->number > 0) {
                    if (ctx->result < INT_MIN / ctx->number) {
                        ctx->current_state = ERROR;
                        return;
                    }
                } else {
                    if (ctx->result != 0 && ctx->number < INT_MAX / ctx->result) {
                        ctx->current_state = ERROR;
                        return;
                    }
                }
            }
            ctx->result *= ctx->number;
            break;
        case 'd':
            if (ctx->number == 0) {
                ctx->current_state = ERROR;
                return;
            }
            if (ctx->result == INT_MIN && ctx->number == -1) {
                ctx->current_state = ERROR;
                return;
            }
            ctx->result /= ctx->number;
            break;
        default:
            ctx->current_state = ERROR;
            return;
    }
    
    printf("Operation: %c %d, Previous: %d, Current: %d\n", 
           operation, ctx->number, prev_result, ctx->result);
    ctx->current_state = READ_COMMAND;
}

void state_error(struct Context *ctx) {
    printf("Error: Invalid input or operation\n");
    ctx->current_state = READ_COMMAND;
}

int main(void) {
    struct Context ctx;
    state_init(&ctx);
    
    while (ctx.current_state != EXIT) {
        switch