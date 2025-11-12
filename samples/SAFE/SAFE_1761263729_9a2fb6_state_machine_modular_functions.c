//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

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
    int numbers_found;
    int words_found;
    char current_word[MAX_INPUT_LEN];
    int word_pos;
} Context;

Event get_event(char c) {
    if (c == '\0' || c == '\n') return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

State handle_start(Event ev, char c, Context *ctx) {
    switch (ev) {
        case EVENT_DIGIT:
            return STATE_READING_NUMBER;
        case EVENT_LETTER:
            if (ctx->word_pos < MAX_INPUT_LEN - 1) {
                ctx->current_word[ctx->word_pos++] = c;
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

State handle_reading_word(Event ev, char c, Context *ctx) {
    switch (ev) {
        case EVENT_LETTER:
            if (ctx->word_pos < MAX_INPUT_LEN - 1) {
                ctx->current_word[ctx->word_pos++] = c;
            }
            return STATE_READING_WORD;
        case EVENT_SPACE:
            if (ctx->word_pos > 0) {
                ctx->current_word[ctx->word_pos] = '\0';
                ctx->words_found++;
                ctx->word_pos = 0;
            }
            return STATE_START;
        case EVENT_EOF:
            if (ctx->word_pos > 0) {
                ctx->current_word[ctx->word_pos] = '\0';
                ctx->words_found++;
                ctx->word_pos = 0;
            }
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_error(Context *ctx) {
    ctx->word_pos = 0;
    return STATE_END;
}

void process_input(const char *input) {
    State current_state = STATE_START;
    Context ctx = {0, 0, {0}, 0};
    size_t input_len = strlen(input);
    
    if (input_len >= MAX_INPUT_LEN) {
        printf("Input too long\n");
        return;
    }
    
    for (size_t i = 0; i <= input_len; i++) {
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
            case STATE_END:
                break;
        }
        
        if (current_state == STATE_ERROR) {
            printf("Invalid input format\n");
            return;
        }
        
        if (current_state == STATE_END) {
            break;
        }
    }
    
    printf("Numbers found: %d\n", ctx.numbers_found);
    printf("Words found: %d\n", ctx.words_found);
}

int main(void) {
    char input[MAX_INPUT_LEN];
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    process_input(input);
    
    return 0;
}