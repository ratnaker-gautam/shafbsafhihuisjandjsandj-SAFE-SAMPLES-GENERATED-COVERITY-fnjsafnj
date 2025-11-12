//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum State {
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
    
    enum State current_state = START;
    int number_count = 0;
    int word_count = 0;
    size_t i = 0;
    
    while (current_state != END) {
        switch (current_state) {
            case START:
                if (i >= len) {
                    current_state = END;
                } else if (isdigit((unsigned char)buffer[i])) {
                    current_state = READING_NUMBER;
                    number_count++;
                } else if (isalpha((unsigned char)buffer[i])) {
                    current_state = READING_WORD;
                    word_count++;
                } else if (isspace((unsigned char)buffer[i])) {
                    current_state = SKIPPING_SPACE;
                } else {
                    i++;
                }
                break;
                
            case READING_NUMBER:
                while (i < len && isdigit((unsigned char)buffer[i])) {
                    i++;
                }
                current_state = START;
                break;
                
            case READING_WORD:
                while (i < len && isalpha((unsigned char)buffer[i])) {
                    i++;
                }
                current_state = START;
                break;
                
            case SKIPPING_SPACE:
                while (i < len && isspace((unsigned char)buffer[i])) {
                    i++;
                }
                current_state = START;
                break;
                
            case END:
                break;
        }
    }
    
    printf("Found %d numbers and %d words\n", number_count, word_count);
    
    return 0;
}