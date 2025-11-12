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
    } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
        return PROCESS_LETTER;
    } else {
        return ERROR;
    }
}

enum State handle_read_char(struct Context *ctx) {
    ctx->position++;
    if (ctx->position >= 256) {
        return ERROR;
    }
    return handle_start(ctx);
}

enum State handle_process_digit(struct Context *ctx) {
    ctx->digit_count++;
    return handle_read_char(ctx);
}

enum State handle_process_letter(struct Context *ctx) {
    ctx->letter_count++;
    return handle_read_char(ctx);
}

void process_state_machine(struct Context *ctx) {
    if (ctx->current_state == END || ctx->current_state == ERROR) {
        return;
    }

    enum State next_state;
    switch (ctx->current_state) {
        case START:
            next_state = handle_start(ctx);
            break;
        case READ_CHAR:
            next_state = handle_read_char(ctx);
            break;
        case PROCESS_DIGIT:
            next_state = handle_process_digit(ctx);
            break;
        case PROCESS_LETTER:
            next_state = handle_process_letter(ctx);
            break;
        default:
            next_state = ERROR;
            break;
    }

    ctx->current_state = next_state;
    process_state_machine(ctx);
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
        printf("Error: Invalid character at position %d\n", ctx.position);
        return 1;
    }
    
    printf("Processing complete:\n");
    printf("Digits: %d\n", ctx.digit_count);
    printf("Letters: %d\n", ctx.letter_count);
    
    return 0;
}