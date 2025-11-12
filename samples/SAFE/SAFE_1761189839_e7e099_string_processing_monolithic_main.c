//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_INPUT_LEN + 1];
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
        printf("No input provided\n");
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
        
        j = i;
        while (j < len && !isspace((unsigned char)input[j])) {
            j++;
        }
        
        int word_len = j - i;
        if (word_len >= MAX_INPUT_LEN) {
            fprintf(stderr, "Word too long\n");
            return 1;
        }
        
        if (word_len > 0) {
            for (k = 0; k < word_len; k++) {
                words[word_count][k] = input[i + k];
            }
            words[word_count][word_len] = '\0';
            word_count++;
        }
        
        i = j;
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    printf("Found %d words:\n", word_count);
    for (i = 0; i < word_count; i++) {
        printf("%d: \"%s\"\n", i + 1, words[i]);
    }
    
    printf("\nProcessing words...\n");
    
    int total_chars = 0;
    int longest_word = 0;
    int shortest_word = MAX_INPUT_LEN;
    
    for (i = 0; i < word_count; i++) {
        int current_len = strlen(words[i]);
        total_chars += current_len;
        
        if (current_len > longest_word) {
            longest_word = current_len;
        }
        
        if (current_len < shortest_word) {
            shortest_word = current_len;
        }
        
        for (j = 0; words[i][j] != '\0'; j++) {
            words[i][j] = toupper((unsigned char)words[i][j]);
        }
    }
    
    printf("Statistics:\n");
    printf("Total words: %d\n", word_count);
    printf("Total characters: %d\n", total_chars);
    printf("Longest word length: %d\n", longest_word);
    printf("Shortest word length: %d\n", shortest_word);
    printf("Average word length: %.2f\n", word_count > 0 ? (float)total_chars / word_count : 0.0);
    
    printf("\nWords in uppercase:\n");
    for (i = 0; i < word_count; i++) {
        printf("%s", words[i]);
        if (i < word_count - 1) {
            printf(" ");
        }
    }
    printf("\n");
    
    return 0;
}