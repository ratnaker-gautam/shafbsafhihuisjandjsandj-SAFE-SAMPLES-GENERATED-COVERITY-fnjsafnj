//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { IDLE, READING, PROCESSING, WRITING, ERROR, DONE };

struct Context {
    enum State current_state;
    char buffer[256];
    size_t buffer_pos;
    int word_count;
    int char_count;
};

void initialize_context(struct Context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = IDLE;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_pos = 0;
    ctx->word_count = 0;
    ctx->char_count = 0;
}

enum State handle_idle(struct Context *ctx, int c) {
    if (c == EOF) return DONE;
    if (isspace(c)) return IDLE;
    if (isprint(c)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = (char)c;
            ctx->char_count++;
        }
        return READING;
    }
    return ERROR;
}

enum State handle_reading(struct Context *ctx, int c) {
    if (c == EOF) return PROCESSING;
    if (isspace(c)) return PROCESSING;
    if (isprint(c)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = (char)c;
            ctx->char_count++;
        }
        return READING;
    }
    return ERROR;
}

enum State handle_processing(struct Context *ctx) {
    if (ctx->buffer_pos > 0) {
        ctx->word_count++;
        ctx->buffer_pos = 0;
    }
    return WRITING;
}

enum State handle_writing(struct Context *ctx) {
    printf("Word %d: %s\n", ctx->word_count, ctx->buffer);
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    return IDLE;
}

enum State handle_error(struct Context *ctx) {
    fprintf(stderr, "Error: Invalid input character encountered\n");
    return DONE;
}

int main(void) {
    struct Context ctx;
    initialize_context(&ctx);
    
    int input_char;
    int continue_processing = 1;
    
    while (continue_processing) {
        switch (ctx.current_state) {
            case IDLE:
                input_char = getchar();
                if (input_char == EOF) {
                    ctx.current_state = DONE;
                    break;
                }
                ctx.current_state = handle_idle(&ctx, input_char);
                break;
                
            case READING:
                input_char = getchar();
                if (input_char == EOF) {
                    ctx.current_state = PROCESSING;
                    break;
                }
                ctx.current_state = handle_reading(&ctx, input_char);
                break;
                
            case PROCESSING:
                ctx.current_state = handle_processing(&ctx);
                break;
                
            case WRITING:
                ctx.current_state = handle_writing(&ctx);
                break;
                
            case ERROR:
                ctx.current_state = handle_error(&ctx);
                break;
                
            case DONE:
                continue_processing = 0;
                break;
        }
    }
    
    if (ctx.word_count > 0) {
        printf("Processed %d words with %d characters total\n", 
               ctx.word_count, ctx.char_count);
    } else {
        printf("No words processed\n");
    }
    
    return ctx.current_state == ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}