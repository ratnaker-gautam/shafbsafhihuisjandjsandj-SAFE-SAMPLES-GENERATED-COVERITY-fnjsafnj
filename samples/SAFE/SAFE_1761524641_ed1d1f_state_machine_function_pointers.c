//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR,
    NUM_STATES
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET,
    NUM_EVENTS
} Event;

typedef struct {
    int data;
    int processed_value;
    int error_code;
} Context;

typedef State (*StateHandler)(Context*, Event);

State handle_idle(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_START:
            ctx->data = 0;
            ctx->processed_value = 0;
            ctx->error_code = 0;
            return STATE_READY;
        default:
            return STATE_IDLE;
    }
}

State handle_ready(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_DATA_READY:
            ctx->data = 42;
            return STATE_PROCESSING;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_READY;
    }
}

State handle_processing(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_PROCESS:
            if (ctx->data > 1000) {
                ctx->error_code = 1;
                return STATE_ERROR;
            }
            ctx->processed_value = ctx->data * 2;
            if (ctx->processed_value < 0) {
                ctx->error_code = 2;
                return STATE_ERROR;
            }
            return STATE_COMPLETE;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_PROCESSING;
    }
}

State handle_complete(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_SUCCESS:
            return STATE_IDLE;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_COMPLETE;
    }
}

State handle_error(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_RESET:
            ctx->error_code = 0;
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

StateHandler state_handlers[NUM_STATES] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

Event get_user_input(void) {
    char input[32];
    printf("Enter event (start, data, process, success, failure, reset, quit): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_FAILURE;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "data") == 0) return EVENT_DATA_READY;
    if (strcmp(input, "process") == 0) return EVENT_PROCESS;
    if (strcmp(input, "success") == 0) return EVENT_SUCCESS;
    if (strcmp(input, "failure") == 0) return EVENT_FAILURE;
    if (strcmp(input, "reset") == 0) return EVENT_RESET;
    if (strcmp(input, "quit") == 0) return NUM_EVENTS;
    
    return EVENT_FAILURE;
}

void print_state_info(State current_state, Context* ctx) {
    if (ctx == NULL) return;
    
    const char* state_names[] = {
        "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
    };
    
    printf("Current state: %s\n", state_names[current_state]);
    printf("Data: %d, Processed: %d, Error: %d\n", 
           ctx->data, ctx->processed_value, ctx->error_code);
}

int main(void) {
    State current_state = STATE_IDLE;
    Context ctx = {0};
    Event event;
    
    printf("State Machine Demo\n");
    printf("Available events: start, data, process, success, failure, reset, quit\n");
    
    while (1) {
        print_state_info(current_state, &ctx);
        event = get_user_input();
        
        if (event == NUM_EVENTS) {
            break;
        }
        
        if (current_state >= 0 && current_state < NUM_STATES) {
            StateHandler handler = state_handlers[current_state];
            if (handler != NULL) {
                State new_state = handler(&ctx, event);
                if (new_state >= 0 && new_state < NUM_STATES) {
                    current_state = new_state;
                } else {
                    current_state = STATE_ERROR;
                    ctx.error_code = 3;
                }
            } else {
                current_state = STATE_ERROR;
                ctx.error_code = 4;
            }
        } else {
            current_state = STATE_ERROR;
            ctx.error_code = 5;
        }
    }
    
    printf("Exiting state machine.\n");
    return 0;
}