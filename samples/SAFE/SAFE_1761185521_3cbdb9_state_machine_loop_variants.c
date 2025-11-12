//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUMBER, READING_WORD, ERROR, DONE };

struct Context {
    enum State current_state;
    int number_count;
    int word_count;
    char buffer[256];
    int buffer_index;
};

void process_character(struct Context *ctx, char c) {
    switch (ctx->current_state) {
        case START:
            if (isdigit(c)) {
                ctx->current_state = READING_NUMBER;
                ctx->buffer[0] = c;
                ctx->buffer_index = 1;
            } else if (isalpha(c)) {
                ctx->current_state = READING_WORD;
                ctx->buffer[0] = c;
                ctx->buffer_index = 1;
            } else if (c == '\n' || c == EOF) {
                ctx->current_state = DONE;
            } else if (!isspace(c)) {
                ctx->current_state = ERROR;
            }
            break;
            
        case READING_NUMBER:
            if (isdigit(c)) {
                if (ctx->buffer_index < 255) {
                    ctx->buffer[ctx->buffer_index++] = c;
                } else {
                    ctx->current_state = ERROR;
                }
            } else if (isspace(c) || c == EOF) {
                ctx->buffer[ctx->buffer_index] = '\0';
                ctx->number_count++;
                ctx->current_state = (c == '\n' || c == EOF) ? DONE : START;
            } else {
                ctx->current_state = ERROR;
            }
            break;
            
        case READING_WORD:
            if (isalpha(c)) {
                if (ctx->buffer_index < 255) {
                    ctx->buffer[ctx->buffer_index++] = c;
                } else {
                    ctx->current_state = ERROR;
                }
            } else if (isspace(c) || c == EOF) {
                ctx->buffer[ctx->buffer_index] = '\0';
                ctx->word_count++;
                ctx->current_state = (c == '\n' || c == EOF) ? DONE : START;
            } else {
                ctx->current_state = ERROR;
            }
            break;
            
        case ERROR:
            if (c == '\n' || c == EOF) {
                ctx->current_state = DONE;
            }
            break;
            
        case DONE:
            break;
    }
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.number_count = 0;
    ctx.word_count = 0;
    ctx.buffer_index = 0;
    
    printf("Enter text (numbers and words, end with newline): ");
    
    int ch;
    while ((ch = getchar()) != EOF) {
        process_character(&ctx, (char)ch);
        if (ctx.current_state == DONE) {
            break;
        }
        if (ctx.current_state == ERROR) {
            printf("Invalid input format\n");
            return 1;
        }
    }
    
    if (ctx.current_state != DONE && ctx.current_state != START) {
        printf("Unexpected end of input\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    
    return 0;
}