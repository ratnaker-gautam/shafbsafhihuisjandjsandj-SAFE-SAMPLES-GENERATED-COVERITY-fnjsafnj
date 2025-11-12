//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
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
    int i = 0;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    buffer[strcspn(buffer, "\n")] = '\0';
    
    while (buffer[i] != '\0' && current_state != ERROR && current_state != DONE) {
        switch (current_state) {
            case START:
                if (isdigit(buffer[i])) {
                    current_state = READING_NUMBER;
                    numbers[num_count] = 0;
                } else if (isalpha(buffer[i])) {
                    current_state = READING_WORD;
                    pos = 0;
                    words[word_count][pos++] = buffer[i];
                } else if (buffer[i] == ' ') {
                    i++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                for (; buffer[i] != '\0' && isdigit(buffer[i]); i++) {
                    if (num_count < 50) {
                        numbers[num_count] = numbers[num_count] * 10 + (buffer[i] - '0');
                    }
                }
                if (num_count < 50) {
                    num_count++;
                }
                if (buffer[i] == ' ' || buffer[i] == '\0') {
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                for (; buffer[i] != '\0' && isalpha(buffer[i]); i++) {
                    if (word_count < 50 && pos < 49) {
                        words[word_count][pos++] = buffer[i];
                    }
                }
                if (word_count < 50 && pos < 49) {
                    words[word_count][pos] = '\0';
                    word_count++;
                }
                if (buffer[i] == ' ' || buffer[i] == '\0') {
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            default:
                break;
        }
        
        if (current_state != READING_NUMBER && current_state != READING_WORD) {
            i++;
        }
    }
    
    if (current_state == ERROR) {
        printf("Error: Invalid input format\n");
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