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
        printf("Input error\n");
        return 1;
    }
    
    buffer[strcspn(buffer, "\n")] = '\0';
    
    while (buffer[i] != '\0') {
        switch (current_state) {
            case START:
                if (isdigit(buffer[i])) {
                    current_state = READING_NUMBER;
                    numbers[num_count] = 0;
                    pos = 0;
                    i--;
                } else if (isalpha(buffer[i])) {
                    current_state = READING_WORD;
                    pos = 0;
                    i--;
                } else if (buffer[i] != ' ') {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                for (; isdigit(buffer[i]) && buffer[i] != '\0'; i++) {
                    if (num_count >= 50) {
                        current_state = ERROR;
                        break;
                    }
                    numbers[num_count] = numbers[num_count] * 10 + (buffer[i] - '0');
                }
                if (current_state != ERROR) {
                    num_count++;
                    current_state = START;
                }
                break;
                
            case READING_WORD:
                for (; isalpha(buffer[i]) && buffer[i] != '\0'; i++) {
                    if (word_count >= 50 || pos >= 49) {
                        current_state = ERROR;
                        break;
                    }
                    words[word_count][pos++] = buffer[i];
                }
                if (current_state != ERROR) {
                    words[word_count][pos] = '\0';
                    word_count++;
                    current_state = START;
                }
                break;
                
            case ERROR:
                printf("Processing error at position %d\n", i);
                return 1;
                
            case DONE:
                break;
        }
        
        if (current_state != READING_NUMBER && current_state != READING_WORD) {
            i++;
        }
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