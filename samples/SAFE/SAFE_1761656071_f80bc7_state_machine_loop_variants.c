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
        if (input[i] < 32 || input[i] > 126) {
            return 0;
        }
    }
    return 1;
}

void process_buffer(struct context* ctx) {
    if (ctx == NULL || ctx->buffer_pos == 0) {
        ctx->error_code = 2;
        ctx->current_state = ERROR;
        return;
    }
    
    for (size_t i = 0; i < ctx->buffer_pos; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    ctx->processed_count++;
}

int main(void) {
    struct context ctx;
    ctx.current_state = IDLE;
    ctx.buffer_pos = 0;
    ctx.processed_count = 0;
    ctx.error_code = 0;
    
    char input_line[512];
    
    while (ctx.current_state != DONE && ctx.current_state != ERROR) {
        switch (ctx.current_state) {
            case IDLE:
                printf("Enter text (or 'quit' to exit): ");
                if (fgets(input_line, sizeof(input_line), stdin) != NULL) {
                    size_t len = strlen(input_line);
                    if (len > 0 && input_line[len - 1] == '\n') {
                        input_line[len - 1] = '\0';
                        len--;
                    }
                    
                    if (strcmp(input_line, "quit") == 0) {
                        ctx.current_state = DONE;
                    } else if (validate_input(input_line, len)) {
                        if (len < sizeof(ctx.buffer)) {
                            memcpy(ctx.buffer, input_line, len);
                            ctx.buffer_pos = len;
                            ctx.buffer[ctx.buffer_pos] = '\0';
                            ctx.current_state = PROCESSING;
                        } else {
                            ctx.error_code = 3;
                            ctx.current_state = ERROR;
                        }
                    } else {
                        ctx.error_code = 1;
                        ctx.current_state = ERROR;
                    }
                } else {
                    ctx.error_code = 4;
                    ctx.current_state = ERROR;
                }
                break;
                
            case PROCESSING:
                process_buffer(&ctx);
                if (ctx.current_state == PROCESSING) {
                    ctx.current_state = WRITING;
                }
                break;
                
            case WRITING:
                printf("Processed: %s\n", ctx.buffer);
                printf("Total processed: %d\n", ctx.processed_count);
                ctx.current_state = IDLE;
                break;
                
            case ERROR:
                printf("Error %d occurred\n", ctx.error_code);
                ctx.current_state = DONE;
                break;
                
            case DONE:
                break;
                
            default:
                ctx.error_code = 5;
                ctx.current_state = ERROR;
                break;
        }
    }
    
    printf("Program completed. Processed %d items.\n", ctx.processed_count);
    return ctx.error_code;
}