//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: state_machine
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
    int c;
    
    printf("Enter text (max 255 chars): ");
    
    while (current_state != DONE && current_state != ERROR) {
        c = getchar();
        
        if (c == EOF || c == '\n') {
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
                if (isspace(c)) {
                    continue;
                } else if (isdigit(c)) {
                    current_state = READING_NUMBER;
                    buffer[buffer_index++] = (char)c;
                } else if (isalpha(c)) {
                    current_state = READING_WORD;
                    buffer[buffer_index++] = (char)c;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(c)) {
                    buffer[buffer_index++] = (char)c;
                } else if (isspace(c)) {
                    buffer[buffer_index] = '\0';
                    total_numbers++;
                    printf("Found number: %s\n", buffer);
                    buffer_index = 0;
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(c)) {
                    buffer[buffer_index++] = (char)c;
                } else if (isspace(c)) {
                    buffer[buffer_index] = '\0';
                    total_words++;
                    printf("Found word: %s\n", buffer);
                    buffer_index = 0;
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
    
    printf("Summary: %d numbers, %d words\n", total_numbers, total_words);
    return 0;
}