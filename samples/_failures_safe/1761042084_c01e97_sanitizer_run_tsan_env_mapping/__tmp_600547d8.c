//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: state_machine
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
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    if (c == ' ' || c == '\t') return EVENT_SPACE;
    if (c == '\0' || c == '\n') return EVENT_TERMINATOR;
    return EVENT_INVALID;
}

State handle_start(Event ev, Context* ctx) {
    switch (ev) {
        case EVENT_DIGIT:
            ctx->token_pos = 0;
            ctx->current_token[ctx->token_pos++] = '0';
            return STATE_READING_NUMBER;
        case EVENT_LETTER:
            ctx->token_pos = 0;
            ctx->current_token[ctx->token_pos++] = 'a';
            return STATE_READING_WORD;
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_TERMINATOR:
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_number(Event ev, Context* ctx) {
    switch (ev) {
        case EVENT_DIGIT:
            if (ctx->token_pos < MAX_INPUT_LEN - 1) {
                ctx->current_token[ctx->token_pos++] = '0';
            }
            return STATE_READING_NUMBER;
        case EVENT_SPACE:
            ctx->numbers_found++;
            ctx->token_pos = 0;
            return STATE_START;
        case EVENT_TERMINATOR:
            ctx->numbers_found++;
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_word(Event ev, Context* ctx) {
    switch (ev) {
        case EVENT_LETTER:
            if (ctx->token_pos < MAX_INPUT_LEN - 1) {
                ctx->current_token[ctx->token_pos++] = 'a';
            }
            return STATE_READING_WORD;
        case EVENT_SPACE:
            ctx->words_found++;
            ctx->token_pos = 0;
            return STATE_START;
        case EVENT_TERMINATOR:
            ctx->words_found++;
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_error(Context* ctx) {
    ctx->token_pos = 0;
    return STATE_ERROR;
}

State process_event(State current, Event ev, Context* ctx) {
    switch (current) {
        case STATE_START:
            return handle_start(ev, ctx);
        case STATE_READING_NUMBER:
            return handle_reading_number(ev, ctx);
        case STATE_READING_WORD:
            return handle_reading_word(ev, ctx);
        case STATE_ERROR:
            return handle_error(ctx);
        case STATE_END:
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

int validate_input(const char* input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') break;
        Event ev = classify_char(input[i]);
        if (ev == EVENT_INVALID) return 0;
    }
    return 1;
}

void run_state_machine(const char* input) {
    Context ctx = {0, 0, {0}, 0};
    State current_state = STATE_START;
    size_t input_len = strnlen(input, MAX_INPUT_LEN);
    
    if (!validate_input(input, input_len)) {
        printf("Invalid input\n");
        return;
    }
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        Event ev = classify_char(c);
        current_state = process_event(current_state, ev, &ctx);
        
        if (current_state == STATE_ERROR) {
            printf("Processing error at position %zu\n", i);
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
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t len = strnlen(input, sizeof(input));
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    run_state_machine(input);
    return 0;
}