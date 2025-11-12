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
    EVENT_EOF,
    EVENT_INVALID
} Event;

typedef struct {
    int number_count;
    int word_count;
    int current_number;
    char current_word[MAX_INPUT_LEN];
} Context;

Event get_event(char c) {
    if (c == EOF) return EVENT_EOF;
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
            if (ctx->current_word[0] == '\0') {
                ctx->current_word[0] = c;
                ctx->current_word[1] = '\0';
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

State handle_reading_number(Event ev, char c, Context *ctx) {
    switch (ev) {
        case EVENT_DIGIT:
            if (ctx->current_number <= (INT_MAX - (c - '0')) / 10) {
                ctx->current_number = ctx->current_number * 10 + (c - '0');
            } else {
                return STATE_ERROR;
            }
            return STATE_READING_NUMBER;
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

State handle_reading_word(Event ev, char c, Context *ctx) {
    size_t len = strlen(ctx->current_word);
    switch (ev) {
        case EVENT_LETTER:
            if (len < MAX_INPUT_LEN - 1) {
                ctx->current_word[len] = c;
                ctx->current_word[len + 1] = '\0';
            } else {
                return STATE_ERROR;
            }
            return STATE_READING_WORD;
        case EVENT_SPACE:
            ctx->word_count++;
            ctx->current_word[0] = '\0';
            return STATE_START;
        case EVENT_EOF:
            ctx->word_count++;
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_error(Context *ctx) {
    ctx->current_word[0] = '\0';
    ctx->current_number = 0;
    return STATE_ERROR;
}

void process_input(Context *ctx) {
    State current_state = STATE_START;
    char input_buffer[MAX_INPUT_LEN + 2];
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        return;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    for (size_t i = 0; i <= input_len; i++) {
        char current_char = (i < input_len) ? input_buffer[i] : ' ';
        Event event = get_event(current_char);
        
        switch (current_state) {
            case STATE_START:
                current_state = handle_start(event, current_char, ctx);
                break;
            case STATE_READING_NUMBER:
                current_state = handle_reading_number(event, current_char, ctx);
                break;
            case STATE_READING_WORD:
                current_state = handle_reading_word(event, current_char, ctx);
                break;
            case STATE_ERROR:
                current_state = handle_error(ctx);
                break;
            case STATE_END:
                return;
        }
        
        if (current_state == STATE_ERROR) {
            break;
        }
    }
}

void initialize_context(Context *ctx) {
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->current_number = 0;
    ctx->current_word[0] = '\0';
}

int main(void) {
    Context ctx;
    initialize_context(&ctx);
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    process_input(&ctx);
    
    if (ctx.current_word[0] != '\0' && strlen(ctx.current_word) > 0) {
        ctx.word_count++;
    }
    
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    
    return 0;
}