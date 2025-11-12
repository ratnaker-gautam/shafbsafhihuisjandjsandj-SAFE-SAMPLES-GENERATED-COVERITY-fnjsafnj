//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
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
    
    printf("IDLE: Enter command (start/exit): ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) return STATE_ERROR;
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "start") == 0) {
        ctx->counter = 0;
        return STATE_READY;
    } else if (strcmp(input, "exit") == 0) {
        return STATE_COMPLETE;
    }
    
    printf("Invalid command\n");
    return STATE_IDLE;
}

state_t handle_ready(context_t* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("READY: Enter number to process (1-100) or 'back': ");
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
    
    if (ctx->data > 0 && ctx->counter < 100) {
        if (ctx->data > 0 && ctx->counter < 100 - ctx->data) {
            ctx->data += 5;
            ctx->counter++;
        } else {
            return STATE_ERROR;
        }
    } else {
        return STATE_ERROR;
    }
    
    return STATE_PROCESSING;
}

state_t handle_complete(context_t* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    printf("COMPLETE: Exiting program\n");
    return STATE_COMPLETE;
}

state_t handle_error(context_t* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    printf("ERROR: Invalid state detected\n");
    return STATE_COMPLETE;
}

int main(void) {
    context_t ctx = {STATE_IDLE, 0, 0};
    
    state_handler_t handlers[] = {
        handle_idle,
        handle_ready,
        handle_processing,
        handle_complete,
        handle_error
    };
    
    const int num_states = sizeof(handlers) / sizeof(handlers[0]);
    
    while (ctx.current_state != STATE_COMPLETE) {
        if (ctx.current_state < 0 || ctx.current_state >= num_states) {
            printf("Invalid state detected\n");
            break;
        }
        
        state_t next_state = handlers[ctx.current_state](&ctx);
        
        if (next_state < 0 || next_state >= num_states) {
            printf("Invalid next state\n");
            break;
        }
        
        ctx.current_state = next_state;
    }
    
    return 0;
}