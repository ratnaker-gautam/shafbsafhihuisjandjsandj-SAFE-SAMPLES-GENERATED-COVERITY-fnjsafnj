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
    int count;
};

enum State handle_start(struct Context* ctx, char input);
enum State handle_reading(struct Context* ctx, char input);
enum State handle_processing(struct Context* ctx, char input);
enum State handle_finished(struct Context* ctx, char input);
enum State handle_error(struct Context* ctx, char input);

enum State transition(struct Context* ctx, char input) {
    switch (ctx->current_state) {
        case START:
            return handle_start(ctx, input);
        case READING:
            return handle_reading(ctx, input);
        case PROCESSING:
            return handle_processing(ctx, input);
        case FINISHED:
            return handle_finished(ctx, input);
        case ERROR:
            return handle_error(ctx, input);
        default:
            return ERROR;
    }
}

enum State handle_start(struct Context* ctx, char input) {
    if (input == 'B') {
        ctx->position = 0;
        ctx->count = 0;
        ctx->value = 0;
        return READING;
    }
    return ERROR;
}

enum State handle_reading(struct Context* ctx, char input) {
    if (input >= '0' && input <= '9') {
        if (ctx->position < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->position++] = input;
            ctx->buffer[ctx->position] = '\0';
            return READING;
        }
        return ERROR;
    } else if (input == 'E') {
        if (ctx->position > 0) {
            ctx->buffer[ctx->position] = '\0';
            return PROCESSING;
        }
        return ERROR;
    }
    return ERROR;
}

enum State handle_processing(struct Context* ctx, char input) {
    if (input == 'P') {
        long temp = 0;
        for (size_t i = 0; i < ctx->position; i++) {
            if (temp > 214748364 || (temp == 214748364 && ctx->buffer[i] > '7')) {
                return ERROR;
            }
            temp = temp * 10 + (ctx->buffer[i] - '0');
            if (temp > 2147483647) {
                return ERROR;
            }
        }
        ctx->value = (int)temp;
        ctx->count++;
        if (ctx->count >= 3) {
            return FINISHED;
        }
        return START;
    }
    return ERROR;
}

enum State handle_finished(struct Context* ctx, char input) {
    if (input == 'X') {
        return FINISHED;
    }
    return ERROR;
}

enum State handle_error(struct Context* ctx, char input) {
    return ERROR;
}

void process_sequence_recursive(struct Context* ctx, const char* sequence, size_t index) {
    if (sequence == NULL || index >= strlen(sequence)) {
        return;
    }
    
    char input = sequence[index];
    ctx->current_state = transition(ctx, input);
    
    if (ctx->current_state == FINISHED) {
        printf("Success: Processed %d values\n", ctx->count);
        return;
    } else if (ctx->current_state == ERROR) {
        printf("Error: Invalid sequence at position %zu\n", index);
        return;
    }
    
    process_sequence_recursive(ctx, sequence, index + 1);
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    ctx.position = 0;
    ctx.value = 0;
    ctx.count = 0;
    
    char input_sequence[512];
    printf("Enter sequence: ");
    if (fgets(input_sequence, sizeof(input_sequence), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_sequence);
    if (len > 0 && input_sequence[len - 1] == '\n') {
        input_sequence[len - 1] = '\0';
    }
    
    if (strlen(input_sequence) == 0) {
        printf("Error: Empty input\n");
        return 1;
    }
    
    process_sequence_recursive(&ctx, input_sequence, 0);
    
    if (ctx.current_state != FINISHED && ctx.current_state != ERROR) {
        printf("Error: Incomplete sequence\n");
        return 1;
    }
    
    return 0;
}