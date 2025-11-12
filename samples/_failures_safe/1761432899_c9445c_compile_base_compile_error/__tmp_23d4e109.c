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
    size_t position;
    size_t length;
    int processed_count;
} Context;

static const Transition transitions[] = {
    {STATE_IDLE, ' ', STATE_IDLE},
    {STATE_IDLE, '\t', STATE_IDLE},
    {STATE_IDLE, '\n', STATE_IDLE},
    {STATE_IDLE, '\0', STATE_DONE},
    {STATE_IDLE, 'a', STATE_READING},
    {STATE_IDLE, 'b', STATE_READING},
    {STATE_READING, 'a', STATE_READING},
    {STATE_READING, 'b', STATE_READING},
    {STATE_READING, ' ', STATE_PROCESSING},
    {STATE_READING, '\t', STATE_PROCESSING},
    {STATE_READING, '\n', STATE_PROCESSING},
    {STATE_READING, '\0', STATE_PROCESSING},
    {STATE_PROCESSING, ' ', STATE_PROCESSING},
    {STATE_PROCESSING, '\t', STATE_PROCESSING},
    {STATE_PROCESSING, '\n', STATE_PROCESSING},
    {STATE_PROCESSING, '\0', STATE_OUTPUT},
    {STATE_PROCESSING, 'a', STATE_READING},
    {STATE_PROCESSING, 'b', STATE_READING},
    {STATE_OUTPUT, ' ', STATE_IDLE},
    {STATE_OUTPUT, '\t', STATE_IDLE},
    {STATE_OUTPUT, '\n', STATE_IDLE},
    {STATE_OUTPUT, '\0', STATE_DONE},
    {STATE_OUTPUT, 'a', STATE_READING},
    {STATE_OUTPUT, 'b', STATE_READING},
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

void process_buffer(Context *ctx) {
    if (ctx->position == 0) return;
    
    int a_count = 0;
    int b_count = 0;
    for (size_t i = 0; i < ctx->position; i++) {
        if (ctx->buffer[i] == 'a') a_count++;
        else if (ctx->buffer[i] == 'b') b_count++;
    }
    
    if (a_count > 0 && b_count > 0) {
        int result = 1;
        int iterations = a_count * b_count;
        if (iterations > 0 && iterations < 1000000) {
            for (int i = 0; i < iterations; i++) {
                result = (result * 31) % 10007;
            }
            ctx->processed_count++;
        }
    }
    
    ctx->position = 0;
}

int main(void) {
    char input[MAX_INPUT_LEN + 2];
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        if (input[i] != 'a' && input[i] != 'b' && input[i] != ' ' && 
            input[i] != '\t' && input[i] != '\n' && input[i] != '\0') {
            return EXIT_FAILURE;
        }
    }
    
    Context ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.length = input_len;
    
    State current_state = STATE_IDLE;
    size_t index = 0;
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR && index <= input_len) {
        char current_char = (index < input_len) ? input[index] : '\0';
        
        switch (current_state) {
            case STATE_IDLE:
                break;
                
            case STATE_READING:
                if (ctx.position < MAX_INPUT_LEN) {
                    ctx.buffer[ctx.position++] = current_char;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_PROCESSING:
                process_buffer(&ctx);
                break;
                
            case STATE_OUTPUT:
                break;
                
            case STATE_ERROR:
                break;
                
            case STATE_DONE:
                break;
        }
        
        current_state = get_next_state(current_state, current_char);
        index++;