//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: state_machine
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
    char input[16];
    printf("Enter command (number/text/quit): ");
    
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
    char input[32];
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
        printf("Number out of range (0-100)\n");
        ctx->current_state = READ_COMMAND;
        return;
    }
    
    ctx->number = (int)num;
    printf("Number processed: %d\n", ctx->number);
    ctx->current_state = READ_COMMAND;
}

void state_process_text(struct context *ctx) {
    char input[128];
    printf("Enter text (max 63 chars): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    size_t len = strlen(input);
    if (len == 0) {
        printf("Empty text not allowed\n");
        ctx->current_state = READ_COMMAND;
        return;
    }
    
    if (len >= sizeof(ctx->text)) {
        printf("Text too long\n");
        ctx->current_state = READ_COMMAND;
        return;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)input[i])) {
            printf("Invalid characters in text\n");
            ctx->current_state = READ_COMMAND;
            return;
        }
    }
    
    strncpy(ctx->text, input, sizeof(ctx->text) - 1);
    ctx->text[sizeof(ctx->text) - 1] = '\0';
    ctx->text_len = (int)len;
    printf("Text processed: %s\n", ctx->text);
    ctx->current_state = READ_COMMAND;
}

int main(void) {
    struct context ctx;
    state_init(&ctx);
    
    while (1) {
        switch (ctx.current_state) {
            case READ_COMMAND:
                state_read_command(&ctx);
                break;
            case PROCESS_NUMBER:
                state_process_number(&ctx);
                break;
            case PROCESS_TEXT:
                state_process_text(&ctx);
                break;
            case EXIT:
                printf("Goodbye!\n");
                return 0;
            case ERROR:
                printf("An error occurred\n");
                return 1;
            case INIT:
            default:
                ctx.current_state = ERROR;
                break;
        }
        
        if (ctx.current_state == ERROR) {
            printf("State machine error\n");
            return 1;
        }
    }
    
    return 0;
}