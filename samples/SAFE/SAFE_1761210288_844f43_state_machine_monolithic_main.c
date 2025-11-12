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
                if (isspace(ch)) {
                    pos++;
                } else if (isdigit(ch)) {
                    current_state = READING_NUMBER;
                } else if (isalpha(ch)) {
                    current_state = READING_WORD;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(ch)) {
                    pos++;
                } else if (isspace(ch) || ch == '\0') {
                    char num_str[20];
                    int start = pos;
                    while (start > 0 && isdigit(buffer[start - 1])) {
                        start--;
                    }
                    int len = pos - start;
                    if (len >= sizeof(num_str)) {
                        current_state = ERROR;
                        break;
                    }
                    memcpy(num_str, &buffer[start], len);
                    num_str[len] = '\0';
                    
                    long val = strtol(num_str, NULL, 10);
                    if (val < 0 || val > 1000000) {
                        current_state = ERROR;
                        break;
                    }
                    if (num_count < 50) {
                        numbers[num_count++] = (int)val;
                    }
                    current_state = START;
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(ch)) {
                    pos++;
                } else if (isspace(ch) || ch == '\0') {
                    int start = pos;
                    while (start > 0 && isalpha(buffer[start - 1])) {
                        start--;
                    }
                    int len = pos - start;
                    if (len >= 50) {
                        current_state = ERROR;
                        break;
                    }
                    if (word_count < 50) {
                        memcpy(words[word_count], &buffer[start], len);
                        words[word_count][len] = '\0';
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
    
    if (current_state == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    if (current_state == READING_NUMBER || current_state == READING_WORD) {
        printf("Error: Incomplete input\n");
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