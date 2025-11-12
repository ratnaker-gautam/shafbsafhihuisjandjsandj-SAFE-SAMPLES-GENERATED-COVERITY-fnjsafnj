//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    INIT,
    READING,
    PROCESSING,
    OUTPUT,
    ERROR,
    DONE
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_pos;
    int word_count;
    int char_count;
    int line_count;
};

void state_init(struct context *ctx) {
    memset(ctx, 0, sizeof(*ctx));
    ctx->current_state = INIT;
}

int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= 256) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0' && i != len - 1) {
            return 0;
        }
    }
    return 1;
}

void state_transition(struct context *ctx, enum state next_state) {
    if (next_state >= INIT && next_state <= DONE) {
        ctx->current_state = next_state;
    }
}

int process_buffer(struct context *ctx) {
    if (ctx->buffer_pos == 0) {
        return 0;
    }
    
    int in_word = 0;
    for (size_t i = 0; i < ctx->buffer_pos; i++) {
        ctx->char_count++;
        
        if (ctx->buffer[i] == '\n') {
            ctx->line_count++;
        }
        
        if (isalpha((unsigned char)ctx->buffer[i])) {
            if (!in_word) {
                ctx->word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
    }
    
    return 1;
}

int main(void) {
    struct context ctx;
    state_init(&ctx);
    
    printf("Enter text (empty line to finish):\n");
    
    while (ctx.current_state != DONE && ctx.current_state != ERROR) {
        switch (ctx.current_state) {
            case INIT:
                state_transition(&ctx, READING);
                break;
                
            case READING: {
                char input[256];
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    state_transition(&ctx, ERROR);
                    break;
                }
                
                size_t len = strlen(input);
                if (!validate_input(input, len)) {
                    state_transition(&ctx, ERROR);
                    break;
                }
                
                if (len == 1 && input[0] == '\n') {
                    state_transition(&ctx, PROCESSING);
                    break;
                }
                
                if (ctx.buffer_pos + len >= sizeof(ctx.buffer)) {
                    state_transition(&ctx, ERROR);
                    break;
                }
                
                memcpy(ctx.buffer + ctx.buffer_pos, input, len);
                ctx.buffer_pos += len;
                break;
            }
            
            case PROCESSING:
                if (!process_buffer(&ctx)) {
                    state_transition(&ctx, ERROR);
                    break;
                }
                state_transition(&ctx, OUTPUT);
                break;
                
            case OUTPUT:
                printf("\nStatistics:\n");
                printf("Characters: %d\n", ctx.char_count);
                printf("Words: %d\n", ctx.word_count);
                printf("Lines: %d\n", ctx.line_count);
                state_transition(&ctx, DONE);
                break;
                
            case ERROR:
                printf("Error: Invalid input or buffer overflow\n");
                return 1;
                
            case DONE:
                break;
        }
    }
    
    return 0;
}