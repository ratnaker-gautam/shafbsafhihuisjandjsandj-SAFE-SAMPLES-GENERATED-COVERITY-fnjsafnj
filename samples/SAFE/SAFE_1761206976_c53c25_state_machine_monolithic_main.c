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
    
    printf("Enter input (numbers or words, 'quit' to finish):\n");
    
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
            break;
        }
        
        int is_number = 1;
        int is_word = 1;
        int has_digit = 0;
        int has_alpha = 0;
        
        for (size_t i = 0; i < len; i++) {
            if (isdigit(buffer[i])) {
                has_digit = 1;
            } else if (isalpha(buffer[i])) {
                has_alpha = 1;
            } else if (!isspace(buffer[i])) {
                is_number = 0;
                is_word = 0;
                break;
            }
        }
        
        if (has_digit && !has_alpha) {
            is_word = 0;
        } else if (has_alpha && !has_digit) {
            is_number = 0;
        } else if (has_digit && has_alpha) {
            is_number = 0;
            is_word = 0;
        }
        
        switch (current_state) {
            case INIT:
                if (is_number) {
                    current_state = READING_NUMBERS;
                    number_count++;
                } else if (is_word) {
                    current_state = READING_WORDS;
                    word_count++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBERS:
                if (is_number) {
                    number_count++;
                } else if (is_word) {
                    current_state = READING_WORDS;
                    word_count++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORDS:
                if (is_word) {
                    word_count++;
                } else if (is_number) {
                    current_state = READING_NUMBERS;
                    number_count++;
                } else {
                    current_state = ERROR;
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
        printf("Error: Invalid input sequence detected\n");
        valid_input = 0;
    } else if (current_state == DONE) {
        printf("Processing complete.\n");
    }
    
    if (valid_input) {
        printf("Numbers processed: %d\n", number_count);
        printf("Words processed: %d\n", word_count);
    }
    
    return valid_input ? 0 : 1;
}