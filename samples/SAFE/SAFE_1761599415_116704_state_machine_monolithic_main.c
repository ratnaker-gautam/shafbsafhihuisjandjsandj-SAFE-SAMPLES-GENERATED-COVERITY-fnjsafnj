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
    
    printf("Enter text (max 255 chars, end with newline): ");
    
    while (current_state != DONE && current_state != ERROR) {
        ch = getchar();
        
        if (ch == EOF) {
            current_state = DONE;
            continue;
        }
        
        switch (current_state) {
            case START:
                if (ch == '\n') {
                    current_state = DONE;
                } else if (isdigit(ch)) {
                    buffer_index = 0;
                    buffer[buffer_index++] = (char)ch;
                    current_state = READING_NUMBER;
                } else if (isalpha(ch)) {
                    buffer_index = 0;
                    buffer[buffer_index++] = (char)tolower(ch);
                    current_state = READING_WORD;
                } else if (isspace(ch)) {
                    continue;
                } else {
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
                } else if (ch == '\n' || isspace(ch)) {
                    buffer[buffer_index] = '\0';
                    number_count++;
                    printf("Found number: %s\n", buffer);
                    if (ch == '\n') {
                        current_state = DONE;
                    } else {
                        current_state = START;
                    }
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(ch)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)tolower(ch);
                    } else {
                        current_state = ERROR;
                    }
                } else if (ch == '\n' || isspace(ch)) {
                    buffer[buffer_index] = '\0';
                    word_count++;
                    printf("Found word: %s\n", buffer);
                    if (ch == '\n') {
                        current_state = DONE;
                    } else {
                        current_state = START;
                    }
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                printf("Error: Invalid input format\n");
                while ((ch = getchar()) != '\n' && ch != EOF);
                current_state = DONE;
                break;
                
            case DONE:
                break;
        }
    }
    
    if (current_state == DONE) {
        printf("Summary: %d numbers, %d words\n", number_count, word_count);
    }
    
    return current_state == ERROR ? 1 : 0;
}