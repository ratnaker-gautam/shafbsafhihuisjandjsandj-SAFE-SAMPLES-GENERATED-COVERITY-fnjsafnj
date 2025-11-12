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

int state_machine(struct Context* ctx, const char* input, size_t pos);

int process_start(struct Context* ctx, const char* input, size_t pos) {
    size_t input_len = strlen(input);
    if (pos >= input_len) {
        ctx->current_state = FINISHED;
        return state_machine(ctx, input, pos);
    }
    
    char c = input[pos];
    if (c == ' ' || c == '\t' || c == '\n') {
        return state_machine(ctx, input, pos + 1);
    } else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        ctx->current_state = PROCESS_WORD;
        ctx->word_count++;
        return state_machine(ctx, input, pos);
    } else if (c >= '0' && c <= '9') {
        ctx->current_state = COUNT_DIGITS;
        ctx->digit_count = 1;
        return state_machine(ctx, input, pos + 1);
    } else {
        ctx->current_state = ERROR;
        return state_machine(ctx, input, pos);
    }
}

int process_read_char(struct Context* ctx, const char* input, size_t pos) {
    size_t input_len = strlen(input);
    if (pos >= input_len) {
        ctx->current_state = FINISHED;
        return state_machine(ctx, input, pos);
    }
    
    char c = input[pos];
    if (c == ' ' || c == '\t' || c == '\n') {
        ctx->current_state = START;
        return state_machine(ctx, input, pos + 1);
    } else {
        return state_machine(ctx, input, pos + 1);
    }
}

int process_count_digits(struct Context* ctx, const char* input, size_t pos) {
    size_t input_len = strlen(input);
    if (pos >= input_len) {
        ctx->current_state = FINISHED;
        return state_machine(ctx, input, pos);
    }
    
    char c = input[pos];
    if (c >= '0' && c <= '9') {
        if (ctx->digit_count < 1000) {
            ctx->digit_count++;
        }
        return state_machine(ctx, input, pos + 1);
    } else if (c == ' ' || c == '\t' || c == '\n') {
        ctx->current_state = START;
        return state_machine(ctx, input, pos + 1);
    } else {
        ctx->current_state = ERROR;
        return state_machine(ctx, input, pos);
    }
}

int process_word(struct Context* ctx, const char* input, size_t pos) {
    size_t input_len = strlen(input);
    if (pos >= input_len) {
        ctx->current_state = FINISHED;
        return state_machine(ctx, input, pos);
    }
    
    char c = input[pos];
    if (c == ' ' || c == '\t' || c == '\n') {
        ctx->current_state = START;
        return state_machine(ctx, input, pos + 1);
    } else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        return state_machine(ctx, input, pos + 1);
    } else {
        ctx->current_state = ERROR;
        return state_machine(ctx, input, pos);
    }
}

int process_finished(struct Context* ctx, const char* input, size_t pos) {
    return 0;
}

int process_error(struct Context* ctx, const char* input, size_t pos) {
    return -1;
}

int state_machine(struct Context* ctx, const char* input, size_t pos) {
    if (ctx->recursion_depth >= 1000) {
        ctx->current_state = ERROR;
        return -1;
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
            ctx->current_state = ERROR;
            return -1;
    }
}

int main() {
    char input[512];
    struct Context ctx;
    
    printf("Enter text to analyze: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    if (strlen(input) >= 500