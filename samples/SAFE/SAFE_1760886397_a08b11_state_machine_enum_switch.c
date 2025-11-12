//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_ERROR,
    STATE_DONE
};

struct context {
    enum state current_state;
    int word_count;
    int number_count;
    char buffer[256];
    size_t buffer_pos;
};

void context_init(struct context *ctx) {
    ctx->current_state = STATE_START;
    ctx->word_count = 0;
    ctx->number_count = 0;
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int is_valid_input_char(char c) {
    return isalnum((unsigned char)c) || isspace((unsigned char)c) || c == '\0';
}

void process_char(struct context *ctx, char c) {
    if (!is_valid_input_char(c)) {
        ctx->current_state = STATE_ERROR;
        return;
    }

    switch (ctx->current_state) {
        case STATE_START:
            if (isalpha((unsigned char)c)) {
                ctx->current_state = STATE_READING_WORD;
                ctx->buffer[ctx->buffer_pos++] = c;
            } else if (isdigit((unsigned char)c)) {
                ctx->current_state = STATE_READING_NUMBER;
                ctx->buffer[ctx->buffer_pos++] = c;
            } else if (c == '\0') {
                ctx->current_state = STATE_DONE;
            }
            break;

        case STATE_READING_WORD:
            if (isalnum((unsigned char)c)) {
                if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buffer_pos++] = c;
                } else {
                    ctx->current_state = STATE_ERROR;
                }
            } else {
                ctx->word_count++;
                ctx->buffer_pos = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                if (isdigit((unsigned char)c)) {
                    ctx->current_state = STATE_READING_NUMBER;
                    ctx->buffer[ctx->buffer_pos++] = c;
                } else if (c == '\0') {
                    ctx->current_state = STATE_DONE;
                } else {
                    ctx->current_state = STATE_START;
                }
            }
            break;

        case STATE_READING_NUMBER:
            if (isdigit((unsigned char)c)) {
                if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buffer_pos++] = c;
                } else {
                    ctx->current_state = STATE_ERROR;
                }
            } else {
                ctx->number_count++;
                ctx->buffer_pos = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                if (isalpha((unsigned char)c)) {
                    ctx->current_state = STATE_READING_WORD;
                    ctx->buffer[ctx->buffer_pos++] = c;
                } else if (c == '\0') {
                    ctx->current_state = STATE_DONE;
                } else {
                    ctx->current_state = STATE_START;
                }
            }
            break;

        case STATE_ERROR:
        case STATE_DONE:
            break;
    }
}

int main(void) {
    struct context ctx;
    context_init(&ctx);
    
    char input[1024];
    printf("Enter text (max 1023 chars): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; i <= input_len; i++) {
        process_char(&ctx, input[i]);
        if (ctx.current_state == STATE_ERROR) {
            fprintf(stderr, "Invalid input character\n");
            return 1;
        }
        if (ctx.current_state == STATE_DONE) {
            break;
        }
    }
    
    if (ctx.current_state == STATE_READING_WORD) {
        ctx.word_count++;
    } else if (ctx.current_state == STATE_READING_NUMBER) {
        ctx.number_count++;
    }
    
    printf("Words: %d\n", ctx.word_count);
    printf("Numbers: %d\n", ctx.number_count);
    
    return 0;
}