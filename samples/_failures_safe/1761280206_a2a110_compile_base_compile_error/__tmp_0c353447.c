//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

enum state {
    INIT,
    READ_COMMAND,
    PROCESS_NUMBER,
    EXECUTE_OPERATION,
    DISPLAY_RESULT,
    ERROR_STATE,
    EXIT_STATE
};

struct context {
    enum state current_state;
    int accumulator;
    int operand;
    char command[32];
    int command_len;
};

void state_init(struct context *ctx) {
    ctx->current_state = INIT;
    ctx->accumulator = 0;
    ctx->operand = 0;
    memset(ctx->command, 0, sizeof(ctx->command));
    ctx->command_len = 0;
}

int is_valid_command(const char *cmd) {
    if (cmd[0] == '\0') return 0;
    for (int i = 0; cmd[i] != '\0'; i++) {
        if (!isalpha(cmd[i])) return 0;
    }
    return 1;
}

int safe_str_to_int(const char *str, int *result) {
    if (str == NULL || *str == '\0') return 0;
    char *endptr;
    long val = strtol(str, &endptr, 10);
    if (*endptr != '\0') return 0;
    if (val > 1000000 || val < -1000000) return 0;
    *result = (int)val;
    return 1;
}

void state_machine_step(struct context *ctx) {
    switch (ctx->current_state) {
        case INIT:
            printf("Calculator started. Commands: add, sub, mul, div, clear, exit\n");
            ctx->current_state = READ_COMMAND;
            break;
            
        case READ_COMMAND:
            printf("Enter command: ");
            if (fgets(ctx->command, sizeof(ctx->command), stdin) == NULL) {
                ctx->current_state = ERROR_STATE;
                break;
            }
            ctx->command_len = strlen(ctx->command);
            if (ctx->command_len > 0 && ctx->command[ctx->command_len-1] == '\n') {
                ctx->command[ctx->command_len-1] = '\0';
                ctx->command_len--;
            }
            if (ctx->command_len == 0) {
                ctx->current_state = READ_COMMAND;
                break;
            }
            if (!is_valid_command(ctx->command)) {
                ctx->current_state = ERROR_STATE;
                break;
            }
            if (strcmp(ctx->command, "exit") == 0) {
                ctx->current_state = EXIT_STATE;
                break;
            }
            if (strcmp(ctx->command, "clear") == 0) {
                ctx->accumulator = 0;
                printf("Accumulator cleared: %d\n", ctx->accumulator);
                ctx->current_state = READ_COMMAND;
                break;
            }
            ctx->current_state = PROCESS_NUMBER;
            break;
            
        case PROCESS_NUMBER:
            printf("Enter number: ");
            char num_str[32];
            if (fgets(num_str, sizeof(num_str), stdin) == NULL) {
                ctx->current_state = ERROR_STATE;
                break;
            }
            int num_len = strlen(num_str);
            if (num_len > 0 && num_str[num_len-1] == '\n') {
                num_str[num_len-1] = '\0';
                num_len--;
            }
            if (!safe_str_to_int(num_str, &ctx->operand)) {
                ctx->current_state = ERROR_STATE;
                break;
            }
            ctx->current_state = EXECUTE_OPERATION;
            break;
            
        case EXECUTE_OPERATION:
            if (strcmp(ctx->command, "add") == 0) {
                if ((ctx->operand > 0 && ctx->accumulator > INT_MAX - ctx->operand) ||
                    (ctx->operand < 0 && ctx->accumulator < INT_MIN - ctx->operand)) {
                    ctx->current_state = ERROR_STATE;
                    break;
                }
                ctx->accumulator += ctx->operand;
            } else if (strcmp(ctx->command, "sub") == 0) {
                if ((ctx->operand < 0 && ctx->accumulator > INT_MAX + ctx->operand) ||
                    (ctx->operand > 0 && ctx->accumulator < INT_MIN + ctx->operand)) {
                    ctx->current_state = ERROR_STATE;
                    break;
                }
                ctx->accumulator -= ctx->operand;
            } else if (strcmp(ctx->command, "mul") == 0) {
                if (ctx->operand != 0 && ctx->accumulator > INT_MAX / ctx->operand) {
                    ctx->current_state = ERROR_STATE;
                    break;
                }
                if (ctx->operand != 0 && ctx->accumulator < INT_MIN / ctx->operand) {
                    ctx->current_state = ERROR_STATE;
                    break;
                }
                ctx->accumulator *= ctx->operand;
            } else if (strcmp(ctx->command, "div") == 0) {
                if (ctx->operand == 0) {
                    ctx->current_state = ERROR_STATE;
                    break;
                }
                ctx->accumulator /= ctx->operand;
            } else {
                ctx->current_state = ERROR_STATE;
                break;
            }
            ctx->current_state = DISPLAY_RESULT;
            break;
            
        case DISPLAY