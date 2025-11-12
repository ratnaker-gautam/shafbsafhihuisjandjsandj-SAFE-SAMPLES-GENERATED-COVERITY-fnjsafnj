//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    START,
    READ_CHAR,
    COUNT_DIGITS,
    PROCESS_WORD,
    FINISHED,
    ERROR
};

struct Context {
    const char *input;
    int position;
    int digit_count;
    int word_count;
    enum State current_state;
};

enum State handle_start(struct Context *ctx) {
    if (ctx->input == NULL) return ERROR;
    if (ctx->position < 0) return ERROR;
    if (ctx->input[ctx->position] == '\0') return FINISHED;
    return READ_CHAR;
}

enum State handle_read_char(struct Context *ctx) {
    char c = ctx->input[ctx->position];
    if (c >= '0' && c <= '9') {
        return COUNT_DIGITS;
    } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
        return PROCESS_WORD;
    } else if (c == ' ' || c == '\t') {
        ctx->position++;
        return READ_CHAR;
    } else if (c == '\0') {
        return FINISHED;
    } else {
        return ERROR;
    }
}

enum State handle_count_digits(struct Context *ctx) {
    char c = ctx->input[ctx->position];
    if (c >= '0' && c <= '9') {
        ctx->digit_count++;
        ctx->position++;
        return COUNT_DIGITS;
    } else if (c == ' ' || c == '\t' || c == '\0') {
        return READ_CHAR;
    } else {
        return ERROR;
    }
}

enum State handle_process_word(struct Context *ctx) {
    char c = ctx->input[ctx->position];
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
        ctx->position++;
        return PROCESS_WORD;
    } else if (c == ' ' || c == '\t' || c == '\0') {
        ctx->word_count++;
        return READ_CHAR;
    } else {
        return ERROR;
    }
}

void state_machine(struct Context *ctx) {
    if (ctx == NULL) return;
    
    switch (ctx->current_state) {
        case START:
            ctx->current_state = handle_start(ctx);
            break;
        case READ_CHAR:
            ctx->current_state = handle_read_char(ctx);
            break;
        case COUNT_DIGITS:
            ctx->current_state = handle_count_digits(ctx);
            break;
        case PROCESS_WORD:
            ctx->current_state = handle_process_word(ctx);
            break;
        case FINISHED:
            return;
        case ERROR:
            return;
    }
    
    if (ctx->current_state != FINISHED && ctx->current_state != ERROR) {
        state_machine(ctx);
    }
}

int main(void) {
    char input_buffer[1024];
    
    printf("Enter text to analyze: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (len == 1 && input_buffer[0] == '\n') {
        printf("Empty input\n");
        return 0;
    }
    
    struct Context ctx = {
        .input = input_buffer,
        .position = 0,
        .digit_count = 0,
        .word_count = 0,
        .current_state = START
    };
    
    state_machine(&ctx);
    
    if (ctx.current_state == ERROR) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    printf("Words: %d\n", ctx.word_count);
    printf("Digits: %d\n", ctx.digit_count);
    
    return 0;
}