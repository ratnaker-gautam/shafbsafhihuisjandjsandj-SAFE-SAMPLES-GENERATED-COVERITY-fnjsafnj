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
    
    printf("Enter text (max 255 chars): ");
    
    while (current_state != DONE && current_state != ERROR) {
        ch = getchar();
        
        if (ch == EOF || ch == '\n') {
            switch (current_state) {
                case START:
                    current_state = DONE;
                    break;
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
                default:
                    current_state = DONE;
                    break;
            }
            continue;
        }
        
        switch (current_state) {
            case START:
                if (isdigit(ch)) {
                    current_state = READING_NUMBER;
                    buffer_index = 0;
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    }
                } else if (isalpha(ch)) {
                    current_state = READING_WORD;
                    buffer_index = 0;
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    }
                } else if (!isspace(ch)) {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(ch)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isalpha(ch) || ch == '_') {
                    current_state = ERROR;
                } else if (isspace(ch)) {
                    if (buffer_index > 0) {
                        buffer[buffer_index] = '\0';
                        number_count++;
                        printf("Found number: %s\n", buffer);
                    }
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalnum(ch) || ch == '_') {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace(ch)) {
                    if (buffer_index > 0) {
                        buffer[buffer_index] = '\0';
                        word_count++;
                        printf("Found word: %s\n", buffer);
                    }
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                printf("Invalid input format detected.\n");
                current_state = DONE;
                break;
                
            default:
                current_state = ERROR;
                break;
        }
    }
    
    if (current_state == ERROR) {
        printf("Input processing failed due to invalid format.\n");
        return 1;
    }
    
    printf("\nSummary: %d numbers, %d words processed.\n", number_count, word_count);
    return 0;
}