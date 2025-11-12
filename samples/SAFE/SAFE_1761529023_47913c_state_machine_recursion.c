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
    char input_buffer[256];
    size_t position;
    int digit_count;
    int letter_count;
};

enum State handle_start(struct Context *ctx) {
    if (ctx->position >= strlen(ctx->input_buffer)) {
        return END;
    }
    char c = ctx->input_buffer[ctx->position];
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
    if (ctx->position >= strlen(ctx->input_buffer)) {
        return END;
    }
    char c = ctx->input_buffer[ctx->position];
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
    if (fgets(ctx.input_buffer, sizeof(ctx.input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strlen(ctx.input_buffer);
    if (len > 0 && ctx.input_buffer[len - 1] == '\n') {
        ctx.input_buffer[len - 1] = '\0';
    }

    if (strlen(ctx.input_buffer) == 0) {
        fprintf(stderr, "Empty input string\n");
        return 1;
    }

    process_state_machine(&ctx);

    if (ctx.current_state == ERROR) {
        printf("Error: Invalid character encountered\n");
        return 1;
    }

    printf("Digits: %d\n", ctx.digit_count);
    printf("Letters: %d\n", ctx.letter_count);

    return 0;
}