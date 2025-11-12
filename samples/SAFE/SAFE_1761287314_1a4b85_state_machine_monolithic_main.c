//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUMBER, READING_WORD, ERROR, DONE };

int main(void) {
    char input[1024];
    char buffer[1024];
    int buffer_pos = 0;
    enum State state = START;
    int number_count = 0;
    int word_count = 0;
    int i = 0;
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    while (input[i] != '\0' && state != ERROR && state != DONE) {
        char c = input[i];
        
        switch (state) {
            case START:
                if (isspace(c)) {
                    i++;
                } else if (isdigit(c)) {
                    state = READING_NUMBER;
                    buffer[buffer_pos++] = c;
                    i++;
                } else if (isalpha(c)) {
                    state = READING_WORD;
                    buffer[buffer_pos++] = c;
                    i++;
                } else {
                    state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(c)) {
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = c;
                        i++;
                    } else {
                        state = ERROR;
                    }
                } else if (isspace(c) || c == '\0') {
                    buffer[buffer_pos] = '\0';
                    number_count++;
                    printf("Found number: %s\n", buffer);
                    buffer_pos = 0;
                    state = START;
                    i++;
                } else {
                    state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(c)) {
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = c;
                        i++;
                    } else {
                        state = ERROR;
                    }
                } else if (isspace(c) || c == '\0') {
                    buffer[buffer_pos] = '\0';
                    word_count++;
                    printf("Found word: %s\n", buffer);
                    buffer_pos = 0;
                    state = START;
                    i++;
                } else {
                    state = ERROR;
                }
                break;
                
            case ERROR:
                printf("Invalid input format\n");
                return 1;
                
            case DONE:
                break;
        }
    }
    
    if (state == READING_NUMBER || state == READING_WORD) {
        if (buffer_pos > 0) {
            buffer[buffer_pos] = '\0';
            if (state == READING_NUMBER) {
                number_count++;
                printf("Found number: %s\n", buffer);
            } else {
                word_count++;
                printf("Found word: %s\n", buffer);
            }
        }
    }
    
    if (state == ERROR) {
        printf("Invalid input format\n");
        return 1;
    }
    
    printf("Summary: %d numbers, %d words\n", number_count, word_count);
    
    return 0;
}