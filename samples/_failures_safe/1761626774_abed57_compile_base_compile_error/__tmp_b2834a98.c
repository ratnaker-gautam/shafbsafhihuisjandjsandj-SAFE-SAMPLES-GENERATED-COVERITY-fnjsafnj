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
    uint32_t processed_count;
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

void process_data(struct context* ctx) {
    if (ctx == NULL || ctx->buffer_len == 0) {
        return;
    }
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    ctx->processed_count++;
}

enum state handle_event(enum state current, enum event ev, struct context* ctx) {
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
                        printf("Data ready: %zu bytes\n", ctx->buffer_len);
                        return STATE_PROCESSING;
                    } else {
                        return STATE_ERROR;
                    }
                case EVENT_RESET:
                    ctx->buffer_len = 0;
                    ctx->processed_count = 0;
                    return STATE_IDLE;
                case EVENT_ERROR:
                    return STATE_ERROR;
                default:
                    return STATE_ERROR;
            }
        
        case STATE_PROCESSING:
            switch (ev) {
                case EVENT_PROCESS_COMPLETE:
                    printf("Processing complete\n");
                    return STATE_WRITING;
                case EVENT_RESET:
                    ctx->buffer_len = 0;
                    ctx->processed_count = 0;
                    return STATE_IDLE;
                case EVENT_ERROR:
                    return STATE_ERROR;
                default:
                    return STATE_ERROR;
            }
        
        case STATE_WRITING:
            switch (ev) {
                case EVENT_WRITE_COMPLETE:
                    printf("Write complete. Processed: %u\n", ctx->processed_count);
                    return STATE_DONE;
                case EVENT_RESET:
                    ctx->buffer_len = 0;
                    ctx->processed_count = 0;
                    return STATE_IDLE;
                case EVENT_ERROR:
                    return STATE_ERROR;
                default:
                    return STATE_ERROR;
            }
        
        case STATE_ERROR:
            switch (ev) {
                case EVENT_RESET:
                    ctx->buffer_len = 0;
                    ctx->processed_count = 0;
                    return STATE_IDLE;
                default:
                    return STATE_ERROR;
            }
        
        case STATE_DONE:
            switch (ev) {
                case EVENT_RESET:
                    ctx->buffer_len = 0;
                    ctx->processed_count = 0;
                    return STATE_IDLE;
                default:
                    return STATE_DONE;
            }
        
        default:
            return STATE_ERROR;
    }
}

int main(void) {
    struct context ctx = {STATE_IDLE, {0}, 0, 0};
    char input[256];
    int running = 1;
    
    printf("State Machine Demo - Enter commands:\n");
    printf("start, data <text>, process, write, reset, quit\n");
    
    while (running) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "quit") == 0) {
            running = 0;
            continue;
        }
        
        enum event ev = EVENT_ERROR;
        
        if (strcmp(input, "start") == 0) {
            ev = EVENT_START;
        } else if (strncmp(input, "data ", 5) == 0) {
            const char* data = input + 5;
            size_t data_len = strlen(data);
            if (validate_input(data, data_len)) {
                if (data_len < sizeof(ctx.buffer)) {
                    memcpy(ctx.buffer, data, data_len);
                    ctx.buffer_len = data_len;
                    ev = EVENT_DATA_READY;
                }
            }
        } else if (strcmp(input, "process") == 0) {
            if (ctx.current_state == STATE_PROCESSING) {
                process_data(&ctx);
                ev = EVENT_PROCESS_COMPLETE;
            }
        } else if (strcmp(input, "write") == 0) {
            ev = EVENT_WRITE_COMPLETE;
        } else if (strcmp(input, "reset") == 0)