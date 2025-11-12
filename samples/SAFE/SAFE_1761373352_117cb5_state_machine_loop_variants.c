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

int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len >= 256) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (input[i] == 0) {
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
                printf("Enter data (max 255 chars): ");
                if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) != NULL) {
                    size_t len = strlen(ctx->buffer);
                    if (len > 0 && ctx->buffer[len-1] == '\n') {
                        ctx->buffer[len-1] = '\0';
                        len--;
                    }
                    if (validate_input(ctx->buffer, len)) {
                        ctx->buffer_pos = len;
                        ctx->current_state = READING;
                    } else {
                        ctx->error_code = 1;
                        ctx->current_state = ERROR;
                    }
                } else {
                    ctx->error_code = 2;
                    ctx->current_state = ERROR;
                }
                break;
                
            case READING:
                if (ctx->buffer_pos > 0) {
                    ctx->current_state = PROCESSING;
                } else {
                    ctx->error_code = 3;
                    ctx->current_state = ERROR;
                }
                break;
                
            case PROCESSING:
                for (size_t i = 0; i < ctx->buffer_pos; i++) {
                    if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
                        ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
                    }
                }
                ctx->processed_count++;
                ctx->current_state = WRITING;
                break;
                
            case WRITING:
                printf("Processed: %s\n", ctx->buffer);
                printf("Processed %d items so far\n", ctx->processed_count);
                printf("Continue? (y/n): ");
                char response[16];
                if (fgets(response, sizeof(response), stdin) != NULL) {
                    if (response[0] == 'y' || response[0] == 'Y') {
                        ctx->current_state = IDLE;
                    } else {
                        ctx->current_state = DONE;
                    }
                } else {
                    ctx->current_state = DONE;
                }
                break;
                
            case ERROR:
                printf("Error %d occurred\n", ctx->error_code);
                ctx->current_state = DONE;
                break;
                
            case DONE:
                running = 0;
                break;
        }
    }
}

int main(void) {
    struct context ctx;
    ctx.current_state = IDLE;
    ctx.buffer_pos = 0;
    ctx.processed_count = 0;
    ctx.error_code = 0;
    
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    state_machine_loop(&ctx);
    
    printf("Final count: %d\n", ctx.processed_count);
    return 0;
}