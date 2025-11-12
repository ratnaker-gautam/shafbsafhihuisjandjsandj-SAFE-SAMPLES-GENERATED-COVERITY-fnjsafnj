//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_DONE
};

struct context {
    enum state current_state;
    int number_count;
    int word_count;
    char buffer[256];
    size_t buffer_pos;
};

void context_init(struct context *ctx) {
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int is_valid_input_char(char c) {
    return isalnum(c) || isspace(c) || c == '\0';
}

void process_char(struct context *ctx, char c) {
    if (!is_valid_input_char(c)) {
        ctx->current_state = STATE_ERROR;
        return;
    }

    switch (ctx->current_state) {
        case STATE_START:
            if (isdigit(c)) {
                ctx->buffer[ctx->buffer_pos++] = c;
                ctx->current_state = STATE_READING_NUMBER;
            } else if (isalpha(c)) {
                ctx->buffer[ctx->buffer_pos++] = c;
                ctx->current_state = STATE_READING_WORD;
            } else if (c == '\0') {
                ctx->current_state = STATE_DONE;
            }
            break;

        case STATE_READING_NUMBER:
            if (isdigit(c)) {
                if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buffer_pos++] = c;
                } else {
                    ctx->current_state = STATE_ERROR;
                }
            } else if (isspace(c) || c == '\0') {
                ctx->buffer[ctx->buffer_pos] = '\0';
                ctx->number_count++;
                ctx->buffer_pos = 0;
                ctx->current_state = (c == '\0') ? STATE_DONE : STATE_START;
            } else {
                ctx->current_state = STATE_ERROR;
            }
            break;

        case STATE_READING_WORD:
            if (isalpha(c)) {
                if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buffer_pos++] = c;
                } else {
                    ctx->current_state = STATE_ERROR;
                }
            } else if (isspace(c) || c == '\0') {
                ctx->buffer[ctx->buffer_pos] = '\0';
                ctx->word_count++;
                ctx->buffer_pos = 0;
                ctx->current_state = (c == '\0') ? STATE_DONE : STATE_START;
            } else {
                ctx->current_state = STATE_ERROR;
            }
            break;

        case STATE_ERROR:
        case STATE_DONE:
            break;
    }
}

void process_string(struct context *ctx, const char *input) {
    if (input == NULL) {
        ctx->current_state = STATE_ERROR;
        return;
    }

    size_t len = strlen(input);
    if (len > 1024) {
        ctx->current_state = STATE_ERROR;
        return;
    }

    for (size_t i = 0; i <= len; i++) {
        process_char(ctx, input[i]);
        if (ctx->current_state == STATE_ERROR) {
            break;
        }
    }
}

int main(void) {
    struct context ctx;
    char input[1025];
    
    printf("Enter text (max 1024 chars): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    context_init(&ctx);
    process_string(&ctx, input);
    
    switch (ctx.current_state) {
        case STATE_DONE:
            printf("Numbers found: %d\n", ctx.number_count);
            printf("Words found: %d\n", ctx.word_count);
            break;
        case STATE_ERROR:
            fprintf(stderr, "Error: Invalid input format\n");
            return 1;
        default:
            fprintf(stderr, "Error: Processing incomplete\n");
            return 1;
    }
    
    return 0;
}