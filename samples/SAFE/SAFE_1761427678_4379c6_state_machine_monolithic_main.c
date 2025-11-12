//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    int pos = 0;
    enum state current_state = START;
    
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
    
    while (pos < len && current_state != END) {
        char c = buffer[pos];
        
        switch (current_state) {
            case START:
                if (isdigit(c)) {
                    current_state = READING_NUMBER;
                    if (num_count < 50) {
                        numbers[num_count] = c - '0';
                    }
                } else if (isalpha(c)) {
                    current_state = READING_WORD;
                    if (word_count < 50) {
                        words[word_count][0] = tolower(c);
                        words[word_count][1] = '\0';
                    }
                } else {
                    current_state = SKIPPING;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(c)) {
                    if (num_count < 50) {
                        numbers[num_count] = numbers[num_count] * 10 + (c - '0');
                        if (numbers[num_count] < 0) {
                            numbers[num_count] = 0;
                        }
                    }
                } else if (isalpha(c)) {
                    current_state = READING_WORD;
                    num_count++;
                    if (word_count < 50) {
                        words[word_count][0] = tolower(c);
                        words[word_count][1] = '\0';
                    }
                } else {
                    current_state = SKIPPING;
                    num_count++;
                }
                break;
                
            case READING_WORD:
                if (isalpha(c)) {
                    if (word_count < 50) {
                        size_t word_len = strlen(words[word_count]);
                        if (word_len < 49) {
                            words[word_count][word_len] = tolower(c);
                            words[word_count][word_len + 1] = '\0';
                        }
                    }
                } else if (isdigit(c)) {
                    current_state = READING_NUMBER;
                    word_count++;
                    if (num_count < 50) {
                        numbers[num_count] = c - '0';
                    }
                } else {
                    current_state = SKIPPING;
                    word_count++;
                }
                break;
                
            case SKIPPING:
                if (isdigit(c)) {
                    current_state = READING_NUMBER;
                    if (num_count < 50) {
                        numbers[num_count] = c - '0';
                    }
                } else if (isalpha(c)) {
                    current_state = READING_WORD;
                    if (word_count < 50) {
                        words[word_count][0] = tolower(c);
                        words[word_count][1] = '\0';
                    }
                }
                break;
                
            case END:
                break;
        }
        
        pos++;
        if (pos >= len) {
            if (current_state == READING_NUMBER) {
                num_count++;
            } else if (current_state == READING_WORD) {
                word_count++;
            }
            current_state = END;
        }
    }
    
    printf("Found %d numbers: ", num_count);
    for (int i = 0; i < num_count && i < 50; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
    
    printf("Found %d words: ", word_count);
    for (int i = 0; i < word_count && i < 50; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    return 0;
}