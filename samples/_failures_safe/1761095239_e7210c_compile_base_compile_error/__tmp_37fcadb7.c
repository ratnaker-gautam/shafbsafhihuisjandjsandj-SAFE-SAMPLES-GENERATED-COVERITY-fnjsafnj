//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 64
#define MAX_STATES 8

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
    EVENT_SUCCESS,
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
        if (input[i] == '\0') return 0;
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

static event_t get_user_input(context_t *ctx) {
    char input[MAX_INPUT_LEN];
    printf("Enter command (data, process, reset, quit): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_ERROR;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!validate_input(input, len)) {
        return EVENT_ERROR;
    }
    
    if (strcmp(input, "data") == 0) {
        printf("Enter data (max %d chars): ", MAX_INPUT_LEN - 1);
        if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
            return EVENT_ERROR;
        }
        len = strlen(ctx->buffer);
        if (len > 0 && ctx->buffer[len - 1] == '\n') {
            ctx->buffer[len - 1] = '\0';
            len--;
        }
        if (validate_input(ctx->buffer, len)) {
            ctx->buffer_len = len;
            ctx->data_count++;
            return EVENT_DATA;
        }
        return EVENT_ERROR;
    } else if (strcmp(input, "process") == 0) {
        return EVENT_PROCESS;
    } else if (strcmp(input, "reset") == 0) {
        return EVENT_RESET;
    } else if (strcmp(input, "quit") == 0) {
        return EVENT_SUCCESS;
    }
    
    return EVENT_ERROR;
}

static state_t handle_start(context_t *ctx, event_t event) {
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
    switch (event) {
        case EVENT_DATA:
            printf("Data received: '%.*s' (count: %d)\n", 
                   (int)ctx->buffer_len, ctx->buffer, ctx->data_count);
            return STATE_READING;
        case EVENT_PROCESS:
            if (ctx->data_count > 0) {
                printf("Processing %d data items...\n", ctx->data_count);
                return STATE_PROCESSING;
            } else {
                printf("No data to process.\n");
                return STATE_READING;
            }
        case EVENT_RESET:
            printf("Resetting state machine.\n");
            initialize_context(ctx);
            return STATE_START;
        case EVENT_SUCCESS:
            printf("Quitting from reading state.\n");
            return STATE_FINISHED;
        default:
            printf("Invalid event for reading state.\n");
            return STATE_ERROR;
    }
}

static state_t handle_processing(context_t *ctx, event_t event) {
    switch (event) {
        case EVENT_SUCCESS:
            printf("Processing completed successfully.\n");
            return STATE_FINISHED;
        case EVENT_ERROR:
            printf("Processing failed.\n");
            return STATE_ERROR;
        case EVENT_RESET:
            printf("Resetting during processing.\n");
            initialize_context(ctx);
            return STATE_START;
        default:
            printf("Invalid event for processing state.\n");
            return STATE_ERROR;
    }
}

static state_t handle_finished(context_t *ctx, event_t event) {
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from finished state.\n");
            initialize_context(ctx);
            return STATE_START;
        case EVENT_SUCCESS:
            printf("Already finished.\n");
            return STATE_F