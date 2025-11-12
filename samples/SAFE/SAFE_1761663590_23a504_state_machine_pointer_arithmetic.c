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
    char *input_ptr;
    char *end_ptr;
    int processed_count;
} context_t;

state_t handle_start(context_t *ctx);
state_t handle_reading(context_t *ctx);
state_t handle_processing(context_t *ctx);
state_t handle_finished(context_t *ctx);
state_t handle_error(context_t *ctx);

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    context_t ctx;
    
    printf("Enter text to process (max %d chars): ", MAX_INPUT_LEN);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return EXIT_FAILURE;
    }
    
    ctx.current_state = STATE_START;
    ctx.input_ptr = input_buffer;
    ctx.end_ptr = input_buffer + input_len;
    ctx.processed_count = 0;
    
    state_t (*state_handlers[STATE_COUNT])(context_t *) = {
        handle_start,
        handle_reading,
        handle_processing,
        handle_finished,
        handle_error
    };
    
    while (ctx.current_state != STATE_FINISHED && ctx.current_state != STATE_ERROR) {
        if (ctx.current_state < 0 || ctx.current_state >= STATE_COUNT) {
            fprintf(stderr, "Invalid state\n");
            return EXIT_FAILURE;
        }
        ctx.current_state = state_handlers[ctx.current_state](&ctx);
    }
    
    if (ctx.current_state == STATE_ERROR) {
        fprintf(stderr, "Processing failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Successfully processed %d characters\n", ctx.processed_count);
    return EXIT_SUCCESS;
}

state_t handle_start(context_t *ctx) {
    if (ctx == NULL || ctx->input_ptr == NULL || ctx->end_ptr == NULL) {
        return STATE_ERROR;
    }
    if (ctx->input_ptr >= ctx->end_ptr) {
        return STATE_ERROR;
    }
    printf("Starting processing...\n");
    return STATE_READING;
}

state_t handle_reading(context_t *ctx) {
    if (ctx == NULL || ctx->input_ptr == NULL || ctx->end_ptr == NULL) {
        return STATE_ERROR;
    }
    if (ctx->input_ptr >= ctx->end_ptr) {
        return STATE_PROCESSING;
    }
    
    char *current = ctx->input_ptr;
    while (current < ctx->end_ptr) {
        if (*current == ' ') {
            break;
        }
        current++;
    }
    
    if (current > ctx->input_ptr) {
        ctx->processed_count += (int)(current - ctx->input_ptr);
        ctx->input_ptr = current;
    }
    
    if (ctx->input_ptr < ctx->end_ptr && *ctx->input_ptr == ' ') {
        ctx->input_ptr++;
    }
    
    return STATE_PROCESSING;
}

state_t handle_processing(context_t *ctx) {
    if (ctx == NULL || ctx->input_ptr == NULL || ctx->end_ptr == NULL) {
        return STATE_ERROR;
    }
    
    if (ctx->input_ptr >= ctx->end_ptr) {
        return STATE_FINISHED;
    }
    
    char *word_start = ctx->input_ptr;
    char *word_end = word_start;
    
    while (word_end < ctx->end_ptr && *word_end != ' ') {
        word_end++;
    }
    
    if (word_end > word_start) {
        printf("Processed word: ");
        char *ptr = word_start;
        while (ptr < word_end) {
            putchar(*ptr);
            ptr++;
        }
        putchar('\n');
        
        ctx->input_ptr = word_end;
        if (ctx->input_ptr < ctx->end_ptr && *ctx->input_ptr == ' ') {
            ctx->input_ptr++;
        }
    }
    
    return STATE_READING;
}

state_t handle_finished(context_t *ctx) {
    return STATE_FINISHED;
}

state_t handle_error(context_t *ctx) {
    return STATE_ERROR;
}