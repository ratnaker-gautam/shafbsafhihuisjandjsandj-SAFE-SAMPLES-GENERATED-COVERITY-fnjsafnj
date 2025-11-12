//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_INPUT_LEN 100
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_DONE
} state_t;

typedef struct {
    state_t current_state;
    uint32_t data_value;
    uint32_t processed_value;
    char input_buffer[MAX_INPUT_LEN + 1];
} context_t;

static state_t handle_start(context_t *ctx) {
    printf("Enter a positive integer (max %d digits): ", MAX_INPUT_LEN);
    if (fgets(ctx->input_buffer, sizeof(ctx->input_buffer), stdin) == NULL) {
        return STATE_DONE;
    }
    
    size_t len = strlen(ctx->input_buffer);
    if (len > 0 && ctx->input_buffer[len - 1] == '\n') {
        ctx->input_buffer[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        return STATE_DONE;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (ctx->input_buffer[i] < '0' || ctx->input_buffer[i] > '9') {
            printf("Invalid input: must contain only digits\n");
            return STATE_DONE;
        }
    }
    
    if (len > 10) {
        printf("Input too large\n");
        return STATE_DONE;
    }
    
    uint64_t temp = 0;
    for (size_t i = 0; i < len; i++) {
        temp = temp * 10 + (ctx->input_buffer[i] - '0');
        if (temp > UINT32_MAX) {
            printf("Input exceeds maximum value\n");
            return STATE_DONE;
        }
    }
    
    ctx->data_value = (uint32_t)temp;
    return STATE_READING;
}

static state_t handle_reading(context_t *ctx) {
    if (ctx->data_value == 0) {
        return STATE_DONE;
    }
    return STATE_PROCESSING;
}

static state_t handle_processing(context_t *ctx) {
    uint32_t value = ctx->data_value;
    uint32_t result = 0;
    
    for (uint32_t i = 0; i < 1000000; i++) {
        result = 0;
        uint32_t temp = value;
        
        while (temp > 0) {
            uint32_t digit = temp % 10;
            result = result * 10 + digit;
            temp /= 10;
        }
        
        if (result == value) {
            break;
        }
        
        value += result;
        if (value < result) {
            printf("Integer overflow detected\n");
            return STATE_DONE;
        }
    }
    
    ctx->processed_value = value;
    return STATE_WRITING;
}

static state_t handle_writing(context_t *ctx) {
    printf("Processed result: %u\n", ctx->processed_value);
    return STATE_DONE;
}

static state_t handle_done(context_t *ctx) {
    return STATE_DONE;
}

static state_t (*state_handlers[STATE_COUNT])(context_t *) = {
    handle_start,
    handle_reading,
    handle_processing,
    handle_writing,
    handle_done
};

int main(void) {
    context_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.current_state = STATE_START;
    
    while (ctx.current_state != STATE_DONE) {
        if (ctx.current_state >= STATE_COUNT) {
            break;
        }
        
        state_t next_state = state_handlers[ctx.current_state](&ctx);
        ctx.current_state = next_state;
    }
    
    printf("State machine terminated\n");
    return 0;
}