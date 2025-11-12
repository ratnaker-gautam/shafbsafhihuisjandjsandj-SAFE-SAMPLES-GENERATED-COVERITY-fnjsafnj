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

enum State handle_start(struct Context* ctx, char input);
enum State handle_reading(struct Context* ctx, char input);
enum State handle_processing(struct Context* ctx, char input);
enum State handle_finished(struct Context* ctx, char input);
enum State handle_error(struct Context* ctx, char input);

enum State process_input(struct Context* ctx, char input) {
    switch (ctx->current_state) {
        case START: return handle_start(ctx, input);
        case READING: return handle_reading(ctx, input);
        case PROCESSING: return handle_processing(ctx, input);
        case FINISHED: return handle_finished(ctx, input);
        case ERROR: return handle_error(ctx, input);
        default: return ERROR;
    }
}

enum State handle_start(struct Context* ctx, char input) {
    if (input == 'B') {
        ctx->position = 0;
        ctx->buffer[0] = '\0';
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
        if (ctx->position > 0) {
            return PROCESSING;
        }
        return ERROR;
    }
    if ((input >= '0' && input <= '9') || (input >= 'a' && input <= 'z')) {
        ctx->buffer[ctx->position] = input;
        ctx->position++;
        return READING;
    }
    return ERROR;
}

enum State handle_processing(struct Context* ctx, char input) {
    if (input != 'P') {
        return ERROR;
    }
    int sum = 0;
    for (size_t i = 0; i < ctx->position; i++) {
        if (ctx->buffer[i] >= '0' && ctx->buffer[i] <= '9') {
            sum += ctx->buffer[i] - '0';
        }
    }
    ctx->value = sum;
    return FINISHED;
}

enum State handle_finished(struct Context* ctx, char input) {
    if (input == 'X') {
        return START;
    }
    return ERROR;
}

enum State handle_error(struct Context* ctx, char input) {
    if (input == 'R') {
        return START;
    }
    return ERROR;
}

void run_state_machine(struct Context* ctx, const char* input, size_t input_len) {
    for (size_t i = 0; i < input_len; i++) {
        ctx->current_state = process_input(ctx, input[i]);
        if (ctx->current_state == ERROR) {
            printf("Error at position %zu\n", i);
            return;
        }
        if (ctx->current_state == FINISHED) {
            printf("Processing complete. Sum: %d\n", ctx->value);
            return;
        }
    }
    printf("Input processed without completion\n");
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.position = 0;
    ctx.value = 0;
    
    char input[512];
    printf("Enter input sequence: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    run_state_machine(&ctx, input, len);
    
    return 0;
}