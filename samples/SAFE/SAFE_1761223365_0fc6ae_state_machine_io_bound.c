//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

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
    int current_number;
    char current_word[MAX_INPUT_LEN];
} Context;

Event get_event(char c) {
    if (c == '\0') return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

State handle_state(State current, Event event, Context *ctx, char c) {
    switch (current) {
        case STATE_START:
            switch (event) {
                case EVENT_DIGIT:
                    ctx->current_number = c - '0';
                    return STATE_READING_NUMBER;
                case EVENT_LETTER:
                    ctx->current_word[0] = c;
                    ctx->current_word[1] = '\0';
                    return STATE_READING_WORD;
                case EVENT_SPACE:
                    return STATE_START;
                case EVENT_EOF:
                    return STATE_DONE;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_READING_NUMBER:
            switch (event) {
                case EVENT_DIGIT:
                    if (ctx->current_number > (INT_MAX - (c - '0')) / 10) {
                        return STATE_ERROR;
                    }
                    ctx->current_number = ctx->current_number * 10 + (c - '0');
                    return STATE_READING_NUMBER;
                case EVENT_SPACE:
                    ctx->number_count++;
                    return STATE_START;
                case EVENT_EOF:
                    ctx->number_count++;
                    return STATE_DONE;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_READING_WORD:
            switch (event) {
                case EVENT_LETTER:
                    if (strlen(ctx->current_word) < MAX_INPUT_LEN - 1) {
                        size_t len = strlen(ctx->current_word);
                        ctx->current_word[len] = c;
                        ctx->current_word[len + 1] = '\0';
                    }
                    return STATE_READING_WORD;
                case EVENT_SPACE:
                    ctx->word_count++;
                    return STATE_START;
                case EVENT_EOF:
                    ctx->word_count++;
                    return STATE_DONE;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_ERROR:
            return STATE_ERROR;
            
        case STATE_DONE:
            return STATE_DONE;
    }
    return STATE_ERROR;
}

int main(void) {
    State current_state = STATE_START;
    Context ctx = {0, 0, 0, {0}};
    char input[MAX_INPUT_LEN];
    int input_len = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len >= MAX_INPUT_LEN - 1) {
        printf("Input too long\n");
        return 1;
    }
    
    for (int i = 0; i <= input_len; i++) {
        char c = input[i];
        Event event = get_event(c);
        State next_state = handle_state(current_state, event, &ctx, c);
        
        if (next_state == STATE_ERROR) {
            printf("Error: Invalid input format\n");
            return 1;
        }
        
        current_state = next_state;
        
        if (current_state == STATE_DONE) {
            break;
        }
    }
    
    if (current_state != STATE_DONE && current_state != STATE_START) {
        printf("Error: Incomplete input\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    
    return 0;
}