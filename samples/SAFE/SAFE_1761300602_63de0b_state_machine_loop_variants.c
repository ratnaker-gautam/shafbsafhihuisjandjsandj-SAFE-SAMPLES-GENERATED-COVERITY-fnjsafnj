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
    int max_items;
};

int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len >= 256) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (input[i] == 0) {
            return 0;
        }
        if (input[i] < 32 || input[i] > 126) {
            return 0;
        }
    }
    return 1;
}

void state_machine_loop(struct context* ctx) {
    int running = 1;
    
    while (running) {
        switch (ctx->current_state) {
            case IDLE:
                printf("Enter number of items to process (1-10): ");
                {
                    char input[32];
                    if (fgets(input, sizeof(input), stdin) != NULL) {
                        int num = atoi(input);
                        if (num >= 1 && num <= 10) {
                            ctx->max_items = num;
                            ctx->data_count = 0;
                            ctx->current_state = READING;
                        } else {
                            printf("Invalid input. Please enter 1-10.\n");
                        }
                    } else {
                        ctx->current_state = ERROR;
                    }
                }
                break;
                
            case READING:
                if (ctx->data_count < ctx->max_items) {
                    printf("Enter data string %d/%d: ", ctx->data_count + 1, ctx->max_items);
                    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) != NULL) {
                        size_t len = strlen(ctx->buffer);
                        if (len > 0 && ctx->buffer[len - 1] == '\n') {
                            ctx->buffer[len - 1] = 0;
                            len--;
                        }
                        if (validate_input(ctx->buffer, len)) {
                            ctx->buffer_len = len;
                            ctx->current_state = PROCESSING;
                        } else {
                            printf("Invalid input. Use only printable ASCII characters.\n");
                        }
                    } else {
                        ctx->current_state = ERROR;
                    }
                } else {
                    ctx->current_state = DONE;
                }
                break;
                
            case PROCESSING:
                {
                    int processed = 0;
                    for (size_t i = 0; i < ctx->buffer_len; i++) {
                        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
                            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
                            processed++;
                        }
                    }
                    printf("Processed %d characters to uppercase\n", processed);
                    ctx->current_state = WRITING;
                }
                break;
                
            case WRITING:
                printf("Output: %s\n", ctx->buffer);
                ctx->data_count++;
                ctx->current_state = READING;
                break;
                
            case ERROR:
                printf("An error occurred during processing.\n");
                running = 0;
                break;
                
            case DONE:
                printf("Processing complete. Processed %d items.\n", ctx->data_count);
                running = 0;
                break;
        }
    }
}

int main(void) {
    struct context ctx;
    ctx.current_state = IDLE;
    ctx.buffer_len = 0;
    ctx.data_count = 0;
    ctx.max_items = 0;
    
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    state_machine_loop(&ctx);
    
    return 0;
}