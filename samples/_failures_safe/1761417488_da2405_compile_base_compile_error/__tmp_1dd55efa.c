//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 64
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

static state_t handle_start(context_t *ctx, event_t event);
static state_t handle_reading(context_t *ctx, event_t event);
static state_t handle_processing(context_t *ctx, event_t event);
static state_t handle_finished(context_t *ctx, event_t event);
static state_t handle_error(context_t *ctx, event_t event);

static void initialize_context(context_t *ctx);
static event_t get_next_event(void);
static void process_data(context_t *ctx);
static void display_state(state_t state);
static void safe_input(char *buffer, size_t max_len);

int main(void) {
    context_t ctx;
    initialize_context(&ctx);
    
    printf("State Machine Demo - Enter commands: start, data, process, complete, error, reset, quit\n");
    
    while (1) {
        display_state(ctx.current_state);
        event_t event = get_next_event();
        
        if (event == EVENT_RESET && ctx.current_state == STATE_FINISHED) {
            break;
        }
        
        state_t (*handlers[MAX_STATES])(context_t*, event_t) = {
            handle_start,
            handle_reading,
            handle_processing,
            handle_finished,
            handle_error
        };
        
        if (ctx.current_state < MAX_STATES) {
            state_t new_state = handlers[ctx.current_state](&ctx, event);
            ctx.current_state = new_state;
        } else {
            ctx.current_state = STATE_ERROR;
        }
        
        if (ctx.current_state == STATE_ERROR) {
            printf("State machine entered error state. Resetting.\n");
            initialize_context(&ctx);
        }
    }
    
    return 0;
}

static void initialize_context(context_t *ctx) {
    if (ctx == NULL) return;
    
    ctx->current_state = STATE_START;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_len = 0;
    ctx->data_count = 0;
}

static event_t get_next_event(void) {
    char input[MAX_INPUT_LEN];
    safe_input(input, sizeof(input));
    
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "data") == 0) return EVENT_DATA;
    if (strcmp(input, "process") == 0) return EVENT_PROCESS;
    if (strcmp(input, "complete") == 0) return EVENT_COMPLETE;
    if (strcmp(input, "error") == 0) return EVENT_ERROR;
    if (strcmp(input, "reset") == 0) return EVENT_RESET;
    if (strcmp(input, "quit") == 0) return EVENT_RESET;
    
    return EVENT_ERROR;
}

static void safe_input(char *buffer, size_t max_len) {
    if (buffer == NULL || max_len == 0) return;
    
    if (fgets(buffer, (int)max_len, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
}

static void display_state(state_t state) {
    const char *state_names[] = {
        "START", "READING", "PROCESSING", "FINISHED", "ERROR"
    };
    
    if (state < sizeof(state_names) / sizeof(state_names[0])) {
        printf("Current state: %s > ", state_names[state]);
    } else {
        printf("Current state: UNKNOWN > ");
    }
}

static void process_data(context_t *ctx) {
    if (ctx == NULL || ctx->buffer_len == 0) return;
    
    printf("Processing data: '");
    for (size_t i = 0; i < ctx->buffer_len && i < MAX_INPUT_LEN; i++) {
        if (ctx->buffer[i] >= 32 && ctx->buffer[i] <= 126) {
            putchar(ctx->buffer[i]);
        } else {
            putchar('?');
        }
    }
    printf("' (%zu characters)\n", ctx->buffer_len);
    
    ctx->data_count++;
}

static state_t handle_start(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("Starting data collection\n");
            return STATE_READING;
        case EVENT_RESET:
            initialize_context(ctx);
            return STATE_START;
        default:
            printf("Invalid event for START state\n");
            return STATE_ERROR;
    }
}

static state_t handle_reading(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            printf("Enter data: ");
            safe_input(ctx->buffer, sizeof(ctx->