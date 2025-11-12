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
static void reset_context(context_t *ctx);

int main(void) {
    context_t ctx;
    reset_context(&ctx);
    
    printf("Enter data stream (max %d chars, ';' to end, '!' to error): ", MAX_INPUT_LEN - 1);
    
    char input[MAX_INPUT_LEN + 2];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        event_t event = get_next_event(input[i]);
        ctx.current_state = handle_event(ctx.current_state, event, &ctx);
        
        if (ctx.current_state == STATE_ERROR) {
            printf("State machine entered error state\n");
            break;
        }
        
        if (ctx.current_state == STATE_FINISHED) {
            printf("State machine completed successfully\n");
            break;
        }
    }
    
    if (ctx.current_state != STATE_ERROR && ctx.current_state != STATE_FINISHED) {
        printf("State machine ended in state: %d\n", ctx.current_state);
    }
    
    return 0;
}

static event_t get_next_event(const char input) {
    if (input == ';') return EVENT_END;
    if (input == '!') return EVENT_INVALID;
    if (input >= ' ' && input <= '~') return EVENT_DATA;
    return EVENT_INVALID;
}

static state_t handle_event(state_t current_state, event_t event, context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (current_state) {
        case STATE_START:
            switch (event) {
                case EVENT_DATA:
                    ctx->buffer[0] = '>';
                    ctx->buffer[1] = '\0';
                    ctx->buffer_len = 1;
                    return STATE_READING;
                case EVENT_END:
                case EVENT_INVALID:
                    return STATE_ERROR;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_READING:
            switch (event) {
                case EVENT_DATA:
                    if (ctx->buffer_len < MAX_INPUT_LEN - 1) {
                        ctx->buffer[ctx->buffer_len] = event == EVENT_DATA ? 'X' : '?';
                        ctx->buffer_len++;
                        ctx->buffer[ctx->buffer_len] = '\0';
                        return STATE_READING;
                    } else {
                        return STATE_ERROR;
                    }
                case EVENT_END:
                    process_buffer(ctx);
                    return STATE_FINISHED;
                case EVENT_INVALID:
                    return STATE_ERROR;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_PROCESSING:
            return STATE_ERROR;
            
        case STATE_FINISHED:
            return STATE_FINISHED;
            
        case STATE_ERROR:
            return STATE_ERROR;
            
        default:
            return STATE_ERROR;
    }
}

static void process_buffer(context_t *ctx) {
    if (ctx == NULL || ctx->buffer_len == 0) return;
    
    printf("Processing buffer: ");
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (ctx->buffer[i] >= ' ' && ctx->buffer[i] <= '~') {
            putchar(ctx->buffer[i]);
        } else {
            putchar('?');
        }
    }
    printf("\n");
    
    int sum = 0;
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        sum = (sum + (int)ctx->buffer[i]) % 1000;
    }
    printf("Buffer checksum: %d\n", sum);
}

static void reset_context(context_t *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_START;
    ctx->buffer[0] = '\0';
    ctx->buffer_len = 0;
}