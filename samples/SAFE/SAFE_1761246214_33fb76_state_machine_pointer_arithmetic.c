//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_SKIPPING,
    STATE_END
} State;

typedef enum {
    EVENT_SPACE,
    EVENT_DIGIT,
    EVENT_LETTER,
    EVENT_OTHER,
    EVENT_END
} Event;

Event get_event(char c) {
    if (c == '\0') return EVENT_END;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_OTHER;
}

void process_input(const char *input) {
    State current_state = STATE_START;
    const char *ptr = input;
    char buffer[MAX_INPUT];
    char *buf_ptr = buffer;
    
    while (current_state != STATE_END) {
        Event evt = get_event(*ptr);
        
        switch (current_state) {
            case STATE_START:
                switch (evt) {
                    case EVENT_LETTER:
                        current_state = STATE_READING_WORD;
                        *buf_ptr = *ptr;
                        buf_ptr++;
                        break;
                    case EVENT_DIGIT:
                        current_state = STATE_READING_NUMBER;
                        *buf_ptr = *ptr;
                        buf_ptr++;
                        break;
                    case EVENT_SPACE:
                    case EVENT_OTHER:
                        current_state = STATE_SKIPPING;
                        break;
                    case EVENT_END:
                        current_state = STATE_END;
                        break;
                }
                break;
                
            case STATE_READING_WORD:
                switch (evt) {
                    case EVENT_LETTER:
                        *buf_ptr = *ptr;
                        buf_ptr++;
                        break;
                    case EVENT_SPACE:
                    case EVENT_DIGIT:
                    case EVENT_OTHER:
                    case EVENT_END:
                        if (buf_ptr > buffer) {
                            *buf_ptr = '\0';
                            printf("Word: %s\n", buffer);
                        }
                        buf_ptr = buffer;
                        if (evt == EVENT_END) {
                            current_state = STATE_END;
                        } else if (evt == EVENT_DIGIT) {
                            current_state = STATE_READING_NUMBER;
                            *buf_ptr = *ptr;
                            buf_ptr++;
                        } else {
                            current_state = STATE_SKIPPING;
                        }
                        break;
                }
                break;
                
            case STATE_READING_NUMBER:
                switch (evt) {
                    case EVENT_DIGIT:
                        *buf_ptr = *ptr;
                        buf_ptr++;
                        break;
                    case EVENT_SPACE:
                    case EVENT_LETTER:
                    case EVENT_OTHER:
                    case EVENT_END:
                        if (buf_ptr > buffer) {
                            *buf_ptr = '\0';
                            printf("Number: %s\n", buffer);
                        }
                        buf_ptr = buffer;
                        if (evt == EVENT_END) {
                            current_state = STATE_END;
                        } else if (evt == EVENT_LETTER) {
                            current_state = STATE_READING_WORD;
                            *buf_ptr = *ptr;
                            buf_ptr++;
                        } else {
                            current_state = STATE_SKIPPING;
                        }
                        break;
                }
                break;
                
            case STATE_SKIPPING:
                switch (evt) {
                    case EVENT_LETTER:
                        current_state = STATE_READING_WORD;
                        *buf_ptr = *ptr;
                        buf_ptr++;
                        break;
                    case EVENT_DIGIT:
                        current_state = STATE_READING_NUMBER;
                        *buf_ptr = *ptr;
                        buf_ptr++;
                        break;
                    case EVENT_END:
                        current_state = STATE_END;
                        break;
                    default:
                        break;
                }
                break;
                
            case STATE_END:
                break;
        }
        
        if (current_state != STATE_END) {
            ptr++;
            if ((size_t)(ptr - input) >= MAX_INPUT) {
                break;
            }
        }
    }
}

int main() {
    char input[MAX_INPUT];
    
    printf("Enter text to parse (max %d chars): ", MAX_INPUT - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    process_input(input);
    return 0;
}