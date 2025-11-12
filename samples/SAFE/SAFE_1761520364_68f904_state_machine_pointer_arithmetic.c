//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define MAX_STATES 8

typedef enum {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_PUNCTUATION,
    STATE_END,
    STATE_ERROR
} parser_state_t;

typedef struct {
    parser_state_t current;
    int word_count;
    int number_count;
    int punct_count;
} parser_context_t;

void initialize_parser(parser_context_t *ctx) {
    if (ctx == NULL) return;
    ctx->current = STATE_START;
    ctx->word_count = 0;
    ctx->number_count = 0;
    ctx->punct_count = 0;
}

parser_state_t handle_start(char c, parser_context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (isalpha(c)) {
        ctx->word_count++;
        return STATE_READING_WORD;
    } else if (isdigit(c)) {
        ctx->number_count++;
        return STATE_READING_NUMBER;
    } else if (ispunct(c)) {
        ctx->punct_count++;
        return STATE_PUNCTUATION;
    } else if (c == '\0') {
        return STATE_END;
    }
    return STATE_START;
}

parser_state_t handle_reading_word(char c, parser_context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (isalpha(c)) {
        return STATE_READING_WORD;
    } else if (isdigit(c)) {
        ctx->number_count++;
        return STATE_READING_NUMBER;
    } else if (ispunct(c)) {
        ctx->punct_count++;
        return STATE_PUNCTUATION;
    } else if (c == '\0') {
        return STATE_END;
    }
    return STATE_START;
}

parser_state_t handle_reading_number(char c, parser_context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (isdigit(c)) {
        return STATE_READING_NUMBER;
    } else if (isalpha(c)) {
        ctx->word_count++;
        return STATE_READING_WORD;
    } else if (ispunct(c)) {
        ctx->punct_count++;
        return STATE_PUNCTUATION;
    } else if (c == '\0') {
        return STATE_END;
    }
    return STATE_START;
}

parser_state_t handle_punctuation(char c, parser_context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (isalpha(c)) {
        ctx->word_count++;
        return STATE_READING_WORD;
    } else if (isdigit(c)) {
        ctx->number_count++;
        return STATE_READING_NUMBER;
    } else if (ispunct(c)) {
        return STATE_PUNCTUATION;
    } else if (c == '\0') {
        return STATE_END;
    }
    return STATE_START;
}

int parse_input(const char *input, parser_context_t *ctx) {
    if (input == NULL || ctx == NULL) return -1;
    
    size_t len = strnlen(input, MAX_INPUT);
    if (len >= MAX_INPUT) return -1;
    
    const char *ptr = input;
    const char *end = ptr + len;
    
    while (ptr < end) {
        char current = *ptr;
        
        switch (ctx->current) {
            case STATE_START:
                ctx->current = handle_start(current, ctx);
                break;
            case STATE_READING_WORD:
                ctx->current = handle_reading_word(current, ctx);
                break;
            case STATE_READING_NUMBER:
                ctx->current = handle_reading_number(current, ctx);
                break;
            case STATE_PUNCTUATION:
                ctx->current = handle_punctuation(current, ctx);
                break;
            case STATE_END:
                return 0;
            case STATE_ERROR:
                return -1;
        }
        
        if (ctx->current == STATE_ERROR) return -1;
        ptr++;
    }
    
    ctx->current = STATE_END;
    return 0;
}

int main(void) {
    char input[MAX_INPUT + 1] = {0};
    parser_context_t parser;
    
    printf("Enter text to analyze: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strnlen(input, MAX_INPUT);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len >= MAX_INPUT) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    initialize_parser(&parser);
    
    if (parse_input(input, &parser) != 0) {
        fprintf(stderr, "Error parsing input\n");
        return 1;
    }
    
    printf("Analysis results:\n");
    printf("Words: %d\n", parser.word_count);
    printf("Numbers: %d\n", parser.number_count);
    printf("Punctuation sequences: %d\n", parser.punct_count);
    
    return 0;
}