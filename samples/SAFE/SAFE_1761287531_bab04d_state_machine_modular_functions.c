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
    if (isspace((unsigned char)c)) return EVENT_SPACE;
    if (isdigit((unsigned char)c)) return EVENT_DIGIT;
    if (isalpha((unsigned char)c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

void reset_token(Context *ctx) {
    memset(ctx->current_token, 0, sizeof(ctx->current_token));
    ctx->token_pos = 0;
}

void add_to_token(Context *ctx, char c) {
    if (ctx->token_pos < MAX_INPUT_LEN - 1) {
        ctx->current_token[ctx->token_pos++] = c;
        ctx->current_token[ctx->token_pos] = '\0';
    }
}

State handle_start(Event ev, char c, Context *ctx) {
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
            add_to_token(ctx, c);
            return STATE_READING_NUMBER;
        case EVENT_SPACE:
            ctx->numbers_found++;
            return STATE_START;
        case EVENT_TERMINATOR:
            ctx->numbers_found++;
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_word(Event ev, char c, Context *ctx) {
    switch (ev) {
        case EVENT_LETTER:
            add_to_token(ctx, c);
            return STATE_READING_WORD;
        case EVENT_SPACE:
            ctx->words_found++;
            return STATE_START;
        case EVENT_TERMINATOR:
            ctx->words_found++;
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_error(Context *ctx) {
    reset_token(ctx);
    return STATE_ERROR;
}

State process_event(State current_state, Event ev, char c, Context *ctx) {
    switch (current_state) {
        case STATE_START:
            return handle_start(ev, c, ctx);
        case STATE_READING_NUMBER:
            return handle_reading_number(ev, c, ctx);
        case STATE_READING_WORD:
            return handle_reading_word(ev, c, ctx);
        case STATE_ERROR:
            return handle_error(ctx);
        case STATE_END:
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    return 1;
}

void run_state_machine(const char *input) {
    if (!validate_input(input)) {
        printf("Invalid input\n");
        return;
    }

    Context ctx;
    memset(&ctx, 0, sizeof(ctx));
    reset_token(&ctx);
    
    State current_state = STATE_START;
    size_t pos = 0;
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        char c = input[pos];
        Event ev = classify_char(c);
        State next_state = process_event(current_state, ev, c, &ctx);
        
        if (next_state == STATE_ERROR) {
            printf("Error: Invalid character '%c' at position %zu\n", c, pos);
            break;
        }
        
        current_state = next_state;
        pos++;
        
        if (pos >= MAX_INPUT_LEN) {
            printf("Error: Input too long\n");
            break;
        }
    }
    
    if (current_state == STATE_END) {
        printf("Processing complete. Found %d numbers and %d words.\n", 
               ctx.numbers_found, ctx.words_found);
    }
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
    
    run_state_machine(input);
    return 0;
}