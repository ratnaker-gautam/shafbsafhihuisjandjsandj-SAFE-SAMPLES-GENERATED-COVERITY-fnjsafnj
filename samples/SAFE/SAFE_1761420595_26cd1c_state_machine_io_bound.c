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
    printf("State Machine Demo\n");
    printf("Commands: number <num>, text <str>, exit\n");
    ctx->current_state = READ_COMMAND;
}

void state_read_command(struct Context *ctx) {
    char input[128];
    printf("> ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        ctx->current_state = EXIT;
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strncmp(input, "number", 6) == 0) {
        ctx->current_state = PROCESS_NUMBER;
    } else if (strncmp(input, "text", 4) == 0) {
        ctx->current_state = PROCESS_TEXT;
    } else if (strncmp(input, "exit", 4) == 0) {
        ctx->current_state = EXIT;
    } else {
        printf("Unknown command\n");
        ctx->current_state = READ_COMMAND;
    }
}

void state_process_number(struct Context *ctx) {
    char input[128];
    printf("Enter number: ");
    
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
    printf("Stored number: %d\n", ctx->number);
    ctx->current_state = READ_COMMAND;
}

void state_process_text(struct Context *ctx) {
    char input[128];
    printf("Enter text: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        ctx->current_state = EXIT;
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    size_t len = strlen(input);
    if (len == 0 || len >= sizeof(ctx->text)) {
        printf("Invalid text length\n");
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
    printf("Stored text: %s\n", ctx->text);
    ctx->current_state = READ_COMMAND;
}

void state_exit(struct Context *ctx) {
    printf("Final number: %d\n", ctx->number);
    printf("Final text: %s\n", ctx->text);
    printf("Goodbye!\n");
}

void state_error(struct Context *ctx) {
    printf("Error occurred\n");
    ctx->current_state = EXIT;
}

int main(void) {
    struct Context ctx = {0};
    ctx.current_state = INIT;
    
    while (ctx.current_state != EXIT && ctx.current_state != ERROR) {
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
            case EXIT:
                state_exit(&ctx);
                break;
            case ERROR:
                state_error(&ctx);
                break;
        }
    }
    
    return 0;
}