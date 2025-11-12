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
    EVENT_OTHER
} Event;

typedef struct {
    int numbers_found;
    int words_found;
    char current_token[MAX_INPUT_LEN];
    int token_len;
} Context;

Event get_event(char c) {
    if (c == '\0' || c == EOF) return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_OTHER;
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
            return STATE_START;
        case EVENT_EOF:
            return STATE_END;
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
            ctx->numbers_found++;
            printf("Found number: %s\n", ctx->current_token);
            return STATE_START;
        case EVENT_EOF:
            ctx->numbers_found++;
            printf("Found number: %s\n", ctx->current_token);
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_word(Event ev, char c, Context* ctx) {
    switch (ev) {
        case EVENT_LETTER:
            add_to_token(ctx, c);
            return STATE_READING_WORD;
        case EVENT_SPACE:
            ctx->words_found++;
            printf("Found word: %s\n", ctx->current_token);
            return STATE_START;
        case EVENT_EOF:
            ctx->words_found++;
            printf("Found word: %s\n", ctx->current_token);
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_error(Context* ctx) {
    printf("Error: Invalid input format\n");
    reset_token(ctx);
    return STATE_END;
}

void process_input(const char* input) {
    State current_state = STATE_START;
    Context ctx = {0, 0, {0}, 0};
    size_t input_len = strlen(input);
    
    if (input_len >= MAX_INPUT_LEN) {
        printf("Error: Input too long\n");
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
                goto done;
        }
        
        if (current_state == STATE_ERROR) {
            current_state = handle_error(&ctx);
            break;
        }
    }
    
done:
    printf("Summary: %d numbers, %d words found\n", ctx.numbers_found, ctx.words_found);
}

int main() {
    char input[MAX_INPUT_LEN + 1] = {0};
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
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