//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_WRITING 3
#define STATE_ERROR 4

#define MAX_INPUT_LEN 100
#define MAX_BUFFER_SIZE 256

#define TRANSITION(ctx, new_state) do { (ctx)->state = (new_state); } while(0)
#define CHECK_BOUNDS(len, max) ((len) >= 0 && (len) < (max))

typedef struct {
    int state;
    char buffer[MAX_BUFFER_SIZE];
    size_t buffer_len;
    int processed_count;
} context_t;

int validate_input(const char* input, size_t len) {
    if (input == NULL) return 0;
    if (!CHECK_BOUNDS(len, MAX_INPUT_LEN)) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

void state_idle(context_t* ctx) {
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    char input[MAX_INPUT_LEN + 1];
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(ctx, STATE_ERROR);
        return;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (!validate_input(input, len)) {
        TRANSITION(ctx, STATE_ERROR);
        return;
    }
    
    if (len == 0) {
        printf("No input received\n");
        return;
    }
    
    if (len >= MAX_BUFFER_SIZE - 1) {
        TRANSITION(ctx, STATE_ERROR);
        return;
    }
    
    memcpy(ctx->buffer, input, len);
    ctx->buffer[len] = '\0';
    ctx->buffer_len = len;
    TRANSITION(ctx, STATE_READING);
}

void state_reading(context_t* ctx) {
    printf("Read %zu characters: %s\n", ctx->buffer_len, ctx->buffer);
    TRANSITION(ctx, STATE_PROCESSING);
}

void state_processing(context_t* ctx) {
    int processed = 0;
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
            processed++;
        }
    }
    ctx->processed_count = processed;
    printf("Processed %d lowercase characters to uppercase\n", processed);
    TRANSITION(ctx, STATE_WRITING);
}

void state_writing(context_t* ctx) {
    printf("Result: %s\n", ctx->buffer);
    printf("Processing complete\n");
    TRANSITION(ctx, STATE_IDLE);
}

void state_error(context_t* ctx) {
    printf("Error: Invalid input or buffer overflow\n");
    TRANSITION(ctx, STATE_IDLE);
}

int main(void) {
    context_t ctx;
    ctx.state = STATE_IDLE;
    ctx.buffer_len = 0;
    ctx.processed_count = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    int running = 1;
    int cycle_count = 0;
    const int max_cycles = 10;
    
    while (running && cycle_count < max_cycles) {
        switch (ctx.state) {
            case STATE_IDLE:
                state_idle(&ctx);
                break;
            case STATE_READING:
                state_reading(&ctx);
                break;
            case STATE_PROCESSING:
                state_processing(&ctx);
                break;
            case STATE_WRITING:
                state_writing(&ctx);
                break;
            case STATE_ERROR:
                state_error(&ctx);
                break;
            default:
                printf("Invalid state detected\n");
                running = 0;
                break;
        }
        
        cycle_count++;
        
        if (cycle_count >= max_cycles) {
            printf("Maximum cycles reached\n");
            running = 0;
        }
    }
    
    return 0;
}