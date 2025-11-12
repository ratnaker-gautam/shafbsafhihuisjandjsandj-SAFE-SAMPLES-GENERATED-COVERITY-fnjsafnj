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
        int is_word = 0;
        
        for (size_t i = 0; i < len; i++) {
            if (!isdigit((unsigned char)buffer[i])) {
                is_number = 0;
                break;
            }
        }
        
        if (!is_number && len > 0) {
            is_word = 1;
            for (size_t i = 0; i < len; i++) {
                if (!isalpha((unsigned char)buffer[i])) {
                    is_word = 0;
                    break;
                }
            }
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
                    if (number_count > 100) {
                        current_state = ERROR;
                    }
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
                    if (word_count > 100) {
                        current_state = ERROR;
                    }
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
        
        if (current_state == READING_NUMBERS) {
            printf("Number entered. Total numbers: %d\n", number_count);
        } else if (current_state == READING_WORDS) {
            printf("Word entered. Total words: %d\n", word_count);
        }
    }
    
    if (current_state == ERROR) {
        printf("Error: Invalid input detected.\n");
        valid_input = 0;
    }
    
    if (valid_input) {
        printf("Processing complete.\n");
        printf("Final count - Numbers: %d, Words: %d\n", number_count, word_count);
    } else {
        printf("Processing failed due to invalid input.\n");
    }
    
    return valid_input ? 0 : 1;
}