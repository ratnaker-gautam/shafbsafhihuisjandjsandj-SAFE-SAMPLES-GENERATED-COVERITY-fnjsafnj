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
    int total_numbers = 0;
    int total_words = 0;
    int ch;
    
    printf("Enter text (Ctrl+D to end): ");
    
    while (current_state != DONE && current_state != ERROR) {
        ch = getchar();
        
        switch (current_state) {
            case START:
                if (ch == EOF) {
                    current_state = DONE;
                } else if (isdigit(ch)) {
                    if (buffer_pos < 1023) {
                        buffer[buffer_pos++] = (char)ch;
                    }
                    current_state = READING_NUMBER;
                } else if (isalpha(ch)) {
                    if (buffer_pos < 1023) {
                        buffer[buffer_pos++] = (char)ch;
                    }
                    current_state = READING_WORD;
                } else if (isspace(ch)) {
                    continue;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (ch == EOF || isspace(ch)) {
                    if (buffer_pos > 0) {
                        buffer[buffer_pos] = '\0';
                        total_numbers++;
                        printf("Found number: %s\n", buffer);
                        buffer_pos = 0;
                    }
                    current_state = (ch == EOF) ? DONE : START;
                } else if (isdigit(ch)) {
                    if (buffer_pos < 1023) {
                        buffer[buffer_pos++] = (char)ch;
                    }
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (ch == EOF || isspace(ch)) {
                    if (buffer_pos > 0) {
                        buffer[buffer_pos] = '\0';
                        total_words++;
                        printf("Found word: %s\n", buffer);
                        buffer_pos = 0;
                    }
                    current_state = (ch == EOF) ? DONE : START;
                } else if (isalpha(ch)) {
                    if (buffer_pos < 1023) {
                        buffer[buffer_pos++] = (char)ch;
                    }
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                printf("Error: Invalid input character detected\n");
                buffer_pos = 0;
                while ((ch = getchar()) != EOF && !isspace(ch)) {
                    continue;
                }
                current_state = START;
                break;
                
            case DONE:
                break;
        }
    }
    
    if (current_state == DONE) {
        printf("\nProcessing complete.\n");
        printf("Total numbers found: %d\n", total_numbers);
        printf("Total words found: %d\n", total_words);
    }
    
    return 0;
}