//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
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

Event get_event(char ch);
void process_token(State current_state, const char* token, int token_len);

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char token[MAX_INPUT_LEN + 1];
    int token_len = 0;
    State current_state = STATE_START;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    for (size_t i = 0; i <= input_len; i++) {
        char ch = (i < input_len) ? input[i] : '\0';
        Event ev = get_event(ch);
        
        State (*handlers[])(Event, char) = {
            handle_start,
            handle_reading_number,
            handle_reading_word,
            handle_error,
            handle_end
        };
        
        if (current_state < 0 || current_state >= STATE_END) {
            current_state = STATE_ERROR;
        }
        
        State new_state = handlers[current_state](ev, ch);
        
        if (current_state == STATE_READING_NUMBER || current_state == STATE_READING_WORD) {
            if (new_state != current_state && new_state != STATE_START) {
                if (token_len > 0 && token_len < MAX_INPUT_LEN) {
                    token[token_len] = '\0';
                    process_token(current_state, token, token_len);
                    token_len = 0;
                }
            }
        }
        
        if (new_state == STATE_READING_NUMBER || new_state == STATE_READING_WORD) {
            if (token_len < MAX_INPUT_LEN) {
                token[token_len++] = ch;
            } else {
                new_state = STATE_ERROR;
            }
        }
        
        current_state = new_state;
        
        if (current_state == STATE_END || current_state == STATE_ERROR) {
            break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    return 0;
}

Event get_event(char ch) {
    if (ch == '\0') {
        return EVENT_EOF;
    } else if (isspace((unsigned char)ch)) {
        return EVENT_SPACE;
    } else if (isdigit((unsigned char)ch)) {
        return EVENT_DIGIT;
    } else if (isalpha((unsigned char)ch)) {
        return EVENT_LETTER;
    } else {
        return EVENT_OTHER;
    }
}

State handle_start(Event ev, char ch) {
    switch (ev) {
        case EVENT_DIGIT:
            return STATE_READING_NUMBER;
        case EVENT_LETTER:
            return STATE_READING_WORD;
        case EVENT_SPACE:
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
            return STATE_READING_NUMBER;
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_EOF:
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_word(Event ev, char ch) {
    switch (ev) {
        case EVENT_LETTER:
            return STATE_READING_WORD;
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_EOF:
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_error(Event ev, char ch) {
    return STATE_ERROR;
}

State handle_end(Event ev, char ch) {
    return STATE_END;
}

void process_token(State current_state, const char* token, int token_len) {
    if (token == NULL || token_len <= 0) {
        return;
    }
    
    if (current_state == STATE_READING_NUMBER) {
        printf("Found number: %s\n", token);
    } else if (current_state == STATE_READING_WORD) {
        printf("Found word: %s\n", token);
    }
}