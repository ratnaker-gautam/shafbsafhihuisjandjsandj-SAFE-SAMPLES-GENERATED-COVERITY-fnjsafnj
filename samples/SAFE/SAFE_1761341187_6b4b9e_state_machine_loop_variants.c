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

enum State handle_start(struct Context *ctx, char c) {
    if (c == '\n' || c == '\0') {
        return DONE;
    } else if (isdigit(c)) {
        ctx->buffer[ctx->buffer_index++] = c;
        return READING_NUMBER;
    } else if (isalpha(c)) {
        ctx->buffer[ctx->buffer_index++] = c;
        return READING_WORD;
    } else if (isspace(c)) {
        return START;
    } else {
        return ERROR;
    }
}

enum State handle_reading_number(struct Context *ctx, char c) {
    if (isdigit(c)) {
        if (ctx->buffer_index < 255) {
            ctx->buffer[ctx->buffer_index++] = c;
        }
        return READING_NUMBER;
    } else if (isspace(c) || c == '\0') {
        ctx->number_count++;
        ctx->buffer_index = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        if (c == '\0') {
            return DONE;
        }
        return START;
    } else {
        return ERROR;
    }
}

enum State handle_reading_word(struct Context *ctx, char c) {
    if (isalpha(c)) {
        if (ctx->buffer_index < 255) {
            ctx->buffer[ctx->buffer_index++] = c;
        }
        return READING_WORD;
    } else if (isspace(c) || c == '\0') {
        ctx->word_count++;
        ctx->buffer_index = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        if (c == '\0') {
            return DONE;
        }
        return START;
    } else {
        return ERROR;
    }
}

enum State handle_error(struct Context *ctx, char c) {
    if (c == '\n' || c == '\0') {
        return DONE;
    }
    return ERROR;
}

void process_input(struct Context *ctx, const char *input) {
    size_t len = strlen(input);
    if (len > 1024) {
        ctx->current_state = ERROR;
        return;
    }

    for (size_t i = 0; i <= len; i++) {
        char current_char = input[i];
        
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
            case ERROR:
                ctx->current_state = handle_error(ctx, current_char);
                break;
            case DONE:
                return;
        }
        
        if (ctx->current_state == ERROR) {
            break;
        }
    }
}

int main(void) {
    struct Context ctx;
    char input[1025];
    
    printf("Enter text to analyze (max 1024 characters): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    initialize_context(&ctx);
    process_input(&ctx, input);
    
    if (ctx.current_state == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    
    return 0;
}