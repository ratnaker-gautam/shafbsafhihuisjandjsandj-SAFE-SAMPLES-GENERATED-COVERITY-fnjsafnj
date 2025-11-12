//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_DONE
} State;

typedef struct {
    State current_state;
    char buffer[256];
    size_t buffer_pos;
    int data_value;
} Context;

typedef void (*StateHandler)(Context*);

void handle_idle(Context* ctx) {
    if (ctx == NULL) return;
    
    printf("Enter a number (0-100) or 'quit': ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    ctx->buffer[strcspn(ctx->buffer, "\n")] = '\0';
    
    if (strcmp(ctx->buffer, "quit") == 0) {
        ctx->current_state = STATE_DONE;
        return;
    }
    
    char* endptr;
    long value = strtol(ctx->buffer, &endptr, 10);
    if (endptr == ctx->buffer || *endptr != '\0') {
        printf("Invalid input. Please enter a number or 'quit'.\n");
        return;
    }
    
    if (value < 0 || value > 100) {
        printf("Number must be between 0 and 100.\n");
        return;
    }
    
    ctx->data_value = (int)value;
    ctx->current_state = STATE_READING;
}

void handle_reading(Context* ctx) {
    if (ctx == NULL) return;
    
    printf("Read value: %d\n", ctx->data_value);
    ctx->current_state = STATE_PROCESSING;
}

void handle_processing(Context* ctx) {
    if (ctx == NULL) return;
    
    if (ctx->data_value < 0) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    ctx->data_value *= 2;
    if (ctx->data_value < 0) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    printf("Processed value: %d\n", ctx->data_value);
    ctx->current_state = STATE_WRITING;
}

void handle_writing(Context* ctx) {
    if (ctx == NULL) return;
    
    printf("Writing result: %d\n", ctx->data_value);
    ctx->current_state = STATE_IDLE;
}

void handle_error(Context* ctx) {
    if (ctx == NULL) return;
    
    printf("Error occurred. Resetting state machine.\n");
    ctx->current_state = STATE_IDLE;
    ctx->data_value = 0;
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

void handle_done(Context* ctx) {
    if (ctx == NULL) return;
    printf("State machine terminated.\n");
}

int main() {
    Context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.buffer_pos = 0;
    ctx.data_value = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    StateHandler handlers[] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_error,
        handle_done
    };
    
    if (sizeof(handlers) / sizeof(handlers[0]) != STATE_DONE + 1) {
        return EXIT_FAILURE;
    }
    
    while (ctx.current_state != STATE_DONE) {
        if (ctx.current_state < 0 || ctx.current_state > STATE_DONE) {
            ctx.current_state = STATE_ERROR;
        }
        
        handlers[ctx.current_state](&ctx);
    }
    
    return EXIT_SUCCESS;
}