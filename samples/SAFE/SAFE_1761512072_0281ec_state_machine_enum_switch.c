//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_END
};

int main(void) {
    enum State current_state = STATE_START;
    char buffer[256];
    int buffer_index = 0;
    int number_count = 0;
    int word_count = 0;
    int ch;

    printf("Enter text (max 255 chars): ");
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        ch = getchar();
        
        if (ch == EOF || ch == '\n') {
            ch = '\0';
        }

        switch (current_state) {
            case STATE_START:
                if (ch == '\0') {
                    current_state = STATE_END;
                } else if (isdigit(ch)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                        current_state = STATE_READING_NUMBER;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (isalpha(ch)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                        current_state = STATE_READING_WORD;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (isspace(ch)) {
                    continue;
                } else {
                    current_state = STATE_ERROR;
                }
                break;

            case STATE_READING_NUMBER:
                if (ch == '\0') {
                    if (buffer_index > 0) {
                        buffer[buffer_index] = '\0';
                        number_count++;
                        printf("Found number: %s\n", buffer);
                        buffer_index = 0;
                    }
                    current_state = STATE_END;
                } else if (isdigit(ch)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (isspace(ch)) {
                    if (buffer_index > 0) {
                        buffer[buffer_index] = '\0';
                        number_count++;
                        printf("Found number: %s\n", buffer);
                        buffer_index = 0;
                    }
                    current_state = STATE_START;
                } else {
                    current_state = STATE_ERROR;
                }
                break;

            case STATE_READING_WORD:
                if (ch == '\0') {
                    if (buffer_index > 0) {
                        buffer[buffer_index] = '\0';
                        word_count++;
                        printf("Found word: %s\n", buffer);
                        buffer_index = 0;
                    }
                    current_state = STATE_END;
                } else if (isalpha(ch)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (isspace(ch)) {
                    if (buffer_index > 0) {
                        buffer[buffer_index] = '\0';
                        word_count++;
                        printf("Found word: %s\n", buffer);
                        buffer_index = 0;
                    }
                    current_state = STATE_START;
                } else {
                    current_state = STATE_ERROR;
                }
                break;

            case STATE_ERROR:
                printf("Error: Invalid input format\n");
                current_state = STATE_END;
                break;

            case STATE_END:
                break;
        }
    }

    if (current_state == STATE_END) {
        printf("Summary: %d numbers, %d words\n", number_count, word_count);
    }

    return current_state == STATE_ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}