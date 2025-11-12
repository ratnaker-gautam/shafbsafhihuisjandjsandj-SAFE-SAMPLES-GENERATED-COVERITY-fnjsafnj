//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    INIT,
    READING,
    PROCESSING,
    OUTPUT,
    ERROR,
    DONE
};

struct Context {
    enum State current_state;
    char buffer[256];
    size_t buffer_pos;
    int word_count;
    int char_count;
    int line_count;
};

void init_context(struct Context *ctx) {
    ctx->current_state = INIT;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_pos = 0;
    ctx->word_count = 0;
    ctx->char_count = 0;
    ctx->line_count = 0;
}

int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

enum State handle_init(struct Context *ctx) {
    printf("Enter text (empty line to finish):\n");
    return READING;
}

enum State handle_reading(struct Context *ctx, const char *input) {
    if (input == NULL || strlen(input) == 0) {
        return DONE;
    }
    
    size_t input_len = strlen(input);
    if (input_len >= sizeof(ctx->buffer) - ctx->buffer_pos) {
        return ERROR;
    }
    
    memcpy(ctx->buffer + ctx->buffer_pos, input, input_len);
    ctx->buffer_pos += input_len;
    ctx->buffer[ctx->buffer_pos] = '\n';
    ctx->buffer_pos++;
    
    return PROCESSING;
}

enum State handle_processing(struct Context *ctx) {
    int in_word = 0;
    
    for (size_t i = 0; i < ctx->buffer_pos; i++) {
        char c = ctx->buffer[i];
        ctx->char_count++;
        
        if (c == '\n') {
            ctx->line_count++;
        }
        
        if (is_whitespace(c)) {
            if (in_word) {
                ctx->word_count++;
                in_word = 0;
            }
        } else {
            in_word = 1;
        }
    }
    
    if (in_word) {
        ctx->word_count++;
    }
    
    ctx->buffer_pos = 0;
    return OUTPUT;
}

enum State handle_output(struct Context *ctx) {
    printf("Statistics:\n");
    printf("Characters: %d\n", ctx->char_count);
    printf("Words: %d\n", ctx->word_count);
    printf("Lines: %d\n", ctx->line_count);
    return READING;
}

enum State handle_error(struct Context *ctx) {
    printf("Error: Buffer overflow\n");
    return DONE;
}

int main(void) {
    struct Context ctx;
    init_context(&ctx);
    
    char input[512];
    
    while (ctx.current_state != DONE && ctx.current_state != ERROR) {
        switch (ctx.current_state) {
            case INIT:
                ctx.current_state = handle_init(&ctx);
                break;
                
            case READING:
                if (fgets(input, sizeof(input), stdin) != NULL) {
                    size_t len = strlen(input);
                    if (len > 0 && input[len - 1] == '\n') {
                        input[len - 1] = '\0';
                    }
                    ctx.current_state = handle_reading(&ctx, input);
                } else {
                    ctx.current_state = DONE;
                }
                break;
                
            case PROCESSING:
                ctx.current_state = handle_processing(&ctx);
                break;
                
            case OUTPUT:
                ctx.current_state = handle_output(&ctx);
                break;
                
            case ERROR:
                ctx.current_state = handle_error(&ctx);
                break;
                
            case DONE:
                break;
        }
    }
    
    if (ctx.current_state == ERROR) {
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}