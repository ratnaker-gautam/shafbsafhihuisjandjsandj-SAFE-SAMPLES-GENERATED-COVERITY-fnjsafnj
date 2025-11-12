//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

enum state {
    INIT,
    READING_NUMBER,
    READING_WORD,
    ERROR,
    DONE
};

int main(void) {
    enum state current_state = INIT;
    char buffer[256];
    size_t pos = 0;
    int number_sum = 0;
    int word_count = 0;
    int ch;
    
    printf("Enter text (max 255 chars): ");
    
    while ((ch = getchar()) != EOF && ch != '\n' && pos < sizeof(buffer) - 1) {
        if (current_state == ERROR) {
            break;
        }
        
        switch (current_state) {
            case INIT:
                if (isdigit(ch)) {
                    buffer[pos++] = (char)ch;
                    current_state = READING_NUMBER;
                } else if (isalpha(ch)) {
                    buffer[pos++] = (char)ch;
                    current_state = READING_WORD;
                } else if (!isspace(ch)) {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(ch)) {
                    if (pos < sizeof(buffer) - 1) {
                        buffer[pos++] = (char)ch;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace(ch)) {
                    buffer[pos] = '\0';
                    int num;
                    if (sscanf(buffer, "%d", &num) == 1) {
                        if (num > 0 && number_sum <= INT_MAX - num) {
                            number_sum += num;
                        } else {
                            current_state = ERROR;
                        }
                    }
                    pos = 0;
                    current_state = INIT;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(ch)) {
                    if (pos < sizeof(buffer) - 1) {
                        buffer[pos++] = (char)ch;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace(ch)) {
                    buffer[pos] = '\0';
                    word_count++;
                    pos = 0;
                    current_state = INIT;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
            case DONE:
                break;
        }
    }
    
    if (current_state == READING_NUMBER && pos > 0) {
        buffer[pos] = '\0';
        int num;
        if (sscanf(buffer, "%d", &num) == 1) {
            if (num > 0 && number_sum <= INT_MAX - num) {
                number_sum += num;
            } else {
                current_state = ERROR;
            }
        }
    } else if (current_state == READING_WORD && pos > 0) {
        word_count++;
    }
    
    if (current_state == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}