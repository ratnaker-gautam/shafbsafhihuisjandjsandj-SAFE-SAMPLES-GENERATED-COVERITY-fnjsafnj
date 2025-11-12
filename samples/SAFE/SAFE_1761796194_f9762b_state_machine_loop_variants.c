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
    int buffer_pos;
};

void process_character(struct Context *ctx, char c) {
    switch (ctx->current_state) {
        case START:
            if (isdigit(c)) {
                ctx->current_state = READING_NUMBER;
                ctx->buffer[0] = c;
                ctx->buffer_pos = 1;
            } else if (isalpha(c)) {
                ctx->current_state = READING_WORD;
                ctx->buffer[0] = c;
                ctx->buffer_pos = 1;
            } else if (c == '\n' || c == '\0') {
                ctx->current_state = DONE;
            } else if (!isspace(c)) {
                ctx->current_state = ERROR;
            }
            break;
            
        case READING_NUMBER:
            if (isdigit(c)) {
                if (ctx->buffer_pos < 255) {
                    ctx->buffer[ctx->buffer_pos++] = c;
                } else {
                    ctx->current_state = ERROR;
                }
            } else if (isspace(c) || c == '\0') {
                ctx->number_count++;
                ctx->buffer[ctx->buffer_pos] = '\0';
                ctx->current_state = (c == '\n' || c == '\0') ? DONE : START;
            } else {
                ctx->current_state = ERROR;
            }
            break;
            
        case READING_WORD:
            if (isalpha(c)) {
                if (ctx->buffer_pos < 255) {
                    ctx->buffer[ctx->buffer_pos++] = c;
                } else {
                    ctx->current_state = ERROR;
                }
            } else if (isspace(c) || c == '\0') {
                ctx->word_count++;
                ctx->buffer[ctx->buffer_pos] = '\0';
                ctx->current_state = (c == '\n' || c == '\0') ? DONE : START;
            } else {
                ctx->current_state = ERROR;
            }
            break;
            
        case ERROR:
            if (c == '\n' || c == '\0') {
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
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    ctx.current_state = START;
    ctx.number_count = 0;
    ctx.word_count = 0;
    ctx.buffer_pos = 0;
    
    size_t i = 0;
    while (input[i] != '\0' && ctx.current_state != DONE && ctx.current_state != ERROR) {
        process_character(&ctx, input[i]);
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