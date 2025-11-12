//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_DONE
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
    int word_pos;
} Context;

Event get_event(int c) {
    if (c == EOF) return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

State handle_start(Event ev, int c, Context *ctx) {
    switch (ev) {
        case EVENT_DIGIT:
            ctx->current_number = c - '0';
            return STATE_READING_NUMBER;
        case EVENT_LETTER:
            if (ctx->word_pos < MAX_INPUT_LEN - 1) {
                ctx->current_word[0] = (char)c;
                ctx->word_pos = 1;
                return STATE_READING_WORD;
            }
            return STATE_ERROR;
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_EOF:
            return STATE_DONE;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_number(Event ev, int c, Context *ctx) {
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
        case EVENT_EOF:
            ctx->number_count++;
            return STATE_DONE;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_word(Event ev, int c, Context *ctx) {
    switch (ev) {
        case EVENT_LETTER:
            if (ctx->word_pos < MAX_INPUT_LEN - 1) {
                ctx->current_word[ctx->word_pos++] = (char)c;
                return STATE_READING_WORD;
            } else {
                return STATE_ERROR;
            }
        case EVENT_SPACE:
            if (ctx->word_pos > 0) {
                ctx->current_word[ctx->word_pos] = '\0';
                ctx->word_count++;
            }
            return STATE_START;
        case EVENT_EOF:
            if (ctx->word_pos > 0) {
                ctx->current_word[ctx->word_pos] = '\0';
                ctx->word_count++;
            }
            return STATE_DONE;
        default:
            return STATE_ERROR;
    }
}

int main(void) {
    State current_state = STATE_START;
    Context ctx = {0, 0, 0, {0}, 0};
    int c;
    
    printf("Enter text (numbers and words): ");
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        c = getchar();
        if (c == EOF) {
            if (feof(stdin)) {
                c = EOF;
            } else {
                current_state = STATE_ERROR;
                break;
            }
        }
        
        Event ev = get_event(c);
        State next_state = current_state;
        
        switch (current_state) {
            case STATE_START:
                next_state = handle_start(ev, c, &ctx);
                break;
            case STATE_READING_NUMBER:
                next_state = handle_reading_number(ev, c, &ctx);
                break;
            case STATE_READING_WORD:
                next_state = handle_reading_word(ev, c, &ctx);
                break;
            case STATE_ERROR:
            case STATE_DONE:
                break;
        }
        
        current_state = next_state;
    }
    
    if (current_state == STATE_ERROR) {
        printf("\nError: Invalid input format\n");
        return 1;
    }
    
    printf("\nResults: %d numbers, %d words\n", ctx.number_count, ctx.word_count);
    
    return 0;
}