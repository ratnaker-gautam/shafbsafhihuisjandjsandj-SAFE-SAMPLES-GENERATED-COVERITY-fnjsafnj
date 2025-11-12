//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_OUTPUT,
    STATE_ERROR,
    STATE_DONE
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_pos;
    int word_count;
    int line_count;
};

void state_machine_init(struct context *ctx) {
    ctx->current_state = STATE_IDLE;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_pos = 0;
    ctx->word_count = 0;
    ctx->line_count = 0;
}

int is_word_boundary(char c) {
    return isspace(c) || c == '\0';
}

void process_buffer(struct context *ctx) {
    if (ctx->buffer_pos == 0) return;
    
    int in_word = 0;
    for (size_t i = 0; i < ctx->buffer_pos; i++) {
        if (ctx->buffer[i] == '\n') {
            ctx->line_count++;
        }
        
        if (!is_word_boundary(ctx->buffer[i])) {
            if (!in_word) {
                ctx->word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
    }
}

enum state handle_idle(struct context *ctx) {
    printf("Enter text (empty line to finish):\n");
    return STATE_READING;
}

enum state handle_reading(struct context *ctx, const char *input) {
    if (input == NULL) return STATE_ERROR;
    
    size_t input_len = strlen(input);
    if (input_len == 0) return STATE_PROCESSING;
    
    if (ctx->buffer_pos + input_len + 2 >= sizeof(ctx->buffer)) {
        return STATE_ERROR;
    }
    
    memcpy(ctx->buffer + ctx->buffer_pos, input, input_len);
    ctx->buffer_pos += input_len;
    ctx->buffer[ctx->buffer_pos++] = '\n';
    ctx->buffer[ctx->buffer_pos] = '\0';
    
    return STATE_READING;
}

enum state handle_processing(struct context *ctx) {
    process_buffer(ctx);
    return STATE_OUTPUT;
}

enum state handle_output(struct context *ctx) {
    printf("\nAnalysis Results:\n");
    printf("Lines: %d\n", ctx->line_count);
    printf("Words: %d\n", ctx->word_count);
    printf("Characters: %zu\n", ctx->buffer_pos);
    return STATE_DONE;
}

enum state handle_error(struct context *ctx) {
    printf("Error: Buffer overflow or invalid input\n");
    return STATE_DONE;
}

int main(void) {
    struct context ctx;
    state_machine_init(&ctx);
    char input[512];
    
    while (ctx.current_state != STATE_DONE && ctx.current_state != STATE_ERROR) {
        switch (ctx.current_state) {
            case STATE_IDLE:
                ctx.current_state = handle_idle(&ctx);
                break;
                
            case STATE_READING:
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    ctx.current_state = STATE_ERROR;
                    break;
                }
                
                size_t len = strlen(input);
                if (len > 0 && input[len - 1] == '\n') {
                    input[len - 1] = '\0';
                }
                
                ctx.current_state = handle_reading(&ctx, input);
                break;
                
            case STATE_PROCESSING:
                ctx.current_state = handle_processing(&ctx);
                break;
                
            case STATE_OUTPUT:
                ctx.current_state = handle_output(&ctx);
                break;
                
            case STATE_ERROR:
                ctx.current_state = handle_error(&ctx);
                break;
                
            default:
                ctx.current_state = STATE_ERROR;
                break;
        }
    }
    
    return ctx.current_state == STATE_ERROR ? 1 : 0;
}