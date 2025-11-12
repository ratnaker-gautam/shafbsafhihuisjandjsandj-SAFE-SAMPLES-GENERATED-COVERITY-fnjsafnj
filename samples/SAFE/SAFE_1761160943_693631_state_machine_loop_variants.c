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
    char buffer[64];
    int buf_pos;
};

void process_char(struct Context *ctx, char c) {
    switch (ctx->current) {
        case START:
            if (isdigit(c)) {
                ctx->buffer[0] = c;
                ctx->buf_pos = 1;
                ctx->current = READING_NUM;
            } else if (isalpha(c)) {
                ctx->buffer[0] = c;
                ctx->buf_pos = 1;
                ctx->current = READING_WORD;
            } else if (c == '\n') {
                ctx->current = DONE;
            } else if (c != ' ' && c != '\t') {
                ctx->current = ERROR;
            }
            break;
            
        case READING_NUM:
            if (isdigit(c)) {
                if (ctx->buf_pos < 63) {
                    ctx->buffer[ctx->buf_pos++] = c;
                } else {
                    ctx->current = ERROR;
                }
            } else if (c == ' ' || c == '\t' || c == '\n') {
                ctx->buffer[ctx->buf_pos] = '\0';
                ctx->num_count++;
                printf("Number: %s\n", ctx->buffer);
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
                if (ctx->buf_pos < 63) {
                    ctx->buffer[ctx->buf_pos++] = c;
                } else {
                    ctx->current = ERROR;
                }
            } else if (c == ' ' || c == '\t' || c == '\n') {
                ctx->buffer[ctx->buf_pos] = '\0';
                ctx->word_count++;
                printf("Word: %s\n", ctx->buffer);
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
            break;
            
        case DONE:
            break;
    }
}

int main(void) {
    struct Context ctx = {START, 0, 0, {0}, 0};
    char input[1024];
    
    printf("Enter text (max 1023 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    int i = 0;
    while (i < len && ctx.current != ERROR && ctx.current != DONE) {
        process_char(&ctx, input[i]);
        i++;
    }
    
    if (ctx.current == READING_NUM || ctx.current == READING_WORD) {
        ctx.buffer[ctx.buf_pos] = '\0';
        if (ctx.current == READING_NUM) {
            ctx.num_count++;
            printf("Number: %s\n", ctx.buffer);
        } else {
            ctx.word_count++;
            printf("Word: %s\n", ctx.buffer);
        }
    }
    
    if (ctx.current == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Summary: %d numbers, %d words\n", ctx.num_count, ctx.word_count);
    return 0;
}