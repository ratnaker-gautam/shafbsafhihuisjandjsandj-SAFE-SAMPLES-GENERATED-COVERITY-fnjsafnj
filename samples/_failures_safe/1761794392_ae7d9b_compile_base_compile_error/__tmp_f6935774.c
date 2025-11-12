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
    ctx->max_operations = 10;
}

int is_valid_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= 256) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isalnum((unsigned char)input[i]) && input[i] != ' ') return 0;
    }
    return 1;
}

void process_buffer(struct context *ctx) {
    if (ctx == NULL || ctx->buffer_len == 0) return;
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (islower((unsigned char)ctx->buffer[i])) {
            ctx->buffer[i] = (char)toupper((unsigned char)ctx->buffer[i]);
        } else if (isupper((unsigned char)ctx->buffer[i])) {
            ctx->buffer[i] = (char)tolower((unsigned char)ctx->buffer[i]);
        }
    }
}

void state_idle(struct context *ctx) {
    if (ctx == NULL) return;
    printf("State: IDLE\nEnter 'start' to begin: ");
    char input[16];
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = '\0';
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
        ctx->buffer[strcspn(ctx->buffer, "\n")] = '\0';
        ctx->buffer_len = strlen(ctx->buffer);
        if (is_valid_input(ctx->buffer, ctx->buffer_len)) {
            ctx->current_state = STATE_PROCESSING;
        } else {
            printf("Invalid input. Only alphanumeric characters and spaces allowed.\n");
            ctx->current_state = STATE_ERROR;
        }
    } else {
        ctx->current_state = STATE_ERROR;
    }
}

void state_processing(struct context *ctx) {
    if (ctx == NULL) return;
    printf("State: PROCESSING\n");
    process_buffer(ctx);
    ctx->processed_count++;
    if (ctx->processed_count > ctx->max_operations) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    ctx->current_state = STATE_WRITING;
}

void state_writing(struct context *ctx) {
    if (ctx == NULL) return;
    printf("State: WRITING\nProcessed text: %s\n", ctx->buffer);
    printf("Processed %d items so far.\n", ctx->processed_count);
    if (ctx->processed_count >= ctx->max_operations) {
        ctx->current_state = STATE_DONE;
    } else {
        printf("Continue? (y/n): ");
        char response[4];
        if (fgets(response, sizeof(response), stdin) != NULL) {
            response[strcspn(response, "\n")] = '\0';
            if (response[0] == 'y' || response[0] == 'Y') {
                ctx->current_state = STATE_READING;
            } else {
                ctx->current_state = STATE_DONE;
            }
        } else {
            ctx->current_state = STATE_ERROR;
        }
    }
}

void state_error(struct context *ctx) {
    if (ctx == NULL) return;
    printf("State: ERROR\nAn error occurred. Resetting...\n");
    initialize_context(ctx);
    ctx->current_state = STATE_IDLE;
}

void state_done(struct context *ctx) {
    if (ctx == NULL) return;
    printf("State: DONE\nProcessing complete. Processed %d items.\n", ctx->processed_count);
    ctx->current_state = STATE_IDLE;
}

void run_state_machine(struct context *ctx) {
    if (ctx == NULL) return;
    while (1) {
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