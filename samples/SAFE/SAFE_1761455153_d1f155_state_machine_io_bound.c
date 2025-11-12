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
    if (c == '\0') return EVENT_EOF;
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
                ctx->current_word[ctx->word_pos++] = c;
                ctx->current_word[ctx->word_pos] = '\0';
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
                ctx->current_word[ctx->word_pos++] = c;
                ctx->current_word[ctx->word_pos] = '\0';
            }
            return STATE_READING_WORD;
        case EVENT_SPACE:
            ctx->word_count++;
            ctx->word_pos = 0;
            ctx->current_word[0] = '\0';
            return STATE_START;
        case EVENT_EOF:
            ctx->word_count++;
            return STATE_DONE;
        default:
            return STATE_ERROR;
    }
}

State handle_error(Context *ctx) {
    ctx->word_pos = 0;
    ctx->current_word[0] = '\0';
    return STATE_START;
}

int main(void) {
    State current_state = STATE_START;
    Context ctx = {0, 0, 0, {0}, 0};
    char input[MAX_INPUT_LEN];
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len >= MAX_INPUT_LEN - 1) {
        printf("Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; i <= len; i++) {
        char c = input[i];
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
            case STATE_DONE:
                break;
        }
        
        if (current_state == STATE_ERROR) {
            current_state = handle_error(&ctx);
        }
        
        if (current_state == STATE_DONE) {
            break;
        }
    }
    
    if (current_state == STATE_READING_NUMBER) {
        ctx.number_count++;
    } else if (current_state == STATE_READING_WORD) {
        ctx.word_count++;
    }
    
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    
    return 0;
}