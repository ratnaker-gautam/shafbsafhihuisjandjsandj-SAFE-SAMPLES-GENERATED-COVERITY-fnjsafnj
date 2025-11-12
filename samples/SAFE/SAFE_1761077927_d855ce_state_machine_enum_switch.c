//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING_SPACE,
    END
};

int main(void) {
    char buffer[256];
    int buffer_pos = 0;
    enum state current_state = START;
    int ch;
    int number_sum = 0;
    int word_count = 0;
    
    printf("Enter text (max 255 chars, end with Ctrl+D): ");
    
    while (current_state != END && buffer_pos < 255) {
        ch = getchar();
        
        if (ch == EOF) {
            ch = '\n';
        }
        
        if (ch == '\n') {
            current_state = END;
        }
        
        buffer[buffer_pos++] = (char)ch;
        
        switch (current_state) {
            case START:
                if (isdigit(ch)) {
                    current_state = READING_NUMBER;
                } else if (isalpha(ch)) {
                    current_state = READING_WORD;
                } else if (isspace(ch)) {
                    current_state = SKIPPING_SPACE;
                }
                break;
                
            case READING_NUMBER:
                if (!isdigit(ch)) {
                    if (isalpha(ch)) {
                        current_state = READING_WORD;
                    } else if (isspace(ch)) {
                        current_state = SKIPPING_SPACE;
                    }
                }
                break;
                
            case READING_WORD:
                if (!isalpha(ch)) {
                    if (isdigit(ch)) {
                        current_state = READING_NUMBER;
                    } else if (isspace(ch)) {
                        current_state = SKIPPING_SPACE;
                        word_count++;
                    }
                }
                break;
                
            case SKIPPING_SPACE:
                if (isdigit(ch)) {
                    current_state = READING_NUMBER;
                } else if (isalpha(ch)) {
                    current_state = READING_WORD;
                }
                break;
                
            case END:
                break;
        }
    }
    
    buffer[buffer_pos] = '\0';
    
    if (current_state == READING_WORD) {
        word_count++;
    }
    
    for (int i = 0; i < buffer_pos; i++) {
        if (isdigit(buffer[i])) {
            number_sum += buffer[i] - '0';
        }
    }
    
    printf("Processed text: %s\n", buffer);
    printf("Word count: %d\n", word_count);
    printf("Sum of digits: %d\n", number_sum);
    
    return 0;
}