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
                    ctx->processed_count = 0;
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
                        return STATE_PROCESSING;
                    }
                    return STATE_READING;
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
                    return STATE_WRITING;
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
                    if (ctx->processed_count >= ctx->max_iterations) {
                        return STATE_DONE;
                    }
                    return STATE_READING;
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
    
    ctx.current_state = handle_event(&ctx, EVENT_START);
    
    while (ctx.current_state != STATE_DONE && ctx.current_state != STATE_ERROR && iterations < 10) {
        switch (ctx.current_state) {
            case STATE_READING:
                printf("Enter text to process (or 'quit' to exit): ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    ctx.current_state = handle_event(&ctx, EVENT_ERROR);
                    break;
                }
                
                size_t len = strlen(input);
                if (len > 0 && input[len - 1] == '\n') {
                    input[len - 1] = '\0';
                    len--;
                }
                
                if (strcmp(input, "quit") == 0) {
                    ctx.current_state = STATE_DONE;
                    break;
                }
                
                if (!validate_input(input, len)) {
                    printf("Invalid input. Only alphanumeric characters and spaces allowed.\n");
                    ctx.current_state = handle_event(&ctx, EVENT_ERROR);
                    break;
                }
                
                if (len >= sizeof(ctx.buffer)) {
                    printf("Input too long.\n");
                    ctx.current_state = handle_event(&ctx, EVENT_ERROR);
                    break;
                }
                
                memcpy(ctx.buffer, input, len + 1);
                ctx.buffer_len = len;
                ctx.current_state = handle_event(&ctx, EVENT_DATA_READY);
                break;
            
            case STATE_PROCESSING:
                process_buffer(&ctx);
                ctx.current_state = handle_event(&ctx, EVENT_PROCESS_COMPLETE);
                break;
            
            case STATE_WRITING:
                printf("Processed result: %s\n", ctx.buffer);
                printf("Processed