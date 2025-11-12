//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

enum state {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING_SPACE,
    FINISHED,
    ERROR
};

int main(void) {
    char buffer[256];
    printf("Enter text to analyze: ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    if (len >= sizeof(buffer) - 1) {
        printf("Input too long\n");
        return 1;
    }
    
    enum state current_state = START;
    size_t pos = 0;
    int number_count = 0;
    int word_count = 0;
    
    while (current_state != FINISHED && current_state != ERROR && pos <= len) {
        char c = (pos < len) ? buffer[pos] : '\0';
        
        switch (current_state) {
            case START:
                if (c == '\0') {
                    current_state = FINISHED;
                } else if (isdigit((unsigned char)c)) {
                    current_state = READING_NUMBER;
                    number_count++;
                } else if (isalpha((unsigned char)c)) {
                    current_state = READING_WORD;
                    word_count++;
                } else if (isspace((unsigned char)c)) {
                    current_state = SKIPPING_SPACE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (c == '\0') {
                    current_state = FINISHED;
                } else if (isdigit((unsigned char)c)) {
                    current_state = READING_NUMBER;
                } else if (isspace((unsigned char)c)) {
                    current_state = SKIPPING_SPACE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (c == '\0') {
                    current_state = FINISHED;
                } else if (isalpha((unsigned char)c)) {
                    current_state = READING_WORD;
                } else if (isspace((unsigned char)c)) {
                    current_state = SKIPPING_SPACE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case SKIPPING_SPACE:
                if (c == '\0') {
                    current_state = FINISHED;
                } else if (isspace((unsigned char)c)) {
                    current_state = SKIPPING_SPACE;
                } else if (isdigit((unsigned char)c)) {
                    current_state = READING_NUMBER;
                    number_count++;
                } else if (isalpha((unsigned char)c)) {
                    current_state = READING_WORD;
                    word_count++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case FINISHED:
                break;
                
            case ERROR:
                printf("Invalid character at position %zu\n", pos);
                return 1;
        }
        
        pos++;
    }
    
    if (current_state == ERROR) {
        printf("Invalid input format\n");
        return 1;
    }
    
    printf("Found %d numbers and %d words\n", number_count, word_count);
    return 0;
}