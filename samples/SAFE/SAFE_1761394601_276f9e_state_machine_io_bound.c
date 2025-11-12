//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
    char current_token[MAX_INPUT_LEN];
    int token_len;
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
            ctx->current_token[0] = c;
            ctx->token_len = 1;
            return STATE_READING_NUMBER;
        case EVENT_LETTER:
            ctx->current_token[0] = c;
            ctx->token_len = 1;
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
            if (ctx->token_len < MAX_INPUT_LEN - 1) {
                ctx->current_token[ctx->token_len++] = c;
            }
            return STATE_READING_NUMBER;
        case EVENT_SPACE:
        case EVENT_EOF:
            ctx->current_token[ctx->token_len] = '\0';
            ctx->number_count++;
            printf("Number: %s\n", ctx->current_token);
            if (ev == EVENT_EOF) return STATE_DONE;
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_word(Event ev, char c, Context *ctx) {
    switch (ev) {
        case EVENT_LETTER:
            if (ctx->token_len < MAX_INPUT_LEN - 1) {
                ctx->current_token[ctx->token_len++] = c;
            }
            return STATE_READING_WORD;
        case EVENT_SPACE:
        case EVENT_EOF:
            ctx->current_token[ctx->token_len] = '\0';
            ctx->word_count++;
            printf("Word: %s\n", ctx->current_token);
            if (ev == EVENT_EOF) return STATE_DONE;
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_error(Context *ctx) {
    printf("Error: Invalid input format\n");
    return STATE_DONE;
}

int main(void) {
    State current_state = STATE_START;
    Context ctx = {0, 0, {0}, 0};
    char input_buffer[MAX_INPUT_LEN + 2];
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        printf("Error: Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input_buffer[i];
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
        
        if (current_state == STATE_ERROR || current_state == STATE_DONE) {
            break;
        }
    }
    
    if (current_state == STATE_READING_NUMBER || current_state == STATE_READING_WORD) {
        if (ctx.token_len > 0) {
            ctx.current_token[ctx.token_len] = '\0';
            if (current_state == STATE_READING_NUMBER) {
                ctx.number_count++;
                printf("Number: %s\n", ctx.current_token);
            } else {
                ctx.word_count++;
                printf("Word: %s\n", ctx.current_token);
            }
        }
    }
    
    printf("\nSummary: %d numbers, %d words\n", ctx.number_count, ctx.word_count);
    
    return 0;
}