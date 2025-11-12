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
    char input_buffer[256];
    size_t input_len;
    size_t position;
    int number_count;
    int word_count;
};

void initialize_context(struct Context *ctx, const char *input) {
    ctx->current_state = START;
    ctx->input_len = strnlen(input, sizeof(ctx->input_buffer) - 1);
    if (ctx->input_len >= sizeof(ctx->input_buffer)) {
        ctx->current_state = ERROR;
        return;
    }
    memcpy(ctx->input_buffer, input, ctx->input_len);
    ctx->input_buffer[ctx->input_len] = '\0';
    ctx->position = 0;
    ctx->number_count = 0;
    ctx->word_count = 0;
}

enum State handle_start(struct Context *ctx) {
    if (ctx->position >= ctx->input_len) {
        return FINISHED;
    }
    char c = ctx->input_buffer[ctx->position];
    if (isdigit(c)) {
        return READING_NUMBER;
    } else if (isalpha(c)) {
        return READING_WORD;
    } else if (isspace(c)) {
        return SKIPPING_SPACE;
    } else {
        return ERROR;
    }
}

enum State handle_reading_number(struct Context *ctx) {
    while (ctx->position < ctx->input_len) {
        char c = ctx->input_buffer[ctx->position];
        if (isdigit(c)) {
            ctx->position++;
        } else if (isspace(c)) {
            ctx->number_count++;
            return SKIPPING_SPACE;
        } else {
            return ERROR;
        }
    }
    ctx->number_count++;
    return FINISHED;
}

enum State handle_reading_word(struct Context *ctx) {
    while (ctx->position < ctx->input_len) {
        char c = ctx->input_buffer[ctx->position];
        if (isalpha(c)) {
            ctx->position++;
        } else if (isspace(c)) {
            ctx->word_count++;
            return SKIPPING_SPACE;
        } else {
            return ERROR;
        }
    }
    ctx->word_count++;
    return FINISHED;
}

enum State handle_skipping_space(struct Context *ctx) {
    while (ctx->position < ctx->input_len) {
        char c = ctx->input_buffer[ctx->position];
        if (isspace(c)) {
            ctx->position++;
        } else if (isdigit(c)) {
            return READING_NUMBER;
        } else if (isalpha(c)) {
            return READING_WORD;
        } else {
            return ERROR;
        }
    }
    return FINISHED;
}

void run_state_machine(struct Context *ctx) {
    while (ctx->current_state != FINISHED && ctx->current_state != ERROR) {
        switch (ctx->current_state) {
            case START:
                ctx->current_state = handle_start(ctx);
                break;
            case READING_NUMBER:
                ctx->current_state = handle_reading_number(ctx);
                break;
            case READING_WORD:
                ctx->current_state = handle_reading_word(ctx);
                break;
            case SKIPPING_SPACE:
                ctx->current_state = handle_skipping_space(ctx);
                break;
            default:
                ctx->current_state = ERROR;
                break;
        }
    }
}

int main(void) {
    char user_input[256];
    printf("Enter text to analyze (max 255 characters): ");
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strnlen(user_input, sizeof(user_input));
    if (len > 0 && user_input[len - 1] == '\n') {
        user_input[len - 1] = '\0';
    }
    
    struct Context ctx;
    initialize_context(&ctx, user_input);
    
    if (ctx.current_state == ERROR) {
        fprintf(stderr, "Input too long or invalid\n");
        return 1;
    }
    
    run_state_machine(&ctx);
    
    if (ctx.current_state == ERROR) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    printf("Analysis complete:\n");
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    
    return 0;
}