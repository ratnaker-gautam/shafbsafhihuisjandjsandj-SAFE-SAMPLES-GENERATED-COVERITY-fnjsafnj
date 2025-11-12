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
} State;

typedef struct {
    State current_state;
    int data;
    int counter;
} Context;

typedef void (*StateHandler)(Context*);

void handle_idle(Context* ctx) {
    if (ctx->current_state != STATE_IDLE) return;
    
    printf("IDLE: Enter command (start/exit): ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "start") == 0) {
            ctx->current_state = STATE_READY;
            ctx->counter = 0;
            printf("Transitioning to READY state\n");
        } else if (strcmp(input, "exit") == 0) {
            ctx->current_state = STATE_DONE;
            printf("Exiting state machine\n");
        } else {
            printf("Invalid command. Available: start, exit\n");
        }
    }
}

void handle_ready(Context* ctx) {
    if (ctx->current_state != STATE_READY) return;
    
    printf("READY: Enter data value (0-100) or 'back': ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "back") == 0) {
            ctx->current_state = STATE_IDLE;
            printf("Returning to IDLE state\n");
        } else {
            char* endptr;
            long value = strtol(input, &endptr, 10);
            if (endptr != input && *endptr == '\0' && value >= 0 && value <= 100) {
                ctx->data = (int)value;
                ctx->current_state = STATE_PROCESSING;
                printf("Data accepted: %d. Transitioning to PROCESSING\n", ctx->data);
            } else {
                printf("Invalid input. Enter number 0-100 or 'back'\n");
            }
        }
    }
}

void handle_processing(Context* ctx) {
    if (ctx->current_state != STATE_PROCESSING) return;
    
    if (ctx->counter < 3) {
        printf("PROCESSING: Step %d/3 with data %d\n", ctx->counter + 1, ctx->data);
        ctx->data *= 2;
        if (ctx->data > 1000) {
            ctx->current_state = STATE_ERROR;
            printf("Error: Data overflow detected\n");
            return;
        }
        ctx->counter++;
    }
    
    if (ctx->counter >= 3) {
        ctx->current_state = STATE_DONE;
        printf("Processing complete. Final data: %d\n", ctx->data);
    }
}

void handle_done(Context* ctx) {
    if (ctx->current_state != STATE_DONE) return;
    printf("DONE: All operations completed successfully\n");
}

void handle_error(Context* ctx) {
    if (ctx->current_state != STATE_ERROR) return;
    printf("ERROR: An error occurred during processing\n");
}

int main() {
    Context ctx = {STATE_IDLE, 0, 0};
    StateHandler handlers[] = {
        handle_idle,
        handle_ready,
        handle_processing,
        handle_done,
        handle_error
    };
    
    printf("State Machine Demo Started\n");
    printf("Available states: IDLE, READY, PROCESSING, DONE, ERROR\n");
    
    while (ctx.current_state != STATE_DONE && ctx.current_state != STATE_ERROR) {
        if (ctx.current_state >= STATE_IDLE && ctx.current_state <= STATE_ERROR) {
            handlers[ctx.current_state](&ctx);
        } else {
            printf("Invalid state detected\n");
            break;
        }
    }
    
    if (ctx.current_state == STATE_ERROR) {
        handle_error(&ctx);
    }
    
    printf("State Machine Terminated\n");
    return 0;
}