//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_DONE
};

enum Event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
};

struct Context {
    char buffer[256];
    size_t bytes_read;
    size_t bytes_processed;
    size_t bytes_written;
};

int simulate_read(struct Context *ctx) {
    if (ctx == NULL) return -1;
    const char *test_data = "Sample data for processing";
    size_t len = strlen(test_data);
    if (len >= sizeof(ctx->buffer)) return -1;
    memcpy(ctx->buffer, test_data, len);
    ctx->bytes_read = len;
    ctx->buffer[len] = '\0';
    return 0;
}

int simulate_process(struct Context *ctx) {
    if (ctx == NULL) return -1;
    if (ctx->bytes_read == 0) return -1;
    for (size_t i = 0; i < ctx->bytes_read; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    ctx->bytes_processed = ctx->bytes_read;
    return 0;
}

int simulate_write(struct Context *ctx) {
    if (ctx == NULL) return -1;
    if (ctx->bytes_processed == 0) return -1;
    printf("Output: %s\n", ctx->buffer);
    ctx->bytes_written = ctx->bytes_processed;
    return 0;
}

enum Event get_next_event(enum State current_state) {
    switch (current_state) {
        case STATE_IDLE:
            return EVENT_START;
        case STATE_READING:
            return EVENT_DATA_READY;
        case STATE_PROCESSING:
            return EVENT_PROCESS_COMPLETE;
        case STATE_WRITING:
            return EVENT_WRITE_COMPLETE;
        case STATE_ERROR:
            return EVENT_RESET;
        default:
            return EVENT_ERROR;
    }
}

enum State handle_event(enum State current_state, enum Event event, struct Context *ctx) {
    switch (current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                printf("Starting data processing pipeline\n");
                return STATE_READING;
            }
            break;
        case STATE_READING:
            if (event == EVENT_DATA_READY) {
                if (simulate_read(ctx) == 0) {
                    printf("Read %zu bytes\n", ctx->bytes_read);
                    return STATE_PROCESSING;
                } else {
                    return STATE_ERROR;
                }
            }
            break;
        case STATE_PROCESSING:
            if (event == EVENT_PROCESS_COMPLETE) {
                if (simulate_process(ctx) == 0) {
                    printf("Processed %zu bytes\n", ctx->bytes_processed);
                    return STATE_WRITING;
                } else {
                    return STATE_ERROR;
                }
            }
            break;
        case STATE_WRITING:
            if (event == EVENT_WRITE_COMPLETE) {
                if (simulate_write(ctx) == 0) {
                    printf("Wrote %zu bytes\n", ctx->bytes_written);
                    return STATE_DONE;
                } else {
                    return STATE_ERROR;
                }
            }
            break;
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                printf("Resetting state machine\n");
                memset(ctx, 0, sizeof(struct Context));
                return STATE_IDLE;
            }
            break;
        case STATE_DONE:
            if (event == EVENT_START) {
                memset(ctx, 0, sizeof(struct Context));
                return STATE_READING;
            }
            break;
        default:
            break;
    }
    return STATE_ERROR;
}

int main(void) {
    enum State current_state = STATE_IDLE;
    struct Context ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    int max_iterations = 10;
    int iteration = 0;
    
    while (current_state != STATE_DONE && iteration < max_iterations) {
        enum Event event = get_next_event(current_state);
        enum State next_state = handle_event(current_state, event, &ctx);
        
        if (next_state == STATE_ERROR && current_state == STATE_ERROR) {
            printf("Fatal error: stuck in error state\n");
            break;
        }
        
        current_state = next_state;
        iteration++;
        
        if (current_state == STATE_DONE) {
            printf("Processing completed successfully\n");
            break;
        }
    }
    
    if (iteration >= max_iterations) {
        printf("Maximum iterations reached\n");
    }
    
    return 0;
}