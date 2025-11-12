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
    int recursion_depth;
};

int process_state_machine(struct Context* ctx, const char* input, size_t pos);

void initialize_context(struct Context* ctx) {
    ctx->current_state = START;
    ctx->input_pos = 0;
    ctx->digit_count = 0;
    ctx->word_count = 0;
    ctx->recursion_depth = 0;
    memset(ctx->input_buffer, 0, sizeof(ctx->input_buffer));
}

int handle_start(struct Context* ctx, const char* input, size_t pos) {
    if (pos >= strlen(input)) {
        ctx->current_state = FINISHED;
        return process_state_machine(ctx, input, pos);
    }
    
    char c = input[pos];
    if (c == ' ' || c == '\t' || c == '\n') {
        return process_state_machine(ctx, input, pos + 1);
    } else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        ctx->current_state = READ_CHAR;
        ctx->input_buffer[0] = c;
        ctx->input_pos = 1;
        return process_state_machine(ctx, input, pos + 1);
    } else if (c >= '0' && c <= '9') {
        ctx->current_state = COUNT_DIGITS;
        ctx->digit_count = 1;
        return process_state_machine(ctx, input, pos + 1);
    } else {
        ctx->current_state = ERROR;
        return process_state_machine(ctx, input, pos);
    }
}

int handle_read_char(struct Context* ctx, const char* input, size_t pos) {
    if (pos >= strlen(input)) {
        ctx->current_state = PROCESS_WORD;
        return process_state_machine(ctx, input, pos);
    }
    
    char c = input[pos];
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        if (ctx->input_pos < sizeof(ctx->input_buffer) - 1) {
            ctx->input_buffer[ctx->input_pos++] = c;
            return process_state_machine(ctx, input, pos + 1);
        } else {
            ctx->current_state = ERROR;
            return process_state_machine(ctx, input, pos);
        }
    } else if (c == ' ' || c == '\t' || c == '\n') {
        ctx->current_state = PROCESS_WORD;
        return process_state_machine(ctx, input, pos);
    } else {
        ctx->current_state = ERROR;
        return process_state_machine(ctx, input, pos);
    }
}

int handle_count_digits(struct Context* ctx, const char* input, size_t pos) {
    if (pos >= strlen(input)) {
        ctx->current_state = START;
        return process_state_machine(ctx, input, pos);
    }
    
    char c = input[pos];
    if (c >= '0' && c <= '9') {
        if (ctx->digit_count < 1000) {
            ctx->digit_count++;
            return process_state_machine(ctx, input, pos + 1);
        } else {
            ctx->current_state = ERROR;
            return process_state_machine(ctx, input, pos);
        }
    } else if (c == ' ' || c == '\t' || c == '\n') {
        ctx->current_state = START;
        return process_state_machine(ctx, input, pos + 1);
    } else {
        ctx->current_state = ERROR;
        return process_state_machine(ctx, input, pos);
    }
}

int handle_process_word(struct Context* ctx, const char* input, size_t pos) {
    if (ctx->input_pos > 0) {
        ctx->word_count++;
    }
    ctx->input_pos = 0;
    memset(ctx->input_buffer, 0, sizeof(ctx->input_buffer));
    ctx->current_state = START;
    return process_state_machine(ctx, input, pos);
}

int handle_finished(struct Context* ctx, const char* input, size_t pos) {
    return 0;
}

int handle_error(struct Context* ctx, const char* input, size_t pos) {
    return -1;
}

int process_state_machine(struct Context* ctx, const char* input, size_t pos) {
    if (ctx->recursion_depth > 1000) {
        return -1;
    }
    ctx->recursion_depth++;
    
    switch (ctx->current_state) {
        case START:
            return handle_start(ctx, input, pos);
        case READ_CHAR:
            return handle_read_char(ctx, input, pos);
        case COUNT_DIGITS:
            return handle_count_digits(ctx, input, pos);
        case PROCESS_WORD:
            return handle_process_word(ctx, input, pos);
        case FINISHED:
            return handle_finished(ctx, input, pos);
        case ERROR:
            return handle_error(ctx, input, pos);
        default:
            return -1;
    }
}

int main() {
    char input[512];
    printf("Enter text to analyze: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf