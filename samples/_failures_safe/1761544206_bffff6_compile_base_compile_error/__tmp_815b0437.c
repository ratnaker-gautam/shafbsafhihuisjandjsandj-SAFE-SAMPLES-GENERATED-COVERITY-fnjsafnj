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
    int token_pos;
} Context;

Event get_event(char c) {
    if (c == '\0') return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

void reset_token(Context* ctx) {
    if (ctx == NULL) return;
    memset(ctx->current_token, 0, sizeof(ctx->current_token));
    ctx->token_pos = 0;
}

void add_to_token(Context* ctx, char c) {
    if (ctx == NULL) return;
    if (ctx->token_pos < MAX_INPUT_LEN - 1) {
        ctx->current_token[ctx->token_pos++] = c;
        ctx->current_token[ctx->token_pos] = '\0';
    }
}

int is_valid_number(const char* token) {
    if (token == NULL || token[0] == '\0') return 0;
    for (int i = 0; token[i] != '\0'; i++) {
        if (!isdigit(token[i])) return 0;
    }
    return 1;
}

int is_valid_word(const char* token) {
    if (token == NULL || token[0] == '\0') return 0;
    for (int i = 0; token[i] != '\0'; i++) {
        if (!isalpha(token[i])) return 0;
    }
    return 1;
}

State handle_start(Event ev, char c, Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
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
    if (ctx == NULL) return STATE_ERROR;
    switch (ev) {
        case EVENT_DIGIT:
            add_to_token(ctx, c);
            return STATE_READING_NUMBER;
        case EVENT_SPACE:
            if (is_valid_number(ctx->current_token)) {
                ctx->numbers_found++;
                printf("Found number: %s\n", ctx->current_token);
            }
            reset_token(ctx);
            return STATE_START;
        case EVENT_EOF:
            if (is_valid_number(ctx->current_token)) {
                ctx->numbers_found++;
                printf("Found number: %s\n", ctx->current_token);
            }
            reset_token(ctx);
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_word(Event ev, char c, Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    switch (ev) {
        case EVENT_LETTER:
            add_to_token(ctx, c);
            return STATE_READING_WORD;
        case EVENT_SPACE:
            if (is_valid_word(ctx->current_token)) {
                ctx->words_found++;
                printf("Found word: %s\n", ctx->current_token);
            }
            reset_token(ctx);
            return STATE_START;
        case EVENT_EOF:
            if (is_valid_word(ctx->current_token)) {
                ctx->words_found++;
                printf("Found word: %s\n", ctx->current_token);
            }
            reset_token(ctx);
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

void run_state_machine() {
    State current_state = STATE_START;
    Context ctx = {0};
    char input_buffer[MAX_INPUT_LEN + 2];
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
        len--;
    }
    
    if (len > MAX_INPUT_LEN) {
        printf("Input too long\n");
        return;
    }
    
    for (int i = 0; i <= len; i++) {
        char c = input_buffer[i];
        Event ev = get_event(c);
        
        switch (current_state) {
            case STATE_START:
                current_state = handle_start(ev, c, &ctx);
                break;
            case STATE_READING_NUMBER:
                current_state = handle_reading_number(ev, c,