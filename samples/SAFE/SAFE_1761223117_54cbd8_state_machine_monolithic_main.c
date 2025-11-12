//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_DONE
};

int main(void) {
    char buffer[256];
    int buffer_pos = 0;
    enum State current_state = STATE_START;
    int number_count = 0;
    int word_count = 0;
    int ch;
    
    printf("Enter text (max 255 chars, Ctrl+D to end): ");
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        ch = getchar();
        
        if (ch == EOF) {
            if (current_state == STATE_READING_NUMBER) {
                if (buffer_pos > 0) {
                    buffer[buffer_pos] = '\0';
                    number_count++;
                    printf("Found number: %s\n", buffer);
                }
            } else if (current_state == STATE_READING_WORD) {
                if (buffer_pos > 0) {
                    buffer[buffer_pos] = '\0';
                    word_count++;
                    printf("Found word: %s\n", buffer);
                }
            }
            current_state = STATE_DONE;
            continue;
        }
        
        switch (current_state) {
            case STATE_START:
                if (isdigit(ch)) {
                    buffer_pos = 0;
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)ch;
                    }
                    current_state = STATE_READING_NUMBER;
                } else if (isalpha(ch)) {
                    buffer_pos = 0;
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)ch;
                    }
                    current_state = STATE_READING_WORD;
                } else if (isspace(ch)) {
                    continue;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (isdigit(ch)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)ch;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (isspace(ch)) {
                    if (buffer_pos > 0) {
                        buffer[buffer_pos] = '\0';
                        number_count++;
                        printf("Found number: %s\n", buffer);
                    }
                    current_state = STATE_START;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (isalpha(ch)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)ch;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (isspace(ch)) {
                    if (buffer_pos > 0) {
                        buffer[buffer_pos] = '\0';
                        word_count++;
                        printf("Found word: %s\n", buffer);
                    }
                    current_state = STATE_START;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input format\n");
                current_state = STATE_DONE;
                break;
                
            case STATE_DONE:
                break;
        }
    }
    
    if (current_state == STATE_DONE) {
        printf("\nSummary: %d numbers, %d words\n", number_count, word_count);
    } else {
        printf("Processing terminated with error\n");
    }
    
    return current_state == STATE_DONE ? 0 : 1;
}