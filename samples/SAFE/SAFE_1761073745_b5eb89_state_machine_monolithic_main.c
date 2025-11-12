//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: state_machine
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
    int pos = 0;
    enum State current_state = START;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
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
    
    while (pos <= len && current_state != ERROR && current_state != DONE) {
        char c = buffer[pos];
        
        switch (current_state) {
            case START:
                if (c == '\0') {
                    current_state = DONE;
                } else if (isdigit(c)) {
                    if (num_count < 50) {
                        numbers[num_count] = c - '0';
                        current_state = READING_NUMBER;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isalpha(c)) {
                    if (word_count < 50) {
                        words[word_count][0] = c;
                        words[word_count][1] = '\0';
                        current_state = READING_WORD;
                    } else {
                        current_state = ERROR;
                    }
                } else if (!isspace(c)) {
                    current_state = ERROR;
                }
                pos++;
                break;
                
            case READING_NUMBER:
                if (isdigit(c)) {
                    if (numbers[num_count] <= (INT_MAX - (c - '0')) / 10) {
                        numbers[num_count] = numbers[num_count] * 10 + (c - '0');
                    } else {
                        current_state = ERROR;
                    }
                    pos++;
                } else if (isspace(c) || c == '\0') {
                    num_count++;
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(c)) {
                    size_t word_len = strlen(words[word_count]);
                    if (word_len < 49) {
                        words[word_count][word_len] = c;
                        words[word_count][word_len + 1] = '\0';
                    } else {
                        current_state = ERROR;
                    }
                    pos++;
                } else if (isspace(c) || c == '\0') {
                    word_count++;
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                break;
                
            case DONE:
                break;
        }
    }
    
    if (current_state == READING_NUMBER) {
        num_count++;
    } else if (current_state == READING_WORD) {
        word_count++;
    }
    
    if (current_state == ERROR) {
        fprintf(stderr, "Parsing error at position %d\n", pos);
        return 1;
    }
    
    printf("Numbers found: %d\n", num_count);
    for (int i = 0; i < num_count; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
    
    printf("Words found: %d\n", word_count);
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    return 0;
}