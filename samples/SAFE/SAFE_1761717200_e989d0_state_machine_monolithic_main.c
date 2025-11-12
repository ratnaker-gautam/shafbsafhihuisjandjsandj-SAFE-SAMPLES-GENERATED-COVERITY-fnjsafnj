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
    enum State current_state = START;
    int pos = 0;
    int i;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    buffer[strcspn(buffer, "\n")] = '\0';
    
    while (buffer[pos] != '\0' && current_state != ERROR && current_state != DONE) {
        char c = buffer[pos];
        
        switch (current_state) {
            case START:
                if (isspace(c)) {
                    pos++;
                } else if (isdigit(c)) {
                    current_state = READING_NUMBER;
                    numbers[num_count] = 0;
                } else if (isalpha(c)) {
                    current_state = READING_WORD;
                    word_count++;
                    words[word_count-1][0] = c;
                    words[word_count-1][1] = '\0';
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(c)) {
                    if (num_count < 50) {
                        numbers[num_count] = numbers[num_count] * 10 + (c - '0');
                        pos++;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace(c)) {
                    num_count++;
                    current_state = START;
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(c)) {
                    int len = strlen(words[word_count-1]);
                    if (len < 49) {
                        words[word_count-1][len] = c;
                        words[word_count-1][len+1] = '\0';
                        pos++;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace(c)) {
                    current_state = START;
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            default:
                current_state = ERROR;
                break;
        }
    }
    
    if (current_state == READING_NUMBER && num_count < 50) {
        num_count++;
    }
    
    if (current_state == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Numbers found: ");
    for (i = 0; i < num_count; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
    
    printf("Words found: ");
    for (i = 0; i < word_count; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    return 0;
}