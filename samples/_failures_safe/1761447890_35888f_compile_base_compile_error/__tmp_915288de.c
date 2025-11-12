//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_END
} state_t;

typedef enum {
    EVENT_DIGIT,
    EVENT_LETTER,
    EVENT_SPACE,
    EVENT_EOF,
    EVENT_INVALID
} event_t;

typedef struct {
    int numbers_found;
    int words_found;
    int current_number;
    char current_word[MAX_INPUT_LEN];
    int word_pos;
} context_t;

static event_t classify_char(char c) {
    if (c == '\0' || c == '\n') return EVENT_EOF;
    if (isspace((unsigned char)c)) return EVENT_SPACE;
    if (isdigit((unsigned char)c)) return EVENT_DIGIT;
    if (isalpha((unsigned char)c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

static state_t handle_start(event_t event, char c, context_t *ctx) {
    if (event == EVENT_DIGIT) {
        ctx->current_number = c - '0';
        return STATE_READING_NUMBER;
    } else if (event == EVENT_LETTER) {
        ctx->current_word[0] = c;
        ctx->word_pos = 1;
        return STATE_READING_WORD;
    } else if (event == EVENT_SPACE || event == EVENT_EOF) {
        return STATE_START;
    }
    return STATE_ERROR;
}

static state_t handle_reading_number(event_t event, char c, context_t *ctx) {
    if (event == EVENT_DIGIT) {
        if (ctx->current_number <= (INT_MAX - (c - '0')) / 10) {
            ctx->current_number = ctx->current_number * 10 + (c - '0');
            return STATE_READING_NUMBER;
        }
        return STATE_ERROR;
    } else if (event == EVENT_SPACE || event == EVENT_EOF) {
        ctx->numbers_found++;
        printf("Found number: %d\n", ctx->current_number);
        return (event == EVENT_EOF) ? STATE_END : STATE_START;
    }
    return STATE_ERROR;
}

static state_t handle_reading_word(event_t event, char c, context_t *ctx) {
    if (event == EVENT_LETTER) {
        if (ctx->word_pos < MAX_INPUT_LEN - 1) {
            ctx->current_word[ctx->word_pos++] = c;
            return STATE_READING_WORD;
        }
        return STATE_ERROR;
    } else if (event == EVENT_SPACE || event == EVENT_EOF) {
        ctx->current_word[ctx->word_pos] = '\0';
        ctx->words_found++;
        printf("Found word: %s\n", ctx->current_word);
        return (event == EVENT_EOF) ? STATE_END : STATE_START;
    }
    return STATE_ERROR;
}

static state_t handle_error(context_t *ctx) {
    printf("Error: Invalid input format\n");
    return STATE_END;
}

static void process_input(const char *input) {
    state_t current_state = STATE_START;
    context_t ctx = {0, 0, 0, {0}, 0};
    size_t input_len = strnlen(input, MAX_INPUT_LEN * 2);
    
    for (size_t i = 0; i <= input_len && current_state != STATE_END; i++) {
        char c = input[i];
        event_t event = classify_char(c);
        
        switch (current_state) {
            case STATE_START:
                current_state = handle_start(event, c, &ctx);
                break;
            case STATE_READING_NUMBER:
                current_state = handle_reading_number(event, c, &ctx);
                break;
            case STATE_READING_WORD:
                current_state = handle_reading_word(event, c, &ctx);
                break;
            case STATE_ERROR:
                current_state = handle_error(&ctx);
                break;
            case STATE_END:
                break;
        }
    }
    
    printf("Summary: %d numbers, %d words\n", ctx.numbers_found, ctx.words_found);
}

static int read_input_safe(char *buffer, size_t buffer_size) {
    if (buffer == NULL || buffer_size == 0) return 0;
    
    if (fgets(buffer, (int)buffer_size, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strnlen(buffer, buffer_size);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    } else if (len == buffer_size - 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
    
    return 1;
}

int main(void) {
    char input[MAX_INPUT_LEN + 2];
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (!read_input_safe(input, sizeof(input))) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (strnlen(input, sizeof(input)) == 0) {
        fprintf(stderr, "Error: Empty input\n");
        return 1;
    }