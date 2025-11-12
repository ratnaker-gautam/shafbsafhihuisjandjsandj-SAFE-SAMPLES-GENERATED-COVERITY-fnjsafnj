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
    printf("Calculator started. Commands: 'add X', 'sub X', 'mul X', 'div X', 'exit'\n");
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
    
    if (strlen(ctx->command) == 0) {
        ctx->current_state = ERROR_STATE;
        return;
    }
    
    if (strcmp(ctx->command, "exit") == 0) {
        ctx->current_state = EXIT_STATE;
        return;
    }
    
    ctx->current_state = PROCESS_NUMBER;
}

void state_process_number(struct Context *ctx) {
    char op[8];
    int num;
    int parsed = sscanf(ctx->command, "%7s %d", op, &num);
    
    if (parsed != 2) {
        ctx->current_state = ERROR_STATE;
        return;
    }
    
    if (strcmp(op, "add") == 0) {
        if ((num > 0 && ctx->result > INT_MAX - num) || 
            (num < 0 && ctx->result < INT_MIN - num)) {
            ctx->current_state = ERROR_STATE;
            return;
        }
        ctx->result += num;
    } else if (strcmp(op, "sub") == 0) {
        if ((num < 0 && ctx->result > INT_MAX + num) || 
            (num > 0 && ctx->result < INT_MIN + num)) {
            ctx->current_state = ERROR_STATE;
            return;
        }
        ctx->result -= num;
    } else if (strcmp(op, "mul") == 0) {
        if (ctx->result != 0 && num != 0) {
            if ((ctx->result > 0 && num > 0 && ctx->result > INT_MAX / num) ||
                (ctx->result > 0 && num < 0 && num < INT_MIN / ctx->result) ||
                (ctx->result < 0 && num > 0 && ctx->result < INT_MIN / num) ||
                (ctx->result < 0 && num < 0 && ctx->result < INT_MAX / num)) {
                ctx->current_state = ERROR_STATE;
                return;
            }
        }
        ctx->result *= num;
    } else if (strcmp(op, "div") == 0) {
        if (num == 0) {
            ctx->current_state = ERROR_STATE;
            return;
        }
        if (ctx->result == INT_MIN && num == -1) {
            ctx->current_state = ERROR_STATE;
            return;
        }
        ctx->result /= num;
    } else {
        ctx->current_state = ERROR_STATE;
        return;
    }
    
    ctx->current_state = DISPLAY_RESULT;
}

void state_display_result(struct Context *ctx) {
    printf("Result: %d\n", ctx->result);
    ctx->current_state = READ_COMMAND;
}

void state_error(struct Context *ctx) {
    printf("Error: Invalid command or operation\n");
    ctx->current_state = READ_COMMAND;
}

int main(void) {
    struct Context ctx;
    state_init(&ctx);
    
    while (ctx.current_state != EXIT_STATE) {
        switch (ctx.current_state) {
            case READ_COMMAND:
                state_read_command(&ctx);
                break;
            case PROCESS_NUMBER:
                state_process_number(&ctx);
                break;
            case DISPLAY_RESULT:
                state_display_result(&ctx);
                break;
            case ERROR_STATE:
                state_error(&ctx);
                break;
            case INIT:
            case EXIT_STATE:
                break;
        }
    }
    
    printf("Calculator exiting. Final result: %d\n", ctx.result);
    return 0;
}