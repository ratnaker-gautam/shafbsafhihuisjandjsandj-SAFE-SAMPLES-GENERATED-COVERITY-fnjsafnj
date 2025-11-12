//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 1024
#define MAX_WORDS 512

int main() {
    char input[MAX_INPUT];
    char words[MAX_WORDS][64];
    int word_count = 0;
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    int i = 0;
    while (i < len) {
        while (i < len && isspace(input[i])) {
            i++;
        }
        
        if (i >= len) break;
        
        int start = i;
        while (i < len && !isspace(input[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len > 63) {
            word_len = 63;
        }
        
        if (word_count < MAX_WORDS) {
            strncpy(words[word_count], &input[start], word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        } else {
            break;
        }
    }
    
    printf("Words found: %d\n", word_count);
    
    for (int j = 0; j < word_count; j++) {
        printf("Word %d: ", j + 1);
        
        int k = 0;
        do {
            putchar(words[j][k]);
            k++;
        } while (words[j][k] != '\0');
        printf("\n");
    }
    
    printf("Processing words...\n");
    
    int processed = 0;
    for (int j = 0; j < word_count; j++) {
        int has_digit = 0;
        char *ptr = words[j];
        
        while (*ptr) {
            if (isdigit(*ptr)) {
                has_digit = 1;
                break;
            }
            ptr++;
        }
        
        if (!has_digit) {
            int m = 0;
            int n = strlen(words[j]) - 1;
            
            while (m < n) {
                char temp = words[j][m];
                words[j][m] = words[j][n];
                words[j][n] = temp;
                m++;
                n--;
            }
            processed++;
        }
    }
    
    printf("Reversed %d words without digits\n", processed);
    printf("Final word list:\n");
    
    int j = 0;
    while (j < word_count) {
        printf("%s\n", words[j]);
        j++;
    }
    
    return 0;
}