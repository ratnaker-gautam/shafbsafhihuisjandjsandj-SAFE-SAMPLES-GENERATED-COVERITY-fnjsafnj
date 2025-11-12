//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
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
    
    while (pos <= len && state != ERROR && state != DONE) {
        char c = buffer[pos];
        
        switch (state) {
            case START:
                if (c == '\0') {
                    state = DONE;
                } else if (isdigit(c)) {
                    if (num_count < 50) {
                        numbers[num_count] = c - '0';
                        state = READING_NUMBER;
                    } else {
                        state = ERROR;
                    }
                } else if (isalpha(c)) {
                    if (word_count < 50) {
                        words[word_count][0] = c;
                        words[word_count][1] = '\0';
                        state = READING_WORD;
                    } else {
                        state = ERROR;
                    }
                } else if (c != ' ' && c != '\t') {
                    state = ERROR;
                }
                pos++;
                break;
                
            case READING_NUMBER:
                if (isdigit(c)) {
                    if (num_count < 50) {
                        numbers[num_count] = numbers[num_count] * 10 + (c - '0');
                        pos++;
                    } else {
                        state = ERROR;
                    }
                } else if (c == ' ' || c == '\t' || c == '\0') {
                    num_count++;
                    state = START;
                } else {
                    state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(c)) {
                    size_t current_len = strlen(words[word_count]);
                    if (current_len < 49) {
                        words[word_count][current_len] = c;
                        words[word_count][current_len + 1] = '\0';
                        pos++;
                    } else {
                        state = ERROR;
                    }
                } else if (c == ' ' || c == '\t' || c == '\0') {
                    word_count++;
                    state = START;
                } else {
                    state = ERROR;
                }
                break;
                
            case ERROR:
                fprintf(stderr, "Invalid input format\n");
                return 1;
                
            case DONE:
                break;
        }
    }
    
    if (state == READING_NUMBER) {
        num_count++;
    } else if (state == READING_WORD) {
        word_count++;
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