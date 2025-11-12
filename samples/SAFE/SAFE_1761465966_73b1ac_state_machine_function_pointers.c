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
    char buffer[64];
} Context;

typedef State (*StateHandler)(Context*, Event);

State handle_idle(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_START:
            printf("Starting data processing\n");
            ctx->data = 0;
            memset(ctx->buffer, 0, sizeof(ctx->buffer));
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
            if (scanf("%d", &ctx->data) != 1) {
                printf("Invalid input\n");
                return STATE_ERROR;
            }
            if (ctx->data < 0 || ctx->data > 1000) {
                printf("Data out of range\n");
                return STATE_ERROR;
            }
            printf("Read data: %d\n", ctx->data);
            return STATE_PROCESSING;
        case EVENT_RESET:
            printf("Reset during reading\n");
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

State handle_processing(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_PROCESS_COMPLETE:
            ctx->processed_data = ctx->data * 2;
            if (ctx->processed_data < 0) return STATE_ERROR;
            printf("Processed data: %d\n", ctx->processed_data);
            return STATE_WRITING;
        case EVENT_RESET:
            printf("Reset during processing\n");
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

State handle_writing(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_WRITE_COMPLETE:
            snprintf(ctx->buffer, sizeof(ctx->buffer), "Result: %d", ctx->processed_data);
            printf("%s\n", ctx->buffer);
            return STATE_IDLE;
        case EVENT_RESET:
            printf("Reset during writing\n");
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

State handle_error(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_RESET:
            printf("Recovering from error\n");
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

int main() {
    State current_state = STATE_IDLE;
    Context context;
    memset(&context, 0, sizeof(context));
    
    StateHandler handlers[STATE_COUNT] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_error
    };
    
    printf("State Machine Demo\n");
    printf("Commands: 0=Start, 1=DataReady, 2=ProcessComplete, 3=WriteComplete, 4=Error, 5=Reset, 6=Exit\n");
    
    while (1) {
        int input;
        printf("Current state: %d | Enter command: ", current_state);
        
        if (scanf("%d", &input) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (input < 0 || input >= EVENT_COUNT + 1) {
            printf("Invalid command\n");
            continue;
        }
        
        if (input == 6) {
            printf("Exiting\n");
            break;
        }
        
        Event event = (Event)input;
        State new_state = handlers[current_state](&context, event);
        
        if (new_state < 0 || new_state >= STATE_COUNT) {
            printf("Invalid state transition\n");
            current_state = STATE_ERROR;
        } else {
            current_state = new_state;
        }
    }
    
    return 0;
}