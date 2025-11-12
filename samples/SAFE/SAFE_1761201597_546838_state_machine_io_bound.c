//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
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

enum State handle_init(struct Context *ctx, char input) {
    if (input == EOF) {
        return DONE;
    }
    if (is_whitespace(input)) {
        return INIT;
    }
    ctx->buffer[ctx->buffer_pos++] = input;
    ctx->char_count++;
    return READING;
}

enum State handle_reading(struct Context *ctx, char input) {
    if (input == EOF) {
        return PROCESSING;
    }
    if (ctx->buffer_pos >= sizeof(ctx->buffer) - 1) {
        return ERROR;
    }
    ctx->buffer[ctx->buffer_pos++] = input;
    ctx->char_count++;
    if (is_whitespace(input)) {
        return PROCESSING;
    }
    return READING;
}

enum State handle_processing(struct Context *ctx) {
    if (ctx->buffer_pos > 0) {
        ctx->buffer[ctx->buffer_pos] = '\0';
        int in_word = 0;
        for (size_t i = 0; i < ctx->buffer_pos; i++) {
            if (!is_whitespace(ctx->buffer[i])) {
                if (!in_word) {
                    ctx->word_count++;
                    in_word = 1;
                }
            } else {
                in_word = 0;
                if (ctx->buffer[i] == '\n') {
                    ctx->line_count++;
                }
            }
        }
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        ctx->buffer_pos = 0;
    }
    return OUTPUT;
}

enum State handle_output(struct Context *ctx) {
    printf("Words: %d, Characters: %d, Lines: %d\n", 
           ctx->word_count, ctx->char_count, ctx->line_count);
    return INIT;
}

enum State handle_error(struct Context *ctx) {
    printf("Error: Buffer overflow\n");
    return DONE;
}

void run_state_machine(struct Context *ctx) {
    int input_char;
    
    while (ctx->current_state != DONE && ctx->current_state != ERROR) {
        switch (ctx->current_state) {
            case INIT:
                input_char = getchar();
                if (input_char == EOF) {
                    ctx->current_state = DONE;
                } else {
                    ctx->current_state = handle_init(ctx, (char)input_char);
                }
                break;
                
            case READING:
                input_char = getchar();
                if (input_char == EOF) {
                    ctx->current_state = handle_reading(ctx, EOF);
                } else {
                    ctx->current_state = handle_reading(ctx, (char)input_char);
                }
                break;
                
            case PROCESSING:
                ctx->current_state = handle_processing(ctx);
                break;
                
            case OUTPUT:
                ctx->current_state = handle_output(ctx);
                break;
                
            case ERROR:
                ctx->current_state = handle_error(ctx);
                break;
                
            case DONE:
                break;
        }
    }
    
    if (ctx->current_state == PROCESSING) {
        ctx->current_state = handle_processing(ctx);
    }
    if (ctx->current_state == OUTPUT) {
        ctx->current_state = handle_output(ctx);
    }
}

int main(void) {
    struct Context ctx;
    init_context(&ctx);
    
    printf("Enter text (Ctrl+D to end):\n");
    run_state_machine(&ctx);
    
    return 0;
}