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
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        return READING;
    }
    return ERROR;
}

enum State handle_reading(struct Context* ctx, char input) {
    if (input >= '0' && input <= '9') {
        if (ctx->position < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->position++] = input;
            return READING;
        }
        return ERROR;
    } else if (input == 'E') {
        if (ctx->position > 0) {
            ctx->buffer[ctx->position] = '\0';
            ctx->value = atoi(ctx->buffer);
            if (ctx->value > 0 && ctx->value <= 1000) {
                return PROCESSING;
            }
        }
        return ERROR;
    }
    return ERROR;
}

enum State handle_processing(struct Context* ctx, char input) {
    if (input == 'P') {
        if (ctx->count < ctx->value) {
            ctx->count++;
            printf("Processing step %d of %d\n", ctx->count, ctx->value);
            return PROCESSING;
        } else {
            return FINISHED;
        }
    }
    return ERROR;
}

enum State handle_finished(struct Context* ctx, char input) {
    if (input == 'X') {
        printf("Operation completed successfully\n");
        return FINISHED;
    }
    return ERROR;
}

enum State handle_error(struct Context* ctx, char input) {
    printf("Invalid state transition\n");
    return ERROR;
}

void process_sequence(struct Context* ctx, const char* sequence, size_t index) {
    if (index >= strlen(sequence)) {
        return;
    }
    
    char input = sequence[index];
    enum State next_state = transition(ctx, input);
    ctx->current_state = next_state;
    
    process_sequence(ctx, sequence, index + 1);
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.position = 0;
    ctx.value = 0;
    ctx.count = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    char input_sequence[512];
    printf("Enter sequence: ");
    if (fgets(input_sequence, sizeof(input_sequence), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t len = strlen(input_sequence);
    if (len > 0 && input_sequence[len - 1] == '\n') {
        input_sequence[len - 1] = '\0';
    }
    
    if (strlen(input_sequence) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    process_sequence(&ctx, input_sequence, 0);
    
    if (ctx.current_state != FINISHED) {
        printf("Sequence processing failed\n");
        return 1;
    }
    
    return 0;
}