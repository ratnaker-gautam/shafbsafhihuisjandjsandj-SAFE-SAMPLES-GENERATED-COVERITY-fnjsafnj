//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum State {
    INIT,
    READING_NUMBER,
    READING_WORD,
    ERROR,
    DONE
};

struct Context {
    enum State current_state;
    int number_count;
    int word_count;
    char buffer[256];
    size_t buffer_pos;
};

void state_machine_init(struct Context *ctx) {
    ctx->current_state = INIT;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\n' || c == '\t';
}

void process_char(struct Context *ctx, char c) {
    if (!is_valid_char(c)) {
        ctx->current_state = ERROR;
        return;
    }

    switch (ctx->current_state) {
        case INIT:
            if (isdigit((unsigned char)c)) {
                ctx->current_state = READING_NUMBER;
                ctx->buffer[ctx->buffer_pos++] = c;
            } else if (isalpha((unsigned char)c)) {
                ctx->current_state = READING_WORD;
                ctx->buffer[ctx->buffer_pos++] = c;
            } else if (c == ' ' || c == '\n' || c == '\t') {
                break;
            }
            break;

        case READING_NUMBER:
            if (isdigit((unsigned char)c)) {
                if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buffer_pos++] = c;
                } else {
                    ctx->current_state = ERROR;
                }
            } else if (c == ' ' || c == '\n' || c == '\t') {
                ctx->number_count++;
                ctx->buffer_pos = 0;
                ctx->current_state = INIT;
            } else {
                ctx->current_state = ERROR;
            }
            break;

        case READING_WORD:
            if (isalpha((unsigned char)c)) {
                if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buffer_pos++] = c;
                } else {
                    ctx->current_state = ERROR;
                }
            } else if (c == ' ' || c == '\n' || c == '\t') {
                ctx->word_count++;
                ctx->buffer_pos = 0;
                ctx->current_state = INIT;
            } else {
                ctx->current_state = ERROR;
            }
            break;

        case ERROR:
        case DONE:
            break;
    }
}

int main(void) {
    struct Context ctx;
    state_machine_init(&ctx);
    
    printf("Enter text (numbers and words separated by spaces): ");
    fflush(stdout);
    
    char input[1024];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    size_t i = 0;
    while (i < len && ctx.current_state != ERROR && ctx.current_state != DONE) {
        process_char(&ctx, input[i]);
        i++;
    }
    
    if (ctx.current_state == READING_NUMBER) {
        ctx.number_count++;
    } else if (ctx.current_state == READING_WORD) {
        ctx.word_count++;
    }
    
    if (ctx.current_state == ERROR) {
        printf("Invalid input format\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    
    return 0;
}