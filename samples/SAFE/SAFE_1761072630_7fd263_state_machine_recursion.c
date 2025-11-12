//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: state_machine
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
    enum State current_state;
    char input_buffer[256];
    size_t input_pos;
    int digit_count;
    int word_count;
};

int is_valid_char(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
           (c >= '0' && c <= '9') || c == ' ' || c == '\n' || c == '\0';
}

enum State handle_start(struct Context* ctx) {
    if (ctx->input_pos >= sizeof(ctx->input_buffer) - 1) {
        return ERROR;
    }
    return READ_CHAR;
}

enum State handle_read_char(struct Context* ctx) {
    if (ctx->input_pos >= sizeof(ctx->input_buffer) - 1) {
        return FINISHED;
    }
    
    char c = ctx->input_buffer[ctx->input_pos];
    if (!is_valid_char(c)) {
        return ERROR;
    }
    
    if (c == '\0') {
        return FINISHED;
    }
    
    if (c >= '0' && c <= '9') {
        ctx->digit_count++;
        ctx->input_pos++;
        return COUNT_DIGITS;
    }
    
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        ctx->input_pos++;
        return PROCESS_WORD;
    }
    
    if (c == ' ' || c == '\n') {
        ctx->input_pos++;
        return READ_CHAR;
    }
    
    return ERROR;
}

enum State handle_count_digits(struct Context* ctx) {
    if (ctx->input_pos >= sizeof(ctx->input_buffer) - 1) {
        return FINISHED;
    }
    
    char c = ctx->input_buffer[ctx->input_pos];
    if (!is_valid_char(c)) {
        return ERROR;
    }
    
    if (c >= '0' && c <= '9') {
        ctx->digit_count++;
        ctx->input_pos++;
        return COUNT_DIGITS;
    }
    
    if (c == ' ' || c == '\n' || c == '\0') {
        ctx->input_pos++;
        return READ_CHAR;
    }
    
    return PROCESS_WORD;
}

enum State handle_process_word(struct Context* ctx) {
    if (ctx->input_pos >= sizeof(ctx->input_buffer) - 1) {
        return FINISHED;
    }
    
    char c = ctx->input_buffer[ctx->input_pos];
    if (!is_valid_char(c)) {
        return ERROR;
    }
    
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
        (c >= '0' && c <= '9')) {
        ctx->input_pos++;
        return PROCESS_WORD;
    }
    
    if (c == ' ' || c == '\n' || c == '\0') {
        ctx->word_count++;
        ctx->input_pos++;
        return READ_CHAR;
    }
    
    return ERROR;
}

void process_state_machine(struct Context* ctx) {
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
        default:
            ctx->current_state = ERROR;
            return;
    }
    
    process_state_machine(ctx);
}

int main(void) {
    struct Context ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.current_state = START;
    
    printf("Enter text (max 255 characters): ");
    if (fgets(ctx.input_buffer, sizeof(ctx.input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(ctx.input_buffer);
    if (len > 0 && ctx.input_buffer[len - 1] == '\n') {
        ctx.input_buffer[len - 1] = '\0';
    }
    
    if (strlen(ctx.input_buffer) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    process_state_machine(&ctx);
    
    if (ctx.current_state == ERROR) {
        printf("Error processing input\n");
        return 1;
    }
    
    printf("Words: %d\n", ctx.word_count);
    printf("Digits: %d\n", ctx.digit_count);
    
    return 0;
}