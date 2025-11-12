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
    int processed_count;
} context_t;

static event_t get_next_event(void) {
    char input[MAX_INPUT_LEN];
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_ERROR;
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
    }
    
    return EVENT_INVALID;
}

static state_t handle_start_state(event_t event, context_t *ctx) {
    switch (event) {
        case EVENT_START:
            printf("State machine started\n");
            memset(ctx->buffer, 0, sizeof(ctx->buffer));
            ctx->buffer_len = 0;
            ctx->processed_count = 0;
            return STATE_READING;
        default:
            printf("Invalid event for START state\n");
            return STATE_ERROR;
    }
}

static state_t handle_reading_state(event_t event, context_t *ctx) {
    switch (event) {
        case EVENT_DATA:
            if (ctx->buffer_len < MAX_INPUT_LEN - 6) {
                strcat(ctx->buffer, "data ");
                ctx->buffer_len += 5;
                printf("Data added to buffer\n");
            } else {
                printf("Buffer full\n");
                return STATE_ERROR;
            }
            return STATE_READING;
        case EVENT_PROCESS:
            if (ctx->buffer_len > 0) {
                printf("Processing buffer: %s\n", ctx->buffer);
                return STATE_PROCESSING;
            } else {
                printf("No data to process\n");
                return STATE_READING;
            }
        default:
            printf("Invalid event for READING state\n");
            return STATE_ERROR;
    }
}

static state_t handle_processing_state(event_t event, context_t *ctx) {
    switch (event) {
        case EVENT_COMPLETE:
            ctx->processed_count++;
            printf("Processing complete. Count: %d\n", ctx->processed_count);
            memset(ctx->buffer, 0, sizeof(ctx->buffer));
            ctx->buffer_len = 0;
            return STATE_READING;
        case EVENT_ERROR:
            printf("Error during processing\n");
            return STATE_ERROR;
        default:
            printf("Invalid event for PROCESSING state\n");
            return STATE_ERROR;
    }
}

static state_t handle_finished_state(event_t event, context_t *ctx) {
    switch (event) {
        case EVENT_START:
            return handle_start_state(event, ctx);
        default:
            printf("Machine finished. Send 'start' to restart\n");
            return STATE_FINISHED;
    }
}

static state_t handle_error_state(event_t event, context_t *ctx) {
    switch (event) {
        case EVENT_START:
            return handle_start_state(event, ctx);
        default:
            printf("In error state. Send 'start' to reset\n");
            return STATE_ERROR;
    }
}

static state_t process_event(state_t current_state, event_t event, context_t *ctx) {
    switch (current_state) {
        case STATE_START:
            return handle_start_state(event, ctx);
        case STATE_READING:
            return handle_reading_state(event, ctx);
        case STATE_PROCESSING:
            return handle_processing_state(event, ctx);
        case STATE_FINISHED:
            return handle_finished_state(event, ctx);
        case STATE_ERROR:
            return handle_error_state(event, ctx);
        default:
            return STATE_ERROR;
    }
}

static void print_help(void) {
    printf("Available commands:\n");
    printf("  start    - Start the state machine\n");
    printf("  data     - Add data to buffer\n");
    printf("  process  - Process current buffer\n");
    printf("  complete - Complete processing\n");
    printf("  error    - Simulate error\n");
    printf("  quit     - Exit program\n");
}

int main(void) {
    context_t ctx;
    state_t current_state = STATE_START;
    event_t event;
    int running = 1;
    
    memset(&ctx, 0, sizeof(ctx));
    
    printf("State Machine Demo\n");
    print_help();
    
    while (running) {
        printf("\nCurrent state: ");
        switch (current_state) {
            case STATE_START:
                printf("START");