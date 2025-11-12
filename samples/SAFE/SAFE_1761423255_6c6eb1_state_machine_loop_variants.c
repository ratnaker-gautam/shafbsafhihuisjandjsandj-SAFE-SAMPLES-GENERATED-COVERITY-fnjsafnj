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

void initialize_context(struct Context *ctx) {
    ctx->current_state = START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->buffer_index = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || isspace((unsigned char)c);
}

void process_char(struct Context *ctx, char c) {
    if (!is_valid_char(c)) {
        ctx->current_state = ERROR;
        return;
    }

    switch (ctx->current_state) {
        case START:
            if (isdigit((unsigned char)c)) {
                ctx->current_state = READING_NUMBER;
                ctx->buffer[ctx->buffer_index++] = c;
            } else if (isalpha((unsigned char)c)) {
                ctx->current_state = READING_WORD;
                ctx->buffer[ctx->buffer_index++] = c;
            } else if (isspace((unsigned char)c)) {
                ctx->current_state = START;
            }
            break;

        case READING_NUMBER:
            if (isdigit((unsigned char)c)) {
                if (ctx->buffer_index < 255) {
                    ctx->buffer[ctx->buffer_index++] = c;
                } else {
                    ctx->current_state = ERROR;
                }
            } else if (isspace((unsigned char)c)) {
                ctx->number_count++;
                ctx->buffer_index = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                ctx->current_state = START;
            } else {
                ctx->current_state = ERROR;
            }
            break;

        case READING_WORD:
            if (isalpha((unsigned char)c)) {
                if (ctx->buffer_index < 255) {
                    ctx->buffer[ctx->buffer_index++] = c;
                } else {
                    ctx->current_state = ERROR;
                }
            } else if (isspace((unsigned char)c)) {
                ctx->word_count++;
                ctx->buffer_index = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                ctx->current_state = START;
            } else {
                ctx->current_state = ERROR;
            }
            break;

        case ERROR:
            break;

        case DONE:
            break;
    }
}

void finalize_processing(struct Context *ctx) {
    if (ctx->current_state == READING_NUMBER) {
        ctx->number_count++;
    } else if (ctx->current_state == READING_WORD) {
        ctx->word_count++;
    }
    ctx->current_state = DONE;
}

int main(void) {
    struct Context ctx;
    initialize_context(&ctx);

    printf("Enter text (max 1000 characters): ");
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
    while (i < len) {
        process_char(&ctx, input[i]);
        if (ctx.current_state == ERROR) {
            fprintf(stderr, "Invalid input format\n");
            return 1;
        }
        i++;
    }

    finalize_processing(&ctx);

    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);

    return 0;
}