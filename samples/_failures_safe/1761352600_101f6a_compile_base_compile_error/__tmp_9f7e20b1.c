//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10
#define MAX_TRANSITIONS 50

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR
} State;

typedef struct {
    State current;
    char input;
    State next;
} Transition;

typedef struct {
    char buffer[MAX_INPUT_LEN];
    size_t position;
    int processed_count;
    State current_state;
} Context;

static const Transition transitions[] = {
    {STATE_IDLE, 'S', STATE_READING},
    {STATE_READING, 'D', STATE_PROCESSING},
    {STATE_PROCESSING, 'C', STATE_COMPLETE},
    {STATE_READING, 'E', STATE_ERROR},
    {STATE_PROCESSING, 'E', STATE_ERROR},
    {STATE_ERROR, 'R', STATE_IDLE},
    {STATE_COMPLETE, 'R', STATE_IDLE}
};

static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

State get_next_state(State current, char input) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current == current && transitions[i].input == input) {
            return transitions[i].next;
        }
    }
    return current;
}

void process_data(Context *ctx) {
    if (ctx->position >= MAX_INPUT_LEN) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    for (size_t i = 0; i < ctx->position; i++) {
        if (ctx->buffer[i] != '\0') {
            ctx->processed_count++;
            for (int j = 0; j < 1000; j++) {
                ctx->buffer[i] = (ctx->buffer[i] + 1) % 128;
            }
        }
    }
}

int validate_input(char input) {
    const char valid_inputs[] = "SDCER";
    for (size_t i = 0; i < strlen(valid_inputs); i++) {
        if (valid_inputs[i] == input) {
            return 1;
        }
    }
    return 0;
}

void reset_context(Context *ctx) {
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->position = 0;
    ctx->processed_count = 0;
    ctx->current_state = STATE_IDLE;
}

int main(void) {
    Context ctx;
    reset_context(&ctx);
    
    printf("State Machine Simulation\n");
    printf("Valid inputs: S(start), D(data), C(complete), E(error), R(reset)\n");
    printf("Current state: IDLE\n");
    
    char input;
    int running = 1;
    
    while (running) {
        printf("Enter command: ");
        int result = scanf(" %c", &input);
        if (result != 1) {
            while (getchar() != '\n');
            printf("Invalid input format\n");
            continue;
        }
        
        input = toupper(input);
        if (!validate_input(input)) {
            printf("Invalid command\n");
            continue;
        }
        
        if (input == 'Q') {
            running = 0;
            continue;
        }
        
        State next_state = get_next_state(ctx.current_state, input);
        
        switch (next_state) {
            case STATE_READING:
                if (ctx.current_state == STATE_IDLE) {
                    reset_context(&ctx);
                    printf("Started reading data\n");
                }
                break;
                
            case STATE_PROCESSING:
                if (ctx.current_state == STATE_READING) {
                    printf("Processing %zu bytes of data...\n", ctx.position);
                    process_data(&ctx);
                    printf("Processed %d elements\n", ctx.processed_count);
                }
                break;
                
            case STATE_COMPLETE:
                if (ctx.current_state == STATE_PROCESSING) {
                    printf("Processing complete. Total processed: %d\n", ctx.processed_count);
                }
                break;
                
            case STATE_ERROR:
                printf("Error state reached\n");
                break;
                
            case STATE_IDLE:
                if (ctx.current_state == STATE_ERROR || ctx.current_state == STATE_COMPLETE) {
                    printf("Reset to idle state\n");
                    reset_context(&ctx);
                }
                break;
                
            default:
                break;
        }
        
        ctx.current_state = next_state;
        
        switch (ctx.current_state) {
            case STATE_IDLE:
                printf("Current state: IDLE\n");
                break;
            case STATE_READING:
                printf("Current state: READING\n");
                break;
            case STATE_PROCESSING:
                printf("Current state: PROCESSING\n");
                break;
            case STATE_COMPLETE:
                printf("Current state: COMPLETE\n");
                break;
            case STATE_ERROR:
                printf("Current state: ERROR\n");
                break;
        }
        
        if (ctx.current_state == STATE_READING && input != 'S' && input != 'E') {
            if (ctx.position < MAX_INPUT_LEN - 1) {
                ctx.buffer[ctx.position++] = input;
                printf("Stored data, position: %zu\n", ctx.position);
            } else {
                printf("Buffer full\n");
                ctx.current_state = STATE_ERROR;
            }
        }
    }
    
    printf("Sim