//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 5

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

State handle_start(Event ev, char ch);
State handle_reading_number(Event ev, char ch);
State handle_reading_word(Event ev, char ch);
State handle_error(Event ev, char ch);
State handle_end(Event ev, char ch);

typedef State (*StateHandler)(Event, char);

StateHandler handlers[MAX_STATES] = {
    handle_start,
    handle_reading_number,
    handle_reading_word,
    handle_error,
    handle_end
};

Event get_event(char ch) {
    if (ch == EOF) return EVENT_EOF;
    if (isspace(ch)) return EVENT_SPACE;
    if (isdigit(ch)) return EVENT_DIGIT;
    if (isalpha(ch)) return EVENT_LETTER;
    return EVENT_INVALID;
}

State handle_start(Event ev, char ch) {
    if (ev == EVENT_DIGIT) {
        putchar(ch);
        return STATE_READING_NUMBER;
    } else if (ev == EVENT_LETTER) {
        putchar(ch);
        return STATE_READING_WORD;
    } else if (ev == EVENT_SPACE) {
        return STATE_START;
    } else if (ev == EVENT_EOF) {
        return STATE_END;
    } else {
        return STATE_ERROR;
    }
}

State handle_reading_number(Event ev, char ch) {
    if (ev == EVENT_DIGIT) {
        putchar(ch);
        return STATE_READING_NUMBER;
    } else if (ev == EVENT_SPACE) {
        printf(" (number)\n");
        return STATE_START;
    } else if (ev == EVENT_EOF) {
        printf(" (number)\n");
        return STATE_END;
    } else {
        return STATE_ERROR;
    }
}

State handle_reading_word(Event ev, char ch) {
    if (ev == EVENT_LETTER) {
        putchar(ch);
        return STATE_READING_WORD;
    } else if (ev == EVENT_SPACE) {
        printf(" (word)\n");
        return STATE_START;
    } else if (ev == EVENT_EOF) {
        printf(" (word)\n");
        return STATE_END;
    } else {
        return STATE_ERROR;
    }
}

State handle_error(Event ev, char ch) {
    printf("\nError: Invalid input sequence\n");
    return STATE_END;
}

State handle_end(Event ev, char ch) {
    return STATE_END;
}

int main(void) {
    State current_state = STATE_START;
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return EXIT_FAILURE;
    }
    
    printf("Processing: ");
    
    for (size_t i = 0; i < len; i++) {
        char ch = input[i];
        Event ev = get_event(ch);
        
        if (current_state < 0 || current_state >= MAX_STATES) {
            fprintf(stderr, "Invalid state\n");
            return EXIT_FAILURE;
        }
        
        StateHandler handler = handlers[current_state];
        current_state = handler(ev, ch);
        
        if (current_state == STATE_END) {
            break;
        }
    }
    
    if (current_state != STATE_END) {
        Event ev = get_event(EOF);
        StateHandler handler = handlers[current_state];
        current_state = handler(ev, ' ');
    }
    
    return EXIT_SUCCESS;
}