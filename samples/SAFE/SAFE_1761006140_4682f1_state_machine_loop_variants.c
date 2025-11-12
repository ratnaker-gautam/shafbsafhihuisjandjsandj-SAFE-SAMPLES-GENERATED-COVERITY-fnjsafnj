//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: state_machine
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
            } else if (c == '\n') {
                ctx->current_state = DONE;
            } else if (c != ' ' && c != '\t') {
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
            } else if (c == ' ' || c == '\t' || c == '\n') {
                ctx->buffer[ctx->buffer_index] = '\0';
                ctx->number_count++;
                if (c == '\n') {
                    ctx->current_state = DONE;
                } else {
                    ctx->current_state = START;
                }
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
            } else if (c == ' ' || c == '\t' || c == '\n') {
                ctx->buffer[ctx->buffer_index] = '\0';
                ctx->word_count++;
                if (c == '\n') {
                    ctx->current_state = DONE;
                } else {
                    ctx->current_state = START;
                }
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
    char input[1024];
    
    printf("Enter text (max 1023 characters): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len == 0 || input_len >= sizeof(input)) {
        fprintf(stderr, "Invalid input length\n");
        return 1;
    }
    
    ctx.current_state = START;
    ctx.number_count = 0;
    ctx.word_count = 0;
    ctx.buffer_index = 0;
    
    for (size_t i = 0; i < input_len; i++) {
        process_character(&ctx, input[i]);
        if (ctx.current_state == DONE) {
            break;
        }
    }
    
    if (ctx.current_state == READING_NUMBER) {
        ctx.buffer[ctx.buffer_index] = '\0';
        ctx.number_count++;
    } else if (ctx.current_state == READING_WORD) {
        ctx.buffer[ctx.buffer_index] = '\0';
        ctx.word_count++;
    }
    
    if (ctx.current_state == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    
    return 0;
}