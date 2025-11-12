//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
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

enum event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_len;
    int error_code;
};

int validate_input(const char *input, size_t max_len) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= max_len) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

enum event get_user_event(void) {
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) return EVENT_ERROR;
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "data") == 0) return EVENT_DATA_READY;
    if (strcmp(input, "process") == 0) return EVENT_PROCESS_COMPLETE;
    if (strcmp(input, "write") == 0) return EVENT_WRITE_COMPLETE;
    if (strcmp(input, "error") == 0) return EVENT_ERROR;
    if (strcmp(input, "reset") == 0) return EVENT_RESET;
    
    return EVENT_ERROR;
}

void handle_idle(struct context *ctx, enum event ev) {
    switch (ev) {
        case EVENT_START:
            printf("Transitioning from IDLE to READING\n");
            ctx->current_state = STATE_READING;
            break;
        default:
            printf("Invalid event for IDLE state\n");
            ctx->current_state = STATE_ERROR;
            break;
    }
}

void handle_reading(struct context *ctx, enum event ev) {
    switch (ev) {
        case EVENT_DATA_READY:
            printf("Enter data to process: ");
            if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) != NULL) {
                ctx->buffer[strcspn(ctx->buffer, "\n")] = '\0';
                if (validate_input(ctx->buffer, sizeof(ctx->buffer))) {
                    ctx->buffer_len = strlen(ctx->buffer);
                    printf("Data received: %s\n", ctx->buffer);
                    ctx->current_state = STATE_PROCESSING;
                } else {
                    printf("Invalid input data\n");
                    ctx->current_state = STATE_ERROR;
                }
            } else {
                printf("Failed to read input\n");
                ctx->current_state = STATE_ERROR;
            }
            break;
        case EVENT_RESET:
            printf("Resetting from READING to IDLE\n");
            ctx->current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for READING state\n");
            ctx->current_state = STATE_ERROR;
            break;
    }
}

void handle_processing(struct context *ctx, enum event ev) {
    switch (ev) {
        case EVENT_PROCESS_COMPLETE:
            printf("Processing data: ");
            for (size_t i = 0; i < ctx->buffer_len; i++) {
                if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
                    ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
                }
            }
            printf("Processed: %s\n", ctx->buffer);
            ctx->current_state = STATE_WRITING;
            break;
        case EVENT_RESET:
            printf("Resetting from PROCESSING to IDLE\n");
            ctx->current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for PROCESSING state\n");
            ctx->current_state = STATE_ERROR;
            break;
    }
}

void handle_writing(struct context *ctx, enum event ev) {
    switch (ev) {
        case EVENT_WRITE_COMPLETE:
            printf("Writing completed: %s\n", ctx->buffer);
            ctx->current_state = STATE_DONE;
            break;
        case EVENT_RESET:
            printf("Resetting from WRITING to IDLE\n");
            ctx->current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for WRITING state\n");
            ctx->current_state = STATE_ERROR;
            break;
    }
}

void handle_error(struct context *ctx, enum event ev) {
    switch (ev) {
        case EVENT_RESET:
            printf("Resetting from ERROR to IDLE\n");
            ctx->current_state = STATE_IDLE;
            ctx->error_code = 0;
            break;
        default:
            printf("Must reset from ERROR state\n");
            break;
    }
}

void handle_done(struct context *ctx, enum event ev) {
    switch (ev) {
        case EVENT_RESET:
            printf("Resetting from DONE to IDLE\n");
            ctx->current_state = STATE_IDLE;
            break;
        default:
            printf("Must reset from DONE state\n");
            break;
    }
}

int main(void) {
    struct context ctx = {STATE_IDLE, "",