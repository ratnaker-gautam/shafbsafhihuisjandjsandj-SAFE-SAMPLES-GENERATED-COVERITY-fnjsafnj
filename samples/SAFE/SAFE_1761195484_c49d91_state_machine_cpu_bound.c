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
            if (input == INPUT_DATA) return STATE_PROCESSING;
            if (input == INPUT_RESET) return STATE_IDLE;
            break;
        case STATE_PROCESSING:
            if (input == INPUT_DATA) {
                ctx->processed_count++;
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
    return current;
}

static void process_data(context_t *ctx) {
    uint32_t i;
    uint64_t result = 0;
    for (i = 0; i < 1000; i++) {
        result += (ctx->data_count * i) % 1000;
    }
    ctx->data_count++;
}

static input_t parse_input(const char *input_str) {
    if (strcmp(input_str, "start") == 0) return INPUT_START;
    if (strcmp(input_str, "data") == 0) return INPUT_DATA;
    if (strcmp(input_str, "stop") == 0) return INPUT_STOP;
    if (strcmp(input_str, "reset") == 0) return INPUT_RESET;
    return INPUT_INVALID;
}

static void print_state(state_t state) {
    const char *state_names[] = {
        "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
    };
    if (state < STATE_COUNT) {
        printf("Current state: %s\n", state_names[state]);
    }
}

int main(void) {
    context_t ctx = {STATE_IDLE, 0, 0, 0};
    char input_buffer[32];
    input_t input;
    uint32_t input_count = 0;
    
    printf("State Machine Simulation\n");
    printf("Available inputs: start, data, stop, reset\n");
    printf("Enter 'quit' to exit\n\n");
    
    while (input_count < MAX_INPUTS) {
        print_state(ctx.current_state);
        printf("Enter input: ");
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        input_buffer[strcspn(input_buffer, "\n")] = '\0';
        
        if (strcmp(input_buffer, "quit") == 0) {
            break;
        }
        
        input = parse_input(input_buffer);
        if (input == INPUT_INVALID) {
            printf("Invalid input. Try: start, data, stop, reset\n");
            continue;
        }
        
        state_t new_state = state_transition(ctx.current_state, input, &ctx);
        
        if (new_state == STATE_PROCESSING && input == INPUT_DATA) {
            process_data(&ctx);
        }
        
        if (new_state != ctx.current_state) {
            ctx.current_state = new_state;
            printf("State transition occurred\n");
        }
        
        input_count++;
    }
    
    printf("\nFinal statistics:\n");
    printf("Total inputs processed: %u\n", input_count);
    printf("Data items counted: %u\n", ctx.data_count);
    printf("Data items processed: %u\n", ctx.processed_count);
    printf("Errors encountered: %u\n", ctx.error_count);
    
    return 0;
}