//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

enum state {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_DONE
};

int main(void) {
    enum state current_state = STATE_START;
    char buffer[256];
    int buffer_index = 0;
    int number_count = 0;
    int word_count = 0;
    int c;
    
    printf("Enter text (max 255 chars): ");
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        c = getchar();
        
        if (c == EOF) {
            current_state = STATE_DONE;
            continue;
        }
        
        switch (current_state) {
            case STATE_START:
                if (isdigit(c)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)c;
                        current_state = STATE_READING_NUMBER;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (isalpha(c)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)c;
                        current_state = STATE_READING_WORD;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (c == '\n') {
                    current_state = STATE_DONE;
                } else if (!isspace(c)) {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (isdigit(c)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)c;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (isspace(c) || c == '\n') {
                    buffer[buffer_index] = '\0';
                    number_count++;
                    printf("Found number: %s\n", buffer);
                    buffer_index = 0;
                    current_state = STATE_START;
                    if (c == '\n') {
                        current_state = STATE_DONE;
                    }
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (isalpha(c)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)c;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (isspace(c) || c == '\n') {
                    buffer[buffer_index] = '\0';
                    word_count++;
                    printf("Found word: %s\n", buffer);
                    buffer_index = 0;
                    current_state = STATE_START;
                    if (c == '\n') {
                        current_state = STATE_DONE;
                    }
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input format\n");
                while ((c = getchar()) != '\n' && c != EOF);
                current_state = STATE_DONE;
                break;
                
            case STATE_DONE:
                break;
        }
    }
    
    if (current_state == STATE_DONE) {
        printf("\nSummary: Found %d numbers and %d words\n", number_count, word_count);
    }
    
    return 0;
}