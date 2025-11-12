//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUMBER, READING_WORD, ERROR, DONE };

int main(void) {
    enum State current_state = START;
    char buffer[1024];
    int buffer_pos = 0;
    int ch;
    int number_count = 0;
    int word_count = 0;
    
    printf("Enter text (max 1023 chars, Ctrl+D to end): ");
    
    while (current_state != DONE && current_state != ERROR) {
        ch = getchar();
        
        if (ch == EOF) {
            if (current_state == READING_NUMBER) {
                number_count++;
                current_state = DONE;
            } else if (current_state == READING_WORD) {
                word_count++;
                current_state = DONE;
            } else if (current_state == START) {
                current_state = DONE;
            }
            continue;
        }
        
        if (buffer_pos >= 1023) {
            current_state = ERROR;
            continue;
        }
        
        switch (current_state) {
            case START:
                if (isdigit(ch)) {
                    buffer[buffer_pos++] = (char)ch;
                    current_state = READING_NUMBER;
                } else if (isalpha(ch)) {
                    buffer[buffer_pos++] = (char)ch;
                    current_state = READING_WORD;
                } else if (isspace(ch)) {
                    continue;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(ch)) {
                    buffer[buffer_pos++] = (char)ch;
                } else if (isspace(ch)) {
                    buffer[buffer_pos] = '\0';
                    number_count++;
                    buffer_pos = 0;
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(ch)) {
                    buffer[buffer_pos++] = (char)ch;
                } else if (isspace(ch)) {
                    buffer[buffer_pos] = '\0';
                    word_count++;
                    buffer_pos = 0;
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