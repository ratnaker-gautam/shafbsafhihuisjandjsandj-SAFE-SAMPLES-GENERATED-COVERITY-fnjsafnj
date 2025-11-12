//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    START,
    READ_CHAR,
    PROCESS_CHAR,
    COUNT_DIGITS,
    COUNT_LETTERS,
    FINISH,
    ERROR
};

struct Context {
    char input[256];
    size_t position;
    int digit_count;
    int letter_count;
    enum State current_state;
};

int is_valid_char(char c) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '\0';
}

enum State handle_start(struct Context *ctx) {
    if (ctx->position >= sizeof(ctx->input)) {
        return ERROR;
    }
    return READ_CHAR;
}

enum State handle_read_char(struct Context *ctx) {
    if (ctx->position >= sizeof(ctx->input)) {
        return ERROR;
    }
    
    char current = ctx->input[ctx->position];
    
    if (!is_valid_char(current)) {
        return ERROR;
    }
    
    if (current == '\0') {
        return FINISH;
    }
    
    return PROCESS_CHAR;
}

enum State handle_process_char(struct Context *ctx) {
    if (ctx->position >= sizeof(ctx->input)) {
        return ERROR;
    }
    
    char current = ctx->input[ctx->position];
    
    if (current >= '0' && current <= '9') {
        return COUNT_DIGITS;
    } else if ((current >= 'a' && current <= 'z') || (current >= 'A' && current <= 'Z')) {
        return COUNT_LETTERS;
    }
    
    return ERROR;
}

enum State handle_count_digits(struct Context *ctx) {
    if (ctx->digit_count < 0 || ctx->digit_count > 1000) {
        return ERROR;
    }
    
    ctx->digit_count++;
    ctx->position++;
    
    return READ_CHAR;
}

enum State handle_count_letters(struct Context *ctx) {
    if (ctx->letter_count < 0 || ctx->letter_count > 1000) {
        return ERROR;
    }
    
    ctx->letter_count++;
    ctx->position++;
    
    return READ_CHAR;
}

void process_state_machine(struct Context *ctx) {
    if (ctx == NULL) {
        return;
    }
    
    switch (ctx->current_state) {
        case START:
            ctx->current_state = handle_start(ctx);
            break;
        case READ_CHAR:
            ctx->current_state = handle_read_char(ctx);
            break;
        case PROCESS_CHAR:
            ctx->current_state = handle_process_char(ctx);
            break;
        case COUNT_DIGITS:
            ctx->current_state = handle_count_digits(ctx);
            break;
        case COUNT_LETTERS:
            ctx->current_state = handle_count_letters(ctx);
            break;
        case FINISH:
            return;
        case ERROR:
            return;
        default:
            ctx->current_state = ERROR;
            return;
    }
    
    if (ctx->current_state != FINISH && ctx->current_state != ERROR) {
        process_state_machine(ctx);
    }
}

int main(void) {
    struct Context ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    ctx.current_state = START;
    ctx.digit_count = 0;
    ctx.letter_count = 0;
    ctx.position = 0;
    
    printf("Enter a string (letters and digits only, max 255 chars): ");
    
    if (fgets(ctx.input, sizeof(ctx.input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(ctx.input);
    if (len > 0 && ctx.input[len - 1] == '\n') {
        ctx.input[len - 1] = '\0';
    }
    
    if (len >= sizeof(ctx.input)) {
        printf("Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (!is_valid_char(ctx.input[i])) {
            printf("Invalid character in input\n");
            return 1;
        }
    }
    
    process_state_machine(&ctx);
    
    if (ctx.current_state == ERROR) {
        printf("Error processing input\n");
        return 1;
    }
    
    printf("Digits: %d\n", ctx.digit_count);
    printf("Letters: %d\n", ctx.letter_count);
    
    return 0;
}