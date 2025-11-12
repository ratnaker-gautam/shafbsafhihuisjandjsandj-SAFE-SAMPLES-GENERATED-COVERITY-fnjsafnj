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
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
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
        
        if (i >= len) {
            break;
        }
        
        int start = i;
        while (i < len && !isspace(input[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len > 0 && word_len < 64) {
            if (word_count < MAX_WORDS) {
                strncpy(words[word_count], input + start, word_len);
                words[word_count][word_len] = '\0';
                word_count++;
            } else {
                fprintf(stderr, "Too many words\n");
                break;
            }
        }
    }
    
    printf("Words found: %d\n", word_count);
    
    for (int j = 0; j < word_count; j++) {
        int k = 0;
        do {
            if (words[j][k] != '\0') {
                words[j][k] = toupper(words[j][k]);
                k++;
            }
        } while (words[j][k] != '\0');
    }
    
    printf("Processed words:\n");
    int j = 0;
    while (j < word_count) {
        printf("%s\n", words[j]);
        j++;
    }
    
    char longest[64] = "";
    for (int idx = 0; idx < word_count; idx++) {
        size_t current_len = strlen(words[idx]);
        size_t longest_len = strlen(longest);
        if (current_len > longest_len) {
            strncpy(longest, words[idx], sizeof(longest) - 1);
            longest[sizeof(longest) - 1] = '\0';
        }
    }
    
    if (strlen(longest) > 0) {
        printf("Longest word: %s\n", longest);
    }
    
    return 0;
}