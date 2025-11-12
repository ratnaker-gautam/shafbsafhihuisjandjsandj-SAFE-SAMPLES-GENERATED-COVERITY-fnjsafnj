//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#define STATE_COUNT 5
#define MAX_INPUTS 1000

typedef enum {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR
} state_t;

typedef enum {
    INPUT_START,
    INPUT_DATA,
    INPUT_STOP,
    INPUT_RESET,
    INPUT_INVALID
} input_t;

typedef struct {
    state_t current_state;
    uint32_t data_count;
    uint32_t processed_count;
    uint32_t error_count;
} context_t;

static state_t state_transition(state_t current, input_t input, context_t *ctx) {
    switch (current) {
        case STATE_IDLE:
            if (input == INPUT_START) return STATE_READY;
            if (input == INPUT_RESET) return STATE_IDLE;
            break;
        case STATE_READY:
            if (input == INPUT_DATA) {
                ctx->data_count++;
                return STATE_PROCESSING;
            }
            if (input == INPUT_RESET) return STATE_IDLE;
            break;
        case STATE_PROCESSING:
            if (input == INPUT_DATA) {
                ctx->processed_count++;
                if (ctx->processed_count >= 100) return STATE_COMPLETE;
                return STATE_PROCESSING;
            }
            if (input == INPUT_STOP) return STATE_COMPLETE;
            if (input == INPUT_RESET) return STATE_IDLE;
            break;
        case STATE_COMPLETE:
            if (input == INPUT_RESET) return STATE_IDLE;
            break;
        case STATE_ERROR:
            if (input == INPUT_RESET) return STATE_IDLE;
            break;
    }
    
    if (input == INPUT_INVALID) {
        ctx->error_count++;
        return STATE_ERROR;
    }
    
    return current;
}

static input_t parse_input(const char *input_str) {
    if (strcmp(input_str, "start") == 0) return INPUT_START;
    if (strcmp(input_str, "data") == 0) return INPUT_DATA;
    if (strcmp(input_str, "stop") == 0) return INPUT_STOP;
    if (strcmp(input_str, "reset") == 0) return INPUT_RESET;
    return INPUT_INVALID;
}

static void simulate_processing(void) {
    volatile uint64_t counter = 0;
    for (int i = 0; i < 1000000; i++) {
        counter += i * 2;
    }
}

int main(void) {
    context_t ctx = {STATE_IDLE, 0, 0, 0};
    char input_buffer[32];
    input_t input;
    int input_count = 0;
    
    printf("State Machine Simulation\n");
    printf("Available inputs: start, data, stop, reset\n");
    printf("Enter up to %d inputs (empty line to exit):\n", MAX_INPUTS);
    
    while (input_count < MAX_INPUTS) {
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len - 1] == '\n') {
            input_buffer[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) {
            break;
        }
        
        if (len >= sizeof(input_buffer) - 1) {
            printf("Input too long\n");
            while (getchar() != '\n');
            continue;
        }
        
        input = parse_input(input_buffer);
        state_t new_state = state_transition(ctx.current_state, input, &ctx);
        
        printf("State: %d -> %d | Data: %u Processed: %u Errors: %u\n",
               ctx.current_state, new_state, ctx.data_count, ctx.processed_count, ctx.error_count);
        
        ctx.current_state = new_state;
        
        if (ctx.current_state == STATE_PROCESSING) {
            simulate_processing();
        }
        
        input_count++;
        
        if (ctx.current_state == STATE_COMPLETE || ctx.current_state == STATE_ERROR) {
            printf("Final state reached: %d\n", ctx.current_state);
            break;
        }
    }
    
    printf("Simulation ended. Total inputs: %d\n", input_count);
    return 0;
}