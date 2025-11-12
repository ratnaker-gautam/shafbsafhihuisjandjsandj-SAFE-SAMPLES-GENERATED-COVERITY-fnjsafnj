//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUMBER, READING_WORD, ERROR, FINISHED };

int main(void) {
    enum State current_state = START;
    char buffer[1024];
    int buffer_pos = 0;
    int ch;
    int number_count = 0;
    int word_count = 0;
    
    printf("Enter text (Ctrl+D to end): ");
    
    while (current_state != FINISHED && current_state != ERROR) {
        ch = getchar();
        
        if (ch == EOF) {
            switch (current_state) {
                case START:
                    current_state = FINISHED;
                    break;
                case READING_NUMBER:
                    number_count++;
                    current_state = FINISHED;
                    break;
                case READING_WORD:
                    word_count++;
                    current_state = FINISHED;
                    break;
                default:
                    current_state = FINISHED;
                    break;
            }
            continue;
        }
        
        switch (current_state) {
            case START:
                if (isdigit(ch)) {
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = (char)ch;
                    }
                    current_state = READING_NUMBER;
                } else if (isalpha(ch)) {
                    if (buffer_pos < sizeof(buffer) - 1) {
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
                if (isdigit(ch)) {
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = (char)ch;
                    }
                } else if (isspace(ch)) {
                    if (buffer_pos > 0) {
                        buffer[buffer_pos] = '\0';
                        number_count++;
                        buffer_pos = 0;
                    }
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalnum(ch)) {
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = (char)ch;
                    }
                } else if (isspace(ch)) {
                    if (buffer_pos > 0) {
                        buffer[buffer_pos] = '\0';
                        word_count++;
                        buffer_pos = 0;
                    }
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                printf("Error: Invalid input character detected\n");
                current_state = FINISHED;
                break;
                
            case FINISHED:
                break;
        }
    }
    
    if (current_state == FINISHED) {
        printf("\nResults:\n");
        printf("Numbers found: %d\n", number_count);
        printf("Words found: %d\n", word_count);
    }
    
    return current_state == ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}