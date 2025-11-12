//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    STATE_START,
    STATE_READING,
    STATE_NUMBER,
    STATE_WORD,
    STATE_ERROR,
    STATE_END
};

int main(void) {
    enum state current = STATE_START;
    char buffer[256];
    int pos = 0;
    int ch;
    int number_count = 0;
    int word_count = 0;
    
    printf("Enter text (max 255 chars, Ctrl+D to end): ");
    
    while (current != STATE_END && current != STATE_ERROR) {
        ch = getchar();
        
        if (ch == EOF) {
            ch = '\n';
        }
        
        switch (current) {
            case STATE_START:
                if (ch == '\n') {
                    current = STATE_END;
                } else if (isdigit(ch)) {
                    if (pos < 255) {
                        buffer[pos++] = (char)ch;
                    }
                    current = STATE_NUMBER;
                } else if (isalpha(ch)) {
                    if (pos < 255) {
                        buffer[pos++] = (char)ch;
                    }
                    current = STATE_WORD;
                } else if (isspace(ch)) {
                    current = STATE_READING;
                } else {
                    current = STATE_ERROR;
                }
                break;
                
            case STATE_READING:
                if (ch == '\n') {
                    current = STATE_END;
                } else if (isdigit(ch)) {
                    if (pos < 255) {
                        buffer[pos++] = (char)ch;
                    }
                    current = STATE_NUMBER;
                } else if (isalpha(ch)) {
                    if (pos < 255) {
                        buffer[pos++] = (char)ch;
                    }
                    current = STATE_WORD;
                } else if (!isspace(ch)) {
                    current = STATE_ERROR;
                }
                break;
                
            case STATE_NUMBER:
                if (ch == '\n') {
                    if (pos > 0) {
                        buffer[pos] = '\0';
                        number_count++;
                        printf("Found number: %s\n", buffer);
                    }
                    current = STATE_END;
                } else if (isdigit(ch)) {
                    if (pos < 255) {
                        buffer[pos++] = (char)ch;
                    }
                } else if (isspace(ch)) {
                    if (pos > 0) {
                        buffer[pos] = '\0';
                        number_count++;
                        printf("Found number: %s\n", buffer);
                    }
                    pos = 0;
                    current = STATE_READING;
                } else {
                    current = STATE_ERROR;
                }
                break;
                
            case STATE_WORD:
                if (ch == '\n') {
                    if (pos > 0) {
                        buffer[pos] = '\0';
                        word_count++;
                        printf("Found word: %s\n", buffer);
                    }
                    current = STATE_END;
                } else if (isalnum(ch)) {
                    if (pos < 255) {
                        buffer[pos++] = (char)ch;
                    }
                } else if (isspace(ch)) {
                    if (pos > 0) {
                        buffer[pos] = '\0';
                        word_count++;
                        printf("Found word: %s\n", buffer);
                    }
                    pos = 0;
                    current = STATE_READING;
                } else {
                    current = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input character\n");
                current = STATE_END;
                break;
                
            case STATE_END:
                break;
        }
    }
    
    if (current == STATE_END) {
        printf("\nSummary: %d numbers, %d words\n", number_count, word_count);
    }
    
    return current == STATE_ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}