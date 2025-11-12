//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
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
        printf("Empty input\n");
        return 0;
    }
    
    if (len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        j = 0;
        while (i < len && !isspace((unsigned char)input[i]) && j < MAX_WORD_LEN) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        
        if (j > 0) {
            words[word_count][j] = '\0';
            word_count++;
        }
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    printf("\nWord count: %d\n", word_count);
    printf("Words in reverse order:\n");
    
    for (i = word_count - 1; i >= 0; i--) {
        printf("%s\n", words[i]);
    }
    
    printf("\nProcessing words...\n");
    
    for (i = 0; i < word_count; i++) {
        int has_upper = 0;
        int has_lower = 0;
        int has_digit = 0;
        size_t word_len = strlen(words[i]);
        
        for (j = 0; j < word_len; j++) {
            unsigned char c = words[i][j];
            if (isupper(c)) has_upper = 1;
            if (islower(c)) has_lower = 1;
            if (isdigit(c)) has_digit = 1;
        }
        
        printf("Word %d: \"%s\" (length: %zu)", i + 1, words[i], word_len);
        
        if (has_upper && has_lower) {
            printf(" - Mixed case");
        } else if (has_upper) {
            printf(" - All uppercase");
        } else if (has_lower) {
            printf(" - All lowercase");
        }
        
        if (has_digit) {
            printf(" - Contains digits");
        }
        
        printf("\n");
    }
    
    printf("\nFinding palindromes:\n");
    int palindrome_count = 0;
    
    for (i = 0; i < word_count; i++) {
        size_t word_len = strlen(words[i]);
        int is_palindrome = 1;
        
        if (word_len <= 1) {
            continue;
        }
        
        for (j = 0, k = word_len - 1; j < k; j++, k--) {
            if (tolower((unsigned char)words[i][j]) != tolower((unsigned char)words[i][k])) {
                is_palindrome = 0;
                break;
            }
        }
        
        if (is_palindrome) {
            printf("Palindrome found: \"%s\"\n", words[i]);
            palindrome_count++;
        }
    }
    
    if (palindrome_count == 0) {
        printf("No palindromes found\n");
    } else {
        printf("Total palindromes: %d\n", palindrome_count);
    }
    
    return 0;
}