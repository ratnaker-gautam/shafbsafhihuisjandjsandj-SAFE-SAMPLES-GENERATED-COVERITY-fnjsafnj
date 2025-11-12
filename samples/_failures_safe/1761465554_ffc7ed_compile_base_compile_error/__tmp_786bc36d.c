//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

enum state {
    INIT,
    READ_COMMAND,
    PROCESS_NUMBER,
    DISPLAY_RESULT,
    ERROR_STATE,
    EXIT_STATE
};

enum command {
    CMD_ADD,
    CMD_SUBTRACT,
    CMD_MULTIPLY,
    CMD_DIVIDE,
    CMD_UNKNOWN,
    CMD_EXIT
};

struct context {
    enum state current_state;
    int accumulator;
    int operand;
    enum command last_command;
    int error_code;
};

void initialize_context(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = INIT;
    ctx->accumulator = 0;
    ctx->operand = 0;
    ctx->last_command = CMD_UNKNOWN;
    ctx->error_code = 0;
}

enum command parse_command(const char *input) {
    if (input == NULL) return CMD_UNKNOWN;
    
    char buffer[32];
    size_t len = strlen(input);
    if (len == 0 || len >= sizeof(buffer)) return CMD_UNKNOWN;
    
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    for (size_t i = 0; i < len; i++) {
        buffer[i] = tolower((unsigned char)buffer[i]);
    }
    
    if (strcmp(buffer, "add") == 0) return CMD_ADD;
    if (strcmp(buffer, "subtract") == 0) return CMD_SUBTRACT;
    if (strcmp(buffer, "multiply") == 0) return CMD_MULTIPLY;
    if (strcmp(buffer, "divide") == 0) return CMD_DIVIDE;
    if (strcmp(buffer, "exit") == 0) return CMD_EXIT;
    
    return CMD_UNKNOWN;
}

int parse_number(const char *input, int *result) {
    if (input == NULL || result == NULL) return 0;
    
    char *endptr;
    long val = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\0') return 0;
    if (val < -1000000 || val > 1000000) return 0;
    
    *result = (int)val;
    return 1;
}

void execute_operation(struct context *ctx) {
    if (ctx == NULL) return;
    
    switch (ctx->last_command) {
        case CMD_ADD:
            if ((ctx->accumulator > 0 && ctx->operand > INT_MAX - ctx->accumulator) ||
                (ctx->accumulator < 0 && ctx->operand < INT_MIN - ctx->accumulator)) {
                ctx->error_code = 1;
                ctx->current_state = ERROR_STATE;
                return;
            }
            ctx->accumulator += ctx->operand;
            break;
            
        case CMD_SUBTRACT:
            if ((ctx->operand > 0 && ctx->accumulator < INT_MIN + ctx->operand) ||
                (ctx->operand < 0 && ctx->accumulator > INT_MAX + ctx->operand)) {
                ctx->error_code = 1;
                ctx->current_state = ERROR_STATE;
                return;
            }
            ctx->accumulator -= ctx->operand;
            break;
            
        case CMD_MULTIPLY:
            if (ctx->accumulator != 0 && ctx->operand != 0) {
                if (ctx->accumulator > INT_MAX / ctx->operand || 
                    ctx->accumulator < INT_MIN / ctx->operand) {
                    ctx->error_code = 1;
                    ctx->current_state = ERROR_STATE;
                    return;
                }
            }
            ctx->accumulator *= ctx->operand;
            break;
            
        case CMD_DIVIDE:
            if (ctx->operand == 0) {
                ctx->error_code = 2;
                ctx->current_state = ERROR_STATE;
                return;
            }
            ctx->accumulator /= ctx->operand;
            break;
            
        default:
            ctx->error_code = 3;
            ctx->current_state = ERROR_STATE;
            return;
    }
    
    ctx->current_state = DISPLAY_RESULT;
}

int main(void) {
    struct context ctx;
    char input[256];
    
    initialize_context(&ctx);
    
    printf("Simple Calculator State Machine\n");
    printf("Commands: add, subtract, multiply, divide, exit\n");
    printf("Current value: 0\n");
    
    while (ctx.current_state != EXIT_STATE) {
        switch (ctx.current_state) {
            case INIT:
                ctx.current_state = READ_COMMAND;
                break;
                
            case READ_COMMAND:
                printf("Enter command: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    ctx.current_state = EXIT_STATE;
                    break;
                }
                
                size_t len = strlen(input);
                if (len > 0 && input[len - 1] == '\n') {
                    input[len - 1] = '\0';
                }
                
                if (strlen(input) == 0) {
                    printf("Empty input. Please enter a valid command.\n");
                    break;
                }
                
                ctx.last_command = parse_command(input);
                
                if (ctx.last_command == CMD_EXIT)