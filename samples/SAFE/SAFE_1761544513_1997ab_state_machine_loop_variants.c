//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum state {
    IDLE,
    READING,
    PROCESSING,
    WRITING,
    ERROR,
    DONE
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_len;
    int data_value;
    int processed_count;
};

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > 255) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') return 0;
    }
    return 1;
}

void state_idle(struct context* ctx) {
    printf("Enter a number (or 'quit' to exit): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) != NULL) {
        size_t len = strlen(ctx->buffer);
        if (len > 0 && ctx->buffer[len - 1] == '\n') {
            ctx->buffer[len - 1] = '\0';
        }
        if (strcmp(ctx->buffer, "quit") == 0) {
            ctx->current_state = DONE;
        } else if (validate_input(ctx->buffer)) {
            ctx->current_state = READING;
        } else {
            printf("Invalid input. Please enter digits only.\n");
        }
    } else {
        ctx->current_state = ERROR;
    }
}

void state_reading(struct context* ctx) {
    long value = strtol(ctx->buffer, NULL, 10);
    if (value >= 0 && value <= 10000) {
        ctx->data_value = (int)value;
        ctx->current_state = PROCESSING;
    } else {
        printf("Number out of range (0-10000).\n");
        ctx->current_state = IDLE;
    }
}

void state_processing(struct context* ctx) {
    int result = ctx->data_value * 2;
    if (result < ctx->data_value) {
        printf("Overflow detected.\n");
        ctx->current_state = ERROR;
        return;
    }
    ctx->data_value = result;
    ctx->processed_count++;
    ctx->current_state = WRITING;
}

void state_writing(struct context* ctx) {
    printf("Processed value: %d\n", ctx->data_value);
    ctx->current_state = IDLE;
}

void state_error(struct context* ctx) {
    printf("An error occurred. Resetting state machine.\n");
    ctx->current_state = IDLE;
    ctx->buffer_len = 0;
    ctx->data_value = 0;
}

int main(void) {
    struct context ctx = {0};
    ctx.current_state = IDLE;
    ctx.processed_count = 0;

    while (ctx.current_state != DONE) {
        switch (ctx.current_state) {
            case IDLE:
                state_idle(&ctx);
                break;
            case READING:
                state_reading(&ctx);
                break;
            case PROCESSING:
                state_processing(&ctx);
                break;
            case WRITING:
                state_writing(&ctx);
                break;
            case ERROR:
                state_error(&ctx);
                break;
            case DONE:
                break;
        }
    }

    printf("Processing complete. Total operations: %d\n", ctx.processed_count);
    return 0;
}