//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUM, READING_WORD, ERROR, DONE };

struct Context {
    enum State current;
    int num_count;
    int word_count;
    char buffer[256];
    int buf_pos;
};

void process_char(struct Context *ctx, char c) {
    switch (ctx->current) {
        case START:
            if (isdigit(c)) {
                ctx->current = READING_NUM;
                ctx->buffer[0] = c;
                ctx->buf_pos = 1;
            } else if (isalpha(c)) {
                ctx->current = READING_WORD;
                ctx->buffer[0] = c;
                ctx->buf_pos = 1;
            } else if (c == '\n') {
                ctx->current = DONE;
            } else if (c != ' ' && c != '\t') {
                ctx->current = ERROR;
            }
            break;
            
        case READING_NUM:
            if (isdigit(c)) {
                if (ctx->buf_pos < 255) {
                    ctx->buffer[ctx->buf_pos++] = c;
                } else {
                    ctx->current = ERROR;
                }
            } else if (c == ' ' || c == '\t' || c == '\n') {
                ctx->buffer[ctx->buf_pos] = '\0';
                ctx->num_count++;
                if (c == '\n') {
                    ctx->current = DONE;
                } else {
                    ctx->current = START;
                }
            } else {
                ctx->current = ERROR;
            }
            break;
            
        case READING_WORD:
            if (isalpha(c)) {
                if (ctx->buf_pos < 255) {
                    ctx->buffer[ctx->buf_pos++] = c;
                } else {
                    ctx->current = ERROR;
                }
            } else if (c == ' ' || c == '\t' || c == '\n') {
                ctx->buffer[ctx->buf_pos] = '\0';
                ctx->word_count++;
                if (c == '\n') {
                    ctx->current = DONE;
                } else {
                    ctx->current = START;
                }
            } else {
                ctx->current = ERROR;
            }
            break;
            
        case ERROR:
            if (c == '\n') {
                ctx->current = DONE;
            }
            break;
            
        case DONE:
            break;
    }
}

int main(void) {
    struct Context ctx;
    char input[1024];
    
    printf("Enter text (max 1023 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    ctx.current = START;
    ctx.num_count = 0;
    ctx.word_count = 0;
    ctx.buf_pos = 0;
    
    for (int i = 0; input[i] != '\0'; i++) {
        process_char(&ctx, input[i]);
        if (ctx.current == ERROR) {
            break;
        }
        if (ctx.current == DONE) {
            break;
        }
    }
    
    if (ctx.current == ERROR) {
        printf("Invalid input format\n");
        return 1;
    }
    
    if (ctx.current == READING_NUM) {
        ctx.num_count++;
    } else if (ctx.current == READING_WORD) {
        ctx.word_count++;
    }
    
    printf("Numbers: %d\n", ctx.num_count);
    printf("Words: %d\n", ctx.word_count);
    
    return 0;
}