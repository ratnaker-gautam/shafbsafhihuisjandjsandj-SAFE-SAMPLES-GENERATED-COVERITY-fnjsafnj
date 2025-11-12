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

enum State transition(struct Context* ctx, char input) {
    if (ctx == NULL) return ERROR;
    
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
    if (ctx == NULL) return ERROR;
    
    if (input == 'B') {
        ctx->position = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        return READING;
    }
    return ERROR;
}

enum State handle_reading(struct Context* ctx, char input) {
    if (ctx == NULL) return ERROR;
    
    if (input >= '0' && input <= '9') {
        if (ctx->position < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->position++] = input;
            return READING;
        }
        return ERROR;
    } else if (input == 'E') {
        ctx->buffer[ctx->position] = '\0';
        if (ctx->position > 0) {
            return PROCESSING;
        }
        return ERROR;
    }
    return ERROR;
}

enum State handle_processing(struct Context* ctx, char input) {
    if (ctx == NULL) return ERROR;
    
    if (input == 'C') {
        char* endptr;
        long num = strtol(ctx->buffer, &endptr, 10);
        if (endptr != ctx->buffer && *endptr == '\0' && num >= 0 && num <= 1000) {
            ctx->value = (int)num;
            return FINISHED;
        }
        return ERROR;
    }
    return ERROR;
}

enum State handle_finished(struct Context* ctx, char input) {
    if (ctx == NULL) return ERROR;
    
    if (input == 'X') {
        return FINISHED;
    }
    return ERROR;
}

enum State handle_error(struct Context* ctx, char input) {
    return ERROR;
}

void process_sequence_recursive(struct Context* ctx, const char* sequence, size_t index) {
    if (ctx == NULL || sequence == NULL) return;
    
    if (index >= strlen(sequence)) {
        return;
    }
    
    char current_char = sequence[index];
    ctx->current_state = transition(ctx, current_char);
    
    if (ctx->current_state == FINISHED) {
        printf("Success: Processed value %d\n", ctx->value);
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
    ctx.position = 0;
    ctx.value = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    char input_sequence[512];
    printf("Enter sequence: ");
    
    if (fgets(input_sequence, sizeof(input_sequence), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_sequence);
    if (len > 0 && input_sequence[len - 1] == '\n') {
        input_sequence[len - 1] = '\0';
    }
    
    if (strlen(input_sequence) == 0) {
        fprintf(stderr, "Error: Empty input\n");
        return 1;
    }
    
    process_sequence_recursive(&ctx, input_sequence, 0);
    
    if (ctx.current_state != FINISHED) {
        printf("Result: Sequence not completed successfully\n");
    }
    
    return 0;
}