//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 256
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
    char *buffer;
    size_t pos;
    size_t len;
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
    ctx->pos = 0;
    return STATE_READING;
}

state_t handle_reading(context_t *ctx) {
    if (ctx == NULL || ctx->buffer == NULL) return STATE_ERROR;
    
    int c = getchar();
    if (c == EOF) {
        if (ctx->pos == 0) return STATE_ERROR;
        return STATE_PROCESSING;
    }
    
    if (ctx->pos >= ctx->len - 1) return STATE_ERROR;
    
    *(ctx->buffer + ctx->pos) = (char)c;
    ctx->pos++;
    
    if (c == '\n') {
        *(ctx->buffer + ctx->pos) = '\0';
        return STATE_PROCESSING;
    }
    
    return STATE_READING;
}

state_t handle_processing(context_t *ctx) {
    if (ctx == NULL || ctx->buffer == NULL || ctx->pos == 0) return STATE_ERROR;
    
    char *start = ctx->buffer;
    char *end = ctx->buffer + ctx->pos - 1;
    
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
    
    return STATE_FINAL;
}

state_t handle_final(context_t *ctx) {
    if (ctx == NULL || ctx->buffer == NULL) return STATE_ERROR;
    
    char *ptr = ctx->buffer;
    while (*ptr != '\0') {
        putchar(*ptr);
        ptr++;
    }
    putchar('\n');
    
    return STATE_START;
}

state_t handle_error(context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    fputs("Error occurred\n", stderr);
    return STATE_ERROR;
}

int main(void) {
    char buffer[MAX_INPUT_LEN];
    context_t ctx = {
        .current = STATE_START,
        .buffer = buffer,
        .pos = 0,
        .len = MAX_INPUT_LEN
    };
    
    while (1) {
        if (ctx.current >= STATE_COUNT) {
            ctx.current = STATE_ERROR;
        }
        
        state_t next = handlers[ctx.current](&ctx);
        if (next == STATE_ERROR && ctx.current == STATE_ERROR) {
            break;
        }
        ctx.current = next;
        
        if (ctx.current == STATE_START) {
            ctx.pos = 0;
            memset(buffer, 0, MAX_INPUT_LEN);
        }
    }
    
    return EXIT_FAILURE;
}