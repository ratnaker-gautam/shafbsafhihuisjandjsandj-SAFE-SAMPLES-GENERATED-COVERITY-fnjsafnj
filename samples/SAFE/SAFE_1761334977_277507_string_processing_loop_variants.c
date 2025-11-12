//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 256
#define MAX_WORDS 50

int main() {
    char input[MAX_INPUT + 1];
    char words[MAX_WORDS][MAX_INPUT + 1];
    int word_count = 0;
    
    printf("Enter a string: ");
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
    
    if (len >= MAX_INPUT) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        int start = i;
        while (i < len && !isspace((unsigned char)input[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len > 0 && word_len <= MAX_INPUT) {
            strncpy(words[word_count], input + start, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    printf("Words found: %d\n", word_count);
    
    for (int j = 0; j < word_count; j++) {
        int k = 0;
        while (words[j][k] != '\0') {
            words[j][k] = toupper((unsigned char)words[j][k]);
            k++;
        }
    }
    
    printf("Uppercase words:\n");
    for (int j = 0; j < word_count; j++) {
        printf("%s\n", words[j]);
    }
    
    char longest[MAX_INPUT + 1] = "";
    for (int j = 0; j < word_count; j++) {
        if (strlen(words[j]) > strlen(longest)) {
            strncpy(longest, words[j], MAX_INPUT);
            longest[MAX_INPUT] = '\0';
        }
    }
    
    if (strlen(longest) > 0) {
        printf("Longest word: %s\n", longest);
    }
    
    int total_chars = 0;
    for (int j = 0; j < word_count; j++) {
        total_chars += strlen(words[j]);
    }
    
    printf("Total characters in words: %d\n", total_chars);
    
    return 0;
}