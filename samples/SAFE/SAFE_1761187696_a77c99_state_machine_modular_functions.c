//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_END
} state_t;

typedef enum {
    EVENT_DIGIT,
    EVENT_LETTER,
    EVENT_SPACE,
    EVENT_EOF,
    EVENT_INVALID
} event_t;

typedef struct {
    int number_count;
    int word_count;
    int current_number;
    char current_word[MAX_INPUT_LEN];
} context_t;

static event_t get_event(char c) {
    if (c == '\0' || c == '\n') return EVENT_EOF;
    if (isspace((unsigned char)c)) return EVENT_SPACE;
    if (isdigit((unsigned char)c)) return EVENT_DIGIT;
    if (isalpha((unsigned char)c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

static state_t handle_start(event_t event, char c, context_t *ctx) {
    switch (event) {
        case EVENT_DIGIT:
            ctx->current_number = c - '0';
            return STATE_READING_NUMBER;
        case EVENT_LETTER:
            ctx->current_word[0] = c;
            ctx->current_word[1] = '\0';
            return STATE_READING_WORD;
        case EVENT_SPACE:
        case EVENT_EOF:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

static state_t handle_reading_number(event_t event, char c, context_t *ctx) {
    switch (event) {
        case EVENT_DIGIT:
            if (ctx->current_number <= (INT_MAX - (c - '0')) / 10) {
                ctx->current_number = ctx->current_number * 10 + (c - '0');
                return STATE_READING_NUMBER;
            }
            return STATE_ERROR;
        case EVENT_SPACE:
        case EVENT_EOF:
            ctx->number_count++;
            return (event == EVENT_EOF) ? STATE_END : STATE_START;
        default:
            return STATE_ERROR;
    }
}

static state_t handle_reading_word(event_t event, char c, context_t *ctx) {
    size_t len = strlen(ctx->current_word);
    if (len >= MAX_INPUT_LEN - 1) return STATE_ERROR;
    
    switch (event) {
        case EVENT_LETTER:
            ctx->current_word[len] = c;
            ctx->current_word[len + 1] = '\0';
            return STATE_READING_WORD;
        case EVENT_SPACE:
        case EVENT_EOF:
            ctx->word_count++;
            return (event == EVENT_EOF) ? STATE_END : STATE_START;
        default:
            return STATE_ERROR;
    }
}

static state_t handle_error(context_t *ctx) {
    ctx->number_count = 0;
    ctx->word_count = 0;
    return STATE_END;
}

static void process_input(const char *input) {
    state_t current_state = STATE_START;
    context_t ctx = {0, 0, 0, {0}};
    size_t i = 0;
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        char c = input[i];
        event_t event = get_event(c);
        
        switch (current_state) {
            case STATE_START:
                current_state = handle_start(event, c, &ctx);
                break;
            case STATE_READING_NUMBER:
                current_state = handle_reading_number(event, c, &ctx);
                break;
            case STATE_READING_WORD:
                current_state = handle_reading_word(event, c, &ctx);
                break;
            case STATE_ERROR:
                current_state = handle_error(&ctx);
                break;
            default:
                current_state = STATE_ERROR;
                break;
        }
        
        if (event == EVENT_EOF) break;
        i++;
        if (i >= MAX_INPUT_LEN) {
            current_state = STATE_ERROR;
            break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input format\n");
    } else {
        printf("Numbers: %d, Words: %d\n", ctx.number_count, ctx.word_count);
    }
}

static int read_input_safe(char *buffer, size_t size) {
    if (size == 0) return 0;
    
    if (fgets(buffer, (int)size, stdin) == NULL) {
        return 0;
    }
    
    buffer[size - 1] = '\0';
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    return 1;
}

int main(void) {
    char input[MAX_INPUT_LEN + 2] = {0};
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (!read_input_safe(input, sizeof(input))) {
        printf("Error reading input\n");
        return 1;
    }
    
    if (strlen(input) > MAX_INPUT_LEN) {
        printf("Error: Input too long\n");
        return 1;
    }
    
    process_input(input);
    return 0;
}