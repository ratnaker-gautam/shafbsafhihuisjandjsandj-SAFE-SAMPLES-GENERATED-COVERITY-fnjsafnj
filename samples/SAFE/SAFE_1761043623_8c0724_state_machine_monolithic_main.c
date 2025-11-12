//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: state_machine
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
    
    printf("Enter text (max 255 chars, end with newline): ");
    
    while ((ch = getchar()) != EOF && ch != '\n' && buffer_pos < 255) {
        if (buffer_pos < 255) {
            buffer[buffer_pos++] = (char)ch;
        }
        
        switch (current_state) {
            case STATE_START:
                if (isdigit(ch)) {
                    current_state = STATE_READING_NUMBER;
                } else if (isalpha(ch)) {
                    current_state = STATE_READING_WORD;
                } else if (!isspace(ch)) {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (isspace(ch)) {
                    number_count++;
                    current_state = STATE_START;
                } else if (!isdigit(ch)) {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (isspace(ch)) {
                    word_count++;
                    current_state = STATE_START;
                } else if (!isalpha(ch)) {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                break;
                
            case STATE_DONE:
                break;
        }
    }
    
    buffer[buffer_pos] = '\0';
    
    if (current_state == STATE_READING_NUMBER) {
        number_count++;
        current_state = STATE_DONE;
    } else if (current_state == STATE_READING_WORD) {
        word_count++;
        current_state = STATE_DONE;
    } else if (current_state == STATE_START) {
        current_state = STATE_DONE;
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input format detected\n");
        return 1;
    }
    
    printf("Input: %s\n", buffer);
    printf("Numbers found: %d\n", number_count);
    printf("Words found: %d\n", word_count);
    
    return 0;
}