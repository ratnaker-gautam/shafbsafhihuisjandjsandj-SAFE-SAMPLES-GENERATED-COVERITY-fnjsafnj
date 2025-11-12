//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
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
    int buffer_index;
};

void context_init(struct Context *ctx) {
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->buffer_index = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int is_valid_input_char(char c) {
    return isalnum(c) || isspace(c) || c == '\0';
}

enum State handle_start(struct Context *ctx, char c) {
    if (c == '\0') {
        return STATE_END;
    } else if (isdigit(c)) {
        ctx->buffer[ctx->buffer_index++] = c;
        return STATE_READING_NUMBER;
    } else if (isalpha(c)) {
        ctx->buffer[ctx->buffer_index++] = c;
        return STATE_READING_WORD;
    } else if (isspace(c)) {
        return STATE_START;
    } else {
        return STATE_ERROR;
    }
}

enum State handle_reading_number(struct Context *ctx, char c) {
    if (c == '\0') {
        ctx->number_count++;
        ctx->buffer_index = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        return STATE_END;
    } else if (isdigit(c)) {
        if (ctx->buffer_index < 255) {
            ctx->buffer[ctx->buffer_index++] = c;
        }
        return STATE_READING_NUMBER;
    } else if (isspace(c)) {
        ctx->number_count++;
        ctx->buffer_index = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        return STATE_START;
    } else {
        return STATE_ERROR;
    }
}

enum State handle_reading_word(struct Context *ctx, char c) {
    if (c == '\0') {
        ctx->word_count++;
        ctx->buffer_index = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        return STATE_END;
    } else if (isalpha(c)) {
        if (ctx->buffer_index < 255) {
            ctx->buffer[ctx->buffer_index++] = c;
        }
        return STATE_READING_WORD;
    } else if (isspace(c)) {
        ctx->word_count++;
        ctx->buffer_index = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        return STATE_START;
    } else {
        return STATE_ERROR;
    }
}

void process_input(struct Context *ctx, const char *input) {
    size_t len = strlen(input);
    if (len > 1024) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    for (size_t i = 0; i <= len; i++) {
        char c = input[i];
        
        if (!is_valid_input_char(c)) {
            ctx->current_state = STATE_ERROR;
            return;
        }
        
        switch (ctx->current_state) {
            case STATE_START:
                ctx->current_state = handle_start(ctx, c);
                break;
            case STATE_READING_NUMBER:
                ctx->current_state = handle_reading_number(ctx, c);
                break;
            case STATE_READING_WORD:
                ctx->current_state = handle_reading_word(ctx, c);
                break;
            case STATE_ERROR:
            case STATE_END:
                return;
        }
        
        if (ctx->current_state == STATE_ERROR) {
            return;
        }
    }
}

int main(void) {
    struct Context ctx;
    char input[1025];
    
    printf("Enter text (max 1024 characters): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    context_init(&ctx);
    process_input(&ctx, input);
    
    if (ctx.current_state == STATE_ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    
    return 0;
}