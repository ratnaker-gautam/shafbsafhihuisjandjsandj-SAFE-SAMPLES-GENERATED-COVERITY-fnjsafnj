//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    INIT,
    READ_COMMAND,
    PROCESS_NUMBER,
    PROCESS_TEXT,
    EXIT,
    ERROR
};

struct context {
    enum state current_state;
    int number;
    char text[64];
    int text_len;
};

void state_init(struct context *ctx) {
    ctx->current_state = READ_COMMAND;
    ctx->number = 0;
    ctx->text_len = 0;
    memset(ctx->text, 0, sizeof(ctx->text));
}

void state_read_command(struct context *ctx) {
    printf("Enter command (number, text, quit): ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    if (strcmp(input, "number") == 0) {
        ctx->current_state = PROCESS_NUMBER;
    } else if (strcmp(input, "text") == 0) {
        ctx->current_state = PROCESS_TEXT;
    } else if (strcmp(input, "quit") == 0) {
        ctx->current_state = EXIT;
    } else {
        printf("Invalid command\n");
        ctx->current_state = READ_COMMAND;
    }
}

void state_process_number(struct context *ctx) {
    printf("Enter a number (0-100): ");
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    char *endptr;
    long val = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') {
        printf("Invalid number format\n");
        ctx->current_state = READ_COMMAND;
        return;
    }
    
    if (val < 0 || val > 100) {
        printf("Number out of range\n");
        ctx->current_state = READ_COMMAND;
        return;
    }
    
    ctx->number = (int)val;
    printf("Stored number: %d\n", ctx->number);
    ctx->current_state = READ_COMMAND;
}

void state_process_text(struct context *ctx) {
    printf("Enter text (max 63 chars): ");
    if (fgets(ctx->text, sizeof(ctx->text), stdin) == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    ctx->text_len = strcspn(ctx->text, "\n");
    ctx->text[ctx->text_len] = 0;
    
    if (ctx->text_len == 0) {
        printf("Empty text not allowed\n");
        ctx->current_state = READ_COMMAND;
        return;
    }
    
    printf("Stored text: %s\n", ctx->text);
    ctx->current_state = READ_COMMAND;
}

void state_exit(struct context *ctx) {
    printf("Final number: %d\n", ctx->number);
    printf("Final text: %s\n", ctx->text);
    printf("Goodbye!\n");
}

void state_error(struct context *ctx) {
    printf("An error occurred\n");
    ctx->current_state = EXIT;
}

int main(void) {
    struct context ctx;
    state_init(&ctx);
    
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
            case PROCESS_TEXT:
                state_process_text(&ctx);
                break;
            case ERROR:
                state_error(&ctx);
                break;
            case EXIT:
                break;
        }
    }
    
    return 0;
}