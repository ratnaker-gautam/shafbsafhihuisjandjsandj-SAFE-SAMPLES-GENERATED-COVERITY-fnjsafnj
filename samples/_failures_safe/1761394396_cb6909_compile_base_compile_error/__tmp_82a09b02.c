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
    EVENT_PROCESS,
    EVENT_COMPLETE,
    EVENT_ERROR,
    EVENT_INVALID
} event_t;

typedef struct {
    state_t current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_count;
} context_t;

static event_t get_next_event(void) {
    char input[MAX_INPUT_LEN];
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_INVALID;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        return EVENT_INVALID;
    }
    
    if (strcmp(input, "start") == 0) {
        return EVENT_START;
    } else if (strcmp(input, "data") == 0) {
        return EVENT_DATA;
    } else if (strcmp(input, "process") == 0) {
        return EVENT_PROCESS;
    } else if (strcmp(input, "complete") == 0) {
        return EVENT_COMPLETE;
    } else if (strcmp(input, "error") == 0) {
        return EVENT_ERROR;
    } else if (strcmp(input, "quit") == 0) {
        return EVENT_INVALID;
    } else {
        return EVENT_INVALID;
    }
}

static state_t handle_start_state(context_t *ctx, event_t event) {
    if (ctx == NULL) {
        return STATE_ERROR;
    }
    
    switch (event) {
        case EVENT_START:
            memset(ctx->buffer, 0, sizeof(ctx->buffer));
            ctx->buffer_len = 0;
            ctx->data_count = 0;
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

static state_t handle_reading_state(context_t *ctx, event_t event) {
    if (ctx == NULL) {
        return STATE_ERROR;
    }
    
    switch (event) {
        case EVENT_DATA:
            if (ctx->data_count < MAX_STATES - 1) {
                ctx->data_count++;
                return STATE_READING;
            } else {
                return STATE_PROCESSING;
            }
        case EVENT_PROCESS:
            return STATE_PROCESSING;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_READING;
    }
}

static state_t handle_processing_state(context_t *ctx, event_t event) {
    if (ctx == NULL) {
        return STATE_ERROR;
    }
    
    switch (event) {
        case EVENT_COMPLETE:
            return STATE_FINISHED;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_PROCESSING;
    }
}

static state_t handle_finished_state(context_t *ctx, event_t event) {
    if (ctx == NULL) {
        return STATE_ERROR;
    }
    
    return STATE_FINISHED;
}

static state_t handle_error_state(context_t *ctx, event_t event) {
    if (ctx == NULL) {
        return STATE_ERROR;
    }
    
    return STATE_ERROR;
}

static state_t process_event(context_t *ctx, event_t event) {
    if (ctx == NULL) {
        return STATE_ERROR;
    }
    
    switch (ctx->current_state) {
        case STATE_START:
            return handle_start_state(ctx, event);
        case STATE_READING:
            return handle_reading_state(ctx, event);
        case STATE_PROCESSING:
            return handle_processing_state(ctx, event);
        case STATE_FINISHED:
            return handle_finished_state(ctx, event);
        case STATE_ERROR:
            return handle_error_state(ctx, event);
        default:
            return STATE_ERROR;
    }
}

static void print_state_info(state_t state, int data_count) {
    switch (state) {
        case STATE_START:
            printf("State: START\n");
            break;
        case STATE_READING:
            printf("State: READING (Data count: %d)\n", data_count);
            break;
        case STATE_PROCESSING:
            printf("State: PROCESSING\n");
            break;
        case STATE_FINISHED:
            printf("State: FINISHED\n");
            break;
        case STATE_ERROR:
            printf("State: ERROR\n");
            break;
        default:
            printf("State: UNKNOWN\n");
            break;
    }
}

int main(void) {
    context_t ctx;
    ctx.current_state = STATE_START;
    ctx.buffer_len = 0;
    ctx.data_count = 0;
    
    printf("State Machine Demo\n");
    printf("Available commands: start, data, process, complete, error\n");
    printf("Enter 'quit' to exit\n\n");
    
    while (1) {
        print_state_info(ctx.current_state, ctx.data_count);
        printf("Enter command: ");
        
        event_t event = get_next_event();
        if (event == EVENT_INVALID) {
            printf("Invalid command. Try again.\n\n");
            continue;
        }
        
        state_t new_state = process_event(&ctx, event);
        ctx.current_state = new_state;
        
        if (ctx.current_state == STATE_FINISHED) {
            printf