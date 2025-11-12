//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_DONE,
    STATE_ERROR
} state_t;

typedef struct {
    state_t current_state;
    int data;
    int counter;
} context_t;

typedef state_t (*state_handler_t)(context_t*);

state_t handle_idle(context_t* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("IDLE: Enter command (start/quit): ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) return STATE_ERROR;
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "start") == 0) {
        ctx->counter = 0;
        return STATE_READY;
    } else if (strcmp(input, "quit") == 0) {
        return STATE_DONE;
    }
    
    printf("Invalid command\n");
    return STATE_IDLE;
}

state_t handle_ready(context_t* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("READY: Enter number (1-100) or 'back': ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) return STATE_ERROR;
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "back") == 0) {
        return STATE_IDLE;
    }
    
    char* endptr;
    long num = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\0') {
        printf("Invalid number format\n");
        return STATE_READY;
    }
    
    if (num < 1 || num > 100) {
        printf("Number must be between 1 and 100\n");
        return STATE_READY;
    }
    
    ctx->data = (int)num;
    return STATE_PROCESSING;
}

state_t handle_processing(context_t* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("PROCESSING: Data=%d, Counter=%d\n", ctx->data, ctx->counter);
    
    if (ctx->counter >= 5) {
        printf("Processing complete\n");
        return STATE_READY;
    }
    
    int result;
    if (__builtin_add_overflow(ctx->data, ctx->counter, &result)) {
        printf("Integer overflow detected\n");
        return STATE_ERROR;
    }
    
    printf("Result: %d + %d = %d\n", ctx->data, ctx->counter, result);
    ctx->counter++;
    
    return STATE_PROCESSING;
}

state_t handle_error(context_t* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("ERROR: An error occurred. Resetting...\n");
    ctx->data = 0;
    ctx->counter = 0;
    return STATE_IDLE;
}

state_t handle_done(context_t* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    printf("DONE: Exiting state machine\n");
    return STATE_DONE;
}

int main(void) {
    context_t ctx = {STATE_IDLE, 0, 0};
    
    state_handler_t handlers[] = {
        handle_idle,
        handle_ready,
        handle_processing,
        handle_done,
        handle_error
    };
    
    if (sizeof(handlers) / sizeof(handlers[0]) != 5) {
        fprintf(stderr, "Handler array size mismatch\n");
        return EXIT_FAILURE;
    }
    
    while (ctx.current_state != STATE_DONE) {
        if (ctx.current_state < 0 || ctx.current_state >= 5) {
            fprintf(stderr, "Invalid state detected\n");
            return EXIT_FAILURE;
        }
        
        state_handler_t handler = handlers[ctx.current_state];
        if (handler == NULL) {
            fprintf(stderr, "Null handler detected\n");
            return EXIT_FAILURE;
        }
        
        state_t next_state = handler(&ctx);
        
        if (next_state < 0 || next_state >= 5) {
            fprintf(stderr, "Invalid next state\n");
            return EXIT_FAILURE;
        }
        
        ctx.current_state = next_state;
    }
    
    return EXIT_SUCCESS;
}