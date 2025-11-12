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
            return PROCESSING;
        } else {
            return FINISHED;
        }
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
    char sequence[512];
    
    printf("Enter sequence: ");
    if (fgets(sequence, sizeof(sequence), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(sequence);
    if (len > 0 && sequence[len - 1] == '\n') {
        sequence[len - 1] = '\0';
    }
    
    if (strlen(sequence) == 0) {
        printf("Invalid input: empty sequence\n");
        return EXIT_FAILURE;
    }
    
    ctx.current_state = START;
    ctx.position = 0;
    ctx.value = 0;
    ctx.count = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    process_sequence(&ctx, sequence, 0);
    
    switch (ctx.current_state) {
        case FINISHED:
            printf("Success: Processed %d items\n", ctx.value);
            break;
        case ERROR:
            printf("Error: Invalid sequence\n");
            break;
        default:
            printf("Error: Incomplete sequence\n");
            break;
    }
    
    return EXIT_SUCCESS;
}