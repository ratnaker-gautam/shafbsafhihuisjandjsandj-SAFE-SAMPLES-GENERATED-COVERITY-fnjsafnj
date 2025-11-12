//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
        
        j = 0;
        while (i < input_len && j < MAX_WORD_LEN && !isspace((unsigned char)input[i])) {
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
        printf("No words found in input\n");
        return 0;
    }
    
    printf("\nProcessing words...\n");
    printf("Found %d words:\n", word_count);
    
    for (i = 0; i < word_count; i++) {
        printf("Word %d: ", i + 1);
        
        for (j = 0; words[i][j] != '\0'; j++) {
            if (isalpha((unsigned char)words[i][j])) {
                if (islower((unsigned char)words[i][j])) {
                    putchar(toupper((unsigned char)words[i][j]));
                } else {
                    putchar(tolower((unsigned char)words[i][j]));
                }
            } else {
                putchar(words[i][j]);
            }
        }
        printf(" (length: %zu)\n", strlen(words[i]));
    }
    
    printf("\nWord statistics:\n");
    printf("Total characters in all words: ");
    
    size_t total_chars = 0;
    for (i = 0; i < word_count; i++) {
        total_chars += strlen(words[i]);
    }
    printf("%zu\n", total_chars);
    
    printf("Average word length: ");
    if (word_count > 0) {
        printf("%.2f\n", (double)total_chars / word_count);
    } else {
        printf("0.00\n");
    }
    
    printf("Longest word(s): ");
    size_t max_len = 0;
    for (i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        if (len > max_len) {
            max_len = len;
        }
    }
    
    int first_longest = 1;
    for (i = 0; i < word_count; i++) {
        if (strlen(words[i]) == max_len) {
            if (!first_longest) {
                printf(", ");
            }
            printf("\"%s\"", words[i]);
            first_longest = 0;
        }
    }
    printf(" (length: %zu)\n", max_len);
    
    return 0;
}