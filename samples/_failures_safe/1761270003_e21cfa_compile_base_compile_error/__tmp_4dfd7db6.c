//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
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
    STATE_FINISHED,
    STATE_ERROR
} state_t;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
} event_t;

typedef struct {
    state_t current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_count;
} context_t;

state_t handle_start(context_t *ctx, event_t event);
state_t handle_reading(context_t *ctx, event_t event);
state_t handle_processing(context_t *ctx, event_t event);
state_t handle_finished(context_t *ctx, event_t event);
state_t handle_error(context_t *ctx, event_t event);

state_t (*state_handlers[])(context_t*, event_t) = {
    handle_start,
    handle_reading,
    handle_processing,
    handle_finished,
    handle_error
};

state_t handle_start(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            memset(ctx->buffer, 0, sizeof(ctx->buffer));
            ctx->buffer_len = 0;
            ctx->data_count = 0;
            printf("State machine started.\n");
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

state_t handle_reading(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (ctx->buffer_len < MAX_INPUT_LEN - 1) {
                char input[2];
                printf("Enter character (or 'q' to finish): ");
                if (fgets(input, sizeof(input), stdin) != NULL) {
                    if (input[0] == 'q') {
                        return STATE_PROCESSING;
                    }
                    ctx->buffer[ctx->buffer_len++] = input[0];
                    ctx->data_count++;
                    printf("Added '%c', total: %zu characters\n", input[0], ctx->buffer_len);
                }
            } else {
                printf("Buffer full, moving to processing.\n");
                return STATE_PROCESSING;
            }
            return STATE_READING;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_READING;
    }
}

state_t handle_processing(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_COMPLETE:
            printf("Processing complete. Data: ");
            for (size_t i = 0; i < ctx->buffer_len; i++) {
                putchar(ctx->buffer[i]);
            }
            printf("\nTotal characters processed: %zu\n", ctx->buffer_len);
            return STATE_FINISHED;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_PROCESSING;
    }
}

state_t handle_finished(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            printf("Resetting state machine.\n");
            return STATE_START;
        default:
            return STATE_FINISHED;
    }
}

state_t handle_error(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            printf("Error state cleared, resetting.\n");
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

event_t get_user_event(state_t current_state) {
    char input[10];
    
    switch (current_state) {
        case STATE_START:
            return EVENT_START;
        case STATE_READING:
            return EVENT_DATA;
        case STATE_PROCESSING:
            return EVENT_COMPLETE;
        case STATE_FINISHED:
            printf("Press 'r' to reset or 'q' to quit: ");
            break;
        case STATE_ERROR:
            printf("Press 'r' to reset: ");
            break;
        default:
            return EVENT_ERROR;
    }
    
    if (fgets(input, sizeof(input), stdin) != NULL) {
        if (input[0] == 'r') return EVENT_RESET;
        if (input[0] == 'q') return EVENT_ERROR;
    }
    return EVENT_ERROR;
}

int main(void) {
    context_t ctx;
    state_t current_state = STATE_START;
    event_t current_event;
    
    printf("State Machine Demo\n");
    printf("==================\n");
    
    while (1) {
        if (current_state >= STATE_START && current_state <= STATE_ERROR) {
            current_event = get_user_event(current_state);
            state_t new_state = state_handlers[current_state](&ctx, current_event);
            
            if (new_state >= STATE_START && new_state <= STATE_ERROR) {
                current_state = new_state;
            } else {
                current_state = STATE_ERROR;
            }
            
            if (current_state == STATE_FINISHED) {
                current_event = get_user_event(current_state);
                if (current_event == EVENT_RESET) {
                    current_state = state_handlers[current_state](&ctx, current_event);
                } else {
                    break;
                }
            }
        } else