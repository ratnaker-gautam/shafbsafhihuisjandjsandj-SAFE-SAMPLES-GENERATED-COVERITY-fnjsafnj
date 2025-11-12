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
    int number_count;
    int word_count;
    int current_number;
    char current_word[MAX_INPUT_LEN];
} context_t;

static event_t get_event(int c) {
    if (c == EOF) return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

static state_t handle_start(event_t event, int c, context_t *ctx) {
    switch (event) {
        case EVENT_DIGIT:
            ctx->current_number = c - '0';
            return STATE_READING_NUMBER;
        case EVENT_LETTER:
            ctx->current_word[0] = (char)c;
            ctx->current_word[1] = '\0';
            return STATE_READING_WORD;
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_EOF:
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

static state_t handle_reading_number(event_t event, int c, context_t *ctx) {
    switch (event) {
        case EVENT_DIGIT:
            if (ctx->current_number <= (INT_MAX - (c - '0')) / 10) {
                ctx->current_number = ctx->current_number * 10 + (c - '0');
                return STATE_READING_NUMBER;
            }
            return STATE_ERROR;
        case EVENT_SPACE:
            ctx->number_count++;
            return STATE_START;
        case EVENT_EOF:
            ctx->number_count++;
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

static state_t handle_reading_word(event_t event, int c, context_t *ctx) {
    size_t len = strlen(ctx->current_word);
    if (len >= MAX_INPUT_LEN - 1) return STATE_ERROR;
    
    switch (event) {
        case EVENT_LETTER:
            ctx->current_word[len] = (char)c;
            ctx->current_word[len + 1] = '\0';
            return STATE_READING_WORD;
        case EVENT_SPACE:
            ctx->word_count++;
            return STATE_START;
        case EVENT_EOF:
            ctx->word_count++;
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

static state_t handle_error(context_t *ctx) {
    ctx->number_count = 0;
    ctx->word_count = 0;
    return STATE_END;
}

static void print_results(const context_t *ctx) {
    printf("Numbers found: %d\n", ctx->number_count);
    printf("Words found: %d\n", ctx->word_count);
}

int main(void) {
    state_t current_state = STATE_START;
    context_t context = {0, 0, 0, {0}};
    
    printf("Enter text (max %d chars per line): ", MAX_INPUT_LEN);
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        int c = getchar();
        if (c == EOF && feof(stdin)) break;
        
        event_t event = get_event(c);
        
        switch (current_state) {
            case STATE_START:
                current_state = handle_start(event, c, &context);
                break;
            case STATE_READING_NUMBER:
                current_state = handle_reading_number(event, c, &context);
                break;
            case STATE_READING_WORD:
                current_state = handle_reading_word(event, c, &context);
                break;
            case STATE_ERROR:
                current_state = handle_error(&context);
                break;
            default:
                current_state = STATE_ERROR;
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        handle_error(&context);
        printf("Error: Invalid input format\n");
    } else {
        print_results(&context);
    }
    
    return 0;
}