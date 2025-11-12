//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

enum State {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING_SPACE,
    FINISHED,
    ERROR
};

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
    
    buffer[strcspn(buffer, "\n")] = '\0';
    
    while (current_state != FINISHED && current_state != ERROR && buffer[pos] != '\0') {
        switch (current_state) {
            case START:
                if (isdigit(buffer[pos])) {
                    current_state = READING_NUMBER;
                } else if (isalpha(buffer[pos])) {
                    current_state = READING_WORD;
                } else if (isspace(buffer[pos])) {
                    current_state = SKIPPING_SPACE;
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (num_count >= 50) {
                    current_state = ERROR;
                    break;
                }
                numbers[num_count] = 0;
                while (isdigit(buffer[pos])) {
                    if (numbers[num_count] > (INT_MAX - (buffer[pos] - '0')) / 10) {
                        current_state = ERROR;
                        break;
                    }
                    numbers[num_count] = numbers[num_count] * 10 + (buffer[pos] - '0');
                    pos++;
                }
                if (current_state != ERROR) {
                    num_count++;
                    current_state = START;
                }
                break;
                
            case READING_WORD:
                if (word_count >= 50) {
                    current_state = ERROR;
                    break;
                }
                int word_pos = 0;
                while (isalpha(buffer[pos]) && word_pos < 49) {
                    words[word_count][word_pos++] = buffer[pos++];
                }
                words[word_count][word_pos] = '\0';
                word_count++;
                current_state = START;
                break;
                
            case SKIPPING_SPACE:
                while (isspace(buffer[pos])) {
                    pos++;
                }
                current_state = START;
                break;
                
            default:
                current_state = ERROR;
                break;
        }
    }
    
    if (current_state == ERROR) {
        fprintf(stderr, "Error processing input\n");
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