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
    
    printf("Enter text (max 1023 characters, Ctrl+D to end): ");
    
    while (current_state != DONE && current_state != ERROR) {
        ch = getchar();
        
        if (ch == EOF) {
            if (buffer_pos > 0) {
                buffer[buffer_pos] = '\0';
                if (isdigit(buffer[0])) {
                    number_count++;
                } else if (isalpha(buffer[0])) {
                    word_count++;
                }
            }
            current_state = DONE;
            continue;
        }
        
        if (ch == '\n') {
            if (buffer_pos > 0) {
                buffer[buffer_pos] = '\0';
                if (isdigit(buffer[0])) {
                    number_count++;
                } else if (isalpha(buffer[0])) {
                    word_count++;
                }
                buffer_pos = 0;
            }
            continue;
        }
        
        switch (current_state) {
            case START:
                if (isdigit(ch)) {
                    if (buffer_pos < 1023) {
                        buffer[buffer_pos++] = (char)ch;
                        current_state = READING_NUMBER;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isalpha(ch)) {
                    if (buffer_pos < 1023) {
                        buffer[buffer_pos++] = (char)ch;
                        current_state = READING_WORD;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace(ch)) {
                    continue;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(ch)) {
                    if (buffer_pos < 1023) {
                        buffer[buffer_pos++] = (char)ch;
                    } else {
                        current_state = ERROR;
                    }
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
                if (isalnum(ch)) {
                    if (buffer_pos < 1023) {
                        buffer[buffer_pos++] = (char)ch;
                    } else {
                        current_state = ERROR;
                    }
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
                printf("Error: Invalid input format\n");
                return 1;
                
            case DONE:
                break;
        }
    }
    
    if (current_state == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", number_count);
    printf("Words found: %d\n", word_count);
    
    return 0;
}