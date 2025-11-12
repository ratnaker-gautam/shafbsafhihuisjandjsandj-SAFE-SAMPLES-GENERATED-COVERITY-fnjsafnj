//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 5

typedef enum {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR
} State;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    int data_count;
} Context;

typedef State (*StateHandler)(Context*);

State handle_idle(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("IDLE: Enter 'start' to begin: ");
    if (fgets(ctx->buffer, MAX_INPUT_LEN, stdin) == NULL) {
        return STATE_ERROR;
    }
    
    ctx->buffer[strcspn(ctx->buffer, "\n")] = '\0';
    
    if (strcmp(ctx->buffer, "start") == 0) {
        ctx->data_count = 0;
        return STATE_READY;
    }
    
    return STATE_IDLE;
}

State handle_ready(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("READY: Enter a number (or 'done' to finish): ");
    if (fgets(ctx->buffer, MAX_INPUT_LEN, stdin) == NULL) {
        return STATE_ERROR;
    }
    
    ctx->buffer[strcspn(ctx->buffer, "\n")] = '\0';
    
    if (strcmp(ctx->buffer, "done") == 0) {
        if (ctx->data_count > 0) {
            return STATE_COMPLETE;
        } else {
            printf("No data entered. Returning to IDLE.\n");
            return STATE_IDLE;
        }
    }
    
    char* endptr;
    long value = strtol(ctx->buffer, &endptr, 10);
    if (endptr == ctx->buffer || *endptr != '\0') {
        printf("Invalid input. Please enter a number.\n");
        return STATE_READY;
    }
    
    if (value < -1000000 || value > 1000000) {
        printf("Number out of range. Use values between -1000000 and 1000000.\n");
        return STATE_READY;
    }
    
    ctx->data_count++;
    printf("Added number: %ld (Total: %d)\n", value, ctx->data_count);
    
    if (ctx->data_count >= 10) {
        return STATE_COMPLETE;
    }
    
    return STATE_READY;
}

State handle_processing(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    return STATE_COMPLETE;
}

State handle_complete(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("COMPLETE: Processed %d numbers. Enter 'reset' to start over: ", ctx->data_count);
    if (fgets(ctx->buffer, MAX_INPUT_LEN, stdin) == NULL) {
        return STATE_ERROR;
    }
    
    ctx->buffer[strcspn(ctx->buffer, "\n")] = '\0';
    
    if (strcmp(ctx->buffer, "reset") == 0) {
        return STATE_IDLE;
    }
    
    return STATE_COMPLETE;
}

State handle_error(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("ERROR: An error occurred. Enter 'reset' to continue: ");
    if (fgets(ctx->buffer, MAX_INPUT_LEN, stdin) == NULL) {
        return STATE_ERROR;
    }
    
    ctx->buffer[strcspn(ctx->buffer, "\n")] = '\0';
    
    if (strcmp(ctx->buffer, "reset") == 0) {
        return STATE_IDLE;
    }
    
    return STATE_ERROR;
}

int main() {
    Context context = {STATE_IDLE, "", 0};
    StateHandler handlers[MAX_STATES] = {
        handle_idle,
        handle_ready,
        handle_processing,
        handle_complete,
        handle_error
    };
    
    printf("State Machine Demo - Number Collector\n");
    printf("Enter commands as prompted.\n\n");
    
    while (1) {
        if (context.current_state < 0 || context.current_state >= MAX_STATES) {
            context.current_state = STATE_ERROR;
        }
        
        StateHandler handler = handlers[context.current_state];
        if (handler == NULL) {
            break;
        }
        
        State next_state = handler(&context);
        if (next_state < 0 || next_state >= MAX_STATES) {
            break;
        }
        
        context.current_state = next_state;
        
        if (context.current_state == STATE_COMPLETE && context.data_count == 0) {
            break;
        }
    }
    
    printf("Program terminated.\n");
    return 0;
}