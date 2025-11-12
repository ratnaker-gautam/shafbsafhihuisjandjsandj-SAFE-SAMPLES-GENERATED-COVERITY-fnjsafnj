//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
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
    
    printf("Enter event (start/data/process/complete/error): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_INVALID;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "data") == 0) return EVENT_DATA;
    if (strcmp(input, "process") == 0) return EVENT_PROCESS;
    if (strcmp(input, "complete") == 0) return EVENT_COMPLETE;
    if (strcmp(input, "error") == 0) return EVENT_ERROR;
    
    return EVENT_INVALID;
}

static int handle_start_event(context_t *ctx) {
    if (ctx == NULL) return 0;
    
    printf("State machine started\n");
    ctx->current_state = STATE_READING;
    ctx->buffer_len = 0;
    ctx->data_count = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    return 1;
}

static int handle_data_event(context_t *ctx) {
    if (ctx == NULL) return 0;
    
    if (ctx->buffer_len >= MAX_INPUT_LEN - 1) {
        printf("Buffer full, cannot accept more data\n");
        return 0;
    }
    
    char data[MAX_INPUT_LEN];
    printf("Enter data: ");
    if (fgets(data, sizeof(data), stdin) == NULL) {
        return 0;
    }
    
    size_t data_len = strlen(data);
    if (data_len > 0 && data[data_len - 1] == '\n') {
        data[--data_len] = '\0';
    }
    
    if (ctx->buffer_len + data_len >= MAX_INPUT_LEN - 1) {
        printf("Data too large for buffer\n");
        return 0;
    }
    
    memcpy(ctx->buffer + ctx->buffer_len, data, data_len);
    ctx->buffer_len += data_len;
    ctx->data_count++;
    
    printf("Data added. Total data count: %d\n", ctx->data_count);
    return 1;
}

static int handle_process_event(context_t *ctx) {
    if (ctx == NULL) return 0;
    
    if (ctx->data_count == 0) {
        printf("No data to process\n");
        return 0;
    }
    
    printf("Processing %d data items\n", ctx->data_count);
    printf("Buffer content: %s\n", ctx->buffer);
    ctx->current_state = STATE_PROCESSING;
    return 1;
}

static int handle_complete_event(context_t *ctx) {
    if (ctx == NULL) return 0;
    
    printf("Processing complete\n");
    ctx->current_state = STATE_FINISHED;
    return 1;
}

static int handle_error_event(context_t *ctx) {
    if (ctx == NULL) return 0;
    
    printf("Error occurred\n");
    ctx->current_state = STATE_ERROR;
    return 1;
}

static int handle_invalid_event(context_t *ctx) {
    if (ctx == NULL) return 0;
    
    printf("Invalid event\n");
    return 0;
}

static int process_event(context_t *ctx, event_t event) {
    if (ctx == NULL) return 0;
    
    switch (event) {
        case EVENT_START:
            return handle_start_event(ctx);
        case EVENT_DATA:
            return handle_data_event(ctx);
        case EVENT_PROCESS:
            return handle_process_event(ctx);
        case EVENT_COMPLETE:
            return handle_complete_event(ctx);
        case EVENT_ERROR:
            return handle_error_event(ctx);
        case EVENT_INVALID:
            return handle_invalid_event(ctx);
        default:
            return 0;
    }
}

static void print_state(state_t state) {
    switch (state) {
        case STATE_START:
            printf("Current state: START\n");
            break;
        case STATE_READING:
            printf("Current state: READING\n");
            break;
        case STATE_PROCESSING:
            printf("Current state: PROCESSING\n");
            break;
        case STATE_FINISHED:
            printf("Current state: FINISHED\n");
            break;
        case STATE_ERROR:
            printf("Current state: ERROR\n");
            break;
        default:
            printf("Current state: UNKNOWN\n");
            break;
    }
}

int main(void) {
    context_t ctx;
    ctx.current_state = STATE_START;
    ctx.buffer_len = 0;
    ctx.data_count = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    printf("State Machine Demo\n");
    printf("Available events: start, data, process