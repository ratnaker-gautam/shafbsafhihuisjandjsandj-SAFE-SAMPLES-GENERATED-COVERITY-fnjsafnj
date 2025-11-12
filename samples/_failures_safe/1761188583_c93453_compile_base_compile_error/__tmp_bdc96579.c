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

void init_context(struct Context *ctx) {
    ctx->current_state = START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int is_valid_input_char(char c) {
    return isalnum(c) || isspace(c) || c == '\0';
}

enum State handle_start(char c, struct Context *ctx) {
    if (c == '\0') {
        return FINISHED;
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
    } else if (isspace(c)) {
        return SKIPPING_SPACE;
    } else {
        return ERROR;
    }
}

enum State handle_reading_number(char c, struct Context *ctx) {
    if (c == '\0') {
        ctx->number_count++;
        ctx->buffer_pos = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        return FINISHED;
    } else if (isdigit(c)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = c;
        }
        return READING_NUMBER;
    } else if (isspace(c)) {
        ctx->number_count++;
        ctx->buffer_pos = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        return SKIPPING_SPACE;
    } else {
        return ERROR;
    }
}

enum State handle_reading_word(char c, struct Context *ctx) {
    if (c == '\0') {
        ctx->word_count++;
        ctx->buffer_pos = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        return FINISHED;
    } else if (isalpha(c)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = c;
        }
        return READING_WORD;
    } else if (isspace(c)) {
        ctx->word_count++;
        ctx->buffer_pos = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        return SKIPPING_SPACE;
    } else {
        return ERROR;
    }
}

enum State handle_skipping_space(char c, struct Context *ctx) {
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
    
    while (ctx->current_state != FINISHED && ctx->current_state != ERROR) {
        current_char = input[i];
        
        if (!is_valid_input_char(current_char)) {
            ctx->current_state = ERROR;
            break;
        }
        
        switch (ctx->current_state) {
            case START:
                ctx->current_state = handle_start(current_char, ctx);
                break;
            case READING_NUMBER:
                ctx->current_state = handle_reading_number(current_char, ctx);
                break;
            case READING_WORD:
                ctx->current_state = handle_reading_word(current_char, ctx);
                break;
            case SKIPPING_SPACE:
                ctx->current_state = handle_skipping_space(current_char, ctx);
                break;
            case FINISHED:
            case ERROR:
                break;
        }
        
        i++;
        if (i >= 1024) {
            ctx->current_state = ERROR;
            break;
        }
    }
}

int main(void) {
    char input[1025];
    struct Context ctx;
    
    printf("Enter text (max 1024 characters): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    init_context(&ctx);
    process_input(input, &