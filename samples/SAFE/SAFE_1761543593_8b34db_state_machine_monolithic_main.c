//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

enum State { START, READING_NUMBER, READING_WORD, ERROR, DONE };

int main(void) {
    char buffer[256];
    int numbers[50];
    char words[50][50];
    int num_count = 0;
    int word_count = 0;
    enum State current_state = START;
    
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
    
    if (len >= sizeof(buffer) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int i = 0;
    while (i < len && current_state != ERROR && current_state != DONE) {
        char c = buffer[i];
        
        switch (current_state) {
            case START:
                if (isspace(c)) {
                    i++;
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
                int digits = 0;
                
                while (i < len && isdigit(buffer[i])) {
                    if (digits > 9) {
                        current_state = ERROR;
                        break;
                    }
                    
                    int digit = buffer[i] - '0';
                    if (num > (INT_MAX - digit) / 10) {
                        current_state = ERROR;
                        break;
                    }
                    
                    num = num * 10 + digit;
                    digits++;
                    i++;
                }
                
                if (current_state != ERROR) {
                    numbers[num_count++] = num;
                    
                    if (i >= len) {
                        current_state = DONE;
                    } else if (isspace(buffer[i])) {
                        current_state = START;
                        i++;
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
                while (i < len && isalpha(buffer[i])) {
                    if (word_len >= 49) {
                        current_state = ERROR;
                        break;
                    }
                    
                    words[word_count][word_len++] = buffer[i];
                    i++;
                }
                
                if (current_state != ERROR) {
                    words[word_count][word_len] = '\0';
                    word_count++;
                    
                    if (i >= len) {
                        current_state = DONE;
                    } else if (isspace(buffer[i])) {
                        current_state = START;
                        i++;
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
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    printf("Numbers found: ");
    for (int j = 0; j < num_count; j++) {
        printf("%d ", numbers[j]);
    }
    printf("\n");
    
    printf("Words found: ");
    for (int j = 0; j < word_count; j++) {
        printf("%s ", words[j]);
    }
    printf("\n");
    
    return 0;
}