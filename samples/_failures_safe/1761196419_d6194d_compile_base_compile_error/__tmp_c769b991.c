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
    {STATE_IDLE, '\0', STATE_ERROR},
    {STATE_READING, 'A', STATE_PROCESSING},
    {STATE_READING, 'B', STATE_PROCESSING},
    {STATE_READING, 'C', STATE_PROCESSING},
    {STATE_READING, 'E', STATE_DONE},
    {STATE_READING, '\0', STATE_ERROR},
    {STATE_PROCESSING, '1', STATE_OUTPUT},
    {STATE_PROCESSING, '2', STATE_OUTPUT},
    {STATE_PROCESSING, '3', STATE_OUTPUT},
    {STATE_PROCESSING, '\0', STATE_ERROR},
    {STATE_OUTPUT, 'N', STATE_READING},
    {STATE_OUTPUT, 'E', STATE_DONE},
    {STATE_OUTPUT, '\0', STATE_ERROR}
};

static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

State get_next_state(State current, char input) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current == current && transitions[i].input == input) {
            return transitions[i].next;
        }
    }
    return STATE_ERROR;
}

int is_valid_input_char(char c) {
    return c == 'S' || c == 'A' || c == 'B' || c == 'C' || 
           c == '1' || c == '2' || c == '3' || c == 'N' || c == 'E';
}

void process_data(Context *ctx) {
    if (ctx->position >= MAX_INPUT_LEN) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    unsigned long sum = 0;
    for (size_t i = 0; i < ctx->position; i++) {
        if (ctx->buffer[i] >= 'A' && ctx->buffer[i] <= 'C') {
            sum += (unsigned long)(ctx->buffer[i] - 'A' + 1);
        }
    }
    
    for (int i = 0; i < 1000000; i++) {
        sum = (sum * 1103515245 + 12345) % 2147483647;
    }
    
    ctx->processed_count++;
}

int main(void) {
    Context ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.current_state = STATE_IDLE;
    
    printf("Enter sequence (S to start, A/B/C data, 1/2/3 process, N next, E end): ");
    
    char input_line[MAX_INPUT_LEN + 2];
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_line);
    if (input_len > 0 && input_line[input_len - 1] == '\n') {
        input_line[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0 || input_len > MAX_INPUT_LEN) {
        printf("ERROR: Invalid input length\n");
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        char current_char = input_line[i];
        
        if (!is_valid_input_char(current_char)) {
            printf("ERROR: Invalid character '%c'\n", current_char);
            return EXIT_FAILURE;
        }
        
        State next_state = get_next_state(ctx.current_state, current_char);
        if (next_state == STATE_ERROR) {
            printf("ERROR: Invalid transition from state %d with '%c'\n", 
                   ctx.current_state, current_char);
            return EXIT_FAILURE;
        }
        
        ctx.current_state = next_state;
        
        switch (ctx.current_state) {
            case STATE_READING:
                if (ctx.position < MAX_INPUT_LEN) {
                    ctx.buffer[ctx.position++] = current_char;
                }
                break;
                
            case STATE_PROCESSING:
                if (ctx.position < MAX_INPUT_LEN) {
                    ctx.buffer[ctx.position++] = current_char;
                }
                break;
                
            case STATE_OUTPUT:
                process_data(&ctx);
                printf("Processed batch %d, result ready\n", ctx.processed_count);
                break;
                
            case STATE_DONE:
                printf("Processing complete. Total batches: %d\n", ctx.processed_count);
                return EXIT_SUCCESS;
                
            case STATE_ERROR:
                printf("ERROR: State machine error\n");
                return EXIT_FAILURE;
                
            default:
                break;
        }
    }
    
    if (ctx.current_state != STATE_DONE) {
        printf("ERROR: Incomplete sequence\n");
        return EX