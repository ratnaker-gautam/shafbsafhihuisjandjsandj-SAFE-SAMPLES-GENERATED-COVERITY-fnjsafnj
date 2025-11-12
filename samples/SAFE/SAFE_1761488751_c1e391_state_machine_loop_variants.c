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
    size_t buffer_pos;
    int processed_count;
    int error_code;
};

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > 255) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

void process_buffer(struct context* ctx) {
    if (ctx == NULL) return;
    
    size_t i = 0;
    while (i < ctx->buffer_pos) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
        i++;
    }
    ctx->processed_count++;
}

int main(void) {
    struct context ctx;
    ctx.current_state = IDLE;
    ctx.buffer_pos = 0;
    ctx.processed_count = 0;
    ctx.error_code = 0;
    
    char input[512];
    int running = 1;
    
    do {
        switch (ctx.current_state) {
            case IDLE:
                printf("Enter text (or 'quit' to exit): ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    ctx.current_state = ERROR;
                    ctx.error_code = 1;
                    break;
                }
                
                size_t len = strlen(input);
                if (len > 0 && input[len - 1] == '\n') {
                    input[len - 1] = '\0';
                    len--;
                }
                
                if (len == 0) {
                    ctx.current_state = IDLE;
                } else if (strcmp(input, "quit") == 0) {
                    ctx.current_state = DONE;
                } else if (validate_input(input)) {
                    if (len < sizeof(ctx.buffer)) {
                        strncpy(ctx.buffer, input, sizeof(ctx.buffer) - 1);
                        ctx.buffer[sizeof(ctx.buffer) - 1] = '\0';
                        ctx.buffer_pos = len;
                        ctx.current_state = READING;
                    } else {
                        ctx.current_state = ERROR;
                        ctx.error_code = 2;
                    }
                } else {
                    ctx.current_state = ERROR;
                    ctx.error_code = 3;
                }
                break;
                
            case READING:
                printf("Read %zu characters\n", ctx.buffer_pos);
                ctx.current_state = PROCESSING;
                break;
                
            case PROCESSING:
                process_buffer(&ctx);
                printf("Processed: %s\n", ctx.buffer);
                ctx.current_state = WRITING;
                break;
                
            case WRITING:
                printf("Writing completed. Total processed: %d\n", ctx.processed_count);
                ctx.current_state = IDLE;
                break;
                
            case ERROR:
                printf("Error %d occurred\n", ctx.error_code);
                ctx.current_state = IDLE;
                ctx.error_code = 0;
                break;
                
            case DONE:
                printf("Final count: %d strings processed\n", ctx.processed_count);
                running = 0;
                break;
                
            default:
                ctx.current_state = ERROR;
                ctx.error_code = 4;
                break;
        }
    } while (running);
    
    return 0;
}