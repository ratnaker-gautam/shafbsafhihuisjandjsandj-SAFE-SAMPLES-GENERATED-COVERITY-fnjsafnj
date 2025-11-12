//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: state_machine
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
    EVENT_PROCESS_DONE,
    EVENT_WRITE_DONE,
    EVENT_ERROR,
    EVENT_RESET,
    EVENT_COUNT
} Event;

typedef struct {
    int data;
    int processed;
    char buffer[64];
} Context;

typedef State (*StateHandler)(Context*, Event);

State handle_idle(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    switch (evt) {
        case EVENT_START:
            printf("Starting data processing\n");
            ctx->data = 0;
            return STATE_READING;
        default:
            return STATE_IDLE;
    }
}

State handle_reading(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    switch (evt) {
        case EVENT_DATA_READY:
            if (ctx->data < 100) {
                ctx->data += 10;
                printf("Read data: %d\n", ctx->data);
                return STATE_PROCESSING;
            } else {
                return STATE_ERROR;
            }
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_READING;
    }
}

State handle_processing(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    switch (evt) {
        case EVENT_PROCESS_DONE:
            ctx->processed = ctx->data * 2;
            if (ctx->processed < 0 || ctx->processed > 1000) return STATE_ERROR;
            printf("Processed data: %d\n", ctx->processed);
            return STATE_WRITING;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_PROCESSING;
    }
}

State handle_writing(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    switch (evt) {
        case EVENT_WRITE_DONE:
            if (snprintf(ctx->buffer, sizeof(ctx->buffer), "Result: %d", ctx->processed) >= (int)sizeof(ctx->buffer)) {
                return STATE_ERROR;
            }
            printf("Written: %s\n", ctx->buffer);
            if (ctx->processed >= 100) {
                printf("Processing complete\n");
                return STATE_IDLE;
            }
            return STATE_READING;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_WRITING;
    }
}

State handle_error(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    switch (evt) {
        case EVENT_RESET:
            printf("Resetting state machine\n");
            ctx->data = 0;
            ctx->processed = 0;
            memset(ctx->buffer, 0, sizeof(ctx->buffer));
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
    "IDLE", "READING", "PROCESSING", "WRITING", "ERROR"
};

const char* event_names[EVENT_COUNT] = {
    "START", "DATA_READY", "PROCESS_DONE", "WRITE_DONE", "ERROR", "RESET"
};

int main(void) {
    Context ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    State current_state = STATE_IDLE;
    int running = 1;
    int step = 0;
    
    Event events[] = {
        EVENT_START,
        EVENT_DATA_READY,
        EVENT_PROCESS_DONE,
        EVENT_WRITE_DONE,
        EVENT_DATA_READY,
        EVENT_PROCESS_DONE,
        EVENT_WRITE_DONE,
        EVENT_DATA_READY,
        EVENT_PROCESS_DONE,
        EVENT_WRITE_DONE,
        EVENT_DATA_READY,
        EVENT_PROCESS_DONE,
        EVENT_WRITE_DONE,
        EVENT_RESET
    };
    
    int num_events = sizeof(events) / sizeof(events[0]);
    
    printf("State Machine Demo\n");
    printf("Current state: %s\n", state_names[current_state]);
    
    while (running && step < num_events) {
        Event current_event = events[step];
        
        if (current_event < 0 || current_event >= EVENT_COUNT) {
            printf("Invalid event\n");
            break;
        }
        
        printf("Processing event: %s\n", event_names[current_event]);
        
        if (current_state < 0 || current_state >= STATE_COUNT) {
            printf("Invalid state\n");
            break;
        }
        
        StateHandler handler = state_handlers[current_state];
        if (handler == NULL) {
            printf("No handler for state\n");
            break;
        }
        
        State new_state = handler(&ctx, current_event);
        
        if (new_state < 0 || new_state >= STATE_COUNT) {
            printf("Invalid new state\n");
            break;
        }
        
        printf("State transition: %s -> %s\n", 
               state_names[current_state], state_names[new_state]);