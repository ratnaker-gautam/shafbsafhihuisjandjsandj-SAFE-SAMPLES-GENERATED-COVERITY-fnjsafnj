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
    int data_value;
    int processed_value;
    int error_code;
} Context;

typedef State (*StateHandler)(Context*, Event);

State handle_idle(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_START:
            ctx->data_value = 0;
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
            if (ctx->data_value >= 0 && ctx->data_value < 1000) {
                ctx->data_value += 10;
            }
            return STATE_READY;
        case EVENT_PROCESS:
            if (ctx->data_value > 0) {
                return STATE_PROCESSING;
            }
            return STATE_READY;
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
            if (ctx->data_value > 0) {
                ctx->processed_value = ctx->data_value * 2;
                if (ctx->processed_value < ctx->data_value) return STATE_ERROR;
                return STATE_COMPLETE;
            }
            return STATE_ERROR;
        case EVENT_FAILURE:
            ctx->error_code = 1;
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

Event get_user_event() {
    char input[32];
    printf("Enter event (0=START, 1=DATA, 2=PROCESS, 3=SUCCESS, 4=FAILURE, 5=RESET, 6=QUIT): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_COUNT;
    }
    
    char* endptr;
    long value = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\n') {
        return EVENT_COUNT;
    }
    
    if (value < 0 || value >= EVENT_COUNT) {
        return EVENT_COUNT;
    }
    
    return (Event)value;
}

int main() {
    State current_state = STATE_IDLE;
    Context context = {0};
    
    printf("State Machine Demo\n");
    printf("Current state: IDLE\n");
    
    while (1) {
        Event event = get_user_event();
        
        if (event == EVENT_COUNT) {
            printf("Invalid event or input error\n");
            continue;
        }
        
        if (event == 6) {
            break;
        }
        
        if (current_state < 0 || current_state >= STATE_COUNT) {
            printf("Invalid state detected\n");
            break;
        }
        
        StateHandler handler = state_handlers[current_state];
        if (handler == NULL) {
            printf("No handler for state\n");
            break;
        }
        
        State new_state = handler(&context, event);
        
        if (new_state < 0 || new_state >= STATE_COUNT) {
            printf("Invalid state transition\n");
            break;
        }
        
        if (new_state != current_state) {
            const char* state_names[] = {
                "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
            };
            printf("State changed: %s -> %s\n", 
                   state_names[current_state], state_names[new_state]);
            current_state = new_state;
        }
        
        printf("Data: %d, Processed: %d, Error: %d\n", 
               context.data_value, context.processed_value, context.error_code);
    }
    
    printf("Exiting state machine\n");
    return 0;
}