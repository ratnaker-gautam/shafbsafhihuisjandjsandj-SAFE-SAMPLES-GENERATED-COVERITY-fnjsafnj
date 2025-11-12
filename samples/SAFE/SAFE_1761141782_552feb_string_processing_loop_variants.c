//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    char temp[MAX_WORD_LEN + 1];
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN);
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
    
    if (len > MAX_INPUT_LEN) {
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
            temp[j++] = input[i++];
        }
        temp[j] = '\0';
        
        if (j > 0) {
            strncpy(words[word_count], temp, MAX_WORD_LEN);
            words[word_count][MAX_WORD_LEN] = '\0';
            word_count++;
        }
    }
    
    printf("Found %d words:\n", word_count);
    
    for (int k = 0; k < word_count; k++) {
        int word_len = strlen(words[k]);
        char processed[MAX_WORD_LEN + 1];
        int p = 0;
        
        for (int m = 0; m < word_len; m++) {
            if (isalpha(words[k][m])) {
                processed[p++] = tolower(words[k][m]);
            }
        }
        processed[p] = '\0';
        
        if (p > 0) {
            printf("Word %d: '%s' -> '%s' (length: %d)\n", 
                   k + 1, words[k], processed, p);
        } else {
            printf("Word %d: '%s' -> empty after processing\n", k + 1, words[k]);
        }
    }
    
    printf("\nProcessing variants:\n");
    
    int total_chars = 0;
    int k = 0;
    do {
        if (k < word_count) {
            total_chars += strlen(words[k]);
        }
        k++;
    } while (k < word_count);
    printf("Total characters in all words: %d\n", total_chars);
    
    char longest_word[MAX_WORD_LEN + 1] = "";
    int max_len = 0;
    for (int k = 0; k < word_count; k++) {
        int current_len = strlen(words[k]);
        if (current_len > max_len) {
            max_len = current_len;
            strncpy(longest_word, words[k], MAX_WORD_LEN);
            longest_word[MAX_WORD_LEN] = '\0';
        }
    }
    
    if (max_len > 0) {
        printf("Longest word: '%s' (length: %d)\n", longest_word, max_len);
    }
    
    int vowel_count = 0;
    int kk = 0;
    while (kk < word_count) {
        const char* word = words[kk];
        int word_len = strlen(word);
        int w = 0;
        while (w < word_len) {
            char c = tolower(word[w]);
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                vowel_count++;
            }
            w++;
        }
        kk++;
    }
    printf("Total vowels in all words: %d\n", vowel_count);
    
    return 0;
}