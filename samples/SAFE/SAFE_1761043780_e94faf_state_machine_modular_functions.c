//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_END
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

int is_valid_input_char(char c) {
    return isalnum(c) || isspace(c) || c == '\0';
}

enum State handle_start(struct Context *ctx, char c) {
    if (c == '\0') {
        return STATE_END;
    } else if (isdigit(c)) {
        ctx->buffer[ctx->buffer_pos++] = c;
        return STATE_READING_NUMBER;
    } else if (isalpha(c)) {
        ctx->buffer[ctx->buffer_pos++] = c;
        return STATE_READING_WORD;
    } else if (isspace(c)) {
        return STATE_START;
    } else {
        return STATE_ERROR;
    }
}

enum State handle_reading_number(struct Context *ctx, char c) {
    if (c == '\0' || isspace(c)) {
        if (ctx->buffer_pos > 0) {
            ctx->number_count++;
        }
        ctx->buffer_pos = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        return (c == '\0') ? STATE_END : STATE_START;
    } else if (isdigit(c)) {
        if (ctx->buffer_pos < 255) {
            ctx->buffer[ctx->buffer_pos++] = c;
        }
        return STATE_READING_NUMBER;
    } else {
        return STATE_ERROR;
    }
}

enum State handle_reading_word(struct Context *ctx, char c) {
    if (c == '\0' || isspace(c)) {
        if (ctx->buffer_pos > 0) {
            ctx->word_count++;
        }
        ctx->buffer_pos = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        return (c == '\0') ? STATE_END : STATE_START;
    } else if (isalpha(c)) {
        if (ctx->buffer_pos < 255) {
            ctx->buffer[ctx->buffer_pos++] = c;
        }
        return STATE_READING_WORD;
    } else {
        return STATE_ERROR;
    }
}

enum State process_char(struct Context *ctx, char c) {
    if (!is_valid_input_char(c)) {
        return STATE_ERROR;
    }

    switch (ctx->current_state) {
        case STATE_START:
            return handle_start(ctx, c);
        case STATE_READING_NUMBER:
            return handle_reading_number(ctx, c);
        case STATE_READING_WORD:
            return handle_reading_word(ctx, c);
        case STATE_ERROR:
        case STATE_END:
            return ctx->current_state;
        default:
            return STATE_ERROR;
    }
}

int process_input(struct Context *ctx, const char *input) {
    if (input == NULL) {
        return 0;
    }

    size_t len = strlen(input);
    if (len > 1024) {
        return 0;
    }

    for (size_t i = 0; i <= len; i++) {
        char c = input[i];
        ctx->current_state = process_char(ctx, c);
        if (ctx->current_state == STATE_ERROR) {
            return 0;
        }
        if (ctx->current_state == STATE_END) {
            break;
        }
    }

    return 1;
}

int main(void) {
    struct Context ctx;
    char input[1025];
    
    printf("Enter text (max 1024 chars): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    context_init(&ctx);
    
    if (!process_input(&ctx, input)) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    
    return 0;
}