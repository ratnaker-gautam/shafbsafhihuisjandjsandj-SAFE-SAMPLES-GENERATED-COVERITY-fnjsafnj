//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: state_machine
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
    STATE_END
} State;

typedef enum {
    EVENT_DIGIT,
    EVENT_LETTER,
    EVENT_SPACE,
    EVENT_EOF,
    EVENT_OTHER
} Event;

State handle_start(Event ev, char ch);
State handle_reading_number(Event ev, char ch);
State handle_reading_word(Event ev, char ch);
State handle_error(Event ev, char ch);
State handle_end(Event ev, char ch);

typedef State (*StateHandler)(Event, char);

StateHandler handlers[] = {
    handle_start,
    handle_reading_number,
    handle_reading_word,
    handle_error,
    handle_end
};

State current_state = STATE_START;
int number_count = 0;
int word_count = 0;
char current_token[MAX_INPUT_LEN];
int token_pos = 0;

void reset_token(void) {
    memset(current_token, 0, sizeof(current_token));
    token_pos = 0;
}

void add_to_token(char ch) {
    if (token_pos < MAX_INPUT_LEN - 1) {
        current_token[token_pos++] = ch;
    }
}

void finalize_token(void) {
    current_token[token_pos] = '\0';
    if (strlen(current_token) > 0) {
        if (isdigit(current_token[0])) {
            number_count++;
            printf("Number: %s\n", current_token);
        } else {
            word_count++;
            printf("Word: %s\n", current_token);
        }
    }
    reset_token();
}

Event classify_event(char ch) {
    if (ch == EOF) return EVENT_EOF;
    if (isspace(ch)) return EVENT_SPACE;
    if (isdigit(ch)) return EVENT_DIGIT;
    if (isalpha(ch)) return EVENT_LETTER;
    return EVENT_OTHER;
}

State handle_start(Event ev, char ch) {
    switch (ev) {
        case EVENT_DIGIT:
            add_to_token(ch);
            return STATE_READING_NUMBER;
        case EVENT_LETTER:
            add_to_token(ch);
            return STATE_READING_WORD;
        case EVENT_SPACE:
        case EVENT_OTHER:
            return STATE_START;
        case EVENT_EOF:
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_number(Event ev, char ch) {
    switch (ev) {
        case EVENT_DIGIT:
            add_to_token(ch);
            return STATE_READING_NUMBER;
        case EVENT_SPACE:
        case EVENT_OTHER:
            finalize_token();
            return STATE_START;
        case EVENT_EOF:
            finalize_token();
            return STATE_END;
        case EVENT_LETTER:
            return STATE_ERROR;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_word(Event ev, char ch) {
    switch (ev) {
        case EVENT_LETTER:
        case EVENT_DIGIT:
            add_to_token(ch);
            return STATE_READING_WORD;
        case EVENT_SPACE:
        case EVENT_OTHER:
            finalize_token();
            return STATE_START;
        case EVENT_EOF:
            finalize_token();
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_error(Event ev, char ch) {
    printf("Error: Invalid input sequence\n");
    reset_token();
    if (ev == EVENT_EOF) return STATE_END;
    return STATE_START;
}

State handle_end(Event ev, char ch) {
    return STATE_END;
}

void process_input(void) {
    char ch;
    while (current_state != STATE_END) {
        ch = getchar();
        if (ch == EOF && feof(stdin)) {
            ch = EOF;
        }
        
        Event ev = classify_event(ch);
        State next_state = handlers[current_state](ev, ch);
        
        if (next_state == STATE_ERROR && current_state != STATE_ERROR) {
            printf("Error: Invalid state transition\n");
        }
        
        current_state = next_state;
        
        if (current_state == STATE_END) {
            break;
        }
    }
    
    if (token_pos > 0) {
        finalize_token();
    }
}

int main(void) {
    printf("Enter text (Ctrl+D to end):\n");
    process_input();
    printf("\nSummary: %d numbers, %d words\n", number_count, word_count);
    return 0;
}