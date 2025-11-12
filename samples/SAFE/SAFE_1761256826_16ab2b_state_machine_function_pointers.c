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
    char buffer[32];
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
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

State handle_reading(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_DATA_READY:
            if (scanf("%d", &ctx->data) == 1) {
                if (ctx->data >= 0 && ctx->data <= 1000) {
                    printf("Read data: %d\n", ctx->data);
                    return STATE_PROCESSING;
                }
            }
            return STATE_ERROR;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

State handle_processing(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_PROCESS_COMPLETE:
            ctx->processed = ctx->data * 2;
            if (ctx->processed < 0) return STATE_ERROR;
            snprintf(ctx->buffer, sizeof(ctx->buffer), "Processed: %d", ctx->processed);
            printf("Processing complete: %s\n", ctx->buffer);
            return STATE_WRITING;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

State handle_writing(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_WRITE_COMPLETE:
            printf("Output: %s\n", ctx->buffer);
            return STATE_IDLE;
        case EVENT_RESET:
            return STATE_IDLE;
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

int main(void) {
    State current_state = STATE_IDLE;
    Context context = {0};
    int running = 1;
    
    StateHandler handlers[STATE_COUNT] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_error
    };
    
    if (STATE_COUNT != 6 || EVENT_COUNT != 7) {
        return EXIT_FAILURE;
    }
    
    printf("State Machine Demo - Enter numbers 0-1000\n");
    printf("Commands: s=start, r=reset, q=quit\n");
    
    while (running) {
        char input[16] = {0};
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = 0;
        
        Event event = EVENT_COUNT;
        
        if (strcmp(input, "s") == 0) {
            event = EVENT_START;
        } else if (strcmp(input, "r") == 0) {
            event = EVENT_RESET;
        } else if (strcmp(input, "q") == 0) {
            break;
        } else {
            int num;
            if (sscanf(input, "%d", &num) == 1) {
                if (current_state == STATE_READING) {
                    context.data = num;
                    event = EVENT_DATA_READY;
                }
            }
        }
        
        if (event < EVENT_COUNT && current_state < STATE_COUNT) {
            StateHandler handler = handlers[current_state];
            if (handler != NULL) {
                State new_state = handler(&context, event);
                if (new_state < STATE_COUNT) {
                    current_state = new_state;
                    
                    if (current_state == STATE_PROCESSING) {
                        current_state = handlers[current_state](&context, EVENT_PROCESS_COMPLETE);
                    } else if (current_state == STATE_WRITING) {
                        current_state = handlers[current_state](&context, EVENT_WRITE_COMPLETE);
                    }
                } else {
                    current_state = STATE_ERROR;
                }
            } else {
                current_state = STATE_ERROR;
            }
        } else {
            printf("Invalid input or state\n");
        }
    }
    
    return EXIT_SUCCESS;
}