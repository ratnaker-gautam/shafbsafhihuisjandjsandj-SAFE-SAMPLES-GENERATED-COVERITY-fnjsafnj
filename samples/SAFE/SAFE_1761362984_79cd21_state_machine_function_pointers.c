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
    STATE_COUNT
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET,
    EVENT_COUNT
} Event;

typedef struct {
    int data;
    int processed_data;
    char message[64];
} Context;

typedef State (*StateHandler)(Context*, Event);

State handle_idle(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    switch (evt) {
        case EVENT_START:
            printf("Starting data processing\n");
            ctx->data = 0;
            return STATE_READING;
        case EVENT_RESET:
            printf("System reset\n");
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

State handle_reading(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    switch (evt) {
        case EVENT_DATA_READY:
            if (ctx->data < 0) return STATE_ERROR;
            ctx->data = 42;
            printf("Data read: %d\n", ctx->data);
            return STATE_PROCESSING;
        case EVENT_ERROR:
            printf("Error during reading\n");
            return STATE_ERROR;
        default:
            return STATE_ERROR;
    }
}

State handle_processing(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    switch (evt) {
        case EVENT_PROCESS_COMPLETE:
            if (ctx->data > 1000) return STATE_ERROR;
            ctx->processed_data = ctx->data * 2;
            printf("Data processed: %d -> %d\n", ctx->data, ctx->processed_data);
            return STATE_WRITING;
        case EVENT_ERROR:
            printf("Error during processing\n");
            return STATE_ERROR;
        default:
            return STATE_ERROR;
    }
}

State handle_writing(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    switch (evt) {
        case EVENT_WRITE_COMPLETE:
            if (ctx->processed_data < 0) return STATE_ERROR;
            printf("Data written: %d\n", ctx->processed_data);
            return STATE_IDLE;
        case EVENT_ERROR:
            printf("Error during writing\n");
            return STATE_ERROR;
        default:
            return STATE_ERROR;
    }
}

State handle_error(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    switch (evt) {
        case EVENT_RESET:
            printf("Resetting from error state\n");
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

StateHandler state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_reading,
    handle_processing,
    handle_writing,
    handle_error
};

const char* state_names[STATE_COUNT] = {
    "IDLE",
    "READING",
    "PROCESSING",
    "WRITING",
    "ERROR"
};

const char* event_names[EVENT_COUNT] = {
    "START",
    "DATA_READY",
    "PROCESS_COMPLETE",
    "WRITE_COMPLETE",
    "ERROR",
    "RESET"
};

int main() {
    State current_state = STATE_IDLE;
    Context context;
    memset(&context, 0, sizeof(Context));
    
    Event events[] = {
        EVENT_START,
        EVENT_DATA_READY,
        EVENT_PROCESS_COMPLETE,
        EVENT_WRITE_COMPLETE,
        EVENT_START,
        EVENT_DATA_READY,
        EVENT_ERROR,
        EVENT_RESET,
        EVENT_START,
        EVENT_DATA_READY,
        EVENT_PROCESS_COMPLETE,
        EVENT_WRITE_COMPLETE
    };
    
    size_t num_events = sizeof(events) / sizeof(events[0]);
    
    printf("State Machine Demo\n");
    printf("Current state: %s\n", state_names[current_state]);
    
    for (size_t i = 0; i < num_events; i++) {
        Event current_event = events[i];
        if (current_event >= EVENT_COUNT) {
            printf("Invalid event\n");
            continue;
        }
        
        printf("Processing event: %s\n", event_names[current_event]);
        
        if (current_state >= STATE_COUNT) {
            printf("Invalid state\n");
            break;
        }
        
        StateHandler handler = state_handlers[current_state];
        if (handler == NULL) {
            printf("No handler for state\n");
            break;
        }
        
        State new_state = handler(&context, current_event);
        
        if (new_state >= STATE_COUNT) {
            printf("Invalid new state\n");
            break;
        }
        
        printf("State transition: %s -> %s\n", 
               state_names[current_state], state_names[new_state]);
        
        current_state = new_state;
        
        if (i < num_events - 1) {
            printf("---\n");
        }
    }
    
    printf("State machine execution complete\n");
    printf("Final state: %s\n", state_names[current_state]);
    
    return 0;
}