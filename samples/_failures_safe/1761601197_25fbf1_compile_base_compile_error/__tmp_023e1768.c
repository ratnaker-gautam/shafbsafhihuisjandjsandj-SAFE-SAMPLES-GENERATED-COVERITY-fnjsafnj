//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 64
#define MAX_STATES 5

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
static int validate_input(const char *input, size_t len);

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
            printf("Critical error occurred. Resetting...\n");
            initialize_context(&ctx);
        }
    }
    
    printf("State machine completed successfully.\n");
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
    
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "data") == 0) return EVENT_DATA;
    if (strcmp(input, "process") == 0) return EVENT_PROCESS;
    if (strcmp(input, "complete") == 0) return EVENT_COMPLETE;
    if (strcmp(input, "error") == 0) return EVENT_ERROR;
    if (strcmp(input, "reset") == 0) return EVENT_RESET;
    if (strcmp(input, "quit") == 0) return EVENT_RESET;
    
    return EVENT_ERROR;
}

static int validate_input(const char *input, size_t len) {
    if (input == NULL || len >= MAX_INPUT_LEN) {
        return 0;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) {
            return 0;
        }
    }
    
    return 1;
}

static void process_data(context_t *ctx) {
    if (ctx == NULL || ctx->buffer_len == 0) return;
    
    printf("Processing data: ");
    for (size_t i = 0; i < ctx->buffer_len && i < sizeof(ctx->buffer); i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            printf("%c", ctx->buffer[i] - 32);
        } else {
            printf("%c", ctx->buffer[i]);
        }
    }
    printf("\n");
    
    ctx->data_count++;
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

static state_t handle_start(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("State machine started. Ready to receive data.\n");
            return STATE_READING;
        case EVENT_RESET:
            initialize_context(ctx);
            return STATE_START;