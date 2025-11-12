//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 100

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
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
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
            printf("State machine started\n");
            return STATE_READING;
        default:
            printf("Invalid event for START state\n");
            return STATE_ERROR;
    }
}

static state_t handle_reading_state(context_t *ctx, event_t event) {
    if (ctx == NULL) {
        return STATE_ERROR;
    }
    
    switch (event) {
        case EVENT_DATA:
            if (ctx->data_count < 3) {
                ctx->data_count++;
                printf("Data received (%d/3)\n", ctx->data_count);
                return STATE_READING;
            } else {
                printf("Maximum data count reached\n");
                return STATE_PROCESSING;
            }
        case EVENT_PROCESS:
            printf("Moving to processing\n");
            return STATE_PROCESSING;
        case EVENT_ERROR:
            printf("Error during reading\n");
            return STATE_ERROR;
        default:
            printf("Invalid event for READING state\n");
            return STATE_ERROR;
    }
}

static state_t handle_processing_state(context_t *ctx, event_t event) {
    if (ctx == NULL) {
        return STATE_ERROR;
    }
    
    switch (event) {
        case EVENT_COMPLETE:
            printf("Processing completed successfully\n");
            return STATE_FINISHED;
        case EVENT_ERROR:
            printf("Error during processing\n");
            return STATE_ERROR;
        default:
            printf("Invalid event for PROCESSING state\n");
            return STATE_ERROR;
    }
}

static state_t handle_finished_state(context_t *ctx, event_t event) {
    if (ctx == NULL) {
        return STATE_ERROR;
    }
    
    switch (event) {
        case EVENT_START:
            printf("Restarting state machine\n");
            return handle_start_state(ctx, EVENT_START);
        default:
            printf("State machine finished. Send 'start' to restart.\n");
            return STATE_FINISHED;
    }
}

static state_t handle_error_state(context_t *ctx, event_t event) {
    if (ctx == NULL) {
        return STATE_ERROR;
    }
    
    switch (event) {
        case EVENT_START:
            printf("Restarting from error state\n");
            return handle_start_state(ctx, EVENT_START);
        default:
            printf("In error state. Send 'start' to restart.\n");
            return STATE_ERROR;
    }
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

static void print_state_name(state_t state) {
    switch (state) {
        case STATE_START:
            printf("START");
            break;
        case STATE_READING:
            printf("READING");
            break;
        case STATE_PROCESSING:
            printf("PROCESSING");
            break;
        case STATE_FINISHED:
            printf("FINISHED");
            break;
        case STATE_ERROR:
            printf("ERROR");
            break;
        default:
            printf("UNKNOWN");
            break;
    }
}

static void print_instructions(void) {
    printf("Available commands: