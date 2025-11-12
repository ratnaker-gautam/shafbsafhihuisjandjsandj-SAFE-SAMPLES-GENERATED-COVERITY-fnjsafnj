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
    SKIPPING_SPACE,
    FINISHED
};

int main(void) {
    char buffer[256];
    size_t pos = 0;
    enum state current_state = START;
    int number_sum = 0;
    int word_count = 0;
    
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
    
    if (len >= sizeof(buffer)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    size_t i = 0;
    while (i < len) {
        switch (current_state) {
            case START:
                if (isdigit((unsigned char)buffer[i])) {
                    current_state = READING_NUMBER;
                    int num = buffer[i] - '0';
                    size_t j = i + 1;
                    while (j < len && isdigit((unsigned char)buffer[j])) {
                        if (num > INT_MAX / 10) {
                            fprintf(stderr, "Number overflow\n");
                            return 1;
                        }
                        num = num * 10 + (buffer[j] - '0');
                        if (num < 0) {
                            fprintf(stderr, "Number overflow\n");
                            return 1;
                        }
                        j++;
                    }
                    number_sum += num;
                    if (number_sum < 0) {
                        fprintf(stderr, "Sum overflow\n");
                        return 1;
                    }
                    i = j;
                    current_state = SKIPPING_SPACE;
                } else if (isalpha((unsigned char)buffer[i])) {
                    current_state = READING_WORD;
                    word_count++;
                    size_t j = i + 1;
                    while (j < len && isalpha((unsigned char)buffer[j])) {
                        j++;
                    }
                    i = j;
                    current_state = SKIPPING_SPACE;
                } else if (isspace((unsigned char)buffer[i])) {
                    current_state = SKIPPING_SPACE;
                    i++;
                } else {
                    i++;
                }
                break;
                
            case READING_NUMBER:
                current_state = SKIPPING_SPACE;
                break;
                
            case READING_WORD:
                current_state = SKIPPING_SPACE;
                break;
                
            case SKIPPING_SPACE:
                if (isspace((unsigned char)buffer[i])) {
                    i++;
                } else {
                    current_state = START;
                }
                break;
                
            case FINISHED:
                i = len;
                break;
        }
    }
    
    current_state = FINISHED;
    
    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}