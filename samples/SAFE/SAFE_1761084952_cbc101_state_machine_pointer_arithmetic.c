//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT 256
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_END
} State;

typedef struct {
    State current;
    char buffer[MAX_INPUT];
    size_t position;
    int value;
} Context;

int is_valid_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') return 0;
    }
    return 1;
}

State handle_start(Context *ctx) {
    if (ctx == NULL) return STATE_END;
    ctx->position = 0;
    ctx->value = 0;
    memset(ctx->buffer, 0, MAX_INPUT);
    return STATE_READING;
}

State handle_reading(Context *ctx) {
    if (ctx == NULL) return STATE_END;
    printf("Enter a number: ");
    if (fgets(ctx->buffer, MAX_INPUT, stdin) == NULL) return STATE_END;
    size_t len = strlen(ctx->buffer);
    if (len > 0 && *(ctx->buffer + len - 1) == '\n') {
        *(ctx->buffer + len - 1) = '\0';
    }
    if (!is_valid_input(ctx->buffer)) return STATE_END;
    return STATE_PROCESSING;
}

State handle_processing(Context *ctx) {
    if (ctx == NULL) return STATE_END;
    char *ptr = ctx->buffer;
    while (*ptr != '\0') {
        if (ctx->value > INT_MAX / 10) return STATE_END;
        int digit = *ptr - '0';
        if (ctx->value > (INT_MAX - digit) / 10) return STATE_END;
        ctx->value = ctx->value * 10 + digit;
        ptr++;
    }
    return STATE_WRITING;
}

State handle_writing(Context *ctx) {
    if (ctx == NULL) return STATE_END;
    printf("Processed value: %d\n", ctx->value);
    return STATE_END;
}

State (*state_handlers[STATE_COUNT])(Context *) = {
    handle_start,
    handle_reading,
    handle_processing,
    handle_writing,
    NULL
};

int main(void) {
    Context ctx;
    State current_state = STATE_START;
    
    while (current_state != STATE_END) {
        if (current_state < 0 || current_state >= STATE_COUNT) break;
        State (*handler)(Context *) = *(state_handlers + current_state);
        if (handler == NULL) break;
        current_state = handler(&ctx);
    }
    
    return 0;
}