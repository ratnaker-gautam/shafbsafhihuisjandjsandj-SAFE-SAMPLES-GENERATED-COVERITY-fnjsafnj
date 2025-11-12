//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
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
    STATE_OUTPUT,
    STATE_END
} state_t;

typedef struct {
    state_t current;
    char buffer[MAX_INPUT_LEN];
    size_t pos;
    int result;
} context_t;

static state_t handle_start(context_t *ctx) {
    if (ctx == NULL) return STATE_END;
    ctx->pos = 0;
    ctx->result = 0;
    memset(ctx->buffer, 0, MAX_INPUT_LEN);
    return STATE_READING;
}

static state_t handle_reading(context_t *ctx) {
    if (ctx == NULL) return STATE_END;
    char *ptr = ctx->buffer;
    int c;
    while ((c = getchar()) != EOF && c != '\n') {
        if (ctx->pos < MAX_INPUT_LEN - 1) {
            *(ptr + ctx->pos) = (char)c;
            ctx->pos++;
        }
    }
    if (ctx->pos > 0) {
        *(ptr + ctx->pos) = '\0';
        return STATE_PROCESSING;
    }
    return STATE_END;
}

static state_t handle_processing(context_t *ctx) {
    if (ctx == NULL) return STATE_END;
    char *ptr = ctx->buffer;
    int sum = 0;
    while (*ptr != '\0') {
        if (*ptr >= '0' && *ptr <= '9') {
            sum += *ptr - '0';
        }
        ptr++;
    }
    ctx->result = sum;
    return STATE_OUTPUT;
}

static state_t handle_output(context_t *ctx) {
    if (ctx == NULL) return STATE_END;
    printf("Sum of digits: %d\n", ctx->result);
    return STATE_READING;
}

static state_t (*state_handlers[STATE_COUNT])(context_t *) = {
    handle_start,
    handle_reading,
    handle_processing,
    handle_output,
    NULL
};

int main(void) {
    context_t ctx;
    state_t next_state = STATE_START;
    
    while (next_state != STATE_END) {
        if (next_state < 0 || next_state >= STATE_COUNT) {
            break;
        }
        state_t (*handler)(context_t *) = *(state_handlers + next_state);
        if (handler == NULL) {
            break;
        }
        next_state = handler(&ctx);
    }
    
    return 0;
}