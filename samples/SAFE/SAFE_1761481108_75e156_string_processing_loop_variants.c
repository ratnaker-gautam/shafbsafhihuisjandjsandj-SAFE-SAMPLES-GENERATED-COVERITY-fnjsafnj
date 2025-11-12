//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
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
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace(input[i])) {
            i++;
        }
        
        if (i >= len) break;
        
        int start = i;
        while (i < len && !isspace(input[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len >= 64) {
            word_len = 63;
        }
        
        strncpy(words[word_count], &input[start], word_len);
        words[word_count][word_len] = '\0';
        word_count++;
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 1;
    }
    
    printf("Words found: %d\n", word_count);
    
    for (int j = 0; j < word_count; j++) {
        int k = 0;
        do {
            words[j][k] = toupper(words[j][k]);
            k++;
        } while (words[j][k] != '\0');
    }
    
    printf("Uppercase words:\n");
    int j = 0;
    while (j < word_count) {
        printf("%s\n", words[j]);
        j++;
    }
    
    char longest[64] = "";
    int longest_len = 0;
    
    for (int idx = 0; idx < word_count; idx++) {
        int current_len = strlen(words[idx]);
        if (current_len > longest_len) {
            longest_len = current_len;
            strncpy(longest, words[idx], sizeof(longest)-1);
            longest[sizeof(longest)-1] = '\0';
        }
    }
    
    if (longest_len > 0) {
        printf("Longest word: %s (%d characters)\n", longest, longest_len);
    }
    
    int vowel_counts[5] = {0};
    char vowels[] = "AEIOU";
    
    for (int idx1 = 0; idx1 < word_count; idx1++) {
        int idx2 = 0;
        while (words[idx1][idx2] != '\0') {
            for (int v = 0; v < 5; v++) {
                if (words[idx1][idx2] == vowels[v]) {
                    vowel_counts[v]++;
                }
            }
            idx2++;
        }
    }
    
    printf("Vowel counts:\n");
    printf("A: %d\n", vowel_counts[0]);
    printf("E: %d\n", vowel_counts[1]);
    printf("I: %d\n", vowel_counts[2]);
    printf("O: %d\n", vowel_counts[3]);
    printf("U: %d\n", vowel_counts[4]);
    
    return 0;
}