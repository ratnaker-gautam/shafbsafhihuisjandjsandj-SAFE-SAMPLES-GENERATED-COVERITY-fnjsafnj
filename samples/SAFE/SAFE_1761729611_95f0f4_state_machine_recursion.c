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

enum State process_state(struct Context* ctx, char input) {
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
            long val = strtol(ctx->buffer, NULL, 10);
            if (val > 0 && val <= 1000) {
                ctx->value = (int)val;
                return PROCESSING;
            }
        }
        return ERROR;
    }
    return ERROR;
}

enum State handle_processing(struct Context* ctx, char input) {
    if (input == 'P') {
        if (ctx->value > 0) {
            ctx->count++;
            if (ctx->count < ctx->value) {
                return PROCESSING;
            } else {
                return FINISHED;
            }
        }
        return ERROR;
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

void process_input_recursive(struct Context* ctx, const char* input, size_t index) {
    if (input[index] == '\0') {
        return;
    }
    
    if (index >= 1000) {
        ctx->current_state = ERROR;
        return;
    }
    
    ctx->current_state = process_state(ctx, input[index]);
    
    if (ctx->current_state == ERROR) {
        return;
    }
    
    process_input_recursive(ctx, input, index + 1);
}

int main() {
    char input[1024];
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || len >= sizeof(input)) {
        return 1;
    }
    
    struct Context ctx = {START, "", 0, 0, 0};
    
    process_input_recursive(&ctx, input, 0);
    
    if (ctx.current_state == FINISHED) {
        printf("SUCCESS: Processed %d items\n", ctx.count);
        return 0;
    } else {
        printf("ERROR: Invalid sequence\n");
        return 1;
    }
}