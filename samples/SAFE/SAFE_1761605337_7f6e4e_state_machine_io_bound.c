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
            if (ctx->word_pos < MAX_INPUT_LEN - 1) {
                ctx->current_word[ctx->word_pos++] = tolower(c);
            }
            return STATE_READING_WORD;
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_EOF:
            return STATE_DONE;
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
            return STATE_DONE;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_word(Event ev, char c, Context *ctx) {
    switch (ev) {
        case EVENT_LETTER:
            if (ctx->word_pos < MAX_INPUT_LEN - 1) {
                ctx->current_word[ctx->word_pos++] = tolower(c);
            }
            return STATE_READING_WORD;
        case EVENT_SPACE:
            if (ctx->word_pos > 0) {
                ctx->current_word[ctx->word_pos] = '\0';
                ctx->word_count++;
                ctx->word_pos = 0;
            }
            return STATE_START;
        case EVENT_EOF:
            if (ctx->word_pos > 0) {
                ctx->current_word[ctx->word_pos] = '\0';
                ctx->word_count++;
                ctx->word_pos = 0;
            }
            return STATE_DONE;
        default:
            return STATE_ERROR;
    }
}

State handle_error(Context *ctx) {
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->word_pos = 0;
    return STATE_DONE;
}

void process_input(FILE *input) {
    State current_state = STATE_START;
    Context ctx = {0, 0, 0, {0}, 0};
    char c;
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        int result = fgetc(input);
        if (result == EOF) {
            c = EOF;
        } else {
            c = (char)result;
        }
        
        Event ev = get_event(c);
        
        switch (current_state) {
            case STATE_START:
                current_state = handle_start(ev, c, &ctx);
                break;
            case STATE_READING_NUMBER:
                current_state = handle_reading_number(ev, c, &ctx);
                break;
            case STATE_READING_WORD:
                current_state = handle_reading_word(ev, c, &ctx);
                break;
            case STATE_ERROR:
                current_state = handle_error(&ctx);
                break;
            default:
                current_state = STATE_ERROR;
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input format\n");
    } else {
        printf("Numbers: %d\nWords: %d\n", ctx.number_count, ctx.word_count);
    }
}

int main(void) {
    printf("Enter text (Ctrl+D to end):\n");
    process_input(stdin);
    return 0;
}