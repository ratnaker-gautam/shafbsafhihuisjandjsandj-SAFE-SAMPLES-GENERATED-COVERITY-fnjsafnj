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
    STATE_COUNT
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET,
    EVENT_COUNT
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
        case EVENT_DATA:
            ctx->data = 42;
            return STATE_READY;
        case EVENT_PROCESS:
            if (ctx->data <= 0) {
                ctx->error_code = 1;
                return STATE_ERROR;
            }
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
        case EVENT_SUCCESS:
            if (ctx->data > 1000) return STATE_ERROR;
            ctx->processed_value = ctx->data * 2;
            return STATE_COMPLETE;
        case EVENT_FAILURE:
            ctx->error_code = 2;
            return STATE_ERROR;
        default:
            return STATE_PROCESSING;
    }
}

State handle_complete(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (evt) {
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

StateHandler state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

const char* state_names[STATE_COUNT] = {
    "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
};

const char* event_names[EVENT_COUNT] = {
    "START", "DATA", "PROCESS", "SUCCESS", "FAILURE", "RESET"
};

int main() {
    State current_state = STATE_IDLE;
    Context ctx = {0, 0, 0};
    
    Event events[] = {
        EVENT_START,
        EVENT_DATA,
        EVENT_PROCESS,
        EVENT_SUCCESS,
        EVENT_RESET,
        EVENT_START,
        EVENT_PROCESS,
        EVENT_FAILURE,
        EVENT_RESET
    };
    
    int num_events = sizeof(events) / sizeof(events[0]);
    
    printf("State Machine Demo\n");
    printf("Initial state: %s\n", state_names[current_state]);
    
    for (int i = 0; i < num_events; i++) {
        Event evt = events[i];
        if (evt < 0 || evt >= EVENT_COUNT) {
            printf("Invalid event encountered\n");
            continue;
        }
        
        printf("Processing event: %s\n", event_names[evt]);
        
        StateHandler handler = state_handlers[current_state];
        if (handler == NULL) {
            printf("No handler for state %s\n", state_names[current_state]);
            break;
        }
        
        State new_state = handler(&ctx, evt);
        
        if (new_state < 0 || new_state >= STATE_COUNT) {
            printf("Invalid state transition\n");
            break;
        }
        
        printf("State transition: %s -> %s\n", 
               state_names[current_state], state_names[new_state]);
        
        if (new_state == STATE_COMPLETE) {
            printf("Processed value: %d\n", ctx.processed_value);
        } else if (new_state == STATE_ERROR) {
            printf("Error code: %d\n", ctx.error_code);
        }
        
        current_state = new_state;
        printf("---\n");
    }
    
    return 0;
}