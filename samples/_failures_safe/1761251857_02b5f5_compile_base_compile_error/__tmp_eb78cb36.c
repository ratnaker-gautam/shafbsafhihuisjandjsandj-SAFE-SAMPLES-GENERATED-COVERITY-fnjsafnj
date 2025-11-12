//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum State {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING_SPACE,
    FINISHED,
    ERROR
};

struct Context {
    enum State current_state;
    int number_count;
    int word_count;
    char buffer[256];
    size_t buffer_pos;
};

void context_init(struct Context *ctx) {
    ctx->current_state = START;
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
        return FINISHED;
    } else if (isspace(c)) {
        return SKIPPING_SPACE;
    } else if (isdigit(c)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = c;
        }
        return READING_NUMBER;
    } else if (isalpha(c)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = c;
        }
        return READING_WORD;
    } else {
        return ERROR;
    }
}

enum State handle_reading_number(struct Context *ctx, char c) {
    if (c == '\0') {
        ctx->number_count++;
        ctx->buffer_pos = 0;
        return FINISHED;
    } else if (isspace(c)) {
        ctx->number_count++;
        ctx->buffer_pos = 0;
        return SKIPPING_SPACE;
    } else if (isdigit(c)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = c;
        }
        return READING_NUMBER;
    } else {
        return ERROR;
    }
}

enum State handle_reading_word(struct Context *ctx, char c) {
    if (c == '\0') {
        ctx->word_count++;
        ctx->buffer_pos = 0;
        return FINISHED;
    } else if (isspace(c)) {
        ctx->word_count++;
        ctx->buffer_pos = 0;
        return SKIPPING_SPACE;
    } else if (isalpha(c)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = c;
        }
        return READING_WORD;
    } else {
        return ERROR;
    }
}

enum State handle_skipping_space(struct Context *ctx, char c) {
    if (c == '\0') {
        return FINISHED;
    } else if (isspace(c)) {
        return SKIPPING_SPACE;
    } else if (isdigit(c)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = c;
        }
        return READING_NUMBER;
    } else if (isalpha(c)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = c;
        }
        return READING_WORD;
    } else {
        return ERROR;
    }
}

void process_input(const char *input, struct Context *ctx) {
    size_t i = 0;
    char current_char;
    size_t input_len = strlen(input);
    
    while (ctx->current_state != FINISHED && ctx->current_state != ERROR) {
        if (i < input_len) {
            current_char = input[i];
            if (!is_valid_input_char(current_char)) {
                ctx->current_state = ERROR;
                break;
            }
        } else {
            current_char = '\0';
        }
        
        switch (ctx->current_state) {
            case START:
                ctx->current_state = handle_start(ctx, current_char);
                break;
            case READING_NUMBER:
                ctx->current_state = handle_reading_number(ctx, current_char);
                break;
            case READING_WORD:
                ctx->current_state = handle_reading_word(ctx, current_char);
                break;
            case SKIPPING_SPACE:
                ctx->current_state = handle_skipping_space(ctx, current_char);
                break;
            case FINISHED:
            case ERROR:
                break;
        }
        
        if (current_char != '\0') {
            i++;
            if (i > 1024) {
                ctx->current_state = ERROR;
                break;
            }
        }
    }
}

int main(void) {
    struct Context ctx;
    char input[1025];
    
    printf("Enter text to analyze (max 1024 characters): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len >= sizeof(input) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    context_init(&ctx);
    process_input(input, &ctx);
    
    if