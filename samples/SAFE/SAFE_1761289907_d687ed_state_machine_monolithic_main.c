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
                    numbers[num_count] = 0;
                } else if (isalpha(c)) {
                    current_state = READING_WORD;
                    word_count = 0;
                    memset(words[word_count], 0, sizeof(words[word_count]));
                    words[word_count][0] = c;
                    word_count = 1;
                } else {
                    current_state = SKIPPING;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(c)) {
                    if (num_count < 50) {
                        numbers[num_count] = numbers[num_count] * 10 + (c - '0');
                    }
                } else {
                    if (num_count < 50) {
                        num_count++;
                    }
                    if (isalpha(c)) {
                        current_state = READING_WORD;
                        word_count = 0;
                        memset(words[word_count], 0, sizeof(words[word_count]));
                        words[word_count][0] = c;
                        word_count = 1;
                    } else {
                        current_state = SKIPPING;
                    }
                }
                break;
                
            case READING_WORD:
                if (isalpha(c)) {
                    if (word_count > 0 && word_count - 1 < 50) {
                        size_t current_len = strlen(words[word_count - 1]);
                        if (current_len < 49) {
                            words[word_count - 1][current_len] = c;
                        }
                    }
                } else {
                    if (word_count > 0 && word_count - 1 < 50) {
                        word_count++;
                    }
                    if (isdigit(c)) {
                        current_state = READING_NUMBER;
                        if (num_count < 50) {
                            numbers[num_count] = c - '0';
                        }
                    } else {
                        current_state = SKIPPING;
                    }
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
                        memset(words[word_count], 0, sizeof(words[word_count]));
                        words[word_count][0] = c;
                        word_count++;
                    }
                }
                break;
                
            case END:
                break;
        }
        
        pos++;
        
        if (pos >= len) {
            current_state = END;
        }
    }
    
    if (current_state == READING_NUMBER && num_count < 50) {
        num_count++;
    }
    if (current_state == READING_WORD && word_count > 0 && word_count - 1 < 50) {
        word_count++;
    }
    
    printf("Numbers found: ");
    for (int i = 0; i < num_count && i < 50; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
    
    printf("Words found: ");
    for (int i = 0; i < word_count && i < 50; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    return 0;
}