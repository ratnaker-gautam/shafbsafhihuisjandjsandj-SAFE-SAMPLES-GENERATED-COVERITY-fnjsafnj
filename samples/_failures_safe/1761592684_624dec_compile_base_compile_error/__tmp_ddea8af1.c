//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

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
    int processed_count;
    int max_operations;
};

void state_idle(struct context *ctx);
void state_reading(struct context *ctx);
void state_processing(struct context *ctx);
void state_writing(struct context *ctx);
void state_error(struct context *ctx);
void state_done(struct context *ctx);

void initialize_context(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_IDLE;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_len = 0;
    ctx->processed_count = 0;
    ctx->max_operations = 5;
}

int is_valid_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= 256) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isalnum((unsigned char)input[i]) && input[i] != ' ') return 0;
    }
    return 1;
}

void state_idle(struct context *ctx) {
    if (ctx == NULL) return;
    printf("State: IDLE\nEnter 'start' to begin: ");
    char input[16];
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "start") == 0) {
            ctx->current_state = STATE_READING;
        } else {
            printf("Invalid command. Please enter 'start'.\n");
        }
    }
}

void state_reading(struct context *ctx) {
    if (ctx == NULL) return;
    printf("State: READING\nEnter text to process: ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) != NULL) {
        ctx->buffer_len = strcspn(ctx->buffer, "\n");
        ctx->buffer[ctx->buffer_len] = 0;
        if (is_valid_input(ctx->buffer, ctx->buffer_len)) {
            ctx->current_state = STATE_PROCESSING;
        } else {
            ctx->current_state = STATE_ERROR;
        }
    } else {
        ctx->current_state = STATE_ERROR;
    }
}

void state_processing(struct context *ctx) {
    if (ctx == NULL) return;
    printf("State: PROCESSING\n");
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (islower((unsigned char)ctx->buffer[i])) {
            ctx->buffer[i] = (char)toupper((unsigned char)ctx->buffer[i]);
        } else if (isupper((unsigned char)ctx->buffer[i])) {
            ctx->buffer[i] = (char)tolower((unsigned char)ctx->buffer[i]);
        }
    }
    ctx->processed_count++;
    if (ctx->processed_count >= ctx->max_operations) {
        ctx->current_state = STATE_DONE;
    } else {
        ctx->current_state = STATE_WRITING;
    }
}

void state_writing(struct context *ctx) {
    if (ctx == NULL) return;
    printf("State: WRITING\nProcessed text: %s\n", ctx->buffer);
    printf("Operations performed: %d/%d\n", ctx->processed_count, ctx->max_operations);
    printf("Continue? (y/n): ");
    char input[4];
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = 0;
        if (input[0] == 'y' || input[0] == 'Y') {
            ctx->current_state = STATE_READING;
        } else if (input[0] == 'n' || input[0] == 'N') {
            ctx->current_state = STATE_DONE;
        } else {
            printf("Invalid input. Please enter 'y' or 'n'.\n");
        }
    }
}

void state_error(struct context *ctx) {
    if (ctx == NULL) return;
    printf("State: ERROR\nInvalid input detected.\n");
    ctx->current_state = STATE_DONE;
}

void state_done(struct context *ctx) {
    if (ctx == NULL) return;
    printf("State: DONE\nFinal processed count: %d\n", ctx->processed_count);
    printf("Exiting state machine.\n");
}

void run_state_machine(struct context *ctx) {
    if (ctx == NULL) return;
    while (ctx->current_state != STATE_DONE) {
        switch (ctx->current_state) {
            case STATE_IDLE:
                state_idle(ctx);
                break;
            case STATE_READING:
                state_reading(ctx);
                break;
            case STATE_PROCESSING:
                state_processing(ctx);
                break;
            case STATE_WRITING:
                state_writing(ctx);
                break;
            case STATE_ERROR:
                state_error(ctx);
                break;
            case STATE_DONE:
                break;
        }
    }
}

int main(void) {
    struct context ctx;
    initialize_context(&ctx);
    run_state_machine(&ctx);
    return 0