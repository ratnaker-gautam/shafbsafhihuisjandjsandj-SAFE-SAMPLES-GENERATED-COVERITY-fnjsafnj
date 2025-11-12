//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum state {
    START,
    READING_NUMBER,
    READING_WORD,
    FINISHED,
    ERROR
};

struct context {
    enum state current_state;
    int number_count;
    int word_count;
    char buffer[256];
    size_t buffer_pos;
};

void state_machine_init(struct context *ctx) {
    ctx->current_state = START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int is_valid_input_char(int c) {
    return c == EOF || c == '\n' || c == ' ' || c == '\t' || 
           (c >= '0' && c <= '9') || 
           (c >= 'a' && c <= 'z') || 
           (c >= 'A' && c <= 'Z');
}

void process_char(struct context *ctx, int c) {
    switch (ctx->current_state) {
        case START:
            if (c == EOF || c == '\n') {
                ctx->current_state = FINISHED;
            } else if (c == ' ' || c == '\t') {
                break;
            } else if (c >= '0' && c <= '9') {
                ctx->current_state = READING_NUMBER;
                if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buffer_pos++] = (char)c;
                } else {
                    ctx->current_state = ERROR;
                }
            } else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
                ctx->current_state = READING_WORD;
                if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buffer_pos++] = (char)c;
                } else {
                    ctx->current_state = ERROR;
                }
            } else {
                ctx->current_state = ERROR;
            }
            break;
            
        case READING_NUMBER:
            if (c == EOF || c == '\n' || c == ' ' || c == '\t') {
                if (ctx->buffer_pos > 0 && ctx->buffer_pos < sizeof(ctx->buffer)) {
                    ctx->number_count++;
                }
                ctx->buffer_pos = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                ctx->current_state = (c == EOF || c == '\n') ? FINISHED : START;
            } else if (c >= '0' && c <= '9') {
                if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buffer_pos++] = (char)c;
                } else {
                    ctx->current_state = ERROR;
                }
            } else {
                ctx->current_state = ERROR;
            }
            break;
            
        case READING_WORD:
            if (c == EOF || c == '\n' || c == ' ' || c == '\t') {
                if (ctx->buffer_pos > 0 && ctx->buffer_pos < sizeof(ctx->buffer)) {
                    ctx->word_count++;
                }
                ctx->buffer_pos = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                ctx->current_state = (c == EOF || c == '\n') ? FINISHED : START;
            } else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
                if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buffer_pos++] = (char)c;
                } else {
                    ctx->current_state = ERROR;
                }
            } else {
                ctx->current_state = ERROR;
            }
            break;
            
        case FINISHED:
        case ERROR:
            break;
    }
}

int main(void) {
    struct context ctx;
    state_machine_init(&ctx);
    
    printf("Enter text (numbers and words): ");
    fflush(stdout);
    
    int c;
    while (ctx.current_state != FINISHED && ctx.current_state != ERROR) {
        c = getchar();
        
        if (!is_valid_input_char(c)) {
            ctx.current_state = ERROR;
            break;
        }
        
        process_char(&ctx, c);
        
        if (ctx.buffer_pos >= sizeof(ctx.buffer)) {
            ctx.current_state = ERROR;
            break;
        }
    }
    
    if (ctx.current_state == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Numbers: %d\n", ctx.number_count);
    printf("Words: %d\n", ctx.word_count);
    
    return 0;
}