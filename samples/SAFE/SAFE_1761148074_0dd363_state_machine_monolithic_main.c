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
    int numbers[100];
    char words[100][50];
    int num_count = 0;
    int word_count = 0;
    enum State current_state = INIT;
    int pos = 0;
    int ch;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    buffer[strcspn(buffer, "\n")] = '\0';
    
    while (current_state != DONE && current_state != ERROR) {
        ch = buffer[pos];
        
        switch (current_state) {
            case INIT:
                if (ch == '\0') {
                    current_state = DONE;
                } else if (isdigit(ch)) {
                    current_state = READING_NUMBER;
                    numbers[num_count] = ch - '0';
                } else if (isalpha(ch)) {
                    current_state = READING_WORD;
                    words[word_count][0] = ch;
                    words[word_count][1] = '\0';
                } else if (isspace(ch)) {
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(ch)) {
                    if (num_count < 100) {
                        numbers[num_count] = numbers[num_count] * 10 + (ch - '0');
                        pos++;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace(ch) || ch == '\0') {
                    num_count++;
                    current_state = INIT;
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(ch)) {
                    int len = strlen(words[word_count]);
                    if (len < 49 && word_count < 100) {
                        words[word_count][len] = ch;
                        words[word_count][len + 1] = '\0';
                        pos++;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace(ch) || ch == '\0') {
                    word_count++;
                    current_state = INIT;
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                fprintf(stderr, "Error processing input at position %d\n", pos);
                return 1;
                
            case DONE:
                break;
        }
    }
    
    if (current_state == ERROR) {
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