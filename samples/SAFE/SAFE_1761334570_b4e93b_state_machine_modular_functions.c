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

static event_t get_next_event(char c) {
    if (c == EOF) return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

static state_t handle_start(event_t event, char c, context_t *ctx) {
    switch (event) {
        case EVENT_DIGIT:
            ctx->current_number = c - '0';
            return STATE_READING_NUMBER;
        case EVENT_LETTER:
            if (ctx->word_pos < MAX_INPUT_LEN - 1) {
                ctx->current_word[ctx->word_pos++] = c;
                ctx->current_word[ctx->word_pos] = '\0';
            }
            return STATE_READING_WORD;
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_EOF:
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

static state_t handle_reading_number(event_t event, char c, context_t *ctx) {
    switch (event) {
        case EVENT_DIGIT:
            if (ctx->current_number <= (INT_MAX - (c - '0')) / 10) {
                ctx->current_number = ctx->current_number * 10 + (c - '0');
            }
            return STATE_READING_NUMBER;
        case EVENT_SPACE:
            ctx->numbers_found++;
            return STATE_START;
        case EVENT_EOF:
            ctx->numbers_found++;
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

static state_t handle_reading_word(event_t event, char c, context_t *ctx) {
    switch (event) {
        case EVENT_LETTER:
            if (ctx->word_pos < MAX_INPUT_LEN - 1) {
                ctx->current_word[ctx->word_pos++] = c;
                ctx->current_word[ctx->word_pos] = '\0';
            }
            return STATE_READING_WORD;
        case EVENT_SPACE:
            ctx->words_found++;
            ctx->word_pos = 0;
            ctx->current_word[0] = '\0';
            return STATE_START;
        case EVENT_EOF:
            ctx->words_found++;
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

static state_t handle_error(context_t *ctx) {
    ctx->word_pos = 0;
    ctx->current_word[0] = '\0';
    ctx->current_number = 0;
    return STATE_START;
}

static void process_input(void) {
    state_t current_state = STATE_START;
    context_t ctx = {0};
    char input[MAX_INPUT_LEN];
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    for (size_t i = 0; i <= len; i++) {
        char c = (i < len) ? input[i] : EOF;
        event_t event = get_next_event(c);
        state_t next_state = current_state;
        
        switch (current_state) {
            case STATE_START:
                next_state = handle_start(event, c, &ctx);
                break;
            case STATE_READING_NUMBER:
                next_state = handle_reading_number(event, c, &ctx);
                break;
            case STATE_READING_WORD:
                next_state = handle_reading_word(event, c, &ctx);
                break;
            case STATE_ERROR:
                next_state = handle_error(&ctx);
                break;
            case STATE_END:
                break;
        }
        
        if (next_state == STATE_ERROR) {
            printf("Invalid input at position %zu\n", i + 1);
            next_state = handle_error(&ctx);
        }
        
        current_state = next_state;
        if (current_state == STATE_END) break;
    }
    
    printf("Numbers found: %d\n", ctx.numbers_found);
    printf("Words found: %d\n", ctx.words_found);
}

int main(void) {
    process_input();
    return 0;
}