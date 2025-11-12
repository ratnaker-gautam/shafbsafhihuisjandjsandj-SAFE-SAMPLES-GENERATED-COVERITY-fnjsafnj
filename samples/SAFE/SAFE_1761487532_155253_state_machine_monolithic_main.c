//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { IDLE, READING, PROCESSING, ERROR, DONE };

int main(void) {
    enum State current_state = IDLE;
    char buffer[256];
    int buffer_pos = 0;
    int word_count = 0;
    int char_count = 0;
    int c;
    
    printf("Enter text (max 255 chars, Ctrl+D to end): ");
    
    while (current_state != DONE && current_state != ERROR) {
        c = getchar();
        
        switch (current_state) {
            case IDLE:
                if (c == EOF) {
                    current_state = DONE;
                } else if (isalpha(c)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)c;
                        char_count++;
                        current_state = READING;
                    } else {
                        current_state = ERROR;
                    }
                } else if (!isspace(c) && c != EOF) {
                    current_state = ERROR;
                }
                break;
                
            case READING:
                if (c == EOF || isspace(c)) {
                    if (buffer_pos > 0) {
                        buffer[buffer_pos] = '\0';
                        word_count++;
                        buffer_pos = 0;
                        current_state = PROCESSING;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isalpha(c)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)c;
                        char_count++;
                    } else {
                        current_state = ERROR;
                    }
                } else {
                    current_state = ERROR;
                }
                break;
                
            case PROCESSING:
                printf("Word %d: %s (length: %zu)\n", word_count, buffer, strlen(buffer));
                if (c == EOF) {
                    current_state = DONE;
                } else if (isalpha(c)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)c;
                        char_count++;
                        current_state = READING;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace(c)) {
                    current_state = IDLE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                printf("Error: Invalid input detected\n");
                current_state = DONE;
                break;
                
            case DONE:
                break;
        }
    }
    
    if (current_state == DONE) {
        printf("\nSummary: %d words, %d alphabetic characters\n", word_count, char_count);
    } else {
        printf("Processing terminated with errors\n");
    }
    
    return current_state == ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}