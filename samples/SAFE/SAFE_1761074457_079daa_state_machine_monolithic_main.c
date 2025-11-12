//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUMBER, READING_WORD, ERROR, DONE };

int main(void) {
    char buffer[256];
    int numbers[50];
    char words[50][50];
    int num_count = 0;
    int word_count = 0;
    int pos = 0;
    enum State state = START;
    
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
    
    while (pos < len && state != ERROR && state != DONE) {
        char c = buffer[pos];
        
        switch (state) {
            case START:
                if (isspace(c)) {
                    pos++;
                } else if (isdigit(c)) {
                    state = READING_NUMBER;
                } else if (isalpha(c)) {
                    state = READING_WORD;
                } else {
                    state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (num_count >= 50) {
                    state = ERROR;
                    break;
                }
                
                int num = 0;
                int digit_count = 0;
                
                while (pos < len && isdigit(buffer[pos])) {
                    if (digit_count >= 9) {
                        state = ERROR;
                        break;
                    }
                    
                    int digit = buffer[pos] - '0';
                    if (num > (2147483647 - digit) / 10) {
                        state = ERROR;
                        break;
                    }
                    
                    num = num * 10 + digit;
                    digit_count++;
                    pos++;
                }
                
                if (state != ERROR) {
                    numbers[num_count++] = num;
                    
                    if (pos >= len || isspace(buffer[pos])) {
                        state = START;
                    } else {
                        state = ERROR;
                    }
                }
                break;
                
            case READING_WORD:
                if (word_count >= 50) {
                    state = ERROR;
                    break;
                }
                
                int word_len = 0;
                while (pos < len && isalpha(buffer[pos])) {
                    if (word_len >= 49) {
                        state = ERROR;
                        break;
                    }
                    
                    words[word_count][word_len++] = buffer[pos];
                    pos++;
                }
                
                if (state != ERROR) {
                    words[word_count][word_len] = '\0';
                    word_count++;
                    
                    if (pos >= len || isspace(buffer[pos])) {
                        state = START;
                    } else {
                        state = ERROR;
                    }
                }
                break;
                
            case ERROR:
                break;
                
            case DONE:
                break;
        }
        
        if (state != READING_NUMBER && state != READING_WORD) {
            pos++;
        }
    }
    
    if (state == ERROR) {
        fprintf(stderr, "Error processing input at position %d\n", pos);
        return 1;
    }
    
    printf("Numbers found: ");
    for (int i = 0; i < num_count; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
    
    printf("Words found: ");
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    return 0;
}