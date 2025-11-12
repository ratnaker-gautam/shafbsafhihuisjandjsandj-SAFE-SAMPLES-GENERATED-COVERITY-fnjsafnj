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
    int total_numbers = 0;
    int total_words = 0;
    int ch;
    
    printf("Enter text (max 255 chars): ");
    
    while (current_state != DONE && current_state != ERROR) {
        ch = getchar();
        
        if (ch == EOF || ch == '\n') {
            switch (current_state) {
                case READING_NUMBER:
                    if (buffer_index > 0) {
                        buffer[buffer_index] = '\0';
                        total_numbers++;
                        printf("Found number: %s\n", buffer);
                    }
                    current_state = DONE;
                    break;
                case READING_WORD:
                    if (buffer_index > 0) {
                        buffer[buffer_index] = '\0';
                        total_words++;
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
                } else if (isspace(ch)) {
                    continue;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(ch)) {
                    buffer[buffer_index++] = (char)ch;
                } else if (isspace(ch)) {
                    buffer[buffer_index] = '\0';
                    total_numbers++;
                    printf("Found number: %s\n", buffer);
                    buffer_index = 0;
                    current_state = START;
                } else if (isalpha(ch)) {
                    current_state = ERROR;
                } else {
                    buffer[buffer_index++] = (char)ch;
                }
                break;
                
            case READING_WORD:
                if (isalpha(ch)) {
                    buffer[buffer_index++] = (char)ch;
                } else if (isspace(ch)) {
                    buffer[buffer_index] = '\0';
                    total_words++;
                    printf("Found word: %s\n", buffer);
                    buffer_index = 0;
                    current_state = START;
                } else if (isdigit(ch)) {
                    current_state = ERROR;
                } else {
                    buffer[buffer_index++] = (char)ch;
                }
                break;
                
            case ERROR:
                printf("Invalid input format detected\n");
                current_state = DONE;
                break;
                
            default:
                current_state = ERROR;
                break;
        }
    }
    
    if (current_state == ERROR) {
        printf("Processing error occurred\n");
        return 1;
    }
    
    printf("\nSummary: %d numbers, %d words processed\n", total_numbers, total_words);
    
    return 0;
}