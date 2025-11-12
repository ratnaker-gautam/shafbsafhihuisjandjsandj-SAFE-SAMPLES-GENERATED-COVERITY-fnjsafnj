//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING_SPACE,
    FINISHED,
    ERROR
};

struct context {
    enum state current_state;
    int number_count;
    int word_count;
    char buffer[256];
    int buffer_pos;
};

void init_context(struct context *ctx) {
    ctx->current_state = START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int is_valid_char(char c) {
    return isalnum(c) || isspace(c);
}

void process_char(struct context *ctx, char c) {
    if (!is_valid_char(c)) {
        ctx->current_state = ERROR;
        return;
    }

    switch (ctx->current_state) {
        case START:
            if (isdigit(c)) {
                ctx->buffer[ctx->buffer_pos++] = c;
                ctx->current_state = READING_NUMBER;
            } else if (isalpha(c)) {
                ctx->buffer[ctx->buffer_pos++] = c;
                ctx->current_state = READING_WORD;
            } else if (isspace(c)) {
                ctx->current_state = SKIPPING_SPACE;
            } else {
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
            } else if (isspace(c)) {
                ctx->number_count++;
                ctx->buffer_pos = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                ctx->current_state = SKIPPING_SPACE;
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
            } else if (isspace(c)) {
                ctx->word_count++;
                ctx->buffer_pos = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                ctx->current_state = SKIPPING_SPACE;
            } else {
                ctx->current_state = ERROR;
            }
            break;

        case SKIPPING_SPACE:
            if (isdigit(c)) {
                ctx->buffer[ctx->buffer_pos++] = c;
                ctx->current_state = READING_NUMBER;
            } else if (isalpha(c)) {
                ctx->buffer[ctx->buffer_pos++] = c;
                ctx->current_state = READING_WORD;
            } else if (!isspace(c)) {
                ctx->current_state = ERROR;
            }
            break;

        case FINISHED:
        case ERROR:
            break;
    }
}

void finish_processing(struct context *ctx) {
    switch (ctx->current_state) {
        case READING_NUMBER:
            ctx->number_count++;
            break;
        case READING_WORD:
            ctx->word_count++;
            break;
        case START:
        case SKIPPING_SPACE:
            ctx->current_state = FINISHED;
            break;
        case FINISHED:
        case ERROR:
            break;
    }
}

int main(void) {
    struct context ctx;
    init_context(&ctx);

    printf("Enter text (alphanumeric characters and spaces only):\n");
    
    char input[1024];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }

    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }

    if (len >= sizeof(input)) {
        printf("Input too long\n");
        return 1;
    }

    for (size_t i = 0; i < len && ctx.current_state != ERROR; i++) {
        process_char(&ctx, input[i]);
    }

    if (ctx.current_state != ERROR) {
        finish_processing(&ctx);
    }

    if (ctx.current_state == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }

    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);

    return 0;
}