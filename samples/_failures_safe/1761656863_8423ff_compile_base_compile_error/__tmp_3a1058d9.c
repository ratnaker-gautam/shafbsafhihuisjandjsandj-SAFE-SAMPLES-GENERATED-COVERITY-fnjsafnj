//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define STATE_COUNT 5
#define INPUT_COUNT 3
#define MAX_INPUT_LEN 16

typedef enum {
    STATE_START,
    STATE_PROCESSING,
    STATE_VALIDATING,
    STATE_COMPLETED,
    STATE_ERROR
} state_t;

typedef enum {
    INPUT_NONE,
    INPUT_DATA,
    INPUT_COMMAND
} input_type_t;

typedef struct {
    state_t current_state;
    uint32_t data_value;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_pos;
} context_t;

static state_t transition_table[STATE_COUNT][INPUT_COUNT] = {
    {STATE_START, STATE_PROCESSING, STATE_ERROR},
    {STATE_PROCESSING, STATE_PROCESSING, STATE_VALIDATING},
    {STATE_VALIDATING, STATE_ERROR, STATE_COMPLETED},
    {STATE_COMPLETED, STATE_COMPLETED, STATE_START},
    {STATE_ERROR, STATE_ERROR, STATE_START}
};

static int is_valid_input(const char *input) {
    if (!input || *input == '\0') return 0;
    
    const char *ptr = input;
    while (*ptr) {
        if (*ptr < ' ' || *ptr > '~') return 0;
        ptr++;
    }
    return 1;
}

static input_type_t classify_input(const char *input) {
    if (!input || *input == '\0') return INPUT_NONE;
    
    const char *ptr = input;
    while (*ptr) {
        if (*ptr >= '0' && *ptr <= '9') {
            return INPUT_DATA;
        }
        ptr++;
    }
    
    if (strcmp(input, "done") == 0 || strcmp(input, "reset") == 0) {
        return INPUT_COMMAND;
    }
    
    return INPUT_NONE;
}

static void process_state_machine(context_t *ctx, const char *input) {
    if (!ctx || !input) return;
    
    input_type_t input_type = classify_input(input);
    if (input_type == INPUT_NONE) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    state_t next_state = transition_table[ctx->current_state][input_type];
    
    switch (ctx->current_state) {
        case STATE_START:
            if (input_type == INPUT_DATA) {
                ctx->data_value = 0;
                ctx->buffer_pos = 0;
                char *buf_ptr = ctx->buffer;
                const char *in_ptr = input;
                while (*in_ptr && (size_t)(buf_ptr - ctx->buffer) < MAX_INPUT_LEN - 1) {
                    *buf_ptr = *in_ptr;
                    buf_ptr++;
                    in_ptr++;
                }
                *buf_ptr = '\0';
                ctx->buffer_pos = buf_ptr - ctx->buffer;
            }
            break;
            
        case STATE_PROCESSING:
            if (input_type == INPUT_DATA) {
                char *buf_ptr = ctx->buffer + ctx->buffer_pos;
                const char *in_ptr = input;
                while (*in_ptr && (size_t)(buf_ptr - ctx->buffer) < MAX_INPUT_LEN - 1) {
                    *buf_ptr = *in_ptr;
                    buf_ptr++;
                    in_ptr++;
                }
                *buf_ptr = '\0';
                ctx->buffer_pos = buf_ptr - ctx->buffer;
            }
            break;
            
        case STATE_VALIDATING:
            if (input_type == INPUT_COMMAND && strcmp(input, "done") == 0) {
                const char *buf_ptr = ctx->buffer;
                while (*buf_ptr) {
                    if (*buf_ptr >= '0' && *buf_ptr <= '9') {
                        ctx->data_value = ctx->data_value * 10 + (*buf_ptr - '0');
                    }
                    buf_ptr++;
                }
            }
            break;
            
        case STATE_COMPLETED:
            if (input_type == INPUT_COMMAND && strcmp(input, "reset") == 0) {
                ctx->data_value = 0;
                ctx->buffer_pos = 0;
                char *buf_ptr = ctx->buffer;
                *buf_ptr = '\0';
            }
            break;
            
        case STATE_ERROR:
            if (input_type == INPUT_COMMAND && strcmp(input, "reset") == 0) {
                ctx->data_value = 0;
                ctx->buffer_pos = 0;
                char *buf_ptr = ctx->buffer;
                *buf_ptr = '\0';
            }
            break;
    }
    
    ctx->current_state = next_state;
}

static void print_state_info(const context_t *ctx) {
    if (!ctx) return;
    
    const char *state_names[] = {
        "START", "PROCESSING", "VALIDATING", "COMPLETED", "ERROR"
    };
    
    printf("Current state: %s\n", state_names[ctx->current_state]);
    printf("Data value: %u\n", ctx->data_value);
    printf("Buffer: %s\n", ctx->buffer);
    printf("---\n");
}

int main(void) {
    context_t ctx;
    ctx.current_state = STATE_START;
    ctx.data_value = 0;
    ctx.buffer_pos = 0;
    char *buf_ptr = ctx.buffer;
    *buf_ptr = '\0';
    
    char input[MAX_INPUT_LEN];
    
    printf("State Machine Demo\n");
    printf("Enter data (numbers), 'done' to validate,