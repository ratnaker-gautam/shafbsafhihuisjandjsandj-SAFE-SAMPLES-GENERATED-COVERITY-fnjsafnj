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
    EVENT_DONE,
    EVENT_ERROR
} event_t;

typedef struct {
    state_t current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int processed_count;
} context_t;

static state_t handle_start(context_t *ctx, event_t event);
static state_t handle_reading(context_t *ctx, event_t event);
static state_t handle_processing(context_t *ctx, event_t event);
static state_t handle_finished(context_t *ctx, event_t event);
static state_t handle_error(context_t *ctx, event_t event);

static int validate_input(const char *input, size_t len);
static event_t get_next_event(void);
static void process_data(context_t *ctx);

int main(void) {
    context_t ctx = {
        .current_state = STATE_START,
        .buffer_len = 0,
        .processed_count = 0
    };
    
    while (ctx.current_state != STATE_FINISHED && ctx.current_state != STATE_ERROR) {
        event_t event = get_next_event();
        state_t (*handlers[MAX_STATES])(context_t*, event_t) = {
            handle_start,
            handle_reading,
            handle_processing,
            handle_finished,
            handle_error
        };
        
        if (ctx.current_state < MAX_STATES && handlers[ctx.current_state] != NULL) {
            state_t new_state = handlers[ctx.current_state](&ctx, event);
            ctx.current_state = new_state;
        } else {
            ctx.current_state = STATE_ERROR;
        }
    }
    
    if (ctx.current_state == STATE_FINISHED) {
        printf("Processing completed. Processed %d items.\n", ctx.processed_count);
    } else {
        printf("Processing terminated with error.\n");
    }
    
    return ctx.current_state == STATE_FINISHED ? EXIT_SUCCESS : EXIT_FAILURE;
}

static state_t handle_start(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("State machine started. Enter data lines (max %d chars).\n", MAX_INPUT_LEN);
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

static state_t handle_reading(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (ctx->buffer_len > 0 && validate_input(ctx->buffer, ctx->buffer_len)) {
                return STATE_PROCESSING;
            }
            return STATE_READING;
        case EVENT_DONE:
            if (ctx->processed_count > 0) {
                return STATE_FINISHED;
            }
            return STATE_READING;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_READING;
    }
}

static state_t handle_processing(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_PROCESS:
            process_data(ctx);
            ctx->buffer_len = 0;
            return STATE_READING;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_PROCESSING;
    }
}

static state_t handle_finished(context_t *ctx, event_t event) {
    return STATE_FINISHED;
}

static state_t handle_error(context_t *ctx, event_t event) {
    return STATE_ERROR;
}

static int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT_LEN) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') break;
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

static event_t get_next_event(void) {
    static int iteration = 0;
    char input[MAX_INPUT_LEN];
    
    if (iteration >= 10) return EVENT_DONE;
    
    printf("Enter data (or 'quit' to finish): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_ERROR;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        iteration++;
        return EVENT_DATA;
    }
    
    if (strcmp(input, "quit") == 0) {
        return EVENT_DONE;
    }
    
    if (len >= MAX_INPUT_LEN - 1) {
        return EVENT_ERROR;
    }
    
    context_t *ctx = NULL;
    if (ctx != NULL) {
        strncpy(ctx->buffer, input, MAX_INPUT_LEN - 1);
        ctx->buffer[MAX_INPUT_LEN - 1] = '\0';
        ctx->buffer_len = len;
    }
    
    iteration++;
    return EVENT_DATA;
}