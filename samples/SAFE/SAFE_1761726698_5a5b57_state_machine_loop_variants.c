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
    size_t buffer_len;
    int data_count;
    int max_data;
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
}

int main(void) {
    struct context ctx;
    ctx.current_state = IDLE;
    ctx.buffer_len = 0;
    ctx.data_count = 0;
    ctx.max_data = 5;
    
    int running = 1;
    
    while (running) {
        switch (ctx.current_state) {
            case IDLE:
                printf("Enter data (or 'quit' to exit): ");
                ctx.buffer_len = 0;
                ctx.current_state = READING;
                break;
                
            case READING: {
                char input[256];
                if (fgets(input, sizeof(input), stdin) != NULL) {
                    size_t len = strlen(input);
                    if (len > 0 && input[len - 1] == '\n') {
                        input[len - 1] = '\0';
                        len--;
                    }
                    
                    if (strcmp(input, "quit") == 0) {
                        ctx.current_state = DONE;
                        break;
                    }
                    
                    if (validate_input(input, len)) {
                        if (len < sizeof(ctx.buffer)) {
                            strncpy(ctx.buffer, input, sizeof(ctx.buffer) - 1);
                            ctx.buffer[sizeof(ctx.buffer) - 1] = '\0';
                            ctx.buffer_len = len;
                            ctx.current_state = PROCESSING;
                        } else {
                            ctx.current_state = ERROR;
                        }
                    } else {
                        ctx.current_state = ERROR;
                    }
                } else {
                    ctx.current_state = ERROR;
                }
                break;
            }
            
            case PROCESSING:
                process_data(&ctx);
                ctx.data_count++;
                ctx.current_state = WRITING;
                break;
                
            case WRITING:
                printf("Processed data: %s\n", ctx.buffer);
                if (ctx.data_count >= ctx.max_data) {
                    printf("Maximum data count reached.\n");
                    ctx.current_state = DONE;
                } else {
                    ctx.current_state = IDLE;
                }
                break;
                
            case ERROR:
                printf("Error: Invalid input detected.\n");
                ctx.current_state = IDLE;
                break;
                
            case DONE:
                printf("Processing complete. Total items: %d\n", ctx.data_count);
                running = 0;
                break;
        }
    }
    
    return 0;
}