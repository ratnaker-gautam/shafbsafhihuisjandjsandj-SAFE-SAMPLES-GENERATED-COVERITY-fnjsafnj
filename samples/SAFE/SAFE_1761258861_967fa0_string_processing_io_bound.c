//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
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
        printf("Empty input\n");
        return 0;
    }
    
    i = 0;
    while (i < input_len && word_count < MAX_WORDS) {
        while (i < input_len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        j = 0;
        while (i < input_len && j < MAX_WORD_LEN - 1 && !isspace((unsigned char)input[i])) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        words[word_count][j] = '\0';
        
        if (j > 0) {
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
        size_t len = strlen(words[i]);
        int has_upper = 0;
        int has_lower = 0;
        int has_digit = 0;
        
        for (j = 0; j < len; j++) {
            if (isupper((unsigned char)words[i][j])) {
                has_upper = 1;
            } else if (islower((unsigned char)words[i][j])) {
                has_lower = 1;
            } else if (isdigit((unsigned char)words[i][j])) {
                has_digit = 1;
            }
        }
        
        printf("Word %d: \"%s\" (length: %zu)", i + 1, words[i], len);
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
    
    printf("\nFinding longest word...\n");
    int longest_index = 0;
    size_t max_len = strlen(words[0]);
    for (i = 1; i < word_count; i++) {
        size_t current_len = strlen(words[i]);
        if (current_len > max_len) {
            max_len = current_len;
            longest_index = i;
        }
    }
    printf("Longest word: \"%s\" (length: %zu)\n", words[longest_index], max_len);
    
    return 0;
}