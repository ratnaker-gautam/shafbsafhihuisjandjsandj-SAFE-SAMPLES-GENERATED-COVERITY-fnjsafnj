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
    if (c == '\0') return EVENT_TERMINATOR;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

State handle_start(Event ev, char c, Context *ctx) {
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

State handle_reading_number(Event ev, char c, Context *ctx) {
    switch (ev) {
        case EVENT_DIGIT:
            if (ctx->token_pos < MAX_INPUT_LEN - 1) {
                ctx->current_token[ctx->token_pos++] = c;
            }
            return STATE_READING_NUMBER;
        case EVENT_SPACE:
        case EVENT_TERMINATOR:
            ctx->current_token[ctx->token_pos] = '\0';
            ctx->numbers_found++;
            printf("Found number: %s\n", ctx->current_token);
            if (ev == EVENT_TERMINATOR) return STATE_END;
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_word(Event ev, char c, Context *ctx) {
    switch (ev) {
        case EVENT_LETTER:
            if (ctx->token_pos < MAX_INPUT_LEN - 1) {
                ctx->current_token[ctx->token_pos++] = c;
            }
            return STATE_READING_WORD;
        case EVENT_SPACE:
        case EVENT_TERMINATOR:
            ctx->current_token[ctx->token_pos] = '\0';
            ctx->words_found++;
            printf("Found word: %s\n", ctx->current_token);
            if (ev == EVENT_TERMINATOR) return STATE_END;
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_error(Context *ctx) {
    printf("Error: Invalid input format\n");
    return STATE_END;
}

void process_input(const char *input) {
    State current_state = STATE_START;
    Context ctx = {0, 0, {0}, 0};
    size_t input_len = strnlen(input, MAX_INPUT_LEN);
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        Event ev = classify_char(c);
        
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
    printf("Summary: %d numbers, %d words\n", ctx.numbers_found, ctx.words_found);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return EXIT_SUCCESS;
    }
    
    process_input(input);
    return EXIT_SUCCESS;
}