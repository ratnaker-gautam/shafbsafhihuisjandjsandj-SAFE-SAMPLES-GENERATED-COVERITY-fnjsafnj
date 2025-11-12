//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, READING, PROCESSING, FINISHED, ERROR };

struct Context {
    enum State current_state;
    char buffer[256];
    size_t position;
    int value;
};

enum State handle_start(struct Context* ctx, char input) {
    if (input == 'B') {
        ctx->position = 0;
        ctx->buffer[ctx->position] = input;
        ctx->position++;
        return READING;
    }
    return ERROR;
}

enum State handle_reading(struct Context* ctx, char input) {
    if (ctx->position >= 255) {
        return ERROR;
    }
    if (input == 'E') {
        ctx->buffer[ctx->position] = '\0';
        return PROCESSING;
    }
    if ((input >= '0' && input <= '9') || (input >= 'A' && input <= 'Z')) {
        ctx->buffer[ctx->position] = input;
        ctx->position++;
        return READING;
    }
    return ERROR;
}

enum State handle_processing(struct Context* ctx) {
    if (ctx->position < 2) {
        return ERROR;
    }
    int sum = 0;
    for (size_t i = 1; i < ctx->position - 1; i++) {
        if (ctx->buffer[i] >= '0' && ctx->buffer[i] <= '9') {
            sum += ctx->buffer[i] - '0';
        }
    }
    ctx->value = sum;
    return FINISHED;
}

void process_state_machine(struct Context* ctx, const char* input, size_t input_len) {
    if (input == NULL || input_len == 0) {
        ctx->current_state = ERROR;
        return;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        switch (ctx->current_state) {
            case START:
                ctx->current_state = handle_start(ctx, input[i]);
                break;
            case READING:
                ctx->current_state = handle_reading(ctx, input[i]);
                break;
            case PROCESSING:
                ctx->current_state = handle_processing(ctx);
                i--;
                break;
            case FINISHED:
            case ERROR:
                return;
        }
    }
    
    if (ctx->current_state == READING) {
        ctx->current_state = ERROR;
    } else if (ctx->current_state == PROCESSING) {
        ctx->current_state = handle_processing(ctx);
    }
}

void recursive_output(struct Context* ctx, int depth) {
    if (depth <= 0 || ctx->current_state != FINISHED) {
        return;
    }
    
    printf("Value: %d\n", ctx->value);
    recursive_output(ctx, depth - 1);
}

int main(void) {
    char user_input[512];
    
    printf("Enter input sequence (format: B[data]E): ");
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(user_input);
    if (input_len > 0 && user_input[input_len - 1] == '\n') {
        user_input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    struct Context ctx;
    ctx.current_state = START;
    ctx.position = 0;
    ctx.value = 0;
    
    process_state_machine(&ctx, user_input, input_len);
    
    if (ctx.current_state == FINISHED) {
        printf("Processing successful. ");
        recursive_output(&ctx, 3);
    } else {
        fprintf(stderr, "Processing failed - invalid input format\n");
        return 1;
    }
    
    return 0;
}