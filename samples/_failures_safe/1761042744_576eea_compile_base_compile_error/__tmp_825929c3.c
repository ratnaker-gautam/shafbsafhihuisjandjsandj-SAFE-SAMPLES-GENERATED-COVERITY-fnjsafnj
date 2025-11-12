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
    STATE_FINISHED,
    STATE_ERROR
} state_t;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
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
        if (input[i] == '\0') break;
        if (!(input[i] >= 'a' && input[i] <= 'z') && 
            !(input[i] >= 'A' && input[i] <= 'Z') &&
            !(input[i] >= '0' && input[i] <= '9') &&
            input[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

state_t handle_start(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("State machine started.\n");
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
            if (ctx->buffer_len < MAX_INPUT_LEN - 1) {
                printf("Enter data (alphanumeric only): ");
                if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) != NULL) {
                    size_t len = strlen(ctx->buffer);
                    if (len > 0 && ctx->buffer[len - 1] == '\n') {
                        ctx->buffer[len - 1] = '\0';
                        len--;
                    }
                    if (validate_input(ctx->buffer, len)) {
                        ctx->buffer_len = len;
                        ctx->data_count++;
                        return STATE_PROCESSING;
                    } else {
                        printf("Invalid input format.\n");
                        return STATE_ERROR;
                    }
                }
            }
            return STATE_ERROR;
        case EVENT_RESET:
            initialize_context(ctx);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

state_t handle_processing(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_PROCESS:
            printf("Processing data: %s\n", ctx->buffer);
            printf("Data length: %zu\n", ctx->buffer_len);
            printf("Total data processed: %d\n", ctx->data_count);
            return STATE_FINISHED;
        case EVENT_RESET:
            initialize_context(ctx);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

state_t handle_finished(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_COMPLETE:
            printf("Processing complete.\n");
            return STATE_READING;
        case EVENT_RESET:
            initialize_context(ctx);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

state_t handle_error(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from error state.\n");
            initialize_context(ctx);
            return STATE_START;
        default:
            printf("Error state - reset required.\n");
            return STATE_ERROR;
    }
}

state_t process_event(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (ctx->current_state) {
        case STATE_START:
            return handle_start(ctx, event);
        case STATE_READING:
            return handle_reading(ctx, event);
        case STATE_PROCESSING:
            return handle_processing(ctx, event);
        case STATE_FINISHED:
            return handle_finished(ctx, event);
        case STATE_ERROR:
            return handle_error(ctx, event);
        default:
            return STATE_ERROR;
    }
}

void print_menu(void) {
    printf("\nState Machine Menu:\n");
    printf("1. Start\n");
    printf("2. Enter data\n");
    printf("3. Process data\n");
    printf("4. Complete\n");
    printf("5. Reset\n");
    printf("