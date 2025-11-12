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
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_ERROR;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        return EVENT_INVALID;
    }
    
    if (strcmp(input, "start") == 0) {
        return EVENT_START;
    } else if (strcmp(input, "data") == 0) {
        return EVENT_DATA;
    } else if (strcmp(input, "process") == 0) {
        return EVENT_PROCESS;
    } else if (strcmp(input, "complete") == 0) {
        return EVENT_COMPLETE;
    } else if (strcmp(input, "error") == 0) {
        return EVENT_ERROR;
    } else {
        return EVENT_INVALID;
    }
}

static state_t handle_start_state(event_t event, context_t *ctx) {
    switch (event) {
        case EVENT_START:
            memset(ctx->buffer, 0, sizeof(ctx->buffer));
            ctx->buffer_len = 0;
            ctx->data_count = 0;
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

static state_t handle_reading_state(event_t event, context_t *ctx) {
    switch (event) {
        case EVENT_DATA:
            if (ctx->data_count < 5) {
                ctx->data_count++;
                return STATE_READING;
            } else {
                return STATE_PROCESSING;
            }
        case EVENT_PROCESS:
            if (ctx->data_count >= 3) {
                return STATE_PROCESSING;
            } else {
                return STATE_ERROR;
            }
        default:
            return STATE_ERROR;
    }
}

static state_t handle_processing_state(event_t event, context_t *ctx) {
    switch (event) {
        case EVENT_COMPLETE:
            return STATE_FINISHED;
        case EVENT_DATA:
            if (ctx->data_count < 8) {
                ctx->data_count++;
                return STATE_PROCESSING;
            } else {
                return STATE_FINISHED;
            }
        default:
            return STATE_ERROR;
    }
}

static state_t handle_finished_state(event_t event, context_t *ctx) {
    switch (event) {
        case EVENT_START:
            memset(ctx->buffer, 0, sizeof(ctx->buffer));
            ctx->buffer_len = 0;
            ctx->data_count = 0;
            return STATE_READING;
        default:
            return STATE_FINISHED;
    }
}

static state_t handle_error_state(event_t event, context_t *ctx) {
    switch (event) {
        case EVENT_START:
            memset(ctx->buffer, 0, sizeof(ctx->buffer));
            ctx->buffer_len = 0;
            ctx->data_count = 0;
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

static state_t process_event(state_t current_state, event_t event, context_t *ctx) {
    switch (current_state) {
        case STATE_START:
            return handle_start_state(event, ctx);
        case STATE_READING:
            return handle_reading_state(event, ctx);
        case STATE_PROCESSING:
            return handle_processing_state(event, ctx);
        case STATE_FINISHED:
            return handle_finished_state(event, ctx);
        case STATE_ERROR:
            return handle_error_state(event, ctx);
        default:
            return STATE_ERROR;
    }
}

static void print_state_info(state_t state, context_t *ctx) {
    const char *state_names[] = {
        "START", "READING", "PROCESSING", "FINISHED", "ERROR"
    };
    
    if (state < STATE_START || state > STATE_ERROR) {
        return;
    }
    
    printf("State: %s, Data Count: %d\n", state_names[state], ctx->data_count);
}

int main(void) {
    context_t ctx;
    state_t current_state = STATE_START;
    
    memset(&ctx, 0, sizeof(ctx));
    
    printf("Available commands: start, data, process, complete, error\n");
    printf("Enter commands to control the state machine:\n");
    
    while (1) {
        print_state_info(current_state, &ctx);
        printf("> ");
        fflush(stdout);
        
        event_t event = get_next_event();
        if (event == EVENT_INVALID) {
            printf("Invalid command. Try: start, data, process, complete, error\n");
            continue;
        }
        
        state_t next_state = process_event(current_state, event, &ctx);
        
        if (next_state == STATE_FINISHED && current_state == STATE_FINISHED) {
            printf("Processing complete. Enter 'start' to