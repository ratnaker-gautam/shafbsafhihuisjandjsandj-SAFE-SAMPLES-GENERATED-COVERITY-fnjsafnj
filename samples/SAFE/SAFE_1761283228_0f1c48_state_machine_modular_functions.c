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
    EVENT_TERMINATOR,
    EVENT_INVALID
} Event;

typedef struct {
    int numbers_found;
    int words_found;
    char current_token[MAX_INPUT_LEN];
    int token_pos;
} Context;

Event classify_char(char c) {
    if (c == '\0') return EVENT_TERMINATOR;
    if (c == ' ' || c == '\t' || c == '\n') return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

State handle_start(Event ev, char c, Context* ctx) {
    switch (ev) {
        case EVENT_DIGIT:
            ctx->current_token[0] = c;
            ctx->token_pos = 1;
            return STATE_READING_NUMBER;
        case EVENT_LETTER:
            ctx->current_token[0] = c;
            ctx->token_pos = 1;
            return STATE_READING_WORD;
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_TERMINATOR:
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_number(Event ev, char c, Context* ctx) {
    switch (ev) {
        case EVENT_DIGIT:
            if (ctx->token_pos < MAX_INPUT_LEN - 1) {
                ctx->current_token[ctx->token_pos++] = c;
                return STATE_READING_NUMBER;
            }
            return STATE_ERROR;
        case EVENT_SPACE:
            ctx->current_token[ctx->token_pos] = '\0';
            ctx->numbers_found++;
            return STATE_START;
        case EVENT_TERMINATOR:
            ctx->current_token[ctx->token_pos] = '\0';
            ctx->numbers_found++;
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_word(Event ev, char c, Context* ctx) {
    switch (ev) {
        case EVENT_LETTER:
            if (ctx->token_pos < MAX_INPUT_LEN - 1) {
                ctx->current_token[ctx->token_pos++] = c;
                return STATE_READING_WORD;
            }
            return STATE_ERROR;
        case EVENT_SPACE:
            ctx->current_token[ctx->token_pos] = '\0';
            ctx->words_found++;
            return STATE_START;
        case EVENT_TERMINATOR:
            ctx->current_token[ctx->token_pos] = '\0';
            ctx->words_found++;
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State process_event(State current, Event ev, char c, Context* ctx) {
    switch (current) {
        case STATE_START:
            return handle_start(ev, c, ctx);
        case STATE_READING_NUMBER:
            return handle_reading_number(ev, c, ctx);
        case STATE_READING_WORD:
            return handle_reading_word(ev, c, ctx);
        case STATE_ERROR:
        case STATE_END:
            return current;
        default:
            return STATE_ERROR;
    }
}

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isalnum(input[i]) && input[i] != ' ' && input[i] != '\t' && input[i] != '\n') {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    Context ctx = {0, 0, {0}, 0};
    State current_state = STATE_START;
    
    printf("Enter text (alphanumeric and spaces only): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (!validate_input(input)) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    for (size_t i = 0; i < strlen(input) + 1; i++) {
        char c = input[i];
        Event ev = classify_char(c);
        current_state = process_event(current_state, ev, c, &ctx);
        
        if (current_state == STATE_ERROR) {
            fprintf(stderr, "Error processing input at position %zu\n", i);
            return 1;
        }
        
        if (current_state == STATE_END) {
            break;
        }
    }
    
    printf("Numbers found: %d\n", ctx.numbers_found);
    printf("Words found: %d\n", ctx.words_found);
    
    return 0;
}