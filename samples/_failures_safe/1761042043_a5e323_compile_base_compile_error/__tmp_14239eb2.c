//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_DONE,
    STATE_ERROR
} state_t;

typedef enum {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
} event_t;

typedef struct {
    state_t current_state;
    char buffer[MAX_INPUT_LEN];
    size_t data_length;
    int processed_data;
} context_t;

state_t handle_start(context_t *ctx, event_t event);
state_t handle_reading(context_t *ctx, event_t event);
state_t handle_processing(context_t *ctx, event_t event);
state_t handle_writing(context_t *ctx, event_t event);
state_t handle_done(context_t *ctx, event_t event);
state_t handle_error(context_t *ctx, event_t event);

typedef state_t (*state_handler_t)(context_t *, event_t);

state_handler_t state_handlers[] = {
    handle_start,
    handle_reading,
    handle_processing,
    handle_writing,
    handle_done,
    handle_error
};

state_t handle_start(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("State machine started\n");
            ctx->data_length = 0;
            ctx->processed_data = 0;
            memset(ctx->buffer, 0, sizeof(ctx->buffer));
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

state_t handle_reading(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA_READY:
            printf("Enter data to process (max %d chars): ", MAX_INPUT_LEN - 1);
            
            if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
                return STATE_ERROR;
            }
            
            ctx->data_length = strlen(ctx->buffer);
            if (ctx->data_length > 0 && ctx->buffer[ctx->data_length - 1] == '\n') {
                ctx->buffer[ctx->data_length - 1] = '\0';
                ctx->data_length--;
            }
            
            if (ctx->data_length == 0) {
                printf("No data entered\n");
                return STATE_ERROR;
            }
            
            printf("Read %zu characters\n", ctx->data_length);
            return STATE_PROCESSING;
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

state_t handle_processing(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_PROCESS_COMPLETE:
            if (ctx->data_length == 0 || ctx->data_length >= sizeof(ctx->buffer)) {
                return STATE_ERROR;
            }
            
            ctx->processed_data = 0;
            for (size_t i = 0; i < ctx->data_length; i++) {
                if (ctx->buffer[i] >= '0' && ctx->buffer[i] <= '9') {
                    ctx->processed_data += (ctx->buffer[i] - '0');
                }
            }
            
            printf("Processing complete. Sum of digits: %d\n", ctx->processed_data);
            return STATE_WRITING;
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

state_t handle_writing(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_WRITE_COMPLETE:
            printf("Writing result: %d\n", ctx->processed_data);
            printf("Original data: %s\n", ctx->buffer);
            return STATE_DONE;
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

state_t handle_done(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_DONE;
    }
}

state_t handle_error(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from error state\n");
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

state_t process_event(context_t *ctx, event_t event) {
    if (ctx == NULL || ctx->current_state >= sizeof(state_handlers)/sizeof(state_handlers[0])) {
        return STATE_ERROR;
    }
    
    state_handler_t handler = state_handlers[ctx->current_state];
    if (handler == NULL) {
        return STATE_ERROR;
    }
    
    return handler(ctx, event);
}

int main(void) {
    context_t ctx;
    ctx.current_state = STATE_START;
    
    printf("State Machine Demo - Enter 'q' to quit, 'r' to reset\n");
    
    while (1) {
        event_t next_event;
        char input[10];
        
        switch (ctx.current_state) {
            case STATE_START: