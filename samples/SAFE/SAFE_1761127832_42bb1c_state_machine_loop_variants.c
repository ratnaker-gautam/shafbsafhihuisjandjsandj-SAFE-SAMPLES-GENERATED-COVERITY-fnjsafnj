//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
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

int state_transition(struct context *ctx, int input_char) {
    switch(ctx->current_state) {
        case INIT:
            if (input_char == EOF) {
                ctx->current_state = DONE;
            } else if (input_char == '\n') {
                ctx->line_count++;
                ctx->char_count++;
            } else if (isspace(input_char)) {
                ctx->char_count++;
            } else if (isprint(input_char)) {
                ctx->buffer[ctx->buffer_pos++] = (char)input_char;
                ctx->char_count++;
                ctx->current_state = READING;
            } else {
                ctx->current_state = ERROR;
            }
            break;
            
        case READING:
            if (input_char == EOF) {
                if (ctx->buffer_pos > 0) {
                    ctx->word_count++;
                    ctx->current_state = OUTPUT;
                } else {
                    ctx->current_state = DONE;
                }
            } else if (isspace(input_char)) {
                if (ctx->buffer_pos > 0) {
                    ctx->word_count++;
                    ctx->current_state = OUTPUT;
                }
                if (input_char == '\n') {
                    ctx->line_count++;
                }
                ctx->char_count++;
            } else if (isprint(input_char)) {
                if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buffer_pos++] = (char)input_char;
                    ctx->char_count++;
                } else {
                    ctx->current_state = ERROR;
                }
            } else {
                ctx->current_state = ERROR;
            }
            break;
            
        case OUTPUT:
            if (ctx->buffer_pos > 0) {
                ctx->buffer[ctx->buffer_pos] = '\0';
                printf("Word: %s\n", ctx->buffer);
                ctx->buffer_pos = 0;
            }
            if (input_char == EOF) {
                ctx->current_state = DONE;
            } else {
                ctx->current_state = INIT;
            }
            break;
            
        case PROCESSING:
            ctx->current_state = OUTPUT;
            break;
            
        case ERROR:
            fprintf(stderr, "Error: Invalid input character\n");
            ctx->current_state = DONE;
            break;
            
        case DONE:
            break;
    }
    
    return ctx->current_state != DONE;
}

int main(void) {
    struct context ctx;
    state_init(&ctx);
    
    int continue_processing = 1;
    while (continue_processing) {
        int c = getchar();
        continue_processing = state_transition(&ctx, c);
        
        if (ctx.current_state == OUTPUT) {
            continue_processing = state_transition(&ctx, c);
        }
    }
    
    if (ctx.current_state == DONE) {
        printf("Summary: %d words, %d characters, %d lines\n", 
               ctx.word_count, ctx.char_count, ctx.line_count);
    }
    
    return ctx.current_state == ERROR ? 1 : 0;
}