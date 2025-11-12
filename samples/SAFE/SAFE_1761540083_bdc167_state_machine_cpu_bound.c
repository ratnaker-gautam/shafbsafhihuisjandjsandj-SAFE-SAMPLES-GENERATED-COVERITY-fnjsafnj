//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10
#define MAX_TRANSITIONS 50

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_OUTPUT,
    STATE_ERROR,
    STATE_DONE
} State;

typedef struct {
    State current;
    char input_char;
    State next;
} Transition;

typedef struct {
    char buffer[MAX_INPUT_LEN];
    size_t pos;
    int processed_count;
    int error_count;
} Context;

int is_valid_input(char c) {
    return isalnum(c) || c == ' ' || c == '\n' || c == '\0';
}

State handle_idle(State current, char input, Context *ctx) {
    if (input == '\0') return STATE_DONE;
    if (is_valid_input(input)) {
        ctx->pos = 0;
        ctx->buffer[ctx->pos++] = input;
        return STATE_READING;
    }
    return STATE_ERROR;
}

State handle_reading(State current, char input, Context *ctx) {
    if (input == '\n' || input == '\0') {
        ctx->buffer[ctx->pos] = '\0';
        return STATE_PROCESSING;
    }
    if (is_valid_input(input) && ctx->pos < MAX_INPUT_LEN - 1) {
        ctx->buffer[ctx->pos++] = input;
        return STATE_READING;
    }
    return STATE_ERROR;
}

State handle_processing(State current, char input, Context *ctx) {
    size_t len = strlen(ctx->buffer);
    if (len == 0) return STATE_ERROR;
    
    for (size_t i = 0; i < len; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    
    ctx->processed_count++;
    return STATE_OUTPUT;
}

State handle_output(State current, char input, Context *ctx) {
    printf("Processed: %s\n", ctx->buffer);
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->pos = 0;
    return STATE_IDLE;
}

State handle_error(State current, char input, Context *ctx) {
    ctx->error_count++;
    printf("Error: Invalid input detected\n");
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->pos = 0;
    return STATE_IDLE;
}

State get_next_state(State current, char input, Context *ctx) {
    switch (current) {
        case STATE_IDLE: return handle_idle(current, input, ctx);
        case STATE_READING: return handle_reading(current, input, ctx);
        case STATE_PROCESSING: return handle_processing(current, input, ctx);
        case STATE_OUTPUT: return handle_output(current, input, ctx);
        case STATE_ERROR: return handle_error(current, input, ctx);
        case STATE_DONE: return STATE_DONE;
        default: return STATE_ERROR;
    }
}

void process_input(const char *input) {
    State current_state = STATE_IDLE;
    Context ctx = {0};
    size_t input_len = strlen(input);
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        if (!is_valid_input(c) && c != '\0') {
            current_state = STATE_ERROR;
        }
        
        current_state = get_next_state(current_state, c, &ctx);
        
        if (current_state == STATE_DONE) {
            break;
        }
    }
    
    printf("Processing complete. Processed: %d, Errors: %d\n", 
           ctx.processed_count, ctx.error_count);
}

int main() {
    char input[MAX_INPUT_LEN * 2];
    
    printf("Enter text to process (max %d chars): ", MAX_INPUT_LEN * 2 - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len >= sizeof(input) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    process_input(input);
    
    return 0;
}