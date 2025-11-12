//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_DONE
};

struct Context {
    enum State current_state;
    int number_count;
    int word_count;
    char buffer[256];
    int buffer_pos;
};

void context_init(struct Context *ctx) {
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int is_valid_char(char c) {
    return isalnum(c) || c == ' ' || c == '\n' || c == '\t' || c == '\0';
}

int main(void) {
    struct Context ctx;
    context_init(&ctx);
    
    printf("Enter text (max 1024 chars, Ctrl+D to end): ");
    
    int ch;
    while ((ch = getchar()) != EOF && ctx.current_state != STATE_ERROR && ctx.current_state != STATE_DONE) {
        if (!is_valid_char(ch)) {
            ctx.current_state = STATE_ERROR;
            break;
        }
        
        switch (ctx.current_state) {
            case STATE_START:
                if (isdigit(ch)) {
                    ctx.current_state = STATE_READING_NUMBER;
                    ctx.buffer[ctx.buffer_pos++] = (char)ch;
                } else if (isalpha(ch)) {
                    ctx.current_state = STATE_READING_WORD;
                    ctx.buffer[ctx.buffer_pos++] = (char)ch;
                } else if (ch == '\n' || ch == ' ' || ch == '\t') {
                    continue;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (isdigit(ch)) {
                    if (ctx.buffer_pos < 255) {
                        ctx.buffer[ctx.buffer_pos++] = (char)ch;
                    } else {
                        ctx.current_state = STATE_ERROR;
                    }
                } else if (ch == ' ' || ch == '\n' || ch == '\t') {
                    ctx.buffer[ctx.buffer_pos] = '\0';
                    ctx.number_count++;
                    ctx.buffer_pos = 0;
                    ctx.current_state = STATE_START;
                } else {
                    ctx.current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (isalpha(ch)) {
                    if (ctx.buffer_pos < 255) {
                        ctx.buffer[ctx.buffer_pos++] = (char)ch;
                    } else {
                        ctx.current_state = STATE_ERROR;
                    }
                } else if (ch == ' ' || ch == '\n' || ch == '\t') {
                    ctx.buffer[ctx.buffer_pos] = '\0';
                    ctx.word_count++;
                    ctx.buffer_pos = 0;
                    ctx.current_state = STATE_START;
                } else {
                    ctx.current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
            case STATE_DONE:
                break;
        }
        
        if (ctx.number_count + ctx.word_count > 1000) {
            ctx.current_state = STATE_DONE;
        }
    }
    
    if (ctx.current_state == STATE_READING_NUMBER || ctx.current_state == STATE_READING_WORD) {
        if (ctx.buffer_pos > 0 && ctx.buffer_pos < 256) {
            ctx.buffer[ctx.buffer_pos] = '\0';
            if (ctx.current_state == STATE_READING_NUMBER) {
                ctx.number_count++;
            } else {
                ctx.word_count++;
            }
        }
        ctx.current_state = STATE_DONE;
    }
    
    if (ctx.current_state == STATE_ERROR) {
        printf("Error: Invalid input detected\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    
    return 0;
}