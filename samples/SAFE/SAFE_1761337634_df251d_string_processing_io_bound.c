//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][64];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter text to analyze (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (i < input_len && word_count < MAX_WORDS) {
        while (i < input_len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        j = i;
        while (j < input_len && !isspace((unsigned char)input[j])) {
            j++;
        }
        
        int word_len = j - i;
        if (word_len > 0 && word_len < 64) {
            strncpy(words[word_count], &input[i], word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
        
        i = j;
    }
    
    printf("\nWord analysis:\n");
    printf("Total words: %d\n", word_count);
    
    if (word_count > 0) {
        printf("\nWord frequency:\n");
        
        for (i = 0; i < word_count; i++) {
            if (words[i][0] == '\0') {
                continue;
            }
            
            int count = 1;
            for (j = i + 1; j < word_count; j++) {
                if (strcmp(words[i], words[j]) == 0) {
                    count++;
                    words[j][0] = '\0';
                }
            }
            
            printf("'%s': %d\n", words[i], count);
        }
        
        printf("\nWord lengths:\n");
        for (i = 0; i < word_count; i++) {
            if (words[i][0] != '\0') {
                printf("'%s': %zu characters\n", words[i], strlen(words[i]));
            }
        }
    }
    
    int letter_count = 0;
    int digit_count = 0;
    int space_count = 0;
    
    for (i = 0; i < input_len; i++) {
        unsigned char c = input[i];
        if (isalpha(c)) {
            letter_count++;
        } else if (isdigit(c)) {
            digit_count++;
        } else if (isspace(c)) {
            space_count++;
        }
    }
    
    printf("\nCharacter analysis:\n");
    printf("Letters: %d\n", letter_count);
    printf("Digits: %d\n", digit_count);
    printf("Spaces: %d\n", space_count);
    printf("Total characters: %zu\n", input_len);
    
    return 0;
}