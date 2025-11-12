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
        while (i < input_len && isspace(input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        j = 0;
        while (i < input_len && !isspace(input[i]) && j < MAX_WORD_LEN) {
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
        printf("Word %d: \"%s\" (length: %zu)\n", 
               i + 1, words[i], strlen(words[i]));
        
        for (j = 0; words[i][j] != '\0'; j++) {
            if (islower(words[i][j])) {
                words[i][j] = toupper(words[i][j]);
            } else if (isupper(words[i][j])) {
                words[i][j] = tolower(words[i][j]);
            }
        }
    }
    
    printf("\nModified words (case-swapped):\n");
    for (i = 0; i < word_count; i++) {
        printf("%s", words[i]);
        if (i < word_count - 1) {
            printf(" ");
        }
    }
    printf("\n");
    
    printf("\nWord statistics:\n");
    int total_chars = 0;
    int shortest_len = MAX_WORD_LEN;
    int longest_len = 0;
    
    for (i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        total_chars += len;
        
        if (len < shortest_len) {
            shortest_len = len;
        }
        if (len > longest_len) {
            longest_len = len;
        }
    }
    
    printf("Total characters: %d\n", total_chars);
    printf("Shortest word: %d characters\n", shortest_len);
    printf("Longest word: %d characters\n", longest_len);
    printf("Average word length: %.2f\n", (float)total_chars / word_count);
    
    return 0;
}