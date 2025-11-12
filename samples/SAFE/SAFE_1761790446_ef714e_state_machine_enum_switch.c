//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_ERROR,
    STATE_END
};

enum Event {
    EVENT_WHITESPACE,
    EVENT_LETTER,
    EVENT_DIGIT,
    EVENT_OTHER,
    EVENT_EOF
};

int main(void) {
    enum State current_state = STATE_START;
    int word_count = 0;
    int number_count = 0;
    int ch;
    
    printf("Enter text (Ctrl+D to end):\n");
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        ch = getchar();
        enum Event event;
        
        if (ch == EOF) {
            event = EVENT_EOF;
        } else if (isspace(ch)) {
            event = EVENT_WHITESPACE;
        } else if (isalpha(ch)) {
            event = EVENT_LETTER;
        } else if (isdigit(ch)) {
            event = EVENT_DIGIT;
        } else {
            event = EVENT_OTHER;
        }
        
        switch (current_state) {
            case STATE_START:
                switch (event) {
                    case EVENT_LETTER:
                        current_state = STATE_READING_WORD;
                        break;
                    case EVENT_DIGIT:
                        current_state = STATE_READING_NUMBER;
                        break;
                    case EVENT_WHITESPACE:
                        current_state = STATE_START;
                        break;
                    case EVENT_OTHER:
                        current_state = STATE_ERROR;
                        break;
                    case EVENT_EOF:
                        current_state = STATE_END;
                        break;
                }
                break;
                
            case STATE_READING_WORD:
                switch (event) {
                    case EVENT_WHITESPACE:
                        word_count++;
                        current_state = STATE_START;
                        break;
                    case EVENT_EOF:
                        word_count++;
                        current_state = STATE_END;
                        break;
                    case EVENT_LETTER:
                    case EVENT_DIGIT:
                        current_state = STATE_READING_WORD;
                        break;
                    case EVENT_OTHER:
                        current_state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_READING_NUMBER:
                switch (event) {
                    case EVENT_WHITESPACE:
                        number_count++;
                        current_state = STATE_START;
                        break;
                    case EVENT_EOF:
                        number_count++;
                        current_state = STATE_END;
                        break;
                    case EVENT_DIGIT:
                        current_state = STATE_READING_NUMBER;
                        break;
                    case EVENT_LETTER:
                    case EVENT_OTHER:
                        current_state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_ERROR:
                printf("Invalid input character detected.\n");
                current_state = STATE_END;
                break;
                
            case STATE_END:
                break;
        }
    }
    
    if (current_state == STATE_END) {
        printf("Words: %d\n", word_count);
        printf("Numbers: %d\n", number_count);
    } else {
        printf("Processing terminated with error.\n");
    }
    
    return current_state == STATE_END ? EXIT_SUCCESS : EXIT_FAILURE;
}