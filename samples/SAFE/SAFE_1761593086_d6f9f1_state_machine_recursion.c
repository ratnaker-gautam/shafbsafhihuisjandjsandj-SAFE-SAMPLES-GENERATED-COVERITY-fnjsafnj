//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    START,
    READ_CHAR,
    COUNT_DIGITS,
    COUNT_LETTERS,
    FINISH,
    ERROR
};

struct Context {
    enum State current_state;
    int digit_count;
    int letter_count;
    int position;
    char input[256];
};

enum State handle_start(struct Context *ctx) {
    if (ctx->position >= (int)strlen(ctx->input)) {
        return FINISH;
    }
    char c = ctx->input[ctx->position];
    if (c >= '0' && c <= '9') {
        return COUNT_DIGITS;
    } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
        return COUNT_LETTERS;
    } else {
        return ERROR;
    }
}

enum State handle_read_char(struct Context *ctx) {
    ctx->position++;
    if (ctx->position >= (int)strlen(ctx->input)) {
        return FINISH;
    }
    return START;
}

enum State handle_count_digits(struct Context *ctx) {
    char c = ctx->input[ctx->position];
    if (c >= '0' && c <= '9') {
        ctx->digit_count++;
        return handle_read_char(ctx);
    }
    return ERROR;
}

enum State handle_count_letters(struct Context *ctx) {
    char c = ctx->input[ctx->position];
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
        ctx->letter_count++;
        return handle_read_char(ctx);
    }
    return ERROR;
}

void process_state_machine(struct Context *ctx) {
    if (ctx->current_state == FINISH || ctx->current_state == ERROR) {
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
        case COUNT_DIGITS:
            next_state = handle_count_digits(ctx);
            break;
        case COUNT_LETTERS:
            next_state = handle_count_letters(ctx);
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
    ctx.digit_count = 0;
    ctx.letter_count = 0;
    ctx.position = 0;

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

    printf("Digits: %d\n", ctx.digit_count);
    printf("Letters: %d\n", ctx.letter_count);

    return 0;
}