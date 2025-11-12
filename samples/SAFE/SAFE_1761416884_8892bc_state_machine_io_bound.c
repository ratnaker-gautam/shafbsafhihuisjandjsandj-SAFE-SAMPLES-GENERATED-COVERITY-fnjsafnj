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
    char buffer[64];
};

void state_init(struct context *ctx) {
    ctx->current_state = READ_COMMAND;
    ctx->number = 0;
    ctx->result = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

void state_read_command(struct context *ctx) {
    printf("Enter command (number, reset, exit): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        ctx->current_state = ERROR_STATE;
        return;
    }
    
    size_t len = strlen(ctx->buffer);
    if (len > 0 && ctx->buffer[len - 1] == '\n') {
        ctx->buffer[len - 1] = '\0';
    }
    
    if (strcmp(ctx->buffer, "exit") == 0) {
        ctx->current_state = EXIT_STATE;
    } else if (strcmp(ctx->buffer, "reset") == 0) {
        ctx->current_state = INIT;
    } else {
        ctx->current_state = PROCESS_NUMBER;
    }
}

void state_process_number(struct context *ctx) {
    char *endptr;
    long val = strtol(ctx->buffer, &endptr, 10);
    
    if (endptr == ctx->buffer || *endptr != '\0') {
        ctx->current_state = ERROR_STATE;
        return;
    }
    
    if (val < -1000 || val > 1000) {
        ctx->current_state = ERROR_STATE;
        return;
    }
    
    ctx->number = (int)val;
    ctx->result = ctx->number * 2;
    ctx->current_state = DISPLAY_RESULT;
}

void state_display_result(struct context *ctx) {
    printf("Result: %d\n", ctx->result);
    ctx->current_state = READ_COMMAND;
}

void state_error(struct context *ctx) {
    printf("Error: Invalid input\n");
    ctx->current_state = READ_COMMAND;
}

int main(void) {
    struct context ctx;
    state_init(&ctx);
    
    while (ctx.current_state != EXIT_STATE) {
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
            case ERROR_STATE:
                state_error(&ctx);
                break;
            case EXIT_STATE:
                break;
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}