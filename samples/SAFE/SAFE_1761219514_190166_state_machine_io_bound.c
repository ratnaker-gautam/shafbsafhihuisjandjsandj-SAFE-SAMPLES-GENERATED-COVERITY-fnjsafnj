//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_END
} parser_state_t;

typedef struct {
    parser_state_t current_state;
    int number_count;
    int word_count;
    char current_word[MAX_INPUT_LEN];
    int word_pos;
} parser_context_t;

void init_parser(parser_context_t *ctx) {
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->word_pos = 0;
    memset(ctx->current_word, 0, MAX_INPUT_LEN);
}

parser_state_t handle_start(parser_context_t *ctx, char c) {
    if (c == '\n' || c == '\0') {
        return STATE_END;
    } else if (isdigit(c)) {
        ctx->number_count++;
        return STATE_READING_NUMBER;
    } else if (isalpha(c)) {
        if (ctx->word_pos < MAX_INPUT_LEN - 1) {
            ctx->current_word[ctx->word_pos++] = c;
        }
        return STATE_READING_WORD;
    } else if (isspace(c)) {
        return STATE_START;
    } else {
        return STATE_ERROR;
    }
}

parser_state_t handle_reading_number(parser_context_t *ctx, char c) {
    if (c == '\n' || c == '\0') {
        return STATE_END;
    } else if (isdigit(c)) {
        return STATE_READING_NUMBER;
    } else if (isspace(c)) {
        return STATE_START;
    } else {
        return STATE_ERROR;
    }
}

parser_state_t handle_reading_word(parser_context_t *ctx, char c) {
    if (c == '\n' || c == '\0') {
        if (ctx->word_pos > 0) {
            ctx->word_count++;
            ctx->word_pos = 0;
            memset(ctx->current_word, 0, MAX_INPUT_LEN);
        }
        return STATE_END;
    } else if (isalpha(c)) {
        if (ctx->word_pos < MAX_INPUT_LEN - 1) {
            ctx->current_word[ctx->word_pos++] = c;
        }
        return STATE_READING_WORD;
    } else if (isspace(c)) {
        if (ctx->word_pos > 0) {
            ctx->word_count++;
            ctx->word_pos = 0;
            memset(ctx->current_word, 0, MAX_INPUT_LEN);
        }
        return STATE_START;
    } else {
        return STATE_ERROR;
    }
}

int process_input(const char *input, parser_context_t *ctx) {
    size_t len = strlen(input);
    if (len >= MAX_INPUT_LEN) {
        return -1;
    }

    for (size_t i = 0; i <= len; i++) {
        char c = input[i];
        
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
                return -1;
            case STATE_END:
                return 0;
        }

        if (ctx->current_state == STATE_ERROR) {
            return -1;
        }
    }
    
    return 0;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    parser_context_t ctx;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    init_parser(&ctx);
    
    if (process_input(input, &ctx) != 0) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    
    return 0;
}