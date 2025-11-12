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
    char command[32];
};

void state_init(struct Context *ctx) {
    printf("Calculator started. Commands: add, sub, mul, div, quit\n");
    ctx->current_state = READ_COMMAND;
}

void state_read_command(struct Context *ctx) {
    printf("Enter command: ");
    if (fgets(ctx->command, sizeof(ctx->command), stdin) == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    size_t len = strlen(ctx->command);
    if (len > 0 && ctx->command[len - 1] == '\n') {
        ctx->command[len - 1] = '\0';
    }
    
    if (strcmp(ctx->command, "quit") == 0) {
        ctx->current_state = EXIT;
    } else if (strcmp(ctx->command, "add") == 0 || 
               strcmp(ctx->command, "sub") == 0 ||
               strcmp(ctx->command, "mul") == 0 ||
               strcmp(ctx->command, "div") == 0) {
        ctx->current_state = PROCESS_NUMBER;
    } else {
        ctx->current_state = ERROR;
    }
}

void state_process_number(struct Context *ctx) {
    printf("Enter number: ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    char *endptr;
    long num = strtol(input, &endptr, 10);
    if (endptr == input) {
        ctx->current_state = ERROR;
        return;
    }
    
    while (*endptr != '\0') {
        if (*endptr != '\n' && *endptr != ' ' && *endptr != '\t') {
            ctx->current_state = ERROR;
            return;
        }
        endptr++;
    }
    
    if (num < -1000000 || num > 1000000) {
        ctx->current_state = ERROR;
        return;
    }
    
    if (num > INT_MAX || num < INT_MIN) {
        ctx->current_state = ERROR;
        return;
    }
    
    ctx->number = (int)num;
    ctx->current_state = DISPLAY_RESULT;
}

void state_display_result(struct Context *ctx) {
    if (strcmp(ctx->command, "add") == 0) {
        if ((ctx->result > 0 && ctx->number > INT_MAX - ctx->result) ||
            (ctx->result < 0 && ctx->number < INT_MIN - ctx->result)) {
            ctx->current_state = ERROR;
            return;
        }
        ctx->result += ctx->number;
    } else if (strcmp(ctx->command, "sub") == 0) {
        if ((ctx->number > 0 && ctx->result < INT_MIN + ctx->number) ||
            (ctx->number < 0 && ctx->result > INT_MAX + ctx->number)) {
            ctx->current_state = ERROR;
            return;
        }
        ctx->result -= ctx->number;
    } else if (strcmp(ctx->command, "mul") == 0) {
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
                if (ctx->number < INT_MAX / ctx->result) {
                    ctx->current_state = ERROR;
                    return;
                }
            }
        }
        ctx->result *= ctx->number;
    } else if (strcmp(ctx->command, "div") == 0) {
        if (ctx->number == 0) {
            ctx->current_state = ERROR;
            return;
        }
        if (ctx->result == INT_MIN && ctx->number == -1) {
            ctx->current_state = ERROR;
            return;
        }
        ctx->result /= ctx->number;
    }
    
    printf("Current result: %d\n", ctx->result);
    ctx->current_state = READ_COMMAND;
}

void state_error(struct Context *ctx) {
    printf("Error: Invalid input or operation\n");
    ctx->current_state = READ_COMMAND;
}

int main(void) {
    struct Context ctx;
    ctx.current_state = INIT;
    ctx.result = 0;
    ctx.number = 0;
    memset(ctx.command, 0, sizeof(ctx.command));
    
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
                state