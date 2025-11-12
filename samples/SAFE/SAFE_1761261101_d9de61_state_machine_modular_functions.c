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
    EVENT_TERMINATOR,
    EVENT_INVALID
} Event;

State handle_start(Event ev);
State handle_reading_number(Event ev);
State handle_reading_word(Event ev);
State handle_error(Event ev);
State handle_end(Event ev);

Event get_event(char c);
void process_input(const char* input);

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    process_input(input);
    return 0;
}

Event get_event(char c) {
    if (c == '\0') {
        return EVENT_TERMINATOR;
    } else if (isdigit((unsigned char)c)) {
        return EVENT_DIGIT;
    } else if (isalpha((unsigned char)c)) {
        return EVENT_LETTER;
    } else if (isspace((unsigned char)c)) {
        return EVENT_SPACE;
    } else {
        return EVENT_INVALID;
    }
}

State handle_start(Event ev) {
    switch (ev) {
        case EVENT_DIGIT:
            printf("Found number: ");
            return STATE_READING_NUMBER;
        case EVENT_LETTER:
            printf("Found word: ");
            return STATE_READING_WORD;
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_TERMINATOR:
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_number(Event ev) {
    switch (ev) {
        case EVENT_DIGIT:
            return STATE_READING_NUMBER;
        case EVENT_SPACE:
            printf("\n");
            return STATE_START;
        case EVENT_TERMINATOR:
            printf("\n");
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_word(Event ev) {
    switch (ev) {
        case EVENT_LETTER:
            return STATE_READING_WORD;
        case EVENT_SPACE:
            printf("\n");
            return STATE_START;
        case EVENT_TERMINATOR:
            printf("\n");
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_error(Event ev) {
    printf("Invalid input format\n");
    return STATE_END;
}

State handle_end(Event ev) {
    return STATE_END;
}

void process_input(const char* input) {
    State current_state = STATE_START;
    const char* ptr = input;
    
    while (current_state != STATE_END) {
        Event ev = get_event(*ptr);
        
        State (*handlers[])(Event) = {
            handle_start,
            handle_reading_number,
            handle_reading_word,
            handle_error,
            handle_end
        };
        
        if (current_state < 0 || current_state >= sizeof(handlers)/sizeof(handlers[0])) {
            fprintf(stderr, "Invalid state\n");
            break;
        }
        
        State new_state = handlers[current_state](ev);
        
        if (current_state == STATE_READING_NUMBER || current_state == STATE_READING_WORD) {
            if (ev == EVENT_DIGIT || ev == EVENT_LETTER) {
                putchar(*ptr);
            }
        }
        
        current_state = new_state;
        
        if (ev != EVENT_TERMINATOR) {
            ptr++;
        }
        
        if (ptr - input > MAX_INPUT_LEN) {
            fprintf(stderr, "Input too long\n");
            break;
        }
    }
}