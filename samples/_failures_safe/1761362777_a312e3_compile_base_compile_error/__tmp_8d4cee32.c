//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
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
    int processed_count;
    int max_iterations;
};

static int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= 256) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (!isalnum((unsigned char)input[i]) && input[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

static void process_buffer(struct context *ctx) {
    if (ctx == NULL || ctx->buffer_len == 0) {
        return;
    }
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (islower((unsigned char)ctx->buffer[i])) {
            ctx->buffer[i] = (char)toupper((unsigned char)ctx->buffer[i]);
        } else if (isupper((unsigned char)ctx->buffer[i])) {
            ctx->buffer[i] = (char)tolower((unsigned char)ctx->buffer[i]);
        }
    }
    ctx->processed_count++;
}

static enum state handle_event(struct context *ctx, enum event ev) {
    if (ctx == NULL) {
        return STATE_ERROR;
    }

    switch (ctx->current_state) {
        case STATE_IDLE:
            switch (ev) {
                case EVENT_START:
                    printf("State machine started\n");
                    return STATE_READING;
                case EVENT_RESET:
                    return STATE_IDLE;
                default:
                    return STATE_ERROR;
            }
        
        case STATE_READING:
            switch (ev) {
                case EVENT_DATA_READY:
                    if (ctx->buffer_len > 0) {
                        printf("Processing data: %.*s\n", (int)ctx->buffer_len, ctx->buffer);
                        return STATE_PROCESSING;
                    } else {
                        return STATE_ERROR;
                    }
                case EVENT_RESET:
                    return STATE_IDLE;
                case EVENT_ERROR:
                    return STATE_ERROR;
                default:
                    return STATE_ERROR;
            }
        
        case STATE_PROCESSING:
            switch (ev) {
                case EVENT_PROCESS_COMPLETE:
                    printf("Data processed: %.*s\n", (int)ctx->buffer_len, ctx->buffer);
                    return STATE_WRITING;
                case EVENT_RESET:
                    return STATE_IDLE;
                case EVENT_ERROR:
                    return STATE_ERROR;
                default:
                    return STATE_ERROR;
            }
        
        case STATE_WRITING:
            switch (ev) {
                case EVENT_WRITE_COMPLETE:
                    printf("Data written successfully\n");
                    if (ctx->processed_count < ctx->max_iterations) {
                        return STATE_READING;
                    } else {
                        return STATE_DONE;
                    }
                case EVENT_RESET:
                    return STATE_IDLE;
                case EVENT_ERROR:
                    return STATE_ERROR;
                default:
                    return STATE_ERROR;
            }
        
        case STATE_ERROR:
            switch (ev) {
                case EVENT_RESET:
                    printf("Resetting from error state\n");
                    return STATE_IDLE;
                default:
                    return STATE_ERROR;
            }
        
        case STATE_DONE:
            switch (ev) {
                case EVENT_RESET:
                    return STATE_IDLE;
                default:
                    return STATE_DONE;
            }
        
        default:
            return STATE_ERROR;
    }
}

int main(void) {
    struct context ctx = {0};
    ctx.current_state = STATE_IDLE;
    ctx.max_iterations = 3;
    
    char input[256];
    int iterations = 0;
    
    while (ctx.current_state != STATE_DONE && iterations < 10) {
        switch (ctx.current_state) {
            case STATE_IDLE:
                printf("Enter 'start' to begin or 'quit' to exit: ");
                if (fgets(input, sizeof(input), stdin) != NULL) {
                    input[strcspn(input, "\n")] = '\0';
                    if (strcmp(input, "start") == 0) {
                        ctx.current_state = handle_event(&ctx, EVENT_START);
                    } else if (strcmp(input, "quit") == 0) {
                        return 0;
                    }
                }
                break;
                
            case STATE_READING:
                printf("Enter data to process (alphanumeric only): ");
                if (fgets(input, sizeof(input), stdin) != NULL) {
                    input[strcspn(input, "\n")] = '\0';
                    size_t len = strlen(input);
                    if (validate_input(input, len)) {
                        strncpy(ctx.buffer, input, sizeof(ctx.buffer) - 1);
                        ctx.buffer[sizeof(ctx.buffer) - 1] = '\0';
                        ctx.buffer_len = len;
                        ctx.current_state = handle_event(&ctx, EVENT_DATA_READY);
                    } else {
                        printf("Invalid input\n");
                        ctx.current_state = handle_event(&ctx, EVENT_ERROR);
                    }
                } else {
                    ctx.current_state = handle_event