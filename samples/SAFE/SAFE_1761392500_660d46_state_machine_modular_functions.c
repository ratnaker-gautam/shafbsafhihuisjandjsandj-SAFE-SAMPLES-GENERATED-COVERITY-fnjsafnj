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
    
    if (len == 1 && input[0] == '\n') {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    process_input(input);
    return 0;
}

Event get_event(char ch) {
    if (ch == '\0') return EVENT_EOF;
    if (isspace((unsigned char)ch)) return EVENT_SPACE;
    if (isdigit((unsigned char)ch)) return EVENT_DIGIT;
    if (isalpha((unsigned char)ch)) return EVENT_LETTER;
    return EVENT_OTHER;
}

State handle_start(Event ev, char ch) {
    switch (ev) {
        case EVENT_DIGIT:
            printf("Found number: %c", ch);
            return STATE_READING_NUMBER;
        case EVENT_LETTER:
            printf("Found word: %c", ch);
            return STATE_READING_WORD;
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_EOF:
            return STATE_END;
        default:
            printf("Error: invalid character '%c'\n", ch);
            return STATE_ERROR;
    }
}

State handle_reading_number(Event ev, char ch) {
    switch (ev) {
        case EVENT_DIGIT:
            printf("%c", ch);
            return STATE_READING_NUMBER;
        case EVENT_SPACE:
            printf("\n");
            return STATE_START;
        case EVENT_EOF:
            printf("\n");
            return STATE_END;
        default:
            printf("\nError: invalid character '%c' in number\n", ch);
            return STATE_ERROR;
    }
}

State handle_reading_word(Event ev, char ch) {
    switch (ev) {
        case EVENT_LETTER:
            printf("%c", ch);
            return STATE_READING_WORD;
        case EVENT_SPACE:
            printf("\n");
            return STATE_START;
        case EVENT_EOF:
            printf("\n");
            return STATE_END;
        default:
            printf("\nError: invalid character '%c' in word\n", ch);
            return STATE_ERROR;
    }
}

State handle_error(Event ev, char ch) {
    if (ev == EVENT_EOF) {
        return STATE_END;
    }
    return STATE_ERROR;
}

State handle_end(Event ev, char ch) {
    return STATE_END;
}

void process_input(const char* input) {
    State current_state = STATE_START;
    State (*handlers[])(Event, char) = {
        handle_start,
        handle_reading_number,
        handle_reading_word,
        handle_error,
        handle_end
    };
    
    size_t i = 0;
    char ch;
    
    while (current_state != STATE_END && i < MAX_INPUT_LEN) {
        ch = input[i];
        Event ev = get_event(ch);
        
        if (current_state >= 0 && current_state < sizeof(handlers)/sizeof(handlers[0])) {
            State new_state = handlers[current_state](ev, ch);
            current_state = new_state;
        } else {
            fprintf(stderr, "Invalid state\n");
            break;
        }
        
        i++;
        if (ch == '\0') break;
    }
    
    if (current_state != STATE_END && current_state != STATE_ERROR) {
        printf("\nProcessing incomplete\n");
    }
}