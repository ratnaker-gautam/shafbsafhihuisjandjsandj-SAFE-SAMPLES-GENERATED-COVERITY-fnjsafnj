//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: state_machine
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
    int i = 0;
    int buffer_len = 0;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    buffer_len = strlen(buffer);
    if (buffer_len > 0 && buffer[buffer_len - 1] == '\n') {
        buffer[buffer_len - 1] = '\0';
        buffer_len--;
    }
    
    if (buffer_len <= 0) {
        printf("No input provided\n");
        return 0;
    }
    
    while (i < buffer_len) {
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
                    int number = 0;
                    int valid = 1;
                    int start = i;
                    
                    while (i < buffer_len && isdigit((unsigned char)buffer[i])) {
                        int digit = buffer[i] - '0';
                        if (number > (INT_MAX - digit) / 10) {
                            valid = 0;
                            break;
                        }
                        number = number * 10 + digit;
                        i++;
                    }
                    
                    if (valid && num_count < 50) {
                        numbers[num_count++] = number;
                    }
                    
                    if (i >= buffer_len) {
                        current_state = END;
                    } else if (!isdigit((unsigned char)buffer[i])) {
                        current_state = START;
                    }
                }
                break;
                
            case READING_WORD:
                {
                    int start = i;
                    int word_len = 0;
                    
                    while (i < buffer_len && isalpha((unsigned char)buffer[i])) {
                        if (word_len < 49) {
                            words[word_count][word_len++] = buffer[i];
                        }
                        i++;
                    }
                    
                    if (word_len > 0 && word_count < 50) {
                        words[word_count][word_len] = '\0';
                        word_count++;
                    }
                    
                    if (i >= buffer_len) {
                        current_state = END;
                    } else if (!isalpha((unsigned char)buffer[i])) {
                        current_state = START;
                    }
                }
                break;
                
            case SKIPPING:
                while (i < buffer_len && !isdigit((unsigned char)buffer[i]) && !isalpha((unsigned char)buffer[i])) {
                    i++;
                }
                if (i < buffer_len) {
                    current_state = START;
                } else {
                    current_state = END;
                }
                break;
                
            case END:
                i = buffer_len;
                break;
        }
    }
    
    printf("Found %d numbers:\n", num_count);
    for (int j = 0; j < num_count; j++) {
        printf("%d ", numbers[j]);
    }
    printf("\n");
    
    printf("Found %d words:\n", word_count);
    for (int j = 0; j < word_count; j++) {
        printf("%s ", words[j]);
    }
    printf("\n");
    
    return 0;
}