//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <limits.h>

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
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    size_t i = 0;
    while (i < len && current_state != END) {
        switch (current_state) {
            case START:
                if (isdigit((unsigned char)buffer[i])) {
                    current_state = READING_NUMBER;
                } else if (isalpha((unsigned char)buffer[i])) {
                    current_state = READING_WORD;
                } else {
                    current_state = SKIPPING;
                    i++;
                }
                break;
                
            case READING_NUMBER:
                {
                    int value = 0;
                    int valid = 1;
                    size_t start = i;
                    
                    while (i < len && isdigit((unsigned char)buffer[i])) {
                        int digit = buffer[i] - '0';
                        if (value > (INT_MAX - digit) / 10) {
                            valid = 0;
                            break;
                        }
                        value = value * 10 + digit;
                        i++;
                    }
                    
                    if (valid && num_count < 50) {
                        numbers[num_count++] = value;
                    }
                    
                    if (i >= len) {
                        current_state = END;
                    } else if (!isdigit((unsigned char)buffer[i])) {
                        current_state = START;
                    }
                }
                break;
                
            case READING_WORD:
                {
                    size_t start = i;
                    int pos = 0;
                    
                    while (i < len && isalpha((unsigned char)buffer[i]) && pos < 49) {
                        words[word_count][pos++] = buffer[i];
                        i++;
                    }
                    
                    if (pos > 0 && word_count < 50) {
                        words[word_count][pos] = '\0';
                        word_count++;
                    }
                    
                    if (i >= len) {
                        current_state = END;
                    } else if (!isalpha((unsigned char)buffer[i])) {
                        current_state = START;
                    }
                }
                break;
                
            case SKIPPING:
                while (i < len && !isdigit((unsigned char)buffer[i]) && !isalpha((unsigned char)buffer[i])) {
                    i++;
                }
                if (i < len) {
                    current_state = START;
                } else {
                    current_state = END;
                }
                break;
                
            case END:
                break;
        }
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