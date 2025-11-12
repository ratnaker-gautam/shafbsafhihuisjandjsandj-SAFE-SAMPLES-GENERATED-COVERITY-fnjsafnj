//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LENGTH 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_DONE,
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
    char buffer[MAX_INPUT_LENGTH];
    size_t buffer_index;
    int data_count;
} context_t;

state_t handle_start(context_t *ctx, event_t event);
state_t handle_reading(context_t *ctx, event_t event);
state_t handle_processing(context_t *ctx, event_t event);
state_t handle_done(context_t *ctx, event_t event);
state_t handle_error(context_t *ctx, event_t event);

void initialize_context(context_t *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_START;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_index = 0;
    ctx->data_count = 0;
}

int is_valid_input_char(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == ' ';
}

event_t get_next_event(context_t *ctx) {
    if (ctx == NULL) return EVENT_ERROR;
    
    char input[MAX_INPUT_LENGTH + 2];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_ERROR;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        return EVENT_COMPLETE;
    }
    
    if (strcmp(input, "reset") == 0) {
        return EVENT_RESET;
    }
    
    if (len > MAX_INPUT_LENGTH) {
        return EVENT_ERROR;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (!is_valid_input_char(input[i])) {
            return EVENT_ERROR;
        }
    }
    
    if (ctx->buffer_index + len >= MAX_INPUT_LENGTH) {
        return EVENT_ERROR;
    }
    
    memcpy(ctx->buffer + ctx->buffer_index, input, len);
    ctx->buffer_index += len;
    ctx->data_count++;
    
    return EVENT_DATA;
}

state_t handle_start(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("State machine started. Enter text or 'reset' to reset.\n");
            return STATE_READING;
        case EVENT_RESET:
            initialize_context(ctx);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

state_t handle_reading(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            printf("Data received. Total inputs: %d\n", ctx->data_count);
            if (ctx->data_count >= 3) {
                return STATE_PROCESSING;
            }
            return STATE_READING;
        case EVENT_COMPLETE:
            if (ctx->data_count > 0) {
                return STATE_PROCESSING;
            }
            return STATE_DONE;
        case EVENT_RESET:
            initialize_context(ctx);
            return STATE_START;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_READING;
    }
}

state_t handle_processing(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
        case EVENT_COMPLETE:
            printf("Processing data: '%.*s'\n", (int)ctx->buffer_index, ctx->buffer);
            printf("Total characters processed: %zu\n", ctx->buffer_index);
            return STATE_DONE;
        case EVENT_RESET:
            initialize_context(ctx);
            return STATE_START;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_PROCESSING;
    }
}

state_t handle_done(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            initialize_context(ctx);
            return STATE_START;
        case EVENT_START:
            initialize_context(ctx);
            return STATE_READING;
        default:
            printf("Operation complete. Type 'reset' to start over.\n");
            return STATE_DONE;
    }
}

state_t handle_error(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            initialize_context(ctx);
            return STATE_START;
        default:
            printf("Error occurred. Type 'reset' to restart.\n");
            return STATE_ERROR;
    }
}

state_t process_event(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (ctx->current_state) {
        case STATE_START:
            return handle_start(ctx, event);
        case STATE_READING:
            return handle_