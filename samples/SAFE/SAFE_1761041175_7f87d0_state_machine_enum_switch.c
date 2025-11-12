//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    START,
    READING_NUMBER,
    READING_WORD,
    ERROR,
    DONE
};

int main(void) {
    enum state current_state = START;
    char buffer[256];
    int buffer_index = 0;
    int ch;
    int number_count = 0;
    int word_count = 0;

    printf("Enter text (max 255 chars, Ctrl+D to end): ");
    
    while (current_state != DONE && current_state != ERROR) {
        ch = getchar();
        
        if (ch == EOF) {
            switch (current_state) {
                case START:
                    current_state = DONE;
                    break;
                case READING_NUMBER:
                    if (buffer_index > 0) {
                        number_count++;
                        buffer_index = 0;
                    }
                    current_state = DONE;
                    break;
                case READING_WORD:
                    if (buffer_index > 0) {
                        word_count++;
                        buffer_index = 0;
                    }
                    current_state = DONE;
                    break;
                default:
                    current_state = DONE;
                    break;
            }
            continue;
        }

        if (ch == '\n') {
            switch (current_state) {
                case START:
                    current_state = DONE;
                    break;
                case READING_NUMBER:
                    if (buffer_index > 0) {
                        number_count++;
                        buffer_index = 0;
                    }
                    current_state = START;
                    break;
                case READING_WORD:
                    if (buffer_index > 0) {
                        word_count++;
                        buffer_index = 0;
                    }
                    current_state = START;
                    break;
                default:
                    current_state = START;
                    break;
            }
            continue;
        }

        if (isspace(ch)) {
            switch (current_state) {
                case READING_NUMBER:
                    if (buffer_index > 0) {
                        number_count++;
                        buffer_index = 0;
                    }
                    current_state = START;
                    break;
                case READING_WORD:
                    if (buffer_index > 0) {
                        word_count++;
                        buffer_index = 0;
                    }
                    current_state = START;
                    break;
                default:
                    current_state = START;
                    break;
            }
            continue;
        }

        if (buffer_index >= 255) {
            current_state = ERROR;
            continue;
        }

        switch (current_state) {
            case START:
                if (isdigit(ch)) {
                    buffer[buffer_index++] = (char)ch;
                    current_state = READING_NUMBER;
                } else if (isalpha(ch)) {
                    buffer[buffer_index++] = (char)ch;
                    current_state = READING_WORD;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(ch)) {
                    buffer[buffer_index++] = (char)ch;
                } else if (isalpha(ch)) {
                    current_state = ERROR;
                } else {
                    buffer[buffer_index++] = (char)ch;
                }
                break;
                
            case READING_WORD:
                if (isalnum(ch)) {
                    buffer[buffer_index++] = (char)ch;
                } else {
                    current_state = ERROR;
                }
                break;
                
            default:
                current_state = ERROR;
                break;
        }
    }

    if (current_state == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }

    printf("Numbers found: %d\n", number_count);
    printf("Words found: %d\n", word_count);
    
    return 0;
}