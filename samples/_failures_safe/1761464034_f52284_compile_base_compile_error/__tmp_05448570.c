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

void initialize_context(context_t *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_START;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_len = 0;
    ctx->data_count = 0;
}

int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') return 0;
        if (!(input[i] >= 'a' && input[i] <= 'z') && 
            !(input[i] >= 'A' && input[i] <= 'Z') &&
            !(input[i] >= '0' && input[i] <= '9') &&
            input[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

event_t get_next_event(context_t *ctx) {
    if (ctx == NULL) return EVENT_ERROR;
    
    char input[MAX_INPUT_LEN];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_ERROR;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (len == 0) return EVENT_ERROR;
    
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "complete") == 0) return EVENT_COMPLETE;
    if (strcmp(input, "reset") == 0) return EVENT_RESET;
    
    if (validate_input(input, len)) {
        if (ctx->buffer_len + len < MAX_INPUT_LEN) {
            memcpy(ctx->buffer + ctx->buffer_len, input, len);
            ctx->buffer_len += len;
            ctx->data_count++;
            return EVENT_DATA;
        }
    }
    
    return EVENT_ERROR;
}

state_t handle_start(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("State machine started\n");
            return STATE_READING;
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

state_t handle_reading(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            printf("Data received: %d items\n", ctx->data_count);
            if (ctx->data_count >= 3) {
                return STATE_PROCESSING;
            }
            return STATE_READING;
        case EVENT_COMPLETE:
            return STATE_PROCESSING;
        case EVENT_ERROR:
            return STATE_ERROR;
        case EVENT_RESET:
            initialize_context(ctx);
            return STATE_START;
        default:
            return STATE_READING;
    }
}

state_t handle_processing(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
        case EVENT_COMPLETE:
            printf("Processing data: %s\n", ctx->buffer);
            printf("Total items processed: %d\n", ctx->data_count);
            return STATE_FINISHED;
        case EVENT_ERROR:
            return STATE_ERROR;
        case EVENT_RESET:
            initialize_context(ctx);
            return STATE_START;
        default:
            return STATE_PROCESSING;
    }
}

state_t handle_finished(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            initialize_context(ctx);
            return STATE_START;
        default:
            printf("Operation completed. Type 'reset' to start over.\n");
            return STATE_FINISHED;
    }
}

state_t handle_error(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            initialize_context(ctx);
            return STATE_START;
        default:
            printf("Error state. Type 'reset' to recover.\n");
            return STATE_ERROR;
    }
}

state_t process_event(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (ctx->current_state) {
        case STATE_START:
            return handle_start(ctx, event