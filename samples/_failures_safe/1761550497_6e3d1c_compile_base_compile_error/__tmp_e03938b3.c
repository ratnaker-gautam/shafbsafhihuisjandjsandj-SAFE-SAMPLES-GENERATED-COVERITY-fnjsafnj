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
    enum State current_state;
    char input_buffer[256];
    size_t input_pos;
    int digit_count;
    int word_count;
    int recursion_depth;
};

int process_state_machine(struct Context* ctx, const char* input, size_t pos);

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > 255) return 0;
    return 1;
}

int is_valid_char(char c) {
    return (c >= 'a' && c <= 'z') || 
           (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') ||
           c == ' ' || c == '\t' || c == '\n' || c == '\0';
}

int process_start(struct Context* ctx, const char* input, size_t pos) {
    if (pos >= strlen(input)) {
        ctx->current_state = FINISHED;
        return process_state_machine(ctx, input, pos);
    }
    
    char current_char = input[pos];
    if (!is_valid_char(current_char)) {
        ctx->current_state = ERROR;
        return process_state_machine(ctx, input, pos);
    }
    
    if (current_char == ' ' || current_char == '\t' || current_char == '\n') {
        return process_state_machine(ctx, input, pos + 1);
    }
    
    ctx->current_state = READ_CHAR;
    ctx->input_pos = 0;
    memset(ctx->input_buffer, 0, sizeof(ctx->input_buffer));
    return process_state_machine(ctx, input, pos);
}

int process_read_char(struct Context* ctx, const char* input, size_t pos) {
    if (pos >= strlen(input)) {
        ctx->current_state = COUNT_DIGITS;
        return process_state_machine(ctx, input, pos);
    }
    
    char current_char = input[pos];
    if (!is_valid_char(current_char)) {
        ctx->current_state = ERROR;
        return process_state_machine(ctx, input, pos);
    }
    
    if (current_char == ' ' || current_char == '\t' || current_char == '\n' || current_char == '\0') {
        ctx->current_state = COUNT_DIGITS;
        return process_state_machine(ctx, input, pos);
    }
    
    if (ctx->input_pos < sizeof(ctx->input_buffer) - 1) {
        ctx->input_buffer[ctx->input_pos++] = current_char;
    }
    
    return process_state_machine(ctx, input, pos + 1);
}

int process_count_digits(struct Context* ctx, const char* input, size_t pos) {
    ctx->digit_count = 0;
    for (size_t i = 0; i < ctx->input_pos; i++) {
        if (ctx->input_buffer[i] >= '0' && ctx->input_buffer[i] <= '9') {
            ctx->digit_count++;
        }
    }
    
    ctx->current_state = PROCESS_WORD;
    return process_state_machine(ctx, input, pos);
}

int process_word(struct Context* ctx, const char* input, size_t pos) {
    ctx->word_count++;
    printf("Word %d: '%.*s' contains %d digits\n", 
           ctx->word_count, (int)ctx->input_pos, ctx->input_buffer, ctx->digit_count);
    
    ctx->current_state = START;
    return process_state_machine(ctx, input, pos);
}

int process_finished(struct Context* ctx, const char* input, size_t pos) {
    printf("Processing complete. Found %d words.\n", ctx->word_count);
    return 0;
}

int process_error(struct Context* ctx, const char* input, size_t pos) {
    printf("Error: Invalid input character at position %zu\n", pos);
    return 1;
}

int process_state_machine(struct Context* ctx, const char* input, size_t pos) {
    if (ctx->recursion_depth > 1000) {
        printf("Error: Recursion depth exceeded\n");
        return 1;
    }
    
    ctx->recursion_depth++;
    
    switch (ctx->current_state) {
        case START:
            return process_start(ctx, input, pos);
        case READ_CHAR:
            return process_read_char(ctx, input, pos);
        case COUNT_DIGITS:
            return process_count_digits(ctx, input, pos);
        case PROCESS_WORD:
            return process_word(ctx, input, pos);
        case FINISHED:
            return process_finished(ctx, input, pos);
        case ERROR:
            return process_error(ctx, input, pos);
        default:
            printf("Error: Unknown state\n");
            return 1;
    }
}

int main(void) {
    char input[256];
    
    printf("Enter text to process (max 255 characters): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!validate_input(input)) {
        printf("Error: Invalid input\n");