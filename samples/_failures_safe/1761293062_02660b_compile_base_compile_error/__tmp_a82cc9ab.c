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
    EVENT_ERROR
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
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

static event_t get_next_event(context_t *ctx) {
    if (ctx == NULL) return EVENT_ERROR;
    
    char input[MAX_INPUT_LEN];
    printf("Enter command (start/data/process/done/quit): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_ERROR;
    }
    
    input[strcspn(input, "\n")] = 0;
    
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "data") == 0) return EVENT_DATA;
    if (strcmp(input, "process") == 0) return EVENT_PROCESS;
    if (strcmp(input, "done") == 0) return EVENT_DONE;
    if (strcmp(input, "quit") == 0) return EVENT_ERROR;
    
    return EVENT_ERROR;
}

static state_t handle_start(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("State machine started\n");
            ctx->data_count = 0;
            return STATE_READING;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            printf("Invalid event for START state\n");
            return STATE_START;
    }
}

static state_t handle_reading(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (ctx->data_count >= 3) {
                printf("Maximum data count reached\n");
                return STATE_PROCESSING;
            }
            
            printf("Enter data: ");
            if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
                return STATE_ERROR;
            }
            
            ctx->buffer_len = strcspn(ctx->buffer, "\n");
            ctx->buffer[ctx->buffer_len] = 0;
            
            if (!validate_input(ctx->buffer, ctx->buffer_len)) {
                printf("Invalid input data\n");
                return STATE_READING;
            }
            
            ctx->data_count++;
            printf("Data received: %s (count: %d)\n", ctx->buffer, ctx->data_count);
            return STATE_READING;
            
        case EVENT_PROCESS:
            if (ctx->data_count > 0) {
                return STATE_PROCESSING;
            }
            printf("No data to process\n");
            return STATE_READING;
            
        case EVENT_ERROR:
            return STATE_ERROR;
            
        default:
            printf("Invalid event for READING state\n");
            return STATE_READING;
    }
}

static state_t handle_processing(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_PROCESS:
            printf("Processing %d data items\n", ctx->data_count);
            for (int i = 0; i < ctx->data_count; i++) {
                printf("Processed item %d\n", i + 1);
            }
            return STATE_FINISHED;
            
        case EVENT_ERROR:
            return STATE_ERROR;
            
        default:
            printf("Invalid event for PROCESSING state\n");
            return STATE_PROCESSING;
    }
}

static state_t handle_finished(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DONE:
            printf("Processing completed successfully\n");
            return STATE_START;
            
        case EVENT_ERROR:
            return STATE_ERROR;
            
        default:
            printf("Invalid event for FINISHED state\n");
            return STATE_FINISHED;
    }
}

static state_t handle_error(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;