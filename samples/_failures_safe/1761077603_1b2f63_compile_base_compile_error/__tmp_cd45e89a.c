//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: state_machine
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

static enum state handle_event(enum state current, enum event ev, struct context *ctx) {
    if (ctx == NULL) {
        return STATE_ERROR;
    }

    switch (current) {
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
                        printf("Read %zu bytes: %.*s\n", ctx->buffer_len, (int)ctx->buffer_len, ctx->buffer);
                        return STATE_PROCESSING;
                    }
                    return STATE_ERROR;
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
                    printf("Processing complete: %.*s\n", (int)ctx->buffer_len, ctx->buffer);
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
                    printf("Write complete\n");
                    if (ctx->processed_count >= ctx->max_iterations) {
                        return STATE_DONE;
                    }
                    return STATE_READING;
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
                    printf("Error state reset\n");
                    return STATE_IDLE;
                default:
                    return STATE_ERROR;
            }
        
        case STATE_DONE:
            return STATE_DONE;
        
        default:
            return STATE_ERROR;
    }
}

int main(void) {
    struct context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.buffer_len = 0;
    ctx.processed_count = 0;
    ctx.max_iterations = 3;
    
    const char *test_inputs[] = {
        "Hello World 123",
        "Test Data 456",
        "Final Input 789"
    };
    size_t num_inputs = sizeof(test_inputs) / sizeof(test_inputs[0]);
    
    if (ctx.max_iterations > (int)num_inputs) {
        ctx.max_iterations = (int)num_inputs;
    }
    
    enum event events[] = {
        EVENT_START,
        EVENT_DATA_READY,
        EVENT_PROCESS_COMPLETE,
        EVENT_WRITE_COMPLETE,
        EVENT_DATA_READY,
        EVENT_PROCESS_COMPLETE,
        EVENT_WRITE_COMPLETE,
        EVENT_DATA_READY,
        EVENT_PROCESS_COMPLETE,
        EVENT_WRITE_COMPLETE
    };
    
    size_t num_events = sizeof(events) / sizeof(events[0]);
    size_t event_index = 0;
    size_t input_index = 0;
    
    while (ctx.current_state != STATE_DONE && ctx.current_state != STATE_ERROR) {
        if (event_index >= num_events) {
            ctx.current_state = STATE_ERROR;
            break;
        }
        
        enum event current_event = events[event_index];
        
        switch (current_event) {
            case EVENT_DATA_READY:
                if (input_index < num_inputs) {
                    const char *input = test_inputs[input_index];
                    size_t input_len = strlen(input);
                    if (validate_input(input, input_len)) {
                        if (input