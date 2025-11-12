//DeepSeek-V3 SAFE v1.3 Category: Safe ; Style: modular_functions ; Variation: state_machine
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
    EVENT_INVALID
} Event;

State handle_start(Event evt, char ch);
State handle_reading_number(Event evt, char ch);
State handle_reading_word(Event evt, char ch);
State handle_error(Event evt, char ch);
State handle_end(Event evt, char ch);

Event get_event(char ch);
void process_input(void);

State (*state_handlers[])(Event, char) = {
    handle_start,
    handle_reading_number,
    handle_reading_word,
    handle_error,
    handle_end
};

int number_count = 0;
int word_count = 0;

State handle_start(Event evt, char ch) {
    if (evt == EVENT_DIGIT) {
        number_count++;
        return STATE_READING_NUMBER;
    } else if (evt == EVENT_LETTER) {
        word_count++;
        return STATE_READING_WORD;
    } else if (evt == EVENT_SPACE) {
        return STATE_START;
    } else if (evt == EVENT_EOF) {
        return STATE_END;
    } else {
        return STATE_ERROR;
    }
}

State handle_reading_number(Event evt, char ch) {
    if (evt == EVENT_DIGIT) {
        return STATE_READING_NUMBER;
    } else if (evt == EVENT_SPACE) {
        return STATE_START;
    } else if (evt == EVENT_EOF) {
        return STATE_END;
    } else {
        return STATE_ERROR;
    }
}

State handle_reading_word(Event evt, char ch) {
    if (evt == EVENT_LETTER) {
        return STATE_READING_WORD;
    } else if (evt == EVENT_SPACE) {
        return STATE_START;
    } else if (evt == EVENT_EOF) {
        return STATE_END;
    } else {
        return STATE_ERROR;
    }
}

State handle_error(Event evt, char ch) {
    return STATE_ERROR;
}

State handle_end(Event evt, char ch) {
    return STATE_END;
}

Event get_event(char ch) {
    if (ch == EOF) {
        return EVENT_EOF;
    } else if (isspace(ch)) {
        return EVENT_SPACE;
    } else if (isdigit(ch)) {
        return EVENT_DIGIT;
    } else if (isalpha(ch)) {
        return EVENT_LETTER;
    } else {
        return EVENT_INVALID;
    }
}

void process_input(void) {
    State current_state = STATE_START;
    char input[MAX_INPUT_LEN];
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    for (size_t i = 0; i <= len; i++) {
        char ch = (i < len) ? input[i] : EOF;
        Event evt = get_event(ch);
        
        if (current_state < 0 || current_state >= MAX_STATES) {
            current_state = STATE_ERROR;
            break;
        }
        
        State next_state = state_handlers[current_state](evt, ch);
        current_state = next_state;
        
        if (current_state == STATE_END || current_state == STATE_ERROR) {
            break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Invalid input format\n");
    } else {
        printf("Numbers: %d, Words: %d\n", number_count, word_count);
    }
}

int main(void) {
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    process_input();
    return 0;
}