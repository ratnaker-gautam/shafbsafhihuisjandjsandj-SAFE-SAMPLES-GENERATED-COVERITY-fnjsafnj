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
    int processed;
    char buffer[64];
} Context;

typedef State (*StateHandler)(Context*, Event);

State handle_idle(Context* ctx, Event evt);
State handle_reading(Context* ctx, Event evt);
State handle_processing(Context* ctx, Event evt);
State handle_writing(Context* ctx, Event evt);
State handle_error(Context* ctx, Event evt);

StateHandler handlers[STATE_COUNT] = {
    handle_idle,
    handle_reading,
    handle_processing,
    handle_writing,
    handle_error
};

State handle_idle(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_START:
            printf("Starting data processing...\n");
            ctx->data = 0;
            ctx->processed = 0;
            memset(ctx->buffer, 0, sizeof(ctx->buffer));
            return STATE_READING;
        case EVENT_RESET:
            printf("System reset.\n");
            return STATE_IDLE;
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
            printf("Error during reading.\n");
            return STATE_ERROR;
        case EVENT_RESET:
            printf("Reset during reading.\n");
            return STATE_IDLE;
        default:
            return STATE_READING;
    }
}

State handle_processing(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_PROCESS_COMPLETE:
            ctx->processed = ctx->data * 2;
            if (ctx->processed < 0 || ctx->processed > 1000) {
                return STATE_ERROR;
            }
            printf("Processed data: %d -> %d\n", ctx->data, ctx->processed);
            snprintf(ctx->buffer, sizeof(ctx->buffer), "Result: %d", ctx->processed);
            return STATE_WRITING;
        case EVENT_ERROR:
            printf("Error during processing.\n");
            return STATE_ERROR;
        case EVENT_RESET:
            printf("Reset during processing.\n");
            return STATE_IDLE;
        default:
            return STATE_PROCESSING;
    }
}

State handle_writing(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_WRITE_COMPLETE:
            printf("Written: %s\n", ctx->buffer);
            if (ctx->processed >= 100) {
                printf("Processing complete.\n");
                return STATE_IDLE;
            } else {
                return STATE_READING;
            }
        case EVENT_ERROR:
            printf("Error during writing.\n");
            return STATE_ERROR;
        case EVENT_RESET:
            printf("Reset during writing.\n");
            return STATE_IDLE;
        default:
            return STATE_WRITING;
    }
}

State handle_error(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_RESET:
            printf("Recovering from error state.\n");
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

int main(void) {
    State current_state = STATE_IDLE;
    Context context = {0};
    int running = 1;
    
    printf("State Machine Demo - Enter commands:\n");
    printf("0=START, 1=DATA_READY, 2=PROCESS_COMPLETE, 3=WRITE_COMPLETE, 4=ERROR, 5=RESET, 6=QUIT\n");
    
    while (running) {
        int input;
        printf("Current state: %d > ", current_state);
        
        if (scanf("%d", &input) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (input < 0 || input >= EVENT_COUNT + 1) {
            printf("Invalid event. Use 0-6.\n");
            continue;
        }
        
        if (input == 6) {
            running = 0;
            continue;
        }
        
        Event event = (Event)input;
        
        if (current_state >= 0 && current_state < STATE_COUNT && handlers[current_state] != NULL) {
            State new_state = handlers[current_state](&context, event);
            
            if (new_state >= 0 && new_state < STATE_COUNT) {
                current_state = new_state;
            } else {
                printf("Invalid state transition.\n");
                current_state = STATE_ERROR