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
    int total_chars;
} parser_context_t;

void initialize_parser(parser_context_t *ctx) {
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->total_chars = 0;
}

parser_state_t handle_start(char c, parser_context_t *ctx) {
    if (isspace(c)) {
        return STATE_START;
    } else if (isdigit(c)) {
        ctx->number_count++;
        return STATE_READING_NUMBER;
    } else if (isalpha(c)) {
        ctx->word_count++;
        return STATE_READING_WORD;
    } else if (c == 'q') {
        return STATE_END;
    } else {
        return STATE_ERROR;
    }
}

parser_state_t handle_reading_number(char c, parser_context_t *ctx) {
    if (isdigit(c)) {
        return STATE_READING_NUMBER;
    } else if (isspace(c)) {
        return STATE_START;
    } else if (c == 'q') {
        return STATE_END;
    } else {
        return STATE_ERROR;
    }
}

parser_state_t handle_reading_word(char c, parser_context_t *ctx) {
    if (isalpha(c)) {
        return STATE_READING_WORD;
    } else if (isspace(c)) {
        return STATE_START;
    } else if (c == 'q') {
        return STATE_END;
    } else {
        return STATE_ERROR;
    }
}

parser_state_t handle_error(parser_context_t *ctx) {
    return STATE_ERROR;
}

parser_state_t process_char(char c, parser_context_t *ctx) {
    ctx->total_chars++;
    
    switch (ctx->current_state) {
        case STATE_START:
            return handle_start(c, ctx);
        case STATE_READING_NUMBER:
            return handle_reading_number(c, ctx);
        case STATE_READING_WORD:
            return handle_reading_word(c, ctx);
        case STATE_ERROR:
            return handle_error(ctx);
        case STATE_END:
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

int main(void) {
    parser_context_t ctx;
    char input[MAX_INPUT_LEN + 1];
    int input_len;
    
    initialize_parser(&ctx);
    
    printf("Enter text (q to quit): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (int i = 0; i < input_len; i++) {
        ctx.current_state = process_char(input[i], &ctx);
        if (ctx.current_state == STATE_END) {
            break;
        }
        if (ctx.current_state == STATE_ERROR) {
            fprintf(stderr, "Invalid input character: '%c'\n", input[i]);
            return 1;
        }
    }
    
    if (ctx.current_state == STATE_READING_NUMBER || ctx.current_state == STATE_READING_WORD) {
        ctx.current_state = STATE_START;
    }
    
    printf("Numbers: %d\n", ctx.number_count);
    printf("Words: %d\n", ctx.word_count);
    printf("Total characters: %d\n", ctx.total_chars);
    
    return 0;
}