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
    EVENT_END,
    EVENT_INVALID
} event_t;

typedef struct {
    state_t current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
} context_t;

static event_t get_next_event(const char input);
static state_t handle_event(state_t current_state, event_t event, context_t *ctx);
static void process_buffer(context_t *ctx);
static int is_valid_input(const char c);

int main(void) {
    context_t ctx = {STATE_START, {0}, 0};
    char input[MAX_INPUT_LEN + 2] = {0};
    
    printf("Enter data stream (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return EXIT_FAILURE;
    }
    
    printf("Processing input: %s\n", input);
    
    for (size_t i = 0; i < input_len; i++) {
        event_t event = get_next_event(input[i]);
        ctx.current_state = handle_event(ctx.current_state, event, &ctx);
        
        if (ctx.current_state == STATE_ERROR) {
            fprintf(stderr, "State machine entered error state\n");
            return EXIT_FAILURE;
        }
    }
    
    ctx.current_state = handle_event(ctx.current_state, EVENT_END, &ctx);
    
    if (ctx.current_state == STATE_FINISHED) {
        printf("Processing completed successfully\n");
    } else {
        fprintf(stderr, "Processing did not complete successfully\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

static event_t get_next_event(const char input) {
    if (!is_valid_input(input)) {
        return EVENT_INVALID;
    }
    
    switch (input) {
        case 'S': return EVENT_START;
        case 'E': return EVENT_END;
        default:  return EVENT_DATA;
    }
}

static state_t handle_event(state_t current_state, event_t event, context_t *ctx) {
    if (ctx == NULL) {
        return STATE_ERROR;
    }
    
    switch (current_state) {
        case STATE_START:
            switch (event) {
                case EVENT_START:
                    ctx->buffer_len = 0;
                    memset(ctx->buffer, 0, sizeof(ctx->buffer));
                    return STATE_READING;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_READING:
            switch (event) {
                case EVENT_DATA:
                    if (ctx->buffer_len < MAX_INPUT_LEN - 1) {
                        ctx->buffer[ctx->buffer_len++] = event == EVENT_DATA ? 'X' : '\0';
                        return STATE_READING;
                    } else {
                        return STATE_ERROR;
                    }
                case EVENT_END:
                    return STATE_PROCESSING;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_PROCESSING:
            switch (event) {
                case EVENT_END:
                    process_buffer(ctx);
                    return STATE_FINISHED;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_FINISHED:
        case STATE_ERROR:
        default:
            return STATE_ERROR;
    }
}

static void process_buffer(context_t *ctx) {
    if (ctx == NULL || ctx->buffer_len == 0) {
        return;
    }
    
    printf("Processed %zu data items: ", ctx->buffer_len);
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        printf("%c", ctx->buffer[i]);
    }
    printf("\n");
}

static int is_valid_input(const char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || 
           (c >= '0' && c <= '9') || c == 'S' || c == 'E';
}