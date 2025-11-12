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
    STATE_WRITING,
    STATE_FINISHED,
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
    char buffer[MAX_INPUT_LENGTH];
    size_t data_length;
    int error_code;
} context_t;

state_t handle_start_state(context_t *ctx, event_t event);
state_t handle_reading_state(context_t *ctx, event_t event);
state_t handle_processing_state(context_t *ctx, event_t event);
state_t handle_writing_state(context_t *ctx, event_t event);
state_t handle_finished_state(context_t *ctx, event_t event);
state_t handle_error_state(context_t *ctx, event_t event);

void initialize_context(context_t *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_START;
    memset(ctx->buffer, 0, MAX_INPUT_LENGTH);
    ctx->data_length = 0;
    ctx->error_code = 0;
}

int read_input_data(context_t *ctx) {
    if (ctx == NULL) return -1;
    
    printf("Enter data to process (max %d characters): ", MAX_INPUT_LENGTH - 1);
    
    if (fgets(ctx->buffer, MAX_INPUT_LENGTH, stdin) == NULL) {
        return -1;
    }
    
    ctx->data_length = strlen(ctx->buffer);
    if (ctx->data_length > 0 && ctx->buffer[ctx->data_length - 1] == '\n') {
        ctx->buffer[ctx->data_length - 1] = '\0';
        ctx->data_length--;
    }
    
    if (ctx->data_length == 0) {
        return -1;
    }
    
    return 0;
}

int process_data(context_t *ctx) {
    if (ctx == NULL || ctx->data_length == 0) return -1;
    
    for (size_t i = 0; i < ctx->data_length; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    
    return 0;
}

int write_output(context_t *ctx) {
    if (ctx == NULL || ctx->data_length == 0) return -1;
    
    printf("Processed data: %s\n", ctx->buffer);
    return 0;
}

state_t handle_start_state(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            return STATE_READING;
        case EVENT_RESET:
            initialize_context(ctx);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

state_t handle_reading_state(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA_READY:
            if (read_input_data(ctx) == 0) {
                return STATE_PROCESSING;
            } else {
                ctx->error_code = 1;
                return STATE_ERROR;
            }
        case EVENT_RESET:
            initialize_context(ctx);
            return STATE_START;
        default:
            ctx->error_code = 2;
            return STATE_ERROR;
    }
}

state_t handle_processing_state(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_PROCESS_COMPLETE:
            if (process_data(ctx) == 0) {
                return STATE_WRITING;
            } else {
                ctx->error_code = 3;
                return STATE_ERROR;
            }
        case EVENT_RESET:
            initialize_context(ctx);
            return STATE_START;
        default:
            ctx->error_code = 4;
            return STATE_ERROR;
    }
}

state_t handle_writing_state(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_WRITE_COMPLETE:
            if (write_output(ctx) == 0) {
                return STATE_FINISHED;
            } else {
                ctx->error_code = 5;
                return STATE_ERROR;
            }
        case EVENT_RESET:
            initialize_context(ctx);
            return STATE_START;
        default:
            ctx->error_code = 6;
            return STATE_ERROR;
    }
}

state_t handle_finished_state(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            initialize_context(ctx);
            return STATE_START;
        default:
            return STATE_FINISHED;
    }
}

state_t handle_error_state(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            initialize_context(ctx);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

state_t process_event(context_t *ctx, event_t event