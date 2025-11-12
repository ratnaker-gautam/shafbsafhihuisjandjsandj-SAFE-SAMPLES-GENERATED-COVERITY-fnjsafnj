//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
    ctx->current_state = READ_COMMAND;
    ctx->number = 0;
    ctx->result = 0;
    memset(ctx->command, 0, sizeof(ctx->command));
}

void state_read_command(struct Context *ctx) {
    printf("Enter command (square/cube/double/exit): ");
    if (fgets(ctx->command, sizeof(ctx->command), stdin) == NULL) {
        ctx->current_state = ERROR_STATE;
        return;
    }
    
    size_t len = strlen(ctx->command);
    if (len > 0 && ctx->command[len-1] == '\n') {
        ctx->command[len-1] = '\0';
    }
    
    if (strcmp(ctx->command, "exit") == 0) {
        ctx->current_state = EXIT_STATE;
    } else if (strcmp(ctx->command, "square") == 0 || 
               strcmp(ctx->command, "cube") == 0 || 
               strcmp(ctx->command, "double") == 0) {
        ctx->current_state = PROCESS_NUMBER;
    } else {
        ctx->current_state = ERROR_STATE;
    }
}

void state_process_number(struct Context *ctx) {
    printf("Enter a number (0-100): ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        ctx->current_state = ERROR_STATE;
        return;
    }
    
    char *endptr;
    long num = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') {
        ctx->current_state = ERROR_STATE;
        return;
    }
    
    if (num < 0 || num > 100) {
        ctx->current_state = ERROR_STATE;
        return;
    }
    
    ctx->number = (int)num;
    
    if (strcmp(ctx->command, "square") == 0) {
        if (ctx->number > 46340) {
            ctx->current_state = ERROR_STATE;
            return;
        }
        ctx->result = ctx->number * ctx->number;
    } else if (strcmp(ctx->command, "cube") == 0) {
        if (ctx->number > 1290) {
            ctx->current_state = ERROR_STATE;
            return;
        }
        ctx->result = ctx->number * ctx->number * ctx->number;
    } else if (strcmp(ctx->command, "double") == 0) {
        if (ctx->number > 1073741823) {
            ctx->current_state = ERROR_STATE;
            return;
        }
        ctx->result = ctx->number * 2;
    }
    
    ctx->current_state = DISPLAY_RESULT;
}

void state_display_result(struct Context *ctx) {
    printf("Result: %d\n\n", ctx->result);
    ctx->current_state = READ_COMMAND;
}

void state_error(struct Context *ctx) {
    printf("Error: Invalid input. Please try again.\n\n");
    ctx->current_state = READ_COMMAND;
}

int main(void) {
    struct Context ctx;
    state_init(&ctx);
    
    printf("Number Processor State Machine\n");
    printf("==============================\n\n");
    
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