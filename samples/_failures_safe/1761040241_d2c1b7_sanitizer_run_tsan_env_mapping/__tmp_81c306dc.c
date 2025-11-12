//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: state_machine
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
    if (input == NULL || len == 0) return 0;
    if (len > 255) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') return 1;
        if (!isprint(input[i]) && input[i] != '\n' && input[i] != '\t' && input[i] != '\r') {
            return 0;
        }
    }
    return 1;
}

enum state handle_init(struct context *ctx) {
    printf("Enter text (empty line to finish):\n");
    return READING;
}

enum state handle_reading(struct context *ctx, const char *input) {
    size_t len = strlen(input);
    if (!validate_input(input, len)) {
        return ERROR;
    }
    
    if (len == 0) {
        return PROCESSING;
    }
    
    if (ctx->buffer_pos + len >= sizeof(ctx->buffer) - 1) {
        return ERROR;
    }
    
    memcpy(ctx->buffer + ctx->buffer_pos, input, len);
    ctx->buffer_pos += len;
    ctx->buffer[ctx->buffer_pos] = '\n';
    ctx->buffer_pos++;
    ctx->line_count++;
    
    return READING;
}

enum state handle_processing(struct context *ctx) {
    if (ctx->buffer_pos == 0) {
        return DONE;
    }
    
    ctx->char_count = ctx->buffer_pos;
    ctx->word_count = 0;
    
    int in_word = 0;
    for (size_t i = 0; i < ctx->buffer_pos; i++) {
        if (isspace(ctx->buffer[i])) {
            in_word = 0;
            if (ctx->buffer[i] == '\n') {
                ctx->line_count++;
            }
        } else {
            if (!in_word) {
                ctx->word_count++;
                in_word = 1;
            }
        }
    }
    
    return OUTPUT;
}

enum state handle_output(struct context *ctx) {
    printf("\nText Analysis Results:\n");
    printf("Characters: %d\n", ctx->char_count);
    printf("Words: %d\n", ctx->word_count);
    printf("Lines: %d\n", ctx->line_count);
    return DONE;
}

enum state handle_error(struct context *ctx) {
    printf("Error: Invalid input detected\n");
    return DONE;
}

int main(void) {
    struct context ctx;
    state_init(&ctx);
    
    char input[256];
    
    while (ctx.current_state != DONE && ctx.current_state != ERROR) {
        switch (ctx.current_state) {
            case INIT:
                ctx.current_state = handle_init(&ctx);
                break;
                
            case READING:
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    ctx.current_state = ERROR;
                    break;
                }
                
                size_t len = strlen(input);
                if (len > 0 && input[len - 1] == '\n') {
                    input[len - 1] = '\0';
                    len--;
                }
                
                ctx.current_state = handle_reading(&ctx, input);
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
        handle_error(&ctx);
    }
    
    return 0;
}