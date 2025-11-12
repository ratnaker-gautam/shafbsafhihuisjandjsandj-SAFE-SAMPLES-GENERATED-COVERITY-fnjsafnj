//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, READING, PROCESSING, FINISHED, ERROR };

struct Context {
    enum State current_state;
    char buffer[256];
    size_t position;
    int result;
};

int process_state_machine(struct Context* ctx, const char* input, size_t input_len);

int handle_start(struct Context* ctx, const char* input, size_t input_len) {
    if (ctx->position >= input_len) {
        ctx->current_state = FINISHED;
        return process_state_machine(ctx, input, input_len);
    }
    
    char c = input[ctx->position];
    if (c == ' ' || c == '\t' || c == '\n') {
        ctx->position++;
        return process_state_machine(ctx, input, input_len);
    }
    
    ctx->current_state = READING;
    ctx->buffer[0] = c;
    ctx->buffer[1] = '\0';
    ctx->position++;
    return process_state_machine(ctx, input, input_len);
}

int handle_reading(struct Context* ctx, const char* input, size_t input_len) {
    if (ctx->position >= input_len) {
        ctx->current_state = PROCESSING;
        return process_state_machine(ctx, input, input_len);
    }
    
    char c = input[ctx->position];
    if (c == ' ' || c == '\t' || c == '\n') {
        ctx->current_state = PROCESSING;
        return process_state_machine(ctx, input, input_len);
    }
    
    size_t len = strlen(ctx->buffer);
    if (len >= 255) {
        ctx->current_state = ERROR;
        return process_state_machine(ctx, input, input_len);
    }
    
    ctx->buffer[len] = c;
    ctx->buffer[len + 1] = '\0';
    ctx->position++;
    return process_state_machine(ctx, input, input_len);
}

int handle_processing(struct Context* ctx, const char* input, size_t input_len) {
    int word_value = 0;
    for (size_t i = 0; ctx->buffer[i] != '\0'; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            word_value += (ctx->buffer[i] - 'a' + 1);
        } else if (ctx->buffer[i] >= 'A' && ctx->buffer[i] <= 'Z') {
            word_value += (ctx->buffer[i] - 'A' + 1);
        }
    }
    
    ctx->result += word_value;
    ctx->buffer[0] = '\0';
    
    if (ctx->position >= input_len) {
        ctx->current_state = FINISHED;
        return process_state_machine(ctx, input, input_len);
    }
    
    ctx->current_state = START;
    return process_state_machine(ctx, input, input_len);
}

int handle_finished(struct Context* ctx, const char* input, size_t input_len) {
    return ctx->result;
}

int handle_error(struct Context* ctx, const char* input, size_t input_len) {
    return -1;
}

int process_state_machine(struct Context* ctx, const char* input, size_t input_len) {
    switch (ctx->current_state) {
        case START:
            return handle_start(ctx, input, input_len);
        case READING:
            return handle_reading(ctx, input, input_len);
        case PROCESSING:
            return handle_processing(ctx, input, input_len);
        case FINISHED:
            return handle_finished(ctx, input, input_len);
        case ERROR:
            return handle_error(ctx, input, input_len);
        default:
            ctx->current_state = ERROR;
            return process_state_machine(ctx, input, input_len);
    }
}

int calculate_word_sum(const char* input) {
    if (input == NULL) {
        return -1;
    }
    
    size_t input_len = strlen(input);
    if (input_len == 0) {
        return 0;
    }
    
    struct Context ctx;
    ctx.current_state = START;
    ctx.buffer[0] = '\0';
    ctx.position = 0;
    ctx.result = 0;
    
    return process_state_machine(&ctx, input, input_len);
}

int main() {
    char input[512];
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    int result = calculate_word_sum(input);
    
    if (result >= 0) {
        printf("Word sum: %d\n", result);
    } else {
        printf("Error processing input\n");
    }
    
    return 0;
}