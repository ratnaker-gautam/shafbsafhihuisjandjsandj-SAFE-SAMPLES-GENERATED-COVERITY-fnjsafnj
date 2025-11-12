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

void process_char(struct Context *ctx, char c) {
    switch (ctx->current_state) {
        case START:
            if (isspace(c)) {
                break;
            } else if (isdigit(c)) {
                ctx->current_state = READING_NUMBER;
                ctx->buffer_index = 0;
                ctx->buffer[ctx->buffer_index++] = c;
            } else if (isalpha(c)) {
                ctx->current_state = READING_WORD;
                ctx->buffer_index = 0;
                ctx->buffer[ctx->buffer_index++] = c;
            } else {
                ctx->current_state = ERROR;
            }
            break;
            
        case READING_NUMBER:
            if (isdigit(c)) {
                if (ctx->buffer_index < 255) {
                    ctx->buffer[ctx->buffer_index++] = c;
                }
            } else if (isspace(c)) {
                ctx->buffer[ctx->buffer_index] = '\0';
                ctx->number_count++;
                ctx->current_state = START;
            } else {
                ctx->current_state = ERROR;
            }
            break;
            
        case READING_WORD:
            if (isalpha(c)) {
                if (ctx->buffer_index < 255) {
                    ctx->buffer[ctx->buffer_index++] = c;
                }
            } else if (isspace(c)) {
                ctx->buffer[ctx->buffer_index] = '\0';
                ctx->word_count++;
                ctx->current_state = START;
            } else {
                ctx->current_state = ERROR;
            }
            break;
            
        case ERROR:
            if (c == '\n') {
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
    
    printf("Enter text (max 1000 characters): ");
    fflush(stdout);
    
    char input[1002];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (len >= 1000) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int i = 0;
    while (input[i] != '\0' && ctx.current_state != DONE && ctx.current_state != ERROR) {
        process_char(&ctx, input[i]);
        i++;
    }
    
    if (ctx.current_state == READING_NUMBER || ctx.current_state == READING_WORD) {
        if (ctx.current_state == READING_NUMBER) {
            ctx.number_count++;
        } else {
            ctx.word_count++;
        }
    }
    
    if (ctx.current_state == ERROR) {
        printf("Invalid input format\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    
    return 0;
}