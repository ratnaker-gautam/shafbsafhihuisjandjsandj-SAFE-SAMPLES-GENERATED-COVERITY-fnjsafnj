//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 5

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_OUTPUT,
    STATE_ERROR
} State;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int result;
} Context;

typedef State (*StateHandler)(Context*);

State handle_idle(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("Enter a number (or 'quit' to exit): ");
    if (fgets(ctx->buffer, MAX_INPUT_LEN, stdin) == NULL) {
        return STATE_ERROR;
    }
    
    ctx->buffer_len = strlen(ctx->buffer);
    if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len - 1] == '\n') {
        ctx->buffer[ctx->buffer_len - 1] = '\0';
        ctx->buffer_len--;
    }
    
    if (strcmp(ctx->buffer, "quit") == 0) {
        return STATE_ERROR;
    }
    
    return STATE_READING;
}

State handle_reading(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (ctx->buffer_len == 0) {
        return STATE_IDLE;
    }
    
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (ctx->buffer[i] < '0' || ctx->buffer[i] > '9') {
            printf("Invalid input. Please enter numbers only.\n");
            return STATE_IDLE;
        }
    }
    
    return STATE_PROCESSING;
}

State handle_processing(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    char* endptr;
    long num = strtol(ctx->buffer, &endptr, 10);
    
    if (endptr == ctx->buffer || *endptr != '\0') {
        printf("Conversion error.\n");
        return STATE_IDLE;
    }
    
    if (num < 0 || num > 10000) {
        printf("Number out of range (0-10000).\n");
        return STATE_IDLE;
    }
    
    ctx->result = (int)num * 2;
    return STATE_OUTPUT;
}

State handle_output(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("Result: %d\n", ctx->result);
    return STATE_IDLE;
}

State handle_error(Context* ctx) {
    if (ctx != NULL) {
        printf("Exiting program.\n");
    }
    return STATE_ERROR;
}

int main(void) {
    Context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.buffer_len = 0;
    ctx.result = 0;
    
    StateHandler handlers[MAX_STATES] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_output,
        handle_error
    };
    
    while (ctx.current_state != STATE_ERROR) {
        if (ctx.current_state < 0 || ctx.current_state >= MAX_STATES) {
            break;
        }
        
        StateHandler handler = handlers[ctx.current_state];
        if (handler == NULL) {
            break;
        }
        
        State next_state = handler(&ctx);
        ctx.current_state = next_state;
    }
    
    return 0;
}