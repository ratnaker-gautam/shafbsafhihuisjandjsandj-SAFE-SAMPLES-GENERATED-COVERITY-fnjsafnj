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
    STATE_OUTPUT,
    STATE_ERROR,
    STATE_DONE
} State;

typedef struct {
    State current;
    char input_char;
    State next;
} Transition;

typedef struct {
    char buffer[MAX_INPUT_LEN];
    size_t pos;
    int processed_count;
    State current_state;
} Context;

static const Transition transitions[] = {
    {STATE_IDLE, 'S', STATE_READING},
    {STATE_IDLE, '\0', STATE_ERROR},
    {STATE_READING, 'A', STATE_READING},
    {STATE_READING, 'B', STATE_READING},
    {STATE_READING, 'C', STATE_READING},
    {STATE_READING, 'E', STATE_PROCESSING},
    {STATE_READING, '\0', STATE_ERROR},
    {STATE_PROCESSING, '\0', STATE_OUTPUT},
    {STATE_OUTPUT, '\0', STATE_IDLE},
    {STATE_OUTPUT, 'R', STATE_READING},
    {STATE_ERROR, '\0', STATE_DONE}
};

static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

State get_next_state(State current, char input) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current == current && transitions[i].input_char == input) {
            return transitions[i].next;
        }
    }
    return STATE_ERROR;
}

int is_valid_input_char(char c) {
    return c == 'S' || c == 'A' || c == 'B' || c == 'C' || c == 'E' || c == 'R' || c == '\n' || c == '\0';
}

void process_buffer(Context *ctx) {
    if (ctx->pos == 0) return;
    
    int count_a = 0, count_b = 0, count_c = 0;
    for (size_t i = 0; i < ctx->pos; i++) {
        switch (ctx->buffer[i]) {
            case 'A': count_a++; break;
            case 'B': count_b++; break;
            case 'C': count_c++; break;
        }
    }
    
    ctx->processed_count++;
    printf("Sequence %d: A=%d, B=%d, C=%d\n", ctx->processed_count, count_a, count_b, count_c);
    ctx->pos = 0;
}

int main(void) {
    Context ctx = {0};
    ctx.current_state = STATE_IDLE;
    
    printf("Enter sequences (S to start, A/B/C for data, E to end, R to restart, Ctrl+D to exit):\n");
    
    int input_char;
    while ((input_char = getchar()) != EOF && ctx.current_state != STATE_DONE) {
        if (input_char == '\n') continue;
        
        char c = (char)input_char;
        if (!is_valid_input_char(c)) {
            ctx.current_state = STATE_ERROR;
            break;
        }
        
        State next_state = get_next_state(ctx.current_state, c);
        
        switch (ctx.current_state) {
            case STATE_IDLE:
                if (c == 'S') {
                    ctx.pos = 0;
                }
                break;
                
            case STATE_READING:
                if (c == 'E') {
                    if (ctx.pos >= MAX_INPUT_LEN) {
                        ctx.current_state = STATE_ERROR;
                        continue;
                    }
                    ctx.buffer[ctx.pos] = '\0';
                } else {
                    if (ctx.pos < MAX_INPUT_LEN - 1) {
                        ctx.buffer[ctx.pos++] = c;
                    } else {
                        ctx.current_state = STATE_ERROR;
                        continue;
                    }
                }
                break;
                
            case STATE_PROCESSING:
                process_buffer(&ctx);
                break;
                
            case STATE_OUTPUT:
                if (c == 'R') {
                    ctx.pos = 0;
                }
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid state transition\n");
                break;
                
            default:
                break;
        }
        
        ctx.current_state = next_state;
    }
    
    if (ctx.current_state == STATE_ERROR) {
        printf("State machine entered error state\n");
        return 1;
    }
    
    printf("Processing complete. Total sequences: %d\n", ctx.processed_count);
    return 0;
}