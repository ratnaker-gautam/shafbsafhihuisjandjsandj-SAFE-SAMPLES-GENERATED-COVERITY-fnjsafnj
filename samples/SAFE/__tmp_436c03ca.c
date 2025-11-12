//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
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
            *ptr = *ptr - 'a' + 'A';
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
    return STATE_ERROR;
}

state_t handle_error(context_t *ctx, char input) {
    return STATE_ERROR;
}

void initialize_context(context_t *ctx) {
    ctx->current_state = STATE_START;
    ctx->buffer_pos = 0;
    ctx->processed_count = 0;
    memset(ctx->buffer, 0, MAX_INPUT_LEN);
}

int main(void) {
    context_t ctx;
    initialize_context(&ctx);
    
    printf("State Machine Demo\n");
    printf("Commands: B (Begin), E (End), P (Process), R (Reset)\n");
    printf("Input format: B<letters>EPR (e.g., BHelloEPR)\n");
    printf("Enter commands (Ctrl+D to exit): ");
    
    int input_char;
    while ((input_char = getchar()) != EOF) {
        if (input_char == '\n') {
            continue;
        }
        
        if (input_char < 0 || input_char > 127) {
            ctx.current_state = STATE_ERROR;
            break;
        }
        
        state_t next_state = state_handlers[ctx.current_state](&ctx, (char)input_char);
        ctx.current_state = next_state;
        
        if (ctx.current_state == STATE_FINISHED) {
            printf("Processed: %s (Total: %d)\n", ctx.buffer, ctx.processed_count);
        } else if (ctx.current_state == STATE_ERROR) {
            printf("Error in state machine\n");
            break;
        }
    }
    
    if (ctx.current_state != STATE_ERROR) {
        printf("Final processed count: %d\n", ctx.processed_count);
    }
    
    return ctx.current_state == STATE_ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}