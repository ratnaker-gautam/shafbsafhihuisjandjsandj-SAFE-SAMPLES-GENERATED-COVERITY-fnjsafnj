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
    
    while (pos < len && current_state != ERROR && current_state != DONE) {
        char c = buffer[pos];
        
        switch (current_state) {
            case START:
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
                if (isdigit(c)) {
                    if (numbers[num_count] > 214748364) {
                        current_state = ERROR;
                        break;
                    }
                    numbers[num_count] = numbers[num_count] * 10 + (c - '0');
                    pos++;
                } else if (isspace(c)) {
                    num_count++;
                    current_state = START;
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (word_count >= 50) {
                    current_state = ERROR;
                    break;
                }
                if (isalpha(c)) {
                    int word_len = strlen(words[word_count]);
                    if (word_len >= 49) {
                        current_state = ERROR;
                        break;
                    }
                    words[word_count][word_len] = c;
                    words[word_count][word_len + 1] = '\0';
                    pos++;
                } else if (isspace(c)) {
                    word_count++;
                    current_state = START;
                    pos++;
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
    
    if (current_state == READING_NUMBER && num_count < 50) {
        num_count++;
    } else if (current_state == READING_WORD && word_count < 50) {
        word_count++;
    }
    
    if (current_state == ERROR) {
        fprintf(stderr, "Error: Invalid input format\n");
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