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
    END
};

int main(void) {
    char buffer[256];
    printf("Enter text (max 255 chars): ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 0;
    }
    
    enum state current_state = START;
    int number_count = 0;
    int word_count = 0;
    size_t i = 0;
    
    while (i < len && current_state != END) {
        char c = buffer[i];
        
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
                } else {
                    i++;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit((unsigned char)c)) {
                    i++;
                } else if (isspace((unsigned char)c)) {
                    current_state = SKIPPING_SPACE;
                } else {
                    current_state = START;
                }
                break;
                
            case READING_WORD:
                if (isalpha((unsigned char)c)) {
                    i++;
                } else if (isspace((unsigned char)c)) {
                    current_state = SKIPPING_SPACE;
                } else {
                    current_state = START;
                }
                break;
                
            case SKIPPING_SPACE:
                if (isspace((unsigned char)c)) {
                    i++;
                } else {
                    current_state = START;
                }
                break;
                
            case END:
                break;
        }
        
        if (current_state != END && i >= len) {
            current_state = END;
        }
    }
    
    printf("Numbers found: %d\n", number_count);
    printf("Words found: %d\n", word_count);
    
    return 0;
}