//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 30

int main() {
    char input[MAX_INPUT + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    
    printf("Enter a string (max %d characters): ", MAX_INPUT);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input string\n");
        return 0;
    }
    
    if (len > MAX_INPUT) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace(input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        int j = 0;
        while (i < len && !isspace(input[i]) && j < MAX_WORD_LEN) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        
        if (j > 0) {
            words[word_count][j] = '\0';
            word_count++;
        }
    }
    
    printf("Found %d words:\n", word_count);
    
    for (int idx = 0; idx < word_count; idx++) {
        int has_upper = 0;
        int has_lower = 0;
        int has_digit = 0;
        
        char *ptr = words[idx];
        while (*ptr != '\0') {
            if (isupper(*ptr)) {
                has_upper = 1;
            } else if (islower(*ptr)) {
                has_lower = 1;
            } else if (isdigit(*ptr)) {
                has_digit = 1;
            }
            ptr++;
        }
        
        printf("Word %d: \"%s\" - ", idx + 1, words[idx]);
        
        if (has_upper && has_lower && has_digit) {
            printf("Mixed case with digits\n");
        } else if (has_upper && has_lower) {
            printf("Mixed case\n");
        } else if (has_upper) {
            printf("All uppercase\n");
        } else if (has_lower) {
            printf("All lowercase\n");
        } else if (has_digit) {
            printf("Digits only\n");
        } else {
            printf("Other characters\n");
        }
    }
    
    printf("\nProcessing words with different loop styles:\n");
    
    int k = 0;
    do {
        if (k < word_count) {
            int vowel_count = 0;
            char *ch = words[k];
            do {
                char c = *ch;
                if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
                    c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U') {
                    vowel_count++;
                }
                ch++;
            } while (*ch != '\0');
            
            printf("Word \"%s\" has %d vowels\n", words[k], vowel_count);
        }
        k++;
    } while (k < word_count);
    
    return 0;
}