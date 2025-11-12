//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdint.h>
#include <string.h>

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

int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len >= 256) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') {
            return 0;
        }
    }
    return 1;
}

enum state handle_event(enum state current, enum event ev, struct context* ctx) {
    switch (current) {
        case STATE_IDLE:
            switch (ev) {
                case EVENT_START:
                    printf("Starting data processing\n");
                    return STATE_READING;
                case EVENT_RESET:
                    printf("System reset\n");
                    return STATE_IDLE;
                default:
                    return STATE_ERROR;
            }
        
        case STATE_READING:
            switch (ev) {
                case EVENT_DATA_READY:
                    if (ctx != NULL && ctx->buffer_len > 0) {
                        printf("Data read: %.*s\n", (int)ctx->buffer_len, ctx->buffer);
                        return STATE_PROCESSING;
                    }
                    return STATE_ERROR;
                case EVENT_ERROR:
                    return STATE_ERROR;
                case EVENT_RESET:
                    return STATE_IDLE;
                default:
                    return STATE_ERROR;
            }
        
        case STATE_PROCESSING:
            switch (ev) {
                case EVENT_PROCESS_COMPLETE:
                    if (ctx != NULL) {
                        for (size_t i = 0; i < ctx->buffer_len; i++) {
                            if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
                                ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
                            }
                        }
                        printf("Data processed: %.*s\n", (int)ctx->buffer_len, ctx->buffer);
                        return STATE_WRITING;
                    }
                    return STATE_ERROR;
                case EVENT_ERROR:
                    return STATE_ERROR;
                case EVENT_RESET:
                    return STATE_IDLE;
                default:
                    return STATE_ERROR;
            }
        
        case STATE_WRITING:
            switch (ev) {
                case EVENT_WRITE_COMPLETE:
                    printf("Data written successfully\n");
                    return STATE_DONE;
                case EVENT_ERROR:
                    return STATE_ERROR;
                case EVENT_RESET:
                    return STATE_IDLE;
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
            switch (ev) {
                case EVENT_RESET:
                    printf("Process completed, resetting\n");
                    return STATE_IDLE;
                default:
                    return STATE_DONE;
            }
        
        default:
            return STATE_ERROR;
    }
}

int main(void) {
    struct context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.buffer_len = 0;
    ctx.error_code = 0;
    
    const char* test_data = "Hello World";
    size_t data_len = strlen(test_data);
    
    if (!validate_input(test_data, data_len)) {
        printf("Invalid input data\n");
        return 1;
    }
    
    if (data_len < sizeof(ctx.buffer)) {
        memcpy(ctx.buffer, test_data, data_len);
        ctx.buffer_len = data_len;
        ctx.buffer[data_len] = '\0';
    } else {
        printf("Input data too large\n");
        return 1;
    }
    
    ctx.current_state = handle_event(ctx.current_state, EVENT_START, &ctx);
    ctx.current_state = handle_event(ctx.current_state, EVENT_DATA_READY, &ctx);
    ctx.current_state = handle_event(ctx.current_state, EVENT_PROCESS_COMPLETE, &ctx);
    ctx.current_state = handle_event(ctx.current_state, EVENT_WRITE_COMPLETE, &ctx);
    
    printf("Final state: %d\n", ctx.current_state);
    
    ctx.current_state = handle_event(ctx.current_state, EVENT_RESET, &ctx);
    printf("After reset: %d\n", ctx.current_state);
    
    return 0;
}