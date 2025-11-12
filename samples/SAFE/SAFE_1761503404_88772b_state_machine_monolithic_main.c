//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUMBER, READING_WORD, ERROR, DONE };

int main(void) {
    enum State current_state = START;
    int ch;
    int number_count = 0;
    int word_count = 0;
    int current_length = 0;
    char buffer[256];
    int buffer_pos = 0;
    
    printf("Enter text (Ctrl+D to end): ");
    
    while (current_state != DONE && current_state != ERROR) {
        ch = getchar();
        
        if (ch == EOF) {
            switch (current_state) {
                case START:
                    current_state = DONE;
                    break;
                case READING_NUMBER:
                    if (current_length > 0) {
                        number_count++;
                        current_state = DONE;
                    } else {
                        current_state = DONE;
                    }
                    break;
                case READING_WORD:
                    if (current_length > 0) {
                        word_count++;
                        current_state = DONE;
                    } else {
                        current_state = DONE;
                    }
                    break;
                default:
                    current_state = DONE;
                    break;
            }
            continue;
        }
        
        if (buffer_pos >= 255) {
            current_state = ERROR;
            break;
        }
        
        switch (current_state) {
            case START:
                if (isspace(ch)) {
                    continue;
                } else if (isdigit(ch)) {
                    buffer[buffer_pos++] = (char)ch;
                    current_length = 1;
                    current_state = READING_NUMBER;
                } else if (isalpha(ch)) {
                    buffer[buffer_pos++] = (char)ch;
                    current_length = 1;
                    current_state = READING_WORD;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(ch)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)ch;
                        current_length++;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace(ch)) {
                    buffer[buffer_pos] = '\0';
                    number_count++;
                    buffer_pos = 0;
                    current_length = 0;
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(ch)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)ch;
                        current_length++;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace(ch)) {
                    buffer[buffer_pos] = '\0';
                    word_count++;
                    buffer_pos = 0;
                    current_length = 0;
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                break;
                
            case DONE:
                break;
        }
    }
    
    if (current_state == ERROR) {
        printf("Error: Invalid input format detected.\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", number_count);
    printf("Words found: %d\n", word_count);
    
    return 0;
}