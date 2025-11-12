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
    printf("Calculator State Machine\n");
    printf("Commands: add, sub, mul, div, quit\n");
    ctx->current_state = READ_COMMAND;
}

void state_read_command(struct Context *ctx) {
    printf("Enter command: ");
    if (fgets(ctx->command, sizeof(ctx->command), stdin) == NULL) {
        ctx->current_state = EXIT;
        return;
    }
    
    size_t len = strlen(ctx->command);
    if (len > 0 && ctx->command[len - 1] == '\n') {
        ctx->command[len - 1] = '\0';
    }
    
    for (size_t i = 0; i < len && ctx->command[i] != '\0'; i++) {
        ctx->command[i] = tolower((unsigned char)ctx->command[i]);
    }
    
    if (strcmp(ctx->command, "quit") == 0) {
        ctx->current_state = EXIT;
    } else if (strcmp(ctx->command, "add") == 0 || 
               strcmp(ctx->command, "sub") == 0 ||
               strcmp(ctx->command, "mul") == 0 ||
               strcmp(ctx->command, "div") == 0) {
        ctx->current_state = PROCESS_NUMBER;
    } else {
        printf("Invalid command\n");
        ctx->current_state = READ_COMMAND;
    }
}

void state_process_number(struct Context *ctx) {
    printf("Enter number: ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        ctx->current_state = EXIT;
        return;
    }
    
    char *endptr;
    long num = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') {
        printf("Invalid number\n");
        ctx->current_state = READ_COMMAND;
        return;
    }
    
    if (num < -1000000 || num > 1000000) {
        printf("Number out of range\n");
        ctx->current_state = READ_COMMAND;
        return;
    }
    
    ctx->number = (int)num;
    ctx->current_state = DISPLAY_RESULT;
}

void state_display_result(struct Context *ctx) {
    if (strcmp(ctx->command, "add") == 0) {
        if ((ctx->result > 0 && ctx->number > INT_MAX - ctx->result) ||
            (ctx->result < 0 && ctx->number < INT_MIN - ctx->result)) {
            printf("Addition overflow\n");
            ctx->current_state = READ_COMMAND;
            return;
        }
        ctx->result += ctx->number;
    } else if (strcmp(ctx->command, "sub") == 0) {
        if ((ctx->number > 0 && ctx->result < INT_MIN + ctx->number) ||
            (ctx->number < 0 && ctx->result > INT_MAX + ctx->number)) {
            printf("Subtraction overflow\n");
            ctx->current_state = READ_COMMAND;
            return;
        }
        ctx->result -= ctx->number;
    } else if (strcmp(ctx->command, "mul") == 0) {
        if (ctx->number != 0) {
            if (ctx->result > 0) {
                if (ctx->number > 0) {
                    if (ctx->result > INT_MAX / ctx->number) {
                        printf("Multiplication overflow\n");
                        ctx->current_state = READ_COMMAND;
                        return;
                    }
                } else {
                    if (ctx->number < INT_MIN / ctx->result) {
                        printf("Multiplication overflow\n");
                        ctx->current_state = READ_COMMAND;
                        return;
                    }
                }
            } else if (ctx->result < 0) {
                if (ctx->number > 0) {
                    if (ctx->result < INT_MIN / ctx->number) {
                        printf("Multiplication overflow\n");
                        ctx->current_state = READ_COMMAND;
                        return;
                    }
                } else {
                    if (ctx->number != -1 && ctx->result < INT_MAX / ctx->number) {
                        printf("Multiplication overflow\n");
                        ctx->current_state = READ_COMMAND;
                        return;
                    }
                }
            }
        }
        ctx->result *= ctx->number;
    } else if (strcmp(ctx->command, "div") == 0) {
        if (ctx->number == 0) {
            printf("Division by zero\n");
            ctx->current_state = READ_COMMAND;
            return;
        }
        if (ctx->number == -1 && ctx->result == INT_MIN) {
            printf("Division overflow\n");
            ctx->current_state = READ_COMMAND;
            return;
        }
        ctx->result /= ctx->number;
    }
    
    printf("Current result: %d\n", ctx->result);
    ctx->current_state = READ_COMMAND;
}

int main(void) {
    struct Context ctx;
    ctx.current_state = INIT;
    ctx.result = 0;
    ctx.number = 0;
    memset(ctx.command, 0, sizeof(ctx.command));
    
    while (ctx