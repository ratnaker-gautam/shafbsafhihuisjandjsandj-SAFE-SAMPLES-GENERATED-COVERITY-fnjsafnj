//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    INIT,
    READ_COMMAND,
    PROCESS_NUMBER,
    PROCESS_TEXT,
    EXIT,
    ERROR
};

struct Context {
    enum State current_state;
    int number;
    char text[64];
    int text_len;
};

void state_init(struct Context *ctx) {
    ctx->current_state = READ_COMMAND;
    ctx->number = 0;
    ctx->text_len = 0;
    memset(ctx->text, 0, sizeof(ctx->text));
}

void state_read_command(struct Context *ctx) {
    char input[16];
    printf("Enter command (number/text/exit): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    if (strcmp(input, "number") == 0) {
        ctx->current_state = PROCESS_NUMBER;
    } else if (strcmp(input, "text") == 0) {
        ctx->current_state = PROCESS_TEXT;
    } else if (strcmp(input, "exit") == 0) {
        ctx->current_state = EXIT;
    } else {
        printf("Invalid command\n");
        ctx->current_state = READ_COMMAND;
    }
}

void state_process_number(struct Context *ctx) {
    char input[16];
    printf("Enter a number (0-100): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    char *endptr;
    long num = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\n') {
        printf("Invalid number format\n");
        ctx->current_state = READ_COMMAND;
        return;
    }
    
    if (num < 0 || num > 100) {
        printf("Number out of range\n");
        ctx->current_state = READ_COMMAND;
        return;
    }
    
    ctx->number = (int)num;
    printf("Number processed: %d\n", ctx->number);
    ctx->current_state = READ_COMMAND;
}

void state_process_text(struct Context *ctx) {
    char input[128];
    printf("Enter text (max 63 chars): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    size_t len = strlen(input);
    if (len >= sizeof(ctx->text)) {
        printf("Text too long\n");
        ctx->current_state = READ_COMMAND;
        return;
    }
    
    strncpy(ctx->text, input, sizeof(ctx->text) - 1);
    ctx->text[sizeof(ctx->text) - 1] = '\0';
    ctx->text_len = (int)len;
    
    printf("Text processed: %s (length: %d)\n", ctx->text, ctx->text_len);
    ctx->current_state = READ_COMMAND;
}

void state_exit(struct Context *ctx) {
    printf("Goodbye!\n");
}

void state_error(struct Context *ctx) {
    printf("An error occurred\n");
    ctx->current_state = EXIT;
}

int main(void) {
    struct Context ctx;
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