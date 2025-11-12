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
    enum State current_state = START;
    int pos = 0;
    int ch;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    buffer[strcspn(buffer, "\n")] = '\0';
    
    while (current_state != DONE && current_state != ERROR && buffer[pos] != '\0') {
        ch = buffer[pos];
        
        switch (current_state) {
            case START:
                if (isdigit(ch)) {
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
                    if (num_count < 50) {
                        numbers[num_count] = numbers[num_count] * 10 + (ch - '0');
                    }
                    pos++;
                } else if (isspace(ch) || buffer[pos + 1] == '\0') {
                    if (num_count < 50) {
                        num_count++;
                    }
                    current_state = START;
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(ch)) {
                    int len = strlen(words[word_count]);
                    if (len < 49) {
                        words[word_count][len] = ch;
                        words[word_count][len + 1] = '\0';
                    }
                    pos++;
                } else if (isspace(ch) || buffer[pos + 1] == '\0') {
                    if (word_count < 50) {
                        word_count++;
                    }
                    current_state = START;
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            default:
                break;
        }
    }
    
    if (current_state == READING_NUMBER && num_count < 50) {
        num_count++;
    }
    if (current_state == READING_WORD && word_count < 50) {
        word_count++;
    }
    
    if (current_state == ERROR) {
        printf("Error: Invalid input format at position %d\n", pos);
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