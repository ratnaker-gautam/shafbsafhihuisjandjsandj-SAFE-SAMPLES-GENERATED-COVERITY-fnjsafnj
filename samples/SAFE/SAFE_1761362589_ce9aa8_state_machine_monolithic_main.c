//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    INIT,
    READING_NUMBER,
    READING_WORD,
    ERROR,
    DONE
};

int main(void) {
    char buffer[256];
    int numbers[50];
    char words[50][50];
    int num_count = 0;
    int word_count = 0;
    enum State current_state = INIT;
    
    printf("Enter input (mix of numbers and words, max 255 chars): ");
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
    
    size_t pos = 0;
    while (pos < len && current_state != ERROR && current_state != DONE) {
        char c = buffer[pos];
        
        switch (current_state) {
            case INIT:
                if (isspace(c)) {
                    pos++;
                } else if (isdigit(c)) {
                    current_state = READING_NUMBER;
                } else if (isalpha(c)) {
                    current_state = READING_WORD;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (num_count >= 50) {
                    current_state = ERROR;
                    break;
                }
                
                int num = 0;
                int digit_count = 0;
                
                while (pos < len && isdigit(buffer[pos])) {
                    if (digit_count >= 9) {
                        current_state = ERROR;
                        break;
                    }
                    
                    int digit = buffer[pos] - '0';
                    if (num > (2147483647 - digit) / 10) {
                        current_state = ERROR;
                        break;
                    }
                    
                    num = num * 10 + digit;
                    digit_count++;
                    pos++;
                }
                
                if (current_state != ERROR) {
                    numbers[num_count++] = num;
                    
                    if (pos >= len) {
                        current_state = DONE;
                    } else if (isspace(buffer[pos])) {
                        current_state = INIT;
                        pos++;
                    } else {
                        current_state = ERROR;
                    }
                }
                break;
                
            case READING_WORD:
                if (word_count >= 50) {
                    current_state = ERROR;
                    break;
                }
                
                int word_len = 0;
                while (pos < len && isalpha(buffer[pos])) {
                    if (word_len >= 49) {
                        current_state = ERROR;
                        break;
                    }
                    
                    words[word_count][word_len++] = buffer[pos];
                    pos++;
                }
                
                if (current_state != ERROR) {
                    words[word_count][word_len] = '\0';
                    word_count++;
                    
                    if (pos >= len) {
                        current_state = DONE;
                    } else if (isspace(buffer[pos])) {
                        current_state = INIT;
                        pos++;
                    } else {
                        current_state = ERROR;
                    }
                }
                break;
                
            case ERROR:
                break;
                
            case DONE:
                break;
        }
    }
    
    if (current_state == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Parsed %d numbers and %d words\n", num_count, word_count);
    
    if (num_count > 0) {
        printf("Numbers: ");
        for (int i = 0; i < num_count; i++) {
            printf("%d ", numbers[i]);
        }
        printf("\n");
    }
    
    if (word_count > 0) {
        printf("Words: ");
        for (int i = 0; i < word_count; i++) {
            printf("%s ", words[i]);
        }
        printf("\n");
    }
    
    return 0;
}