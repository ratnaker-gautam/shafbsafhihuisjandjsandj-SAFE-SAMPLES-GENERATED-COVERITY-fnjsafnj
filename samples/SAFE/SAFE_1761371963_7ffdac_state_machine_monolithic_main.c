//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    INIT,
    READING_NUMBERS,
    READING_WORDS,
    ERROR,
    DONE
};

int main(void) {
    enum State current_state = INIT;
    char buffer[256];
    int number_count = 0;
    int word_count = 0;
    int valid_input = 1;
    
    printf("Enter data (numbers or words, 'quit' to finish):\n");
    
    while (current_state != DONE && current_state != ERROR) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            current_state = ERROR;
            break;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }
        
        if (len >= sizeof(buffer) - 1) {
            current_state = ERROR;
            break;
        }
        
        if (strcmp(buffer, "quit") == 0) {
            current_state = DONE;
            continue;
        }
        
        int is_number = 1;
        int has_digit = 0;
        
        for (size_t i = 0; i < len; i++) {
            if (i == 0 && buffer[i] == '-') {
                continue;
            }
            if (!isdigit((unsigned char)buffer[i])) {
                is_number = 0;
                break;
            }
            has_digit = 1;
        }
        
        if (!has_digit) {
            is_number = 0;
        }
        
        switch (current_state) {
            case INIT:
                if (is_number) {
                    current_state = READING_NUMBERS;
                    number_count++;
                } else {
                    current_state = READING_WORDS;
                    word_count++;
                }
                break;
                
            case READING_NUMBERS:
                if (is_number) {
                    number_count++;
                } else {
                    current_state = READING_WORDS;
                    word_count++;
                }
                break;
                
            case READING_WORDS:
                if (is_number) {
                    current_state = READING_NUMBERS;
                    number_count++;
                } else {
                    word_count++;
                }
                break;
                
            default:
                current_state = ERROR;
                break;
        }
        
        if (number_count > 1000 || word_count > 1000) {
            current_state = ERROR;
            break;
        }
    }
    
    if (current_state == ERROR) {
        printf("Error: Invalid input or system error\n");
        return 1;
    }
    
    printf("Processed %d numbers and %d words\n", number_count, word_count);
    
    if (current_state == READING_NUMBERS) {
        printf("Final state: Processing numbers\n");
    } else if (current_state == READING_WORDS) {
        printf("Final state: Processing words\n");
    } else {
        printf("Final state: Initial\n");
    }
    
    return 0;
}