//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    INIT,
    READING_ID,
    READING_NUMBER,
    ERROR,
    DONE
};

int main(void) {
    enum State current_state = INIT;
    char buffer[256];
    int buffer_pos = 0;
    int ch;
    int id_count = 0;
    int number_count = 0;
    
    printf("Enter text with identifiers and numbers (Ctrl+D to end):\n");
    
    while (current_state != DONE && current_state != ERROR) {
        ch = getchar();
        
        if (ch == EOF) {
            if (current_state == READING_ID || current_state == READING_NUMBER) {
                if (buffer_pos > 0) {
                    buffer[buffer_pos] = '\0';
                    if (current_state == READING_ID) {
                        id_count++;
                        printf("Identifier: %s\n", buffer);
                    } else {
                        number_count++;
                        printf("Number: %s\n", buffer);
                    }
                }
            }
            current_state = DONE;
            continue;
        }
        
        switch (current_state) {
            case INIT:
                if (isalpha(ch)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)ch;
                        current_state = READING_ID;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isdigit(ch)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)ch;
                        current_state = READING_NUMBER;
                    } else {
                        current_state = ERROR;
                    }
                } else if (!isspace(ch)) {
                    current_state = ERROR;
                }
                break;
                
            case READING_ID:
                if (isalnum(ch)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)ch;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace(ch)) {
                    buffer[buffer_pos] = '\0';
                    id_count++;
                    printf("Identifier: %s\n", buffer);
                    buffer_pos = 0;
                    current_state = INIT;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(ch)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)ch;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace(ch)) {
                    buffer[buffer_pos] = '\0';
                    number_count++;
                    printf("Number: %s\n", buffer);
                    buffer_pos = 0;
                    current_state = INIT;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                printf("Error: Invalid input format\n");
                while ((ch = getchar()) != EOF && !isspace(ch));
                buffer_pos = 0;
                current_state = INIT;
                break;
                
            case DONE:
                break;
        }
    }
    
    if (current_state == ERROR) {
        printf("Processing terminated due to errors\n");
        return 1;
    }
    
    printf("\nSummary: %d identifiers, %d numbers processed\n", id_count, number_count);
    return 0;
}