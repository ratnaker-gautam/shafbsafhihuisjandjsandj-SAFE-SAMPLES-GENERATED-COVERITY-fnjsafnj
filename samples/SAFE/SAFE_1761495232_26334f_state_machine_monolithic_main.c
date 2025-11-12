//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    START,
    READING_NUMBER,
    READING_WORD,
    ERROR,
    DONE
};

int main(void) {
    char buffer[256];
    int buffer_pos = 0;
    enum State current_state = START;
    int number_count = 0;
    int word_count = 0;
    int ch;
    
    printf("Enter text (max 255 chars, end with newline): ");
    
    while ((ch = getchar()) != EOF && ch != '\n' && buffer_pos < 255) {
        if (current_state == ERROR) {
            break;
        }
        
        buffer[buffer_pos++] = (char)ch;
        
        switch (current_state) {
            case START:
                if (isdigit(ch)) {
                    current_state = READING_NUMBER;
                } else if (isalpha(ch)) {
                    current_state = READING_WORD;
                } else if (!isspace(ch)) {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isspace(ch)) {
                    number_count++;
                    current_state = START;
                } else if (!isdigit(ch)) {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isspace(ch)) {
                    word_count++;
                    current_state = START;
                } else if (!isalpha(ch)) {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
            case DONE:
                break;
        }
    }
    
    buffer[buffer_pos] = '\0';
    
    if (current_state == READING_NUMBER) {
        number_count++;
        current_state = DONE;
    } else if (current_state == READING_WORD) {
        word_count++;
        current_state = DONE;
    } else if (current_state == START) {
        current_state = DONE;
    }
    
    if (current_state == ERROR) {
        printf("Error: Invalid input format. Only numbers and words allowed.\n");
        return 1;
    }
    
    printf("Input: %s\n", buffer);
    printf("Numbers found: %d\n", number_count);
    printf("Words found: %d\n", word_count);
    
    return 0;
}