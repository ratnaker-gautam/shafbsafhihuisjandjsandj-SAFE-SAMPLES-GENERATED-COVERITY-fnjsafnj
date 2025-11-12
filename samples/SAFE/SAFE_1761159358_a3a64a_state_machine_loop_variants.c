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
    char buffer[256];
    size_t buffer_pos;
    int number_count;
    int word_count;
};

void state_machine_init(struct Context *ctx) {
    ctx->current_state = START;
    ctx->buffer_pos = 0;
    ctx->number_count = 0;
    ctx->word_count = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

enum State handle_start(struct Context *ctx, char c) {
    if (c == '\0') {
        return FINISHED;
    } else if (isdigit(c)) {
        ctx->buffer[ctx->buffer_pos++] = c;
        return READING_NUMBER;
    } else if (isalpha(c)) {
        ctx->buffer[ctx->buffer_pos++] = c;
        return READING_WORD;
    } else if (isspace(c)) {
        return SKIPPING_SPACE;
    } else {
        return ERROR;
    }
}

enum State handle_reading_number(struct Context *ctx, char c) {
    if (c == '\0') {
        ctx->number_count++;
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

enum State handle_reading_word(struct Context *ctx, char c) {
    if (c == '\0') {
        ctx->word_count++;
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

enum State handle_skipping_space(struct Context *ctx, char c) {
    if (c == '\0') {
        return FINISHED;
    } else if (isdigit(c)) {
        ctx->buffer[ctx->buffer_pos++] = c;
        return READING_NUMBER;
    } else if (isalpha(c)) {
        ctx->buffer[ctx->buffer_pos++] = c;
        return READING_WORD;
    } else if (isspace(c)) {
        return SKIPPING_SPACE;
    } else {
        return ERROR;
    }
}

void process_input(const char *input, struct Context *ctx) {
    size_t i = 0;
    char current_char;
    
    while (ctx->current_state != FINISHED && ctx->current_state != ERROR) {
        current_char = input[i];
        
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
        
        if (current_char == '\0') {
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
    struct Context ctx;
    char input[1025];
    
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
    
    state_machine_init(&ctx);
    process_input(input, &ctx);
    
    if (ctx.current_state == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    
    return 0;
}