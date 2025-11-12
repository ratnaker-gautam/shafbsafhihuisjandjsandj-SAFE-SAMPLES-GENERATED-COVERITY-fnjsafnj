//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUMBER, READING_WORD, ERROR, DONE };

int main(void) {
    enum State current_state = START;
    char buffer[256];
    int buffer_index = 0;
    int ch;
    int number_count = 0;
    int word_count = 0;
    
    printf("Enter text (max 255 chars, end with EOF or newline): ");
    
    while (current_state != DONE && current_state != ERROR) {
        ch = getchar();
        
        if (ch == EOF || ch == '\n') {
            switch (current_state) {
                case READING_NUMBER:
                    if (buffer_index > 0) {
                        buffer[buffer_index] = '\0';
                        number_count++;
                        printf("Found number: %s\n", buffer);
                    }
                    current_state = DONE;
                    break;
                case READING_WORD:
                    if (buffer_index > 0) {
                        buffer[buffer_index] = '\0';
                        word_count++;
                        printf("Found word: %s\n", buffer);
                    }
                    current_state = DONE;
                    break;
                case START:
                    current_state = DONE;
                    break;
                default:
                    current_state = ERROR;
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
                    buffer_index = 0;
                    buffer[buffer_index++] = (char)ch;
                    current_state = READING_NUMBER;
                } else if (isalpha(ch)) {
                    buffer_index = 0;
                    buffer[buffer_index++] = (char)ch;
                    current_state = READING_WORD;
                } else if (!isspace(ch)) {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(ch)) {
                    buffer[buffer_index++] = (char)ch;
                } else if (isalpha(ch)) {
                    buffer[buffer_index] = '\0';
                    number_count++;
                    printf("Found number: %s\n", buffer);
                    buffer_index = 0;
                    buffer[buffer_index++] = (char)ch;
                    current_state = READING_WORD;
                } else if (isspace(ch)) {
                    buffer[buffer_index] = '\0';
                    number_count++;
                    printf("Found number: %s\n", buffer);
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalnum(ch)) {
                    buffer[buffer_index++] = (char)ch;
                } else if (isdigit(ch)) {
                    buffer[buffer_index] = '\0';
                    word_count++;
                    printf("Found word: %s\n", buffer);
                    buffer_index = 0;
                    buffer[buffer_index++] = (char)ch;
                    current_state = READING_NUMBER;
                } else if (isspace(ch)) {
                    buffer[buffer_index] = '\0';
                    word_count++;
                    printf("Found word: %s\n", buffer);
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
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("\nSummary: Found %d numbers and %d words\n", number_count, word_count);
    
    return 0;
}