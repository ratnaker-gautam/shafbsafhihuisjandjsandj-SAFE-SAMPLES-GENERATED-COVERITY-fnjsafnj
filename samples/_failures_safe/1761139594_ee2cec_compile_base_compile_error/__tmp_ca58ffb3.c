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
    EVENT_DONE,
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
    
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "data") == 0) return EVENT_DATA;
    if (strcmp(input, "process") == 0) return EVENT_PROCESS;
    if (strcmp(input, "done") == 0) return EVENT_DONE;
    if (strcmp(input, "error") == 0) return EVENT_ERROR;
    if (strcmp(input, "quit") == 0) return EVENT_INVALID;
    
    return EVENT_INVALID;
}

static state_t handle_start_state(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("State machine started\n");
            ctx->buffer_len = 0;
            ctx->data_count = 0;
            memset(ctx->buffer, 0, sizeof(ctx->buffer));
            return STATE_READING;
        default:
            printf("Invalid event for START state\n");
            return STATE_ERROR;
    }
}

static state_t handle_reading_state(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (ctx->data_count < MAX_STATES - 1) {
                ctx->data_count++;
                printf("Data received (%d/9)\n", ctx->data_count);
                return STATE_READING;
            } else {
                printf("Maximum data count reached\n");
                return STATE_PROCESSING;
            }
        case EVENT_PROCESS:
            if (ctx->data_count > 0) {
                printf("Processing %d data items\n", ctx->data_count);
                return STATE_PROCESSING;
            } else {
                printf("No data to process\n");
                return STATE_READING;
            }
        case EVENT_ERROR:
            printf("Error during data reading\n");
            return STATE_ERROR;
        default:
            printf("Invalid event for READING state\n");
            return STATE_READING;
    }
}

static state_t handle_processing_state(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DONE:
            printf("Processing completed successfully\n");
            return STATE_FINISHED;
        case EVENT_ERROR:
            printf("Error during processing\n");
            return STATE_ERROR;
        case EVENT_DATA:
            printf("Cannot accept new data during processing\n");
            return STATE_PROCESSING;
        default:
            printf("Invalid event for PROCESSING state\n");
            return STATE_PROCESSING;
    }
}

static state_t handle_finished_state(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("Restarting state machine\n");
            ctx->buffer_len = 0;
            ctx->data_count = 0;
            memset(ctx->buffer, 0, sizeof(ctx->buffer));
            return STATE_READING;
        default:
            printf("State machine finished. Send 'start' to restart.\n");
            return STATE_FINISHED;
    }
}

static state_t handle_error_state(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("Recovering from error state\n");
            ctx->buffer_len = 0;
            ctx->data_count = 0;
            memset(ctx->buffer, 0, sizeof(ctx->buffer));
            return STATE_READING;
        default:
            printf("In error state. Send 'start' to recover.\n");
            return STATE_ERROR;
    }
}

static state_t process_event(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
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

static void print_help(void) {
    printf("Available commands: start, data, process, done, error, quit\n");
}

int main(void) {
    context_t ctx;
    ctx.current_state = STATE_START;
    ctx.buffer_len = 0;
    ctx.data_count = 0;