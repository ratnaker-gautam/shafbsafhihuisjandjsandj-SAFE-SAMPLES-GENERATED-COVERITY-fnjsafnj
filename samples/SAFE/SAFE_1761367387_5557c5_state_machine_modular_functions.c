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
} State;

typedef enum {
    EVENT_DIGIT,
    EVENT_LETTER,
    EVENT_SPACE,
    EVENT_TERMINATOR,
    EVENT_INVALID
} Event;

typedef struct {
    int number_count;
    int word_count;
    int current_number;
    char current_word[MAX_INPUT_LEN];
    int word_pos;
} Context;

Event get_event(char c) {
    if (c == '\0' || c == '\n') return EVENT_TERMINATOR;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

State handle_start(Event ev, char c, Context *ctx) {
    switch (ev) {
        case EVENT_DIGIT:
            ctx->current_number = c - '0';
            return STATE_READING_NUMBER;
        case EVENT_LETTER:
            ctx->current_word[0] = c;
            ctx->word_pos = 1;
            return STATE_READING_WORD;
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_TERMINATOR:
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_number(Event ev, char c, Context *ctx) {
    switch (ev) {
        case EVENT_DIGIT:
            if (ctx->current_number > (INT_MAX - (c - '0')) / 10) {
                return STATE_ERROR;
            }
            ctx->current_number = ctx->current_number * 10 + (c - '0');
            return STATE_READING_NUMBER;
        case EVENT_SPACE:
            ctx->number_count++;
            return STATE_START;
        case EVENT_TERMINATOR:
            ctx->number_count++;
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_word(Event ev, char c, Context *ctx) {
    switch (ev) {
        case EVENT_LETTER:
            if (ctx->word_pos < MAX_INPUT_LEN - 1) {
                ctx->current_word[ctx->word_pos++] = c;
                return STATE_READING_WORD;
            } else {
                return STATE_ERROR;
            }
        case EVENT_SPACE:
            ctx->current_word[ctx->word_pos] = '\0';
            ctx->word_count++;
            return STATE_START;
        case EVENT_TERMINATOR:
            ctx->current_word[ctx->word_pos] = '\0';
            ctx->word_count++;
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_error(Context *ctx) {
    ctx->number_count = 0;
    ctx->word_count = 0;
    return STATE_END;
}

void initialize_context(Context *ctx) {
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->current_number = 0;
    ctx->word_pos = 0;
    memset(ctx->current_word, 0, sizeof(ctx->current_word));
}

void process_input(const char *input, Context *ctx) {
    State current_state = STATE_START;
    size_t input_len = strlen(input);
    
    if (input_len >= MAX_INPUT_LEN) {
        handle_error(ctx);
        return;
    }
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        Event ev = get_event(c);
        
        switch (current_state) {
            case STATE_START:
                current_state = handle_start(ev, c, ctx);
                break;
            case STATE_READING_NUMBER:
                current_state = handle_reading_number(ev, c, ctx);
                break;
            case STATE_READING_WORD:
                current_state = handle_reading_word(ev, c, ctx);
                break;
            case STATE_ERROR:
                current_state = handle_error(ctx);
                break;
            case STATE_END:
                return;
        }
        
        if (current_state == STATE_ERROR) {
            handle_error(ctx);
            return;
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 2];
    Context ctx;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    } else if (len >= MAX_INPUT_LEN) {
        printf("Input too long\n");
        return 1;
    }
    
    initialize_context(&ctx);
    process_input(input, &ctx);
    
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    
    return 0;
}