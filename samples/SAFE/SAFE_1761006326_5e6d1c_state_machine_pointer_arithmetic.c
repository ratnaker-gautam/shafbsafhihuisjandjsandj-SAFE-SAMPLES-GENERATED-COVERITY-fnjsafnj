//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 100
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_FINISHED,
    STATE_ERROR
} state_t;

typedef struct {
    state_t current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_pos;
    int processed_count;
} context_t;

state_t handle_start(context_t *ctx, char input);
state_t handle_reading(context_t *ctx, char input);
state_t handle_processing(context_t *ctx, char input);
state_t handle_finished(context_t *ctx, char input);
state_t handle_error(context_t *ctx, char input);

state_t (*state_handlers[STATE_COUNT])(context_t*, char) = {
    handle_start,
    handle_reading,
    handle_processing,
    handle_finished,
    handle_error
};

state_t handle_start(context_t *ctx, char input) {
    if (input == 'B') {
        ctx->buffer_pos = 0;
        *(ctx->buffer + ctx->buffer_pos) = input;
        ctx->buffer_pos++;
        return STATE_READING;
    }
    return STATE_ERROR;
}

state_t handle_reading(context_t *ctx, char input) {
    if (ctx->buffer_pos >= MAX_INPUT_LEN - 1) {
        return STATE_ERROR;
    }
    
    if (input == 'E') {
        *(ctx->buffer + ctx->buffer_pos) = '\0';
        return STATE_PROCESSING;
    }
    
    if ((input >= 'A' && input <= 'Z') || (input >= 'a' && input <= 'z')) {
        *(ctx->buffer + ctx->buffer_pos) = input;
        ctx->buffer_pos++;
        return STATE_READING;
    }
    
    return STATE_ERROR;
}

state_t handle_processing(context_t *ctx, char input) {
    if (input != 'P') {
        return STATE_ERROR;
    }
    
    char *ptr = ctx->buffer;
    while (*ptr != '\0') {
        if (*ptr >= 'a' && *ptr <= 'z') {
            *ptr = *ptr - 32;
        }
        ptr++;
    }
    
    ctx->processed_count++;
    return STATE_FINISHED;
}

state_t handle_finished(context_t *ctx, char input) {
    if (input == 'R') {
        ctx->buffer_pos = 0;
        return STATE_START;
    }
    if (input == 'Q') {
        return STATE_FINISHED;
    }
    return STATE_ERROR;
}

state_t handle_error(context_t *ctx, char input) {
    return STATE_ERROR;
}

int main(void) {
    context_t ctx;
    ctx.current_state = STATE_START;
    ctx.buffer_pos = 0;
    ctx.processed_count = 0;
    
    printf("State Machine Processor\n");
    printf("Commands: B=Begin, E=End, P=Process, R=Reset, Q=Quit\n");
    printf("Input sequence: B<text>EP[R|Q]\n");
    
    int running = 1;
    while (running) {
        printf("Current state: %d, Processed: %d\n", ctx.current_state, ctx.processed_count);
        printf("Enter command: ");
        
        char input[MAX_INPUT_LEN];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        char *input_ptr = input;
        while (*input_ptr != '\0' && *input_ptr != '\n') {
            state_t new_state = state_handlers[ctx.current_state](&ctx, *input_ptr);
            ctx.current_state = new_state;
            
            if (ctx.current_state == STATE_FINISHED) {
                printf("Processed: %s\n", ctx.buffer);
                printf("Enter R to reset or Q to quit: ");
            } else if (ctx.current_state == STATE_ERROR) {
                printf("Error in state machine\n");
                ctx.current_state = STATE_START;
                ctx.buffer_pos = 0;
            }
            
            input_ptr++;
        }
        
        if (ctx.current_state == STATE_FINISHED) {
            char response[MAX_INPUT_LEN];
            if (fgets(response, sizeof(response), stdin) == NULL) {
                break;
            }
            
            char *resp_ptr = response;
            while (*resp_ptr != '\0' && *resp_ptr != '\n') {
                state_t new_state = state_handlers[ctx.current_state](&ctx, *resp_ptr);
                ctx.current_state = new_state;
                resp_ptr++;
            }
        }
        
        if (ctx.current_state == STATE_FINISHED) {
            running = 0;
        }
    }
    
    printf("Final processed count: %d\n", ctx.processed_count);
    return 0;
}