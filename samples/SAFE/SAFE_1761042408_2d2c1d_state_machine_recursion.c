//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: state_machine
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
    if (input == 'B') {
        ctx->position = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        return READING;
    }
    return ERROR;
}

enum State handle_reading(struct Context* ctx, char input) {
    if (ctx->position >= sizeof(ctx->buffer) - 1) {
        return ERROR;
    }
    
    if (input == 'E') {
        ctx->buffer[ctx->position] = '\0';
        if (ctx->position > 0) {
            return PROCESSING;
        }
        return ERROR;
    }
    
    if ((input >= '0' && input <= '9') || input == '-') {
        ctx->buffer[ctx->position++] = input;
        return READING;
    }
    
    return ERROR;
}

enum State handle_processing(struct Context* ctx, char input) {
    if (input != '\0') return ERROR;
    
    char* endptr;
    long num = strtol(ctx->buffer, &endptr, 10);
    
    if (endptr == ctx->buffer || *endptr != '\0') {
        return ERROR;
    }
    
    if (num < -1000 || num > 1000) {
        return ERROR;
    }
    
    ctx->value = (int)num;
    return FINISHED;
}

enum State handle_finished(struct Context* ctx, char input) {
    if (input == 'R') {
        return START;
    }
    return FINISHED;
}

enum State handle_error(struct Context* ctx, char input) {
    if (input == 'R') {
        return START;
    }
    return ERROR;
}

void run_state_machine(struct Context* ctx, const char* input, size_t input_len) {
    if (ctx == NULL || input == NULL) return;
    
    for (size_t i = 0; i < input_len; i++) {
        ctx->current_state = process_input(ctx, input[i]);
        if (ctx->current_state == FINISHED) {
            printf("Processed value: %d\n", ctx->value);
            return;
        } else if (ctx->current_state == ERROR) {
            printf("Error processing input\n");
            return;
        }
    }
    
    if (ctx->current_state == PROCESSING) {
        ctx->current_state = process_input(ctx, '\0');
        if (ctx->current_state == FINISHED) {
            printf("Processed value: %d\n", ctx->value);
        } else {
            printf("Error processing input\n");
        }
    } else {
        printf("Incomplete input\n");
    }
}

int main() {
    struct Context ctx = {START, {0}, 0, 0};
    
    char input1[] = "B42E";
    char input2[] = "B-123E";
    char input3[] = "B999E";
    char input4[] = "BabcE";
    
    printf("Processing: %s\n", input1);
    run_state_machine(&ctx, input1, strlen(input1));
    
    ctx.current_state = START;
    printf("Processing: %s\n", input2);
    run_state_machine(&ctx, input2, strlen(input2));
    
    ctx.current_state = START;
    printf("Processing: %s\n", input3);
    run_state_machine(&ctx, input3, strlen(input3));
    
    ctx.current_state = START;
    printf("Processing: %s\n", input4);
    run_state_machine(&ctx, input4, strlen(input4));
    
    return 0;
}