//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    INIT,
    READ_COMMAND,
    PROCESS_NUMBER,
    DISPLAY_RESULT,
    ERROR_STATE,
    EXIT_STATE
};

struct context {
    enum state current_state;
    int number;
    int result;
    char buffer[256];
    size_t buffer_pos;
};

void state_init(struct context *ctx) {
    ctx->current_state = READ_COMMAND;
    ctx->number = 0;
    ctx->result = 0;
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    printf("Calculator State Machine\n");
    printf("Commands: 'number X', 'square', 'double', 'exit'\n");
    printf("Enter command: ");
}

void state_read_command(struct context *ctx) {
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        ctx->current_state = EXIT_STATE;
        return;
    }
    
    size_t len = strlen(ctx->buffer);
    if (len > 0 && ctx->buffer[len - 1] == '\n') {
        ctx->buffer[len - 1] = '\0';
    }
    
    if (strcmp(ctx->buffer, "exit") == 0) {
        ctx->current_state = EXIT_STATE;
    } else if (strncmp(ctx->buffer, "number ", 7) == 0) {
        char *num_str = ctx->buffer + 7;
        int valid = 1;
        for (size_t i = 0; num_str[i] != '\0'; i++) {
            if (!isdigit(num_str[i]) && !(i == 0 && num_str[i] == '-')) {
                valid = 0;
                break;
            }
        }
        if (valid) {
            long num = strtol(num_str, NULL, 10);
            if (num >= -1000000 && num <= 1000000) {
                ctx->number = (int)num;
                ctx->current_state = PROCESS_NUMBER;
            } else {
                ctx->current_state = ERROR_STATE;
            }
        } else {
            ctx->current_state = ERROR_STATE;
        }
    } else if (strcmp(ctx->buffer, "square") == 0) {
        ctx->current_state = PROCESS_NUMBER;
    } else if (strcmp(ctx->buffer, "double") == 0) {
        ctx->current_state = PROCESS_NUMBER;
    } else {
        ctx->current_state = ERROR_STATE;
    }
}

void state_process_number(struct context *ctx) {
    if (strcmp(ctx->buffer, "square") == 0) {
        long long temp = (long long)ctx->number * (long long)ctx->number;
        if (temp > 1000000 || temp < -1000000) {
            ctx->current_state = ERROR_STATE;
            return;
        }
        ctx->result = (int)temp;
        ctx->current_state = DISPLAY_RESULT;
    } else if (strcmp(ctx->buffer, "double") == 0) {
        long long temp = (long long)ctx->number * 2LL;
        if (temp > 1000000 || temp < -1000000) {
            ctx->current_state = ERROR_STATE;
            return;
        }
        ctx->result = (int)temp;
        ctx->current_state = DISPLAY_RESULT;
    } else {
        ctx->current_state = DISPLAY_RESULT;
    }
}

void state_display_result(struct context *ctx) {
    if (strcmp(ctx->buffer, "square") == 0) {
        printf("Square of %d is %d\n", ctx->number, ctx->result);
    } else if (strcmp(ctx->buffer, "double") == 0) {
        printf("Double of %d is %d\n", ctx->number, ctx->result);
    } else {
        printf("Number set to: %d\n", ctx->number);
    }
    printf("Enter command: ");
    ctx->current_state = READ_COMMAND;
}

void state_error(struct context *ctx) {
    printf("Error: Invalid command or number out of range\n");
    printf("Enter command: ");
    ctx->current_state = READ_COMMAND;
}

int main(void) {
    struct context ctx;
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
    
    printf("Goodbye!\n");
    return 0;
}