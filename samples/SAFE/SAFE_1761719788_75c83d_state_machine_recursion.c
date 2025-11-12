//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    START,
    READ_CHAR,
    PROCESS_DIGIT,
    PROCESS_LETTER,
    END,
    ERROR
};

struct Context {
    enum State current_state;
    char input[256];
    int position;
    int digit_count;
    int letter_count;
};

enum State handle_start(struct Context *ctx) {
    if (ctx->position >= (int)strlen(ctx->input)) {
        return END;
    }
    char c = ctx->input[ctx->position];
    if (c >= '0' && c <= '9') {
        return PROCESS_DIGIT;
    } else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        return PROCESS_LETTER;
    } else {
        return ERROR;
    }
}

enum State handle_read_char(struct Context *ctx) {
    ctx->position++;
    if (ctx->position >= (int)strlen(ctx->input)) {
        return END;
    }
    char c = ctx->input[ctx->position];
    if (c >= '0' && c <= '9') {
        return PROCESS_DIGIT;
    } else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        return PROCESS_LETTER;
    } else {
        return ERROR;
    }
}

enum State handle_process_digit(struct Context *ctx) {
    ctx->digit_count++;
    return READ_CHAR;
}

enum State handle_process_letter(struct Context *ctx) {
    ctx->letter_count++;
    return READ_CHAR;
}

enum State handle_error(struct Context *ctx) {
    return ERROR;
}

enum State handle_end(struct Context *ctx) {
    return END;
}

void process_state_machine(struct Context *ctx) {
    if (ctx == NULL) return;
    
    switch (ctx->current_state) {
        case START:
            ctx->current_state = handle_start(ctx);
            break;
        case READ_CHAR:
            ctx->current_state = handle_read_char(ctx);
            break;
        case PROCESS_DIGIT:
            ctx->current_state = handle_process_digit(ctx);
            break;
        case PROCESS_LETTER:
            ctx->current_state = handle_process_letter(ctx);
            break;
        case ERROR:
            ctx->current_state = handle_error(ctx);
            break;
        case END:
            ctx->current_state = handle_end(ctx);
            break;
    }
    
    if (ctx->current_state != END && ctx->current_state != ERROR) {
        process_state_machine(ctx);
    }
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.position = 0;
    ctx.digit_count = 0;
    ctx.letter_count = 0;
    
    printf("Enter a string (max 255 characters): ");
    if (fgets(ctx.input, sizeof(ctx.input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(ctx.input);
    if (len > 0 && ctx.input[len - 1] == '\n') {
        ctx.input[len - 1] = '\0';
    }
    
    if (strlen(ctx.input) == 0) {
        printf("Empty input string\n");
        return 1;
    }
    
    process_state_machine(&ctx);
    
    if (ctx.current_state == ERROR) {
        printf("Error: Invalid character encountered at position %d\n", ctx.position);
        return 1;
    }
    
    printf("Processing complete:\n");
    printf("Digits: %d\n", ctx.digit_count);
    printf("Letters: %d\n", ctx.letter_count);
    
    return 0;
}