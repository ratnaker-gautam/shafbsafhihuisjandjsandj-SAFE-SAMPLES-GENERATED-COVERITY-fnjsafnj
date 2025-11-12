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

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || isspace((unsigned char)c);
}

void process_char(struct context *ctx, char c) {
    if (!is_valid_char(c)) {
        ctx->current_state = STATE_ERROR;
        return;
    }

    switch (ctx->current_state) {
        case STATE_START:
            if (isdigit((unsigned char)c)) {
                ctx->current_state = STATE_READING_NUMBER;
                ctx->buffer[ctx->buffer_pos++] = c;
            } else if (isalpha((unsigned char)c)) {
                ctx->current_state = STATE_READING_WORD;
                ctx->buffer[ctx->buffer_pos++] = c;
            } else if (isspace((unsigned char)c)) {
                break;
            } else {
                ctx->current_state = STATE_ERROR;
            }
            break;

        case STATE_READING_NUMBER:
            if (isdigit((unsigned char)c)) {
                if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buffer_pos++] = c;
                } else {
                    ctx->current_state = STATE_ERROR;
                }
            } else if (isspace((unsigned char)c)) {
                ctx->number_count++;
                ctx->buffer_pos = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                ctx->current_state = STATE_START;
            } else {
                ctx->current_state = STATE_ERROR;
            }
            break;

        case STATE_READING_WORD:
            if (isalpha((unsigned char)c)) {
                if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buffer_pos++] = c;
                } else {
                    ctx->current_state = STATE_ERROR;
                }
            } else if (isspace((unsigned char)c)) {
                ctx->word_count++;
                ctx->buffer_pos = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                ctx->current_state = STATE_START;
            } else {
                ctx->current_state = STATE_ERROR;
            }
            break;

        case STATE_ERROR:
            break;

        case STATE_DONE:
            break;
    }
}

void finalize_processing(struct context *ctx) {
    switch (ctx->current_state) {
        case STATE_READING_NUMBER:
            ctx->number_count++;
            break;
        case STATE_READING_WORD:
            ctx->word_count++;
            break;
        case STATE_START:
            break;
        default:
            break;
    }
    ctx->current_state = STATE_DONE;
}

int main(void) {
    struct context ctx;
    context_init(&ctx);

    printf("Enter text (max 1000 characters): ");
    fflush(stdout);

    char input[1002];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }

    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }

    if (input_len >= sizeof(input) - 1) {
        printf("Input too long\n");
        return 1;
    }

    for (size_t i = 0; i < input_len; i++) {
        process_char(&ctx, input[i]);
        if (ctx.current_state == STATE_ERROR) {
            printf("Invalid input character at position %zu\n", i + 1);
            return 1;
        }
    }

    finalize_processing(&ctx);

    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);

    return 0;
}