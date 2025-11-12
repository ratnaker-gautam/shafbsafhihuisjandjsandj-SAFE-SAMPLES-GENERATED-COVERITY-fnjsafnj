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
    int error_count;
} parser_context_t;

void initialize_parser(parser_context_t *ctx) {
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->error_count = 0;
}

parser_state_t handle_start(char c, parser_context_t *ctx) {
    if (isdigit(c)) {
        ctx->number_count++;
        return STATE_READING_NUMBER;
    } else if (isalpha(c)) {
        ctx->word_count++;
        return STATE_READING_WORD;
    } else if (c == ' ' || c == '\t') {
        return STATE_START;
    } else if (c == '\n' || c == '\0') {
        return STATE_END;
    } else {
        ctx->error_count++;
        return STATE_ERROR;
    }
}

parser_state_t handle_reading_number(char c, parser_context_t *ctx) {
    if (isdigit(c)) {
        return STATE_READING_NUMBER;
    } else if (c == ' ' || c == '\t') {
        return STATE_START;
    } else if (c == '\n' || c == '\0') {
        return STATE_END;
    } else {
        ctx->error_count++;
        return STATE_ERROR;
    }
}

parser_state_t handle_reading_word(char c, parser_context_t *ctx) {
    if (isalnum(c)) {
        return STATE_READING_WORD;
    } else if (c == ' ' || c == '\t') {
        return STATE_START;
    } else if (c == '\n' || c == '\0') {
        return STATE_END;
    } else {
        ctx->error_count++;
        return STATE_ERROR;
    }
}

parser_state_t handle_error(char c, parser_context_t *ctx) {
    if (c == ' ' || c == '\t') {
        return STATE_START;
    } else if (c == '\n' || c == '\0') {
        return STATE_END;
    }
    return STATE_ERROR;
}

void process_input(const char *input, parser_context_t *ctx) {
    size_t len = strnlen(input, MAX_INPUT_LEN);
    
    for (size_t i = 0; i < len; i++) {
        char c = input[i];
        
        switch (ctx->current_state) {
            case STATE_START:
                ctx->current_state = handle_start(c, ctx);
                break;
            case STATE_READING_NUMBER:
                ctx->current_state = handle_reading_number(c, ctx);
                break;
            case STATE_READING_WORD:
                ctx->current_state = handle_reading_word(c, ctx);
                break;
            case STATE_ERROR:
                ctx->current_state = handle_error(c, ctx);
                break;
            case STATE_END:
                return;
        }
        
        if (ctx->current_state == STATE_END) {
            break;
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    parser_context_t parser;
    
    printf("Enter text to parse (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strnlen(input, MAX_INPUT_LEN + 1) > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    initialize_parser(&parser);
    process_input(input, &parser);
    
    printf("Parsing results:\n");
    printf("Numbers found: %d\n", parser.number_count);
    printf("Words found: %d\n", parser.word_count);
    printf("Errors found: %d\n", parser.error_count);
    
    return 0;
}