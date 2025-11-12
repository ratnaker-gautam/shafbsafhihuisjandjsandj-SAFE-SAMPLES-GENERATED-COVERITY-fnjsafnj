//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING_SPACE,
    END,
    ERROR
};

int main(void) {
    char buffer[256];
    printf("Enter text (max 255 chars): ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 0;
    }
    
    enum state current_state = START;
    int number_count = 0;
    int word_count = 0;
    size_t pos = 0;
    
    while (current_state != END && current_state != ERROR && pos <= len) {
        char c = (pos < len) ? buffer[pos] : '\0';
        
        switch (current_state) {
            case START:
                if (isdigit((unsigned char)c)) {
                    current_state = READING_NUMBER;
                    number_count++;
                } else if (isalpha((unsigned char)c)) {
                    current_state = READING_WORD;
                    word_count++;
                } else if (isspace((unsigned char)c)) {
                    current_state = SKIPPING_SPACE;
                } else if (c == '\0') {
                    current_state = END;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit((unsigned char)c)) {
                    current_state = READING_NUMBER;
                } else if (isspace((unsigned char)c)) {
                    current_state = SKIPPING_SPACE;
                } else if (c == '\0') {
                    current_state = END;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha((unsigned char)c)) {
                    current_state = READING_WORD;
                } else if (isspace((unsigned char)c)) {
                    current_state = SKIPPING_SPACE;
                } else if (c == '\0') {
                    current_state = END;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case SKIPPING_SPACE:
                if (isspace((unsigned char)c)) {
                    current_state = SKIPPING_SPACE;
                } else if (isdigit((unsigned char)c)) {
                    current_state = READING_NUMBER;
                    number_count++;
                } else if (isalpha((unsigned char)c)) {
                    current_state = READING_WORD;
                    word_count++;
                } else if (c == '\0') {
                    current_state = END;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case END:
                break;
                
            case ERROR:
                break;
        }
        
        pos++;
    }
    
    if (current_state == ERROR) {
        printf("Invalid input format at position %zu\n", pos);
        return 1;
    }
    
    printf("Found %d numbers and %d words\n", number_count, word_count);
    
    return 0;
}