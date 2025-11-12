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

static state_t handle_start(context_t *ctx, event_t event);
static state_t handle_reading(context_t *ctx, event_t event);
static state_t handle_processing(context_t *ctx, event_t event);
static state_t handle_finished(context_t *ctx, event_t event);
static state_t handle_error(context_t *ctx, event_t event);

static void initialize_context(context_t *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_START;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_len = 0;
    ctx->data_count = 0;
}

static int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == 0) break;
        if (!(input[i] >= 'a' && input[i] <= 'z') && 
            !(input[i] >= 'A' && input[i] <= 'Z') &&
            !(input[i] >= '0' && input[i] <= '9') &&
            input[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

static event_t get_event_from_input(const char *input, size_t len) {
    if (input == NULL || len == 0) return EVENT_ERROR;
    
    if (strncmp(input, "start", 5) == 0) return EVENT_START;
    if (strncmp(input, "data", 4) == 0) return EVENT_DATA;
    if (strncmp(input, "complete", 8) == 0) return EVENT_COMPLETE;
    if (strncmp(input, "error", 5) == 0) return EVENT_ERROR;
    if (strncmp(input, "reset", 5) == 0) return EVENT_RESET;
    
    return EVENT_ERROR;
}

static state_t handle_start(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("State machine started.\n");
            return STATE_READING;
        case EVENT_RESET:
            printf("Already in start state.\n");
            return STATE_START;
        default:
            printf("Invalid event for start state.\n");
            return STATE_ERROR;
    }
}

static state_t handle_reading(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (ctx->data_count < 3) {
                ctx->data_count++;
                printf("Data received (%d/3).\n", ctx->data_count);
                return STATE_READING;
            } else {
                printf("All data received.\n");
                return STATE_PROCESSING;
            }
        case EVENT_RESET:
            printf("Resetting from reading state.\n");
            initialize_context(ctx);
            return STATE_START;
        case EVENT_ERROR:
            printf("Error during reading.\n");
            return STATE_ERROR;
        default:
            printf("Invalid event for reading state.\n");
            return STATE_ERROR;
    }
}

static state_t handle_processing(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_COMPLETE:
            printf("Processing complete.\n");
            return STATE_FINISHED;
        case EVENT_RESET:
            printf("Resetting from processing state.\n");
            initialize_context(ctx);
            return STATE_START;
        case EVENT_ERROR:
            printf("Error during processing.\n");
            return STATE_ERROR;
        default:
            printf("Invalid event for processing state.\n");
            return STATE_ERROR;
    }
}

static state_t handle_finished(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from finished state.\n");
            initialize_context(ctx);
            return STATE_START;
        default:
            printf("Invalid event for finished state.\n");
            return STATE_ERROR;
    }
}

static state_t handle_error(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from error state.\n");
            initialize_context(ctx);
            return STATE_START;
        default:
            printf("Invalid event for error state.\n");
            return STATE_ERROR;
    }
}

static state_t process_event(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (ctx->current_state) {
        case STATE_START:
            return handle_start(ctx, event);
        case STATE_READING: