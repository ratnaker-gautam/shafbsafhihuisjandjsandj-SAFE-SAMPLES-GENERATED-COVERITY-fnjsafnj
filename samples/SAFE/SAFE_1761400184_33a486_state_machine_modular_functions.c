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
    char current_token[MAX_INPUT_LEN];
    int token_len;
} Context;

Event get_event(char c) {
    if (c == '\0' || c == '\n') return EVENT_EOF;
    if (isspace((unsigned char)c)) return EVENT_SPACE;
    if (isdigit((unsigned char)c)) return EVENT_DIGIT;
    if (isalpha((unsigned char)c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

void reset_token(Context* ctx) {
    memset(ctx->current_token, 0, sizeof(ctx->current_token));
    ctx->token_len = 0;
}

void add_to_token(Context* ctx, char c) {
    if (ctx->token_len < MAX_INPUT_LEN - 1) {
        ctx->current_token[ctx->token_len++] = c;
        ctx->current_token[ctx->token_len] = '\0';
    }
}

State handle_start(Event ev, char c, Context* ctx) {
    switch (ev) {
        case EVENT_DIGIT:
            reset_token(ctx);
            add_to_token(ctx, c);
            return STATE_READING_NUMBER;
        case EVENT_LETTER:
            reset_token(ctx);
            add_to_token(ctx, c);
            return STATE_READING_WORD;
        case EVENT_SPACE:
        case EVENT_EOF:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_number(Event ev, char c, Context* ctx) {
    switch (ev) {
        case EVENT_DIGIT:
            add_to_token(ctx, c);
            return STATE_READING_NUMBER;
        case EVENT_SPACE:
        case EVENT_EOF:
            ctx->numbers_found++;
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_word(Event ev, char c, Context* ctx) {
    switch (ev) {
        case EVENT_LETTER:
        case EVENT_DIGIT:
            add_to_token(ctx, c);
            return STATE_READING_WORD;
        case EVENT_SPACE:
        case EVENT_EOF:
            ctx->words_found++;
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_error(Context* ctx) {
    reset_token(ctx);
    return STATE_END;
}

State process_state(State current, Event ev, char c, Context* ctx) {
    switch (current) {
        case STATE_START:
            return handle_start(ev, c, ctx);
        case STATE_READING_NUMBER:
            return handle_reading_number(ev, c, ctx);
        case STATE_READING_WORD:
            return handle_reading_word(ev, c, ctx);
        case STATE_ERROR:
            return handle_error(ctx);
        default:
            return STATE_ERROR;
    }
}

int validate_input(const char* input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') break;
        if (!isprint((unsigned char)input[i]) && input[i] != '\n') return 0;
    }
    return 1;
}

int main(void) {
    char input[MAX_INPUT_LEN + 2];
    Context ctx = {0, 0, {0}, 0};
    State current_state = STATE_START;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(input, input_len)) {
        printf("Invalid input\n");
        return 1;
    }
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        Event ev = get_event(c);
        State next_state = process_state(current_state, ev, c, &ctx);
        
        if (next_state == STATE_ERROR) {
            printf("Invalid input format\n");
            return 1;
        }
        
        current_state = next_state;
        
        if (current_state == STATE_END) {
            break;
        }
    }
    
    if (current_state == STATE_READING_NUMBER) {
        ctx.numbers_found++;
    } else if (current_state == STATE_READING_WORD) {
        ctx.words_found++;
    }
    
    printf("Numbers found: %d\n", ctx.numbers_found);
    printf("Words found: %d\n", ctx.words_found);
    
    return 0;
}