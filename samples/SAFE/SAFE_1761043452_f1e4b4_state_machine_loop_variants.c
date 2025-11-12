//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum state {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING,
    END
};

int main(void) {
    char buffer[256];
    int numbers[50];
    char words[50][50];
    int num_count = 0;
    int word_count = 0;
    enum state current_state = START;
    
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
    
    if (len >= sizeof(buffer)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    size_t i = 0;
    while (i < len && current_state != END) {
        char c = buffer[i];
        
        switch (current_state) {
            case START:
                if (isdigit(c)) {
                    current_state = READING_NUMBER;
                    if (num_count < 50) {
                        numbers[num_count] = 0;
                    }
                } else if (isalpha(c)) {
                    current_state = READING_WORD;
                    if (word_count < 50) {
                        words[word_count][0] = '\0';
                    }
                } else {
                    current_state = SKIPPING;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(c)) {
                    if (num_count < 50) {
                        numbers[num_count] = numbers[num_count] * 10 + (c - '0');
                    }
                    i++;
                } else {
                    if (num_count < 50) {
                        num_count++;
                    }
                    current_state = START;
                }
                break;
                
            case READING_WORD:
                if (isalpha(c)) {
                    if (word_count < 50) {
                        size_t word_len = strlen(words[word_count]);
                        if (word_len < 49) {
                            words[word_count][word_len] = c;
                            words[word_count][word_len+1] = '\0';
                        }
                    }
                    i++;
                } else {
                    if (word_count < 50) {
                        word_count++;
                    }
                    current_state = START;
                }
                break;
                
            case SKIPPING:
                if (isdigit(c) || isalpha(c)) {
                    current_state = START;
                } else {
                    i++;
                }
                break;
                
            case END:
                break;
        }
    }
    
    if (current_state == READING_NUMBER && num_count < 50) {
        num_count++;
    }
    if (current_state == READING_WORD && word_count < 50) {
        word_count++;
    }
    
    printf("Found %d numbers: ", num_count);
    for (int j = 0; j < num_count; j++) {
        printf("%d ", numbers[j]);
    }
    printf("\n");
    
    printf("Found %d words: ", word_count);
    for (int j = 0; j < word_count; j++) {
        printf("%s ", words[j]);
    }
    printf("\n");
    
    return 0;
}