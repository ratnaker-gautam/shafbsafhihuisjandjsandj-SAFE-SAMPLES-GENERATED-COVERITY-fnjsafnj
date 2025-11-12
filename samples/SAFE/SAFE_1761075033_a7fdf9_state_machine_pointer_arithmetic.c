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
    STATE_FINAL,
    STATE_ERROR
} state_t;

typedef struct {
    state_t current;
    char *input;
    size_t position;
    size_t length;
} context_t;

state_t handle_start(context_t *ctx);
state_t handle_reading(context_t *ctx);
state_t handle_processing(context_t *ctx);
state_t handle_final(context_t *ctx);
state_t handle_error(context_t *ctx);

state_t (*handlers[STATE_COUNT])(context_t *) = {
    handle_start,
    handle_reading,
    handle_processing,
    handle_final,
    handle_error
};

state_t handle_start(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    printf("State machine started.\n");
    return STATE_READING;
}

state_t handle_reading(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    if (ctx->input == NULL) return STATE_ERROR;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    
    char buffer[MAX_INPUT_LEN];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return STATE_ERROR;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && *(buffer + len - 1) == '\n') {
        *(buffer + len - 1) = '\0';
        len--;
    }
    
    if (len >= MAX_INPUT_LEN - 1) {
        printf("Input too long.\n");
        return STATE_ERROR;
    }
    
    char *input_copy = malloc(len + 1);
    if (input_copy == NULL) return STATE_ERROR;
    
    for (size_t i = 0; i <= len; i++) {
        *(input_copy + i) = *(buffer + i);
    }
    
    ctx->input = input_copy;
    ctx->length = len;
    ctx->position = 0;
    
    return STATE_PROCESSING;
}

state_t handle_processing(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    if (ctx->input == NULL) return STATE_ERROR;
    
    printf("Processing: ");
    
    char *ptr = ctx->input;
    while (ptr < ctx->input + ctx->length) {
        if (*ptr >= 'a' && *ptr <= 'z') {
            *ptr = *ptr - 'a' + 'A';
        }
        putchar(*ptr);
        ptr++;
        ctx->position++;
    }
    printf("\n");
    
    return STATE_FINAL;
}

state_t handle_final(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    printf("Processing complete. Result length: %zu\n", ctx->length);
    return STATE_START;
}

state_t handle_error(context_t *ctx) {
    if (ctx != NULL) {
        printf("Error state encountered.\n");
    }
    return STATE_ERROR;
}

int main(void) {
    context_t ctx;
    ctx.current = STATE_START;
    ctx.input = NULL;
    ctx.position = 0;
    ctx.length = 0;
    
    int cycles = 0;
    const int max_cycles = 3;
    
    while (cycles < max_cycles) {
        if (ctx.current >= STATE_COUNT || ctx.current < 0) {
            break;
        }
        
        state_t next_state = handlers[ctx.current](&ctx);
        
        if (next_state == STATE_START && ctx.current != STATE_START) {
            cycles++;
            if (ctx.input != NULL) {
                free(ctx.input);
                ctx.input = NULL;
            }
        }
        
        ctx.current = next_state;
        
        if (ctx.current == STATE_ERROR) {
            break;
        }
    }
    
    if (ctx.input != NULL) {
        free(ctx.input);
    }
    
    return 0;
}