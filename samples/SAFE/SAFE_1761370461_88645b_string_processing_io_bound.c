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
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty input\n");
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
        printf("No words found\n");
        return 0;
    }
    
    printf("\nProcessing words...\n");
    printf("Found %d words:\n", word_count);
    
    for (i = 0; i < word_count; i++) {
        printf("Word %d: %s", i + 1, words[i]);
        
        size_t word_len = strlen(words[i]);
        int has_upper = 0;
        int has_lower = 0;
        int has_digit = 0;
        
        for (j = 0; j < word_len; j++) {
            unsigned char c = words[i][j];
            if (isupper(c)) has_upper = 1;
            if (islower(c)) has_lower = 1;
            if (isdigit(c)) has_digit = 1;
        }
        
        printf(" (Length: %zu", word_len);
        if (has_upper) printf(", has uppercase");
        if (has_lower) printf(", has lowercase");
        if (has_digit) printf(", has digits");
        printf(")\n");
    }
    
    printf("\nRemoving duplicate words...\n");
    for (i = 0; i < word_count; i++) {
        for (j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                for (k = j; k < word_count - 1; k++) {
                    strcpy(words[k], words[k + 1]);
                }
                word_count--;
                j--;
            }
        }
    }
    
    printf("Unique words: %d\n", word_count);
    for (i = 0; i < word_count; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    return 0;
}