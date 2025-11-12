//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum state {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_DONE
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_len;
    int data_count;
    int max_data;
};

int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len >= 256) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) {
            return 0;
        }
    }
    return 1;
}

void state_idle(struct context* ctx) {
    printf("Enter data (max 255 chars) or 'quit' to exit: ");
    char input[256];
    if (fgets(input, sizeof(input), stdin) != NULL) {
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        if (!validate_input(input, len)) {
            ctx->current_state = STATE_ERROR;
            return;
        }
        if (strcmp(input, "quit") == 0) {
            ctx->current_state = STATE_DONE;
            return;
        }
        if (len > 0) {
            strncpy(ctx->buffer, input, sizeof(ctx->buffer) - 1);
            ctx->buffer[sizeof(ctx->buffer) - 1] = '\0';
            ctx->buffer_len = len;
            ctx->current_state = STATE_READING;
        }
    } else {
        ctx->current_state = STATE_ERROR;
    }
}

void state_reading(struct context* ctx) {
    if (ctx->buffer_len == 0) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    ctx->current_state = STATE_PROCESSING;
}

void state_processing(struct context* ctx) {
    if (ctx->buffer_len == 0) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    ctx->current_state = STATE_WRITING;
}

void state_writing(struct context* ctx) {
    if (ctx->buffer_len == 0) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    printf("Processed data: %s\n", ctx->buffer);
    ctx->data_count++;
    if (ctx->data_count >= ctx->max_data) {
        ctx->current_state = STATE_DONE;
    } else {
        ctx->current_state = STATE_IDLE;
    }
}

void state_error(struct context* ctx) {
    printf("Error occurred in processing\n");
    ctx->current_state = STATE_DONE;
}

void state_done(struct context* ctx) {
    printf("Processing complete. Processed %d items.\n", ctx->data_count);
}

int main(void) {
    struct context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.buffer_len = 0;
    ctx.data_count = 0;
    ctx.max_data = 5;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));

    while (ctx.current_state != STATE_DONE) {
        switch (ctx.current_state) {
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
            case STATE_DONE:
                break;
        }
    }

    return 0;
}