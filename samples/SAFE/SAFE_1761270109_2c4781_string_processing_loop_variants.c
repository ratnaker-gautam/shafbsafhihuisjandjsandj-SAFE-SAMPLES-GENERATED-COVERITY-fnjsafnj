//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 256
#define MAX_WORDS 50

int main() {
    char input[MAX_INPUT];
    char words[MAX_WORDS][MAX_INPUT];
    int word_count = 0;
    
    printf("Enter a sentence: ");
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
        printf("Empty input\n");
        return 0;
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
        while (i < len && !isspace(input[i]) && j < MAX_INPUT - 1) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        words[word_count][j] = '\0';
        word_count++;
    }
    
    printf("Words found: %d\n", word_count);
    
    for (int idx = 0; idx < word_count; idx++) {
        int length = strlen(words[idx]);
        int vowel_count = 0;
        int consonant_count = 0;
        
        for (int pos = 0; pos < length; pos++) {
            char c = tolower(words[idx][pos]);
            if (c >= 'a' && c <= 'z') {
                if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                    vowel_count++;
                } else {
                    consonant_count++;
                }
            }
        }
        
        printf("Word %d: \"%s\" (vowels: %d, consonants: %d)\n", 
               idx + 1, words[idx], vowel_count, consonant_count);
    }
    
    if (word_count > 0) {
        char longest[MAX_INPUT];
        char shortest[MAX_INPUT];
        strcpy(longest, words[0]);
        strcpy(shortest, words[0]);
        
        int k = 1;
        do {
            if (strlen(words[k]) > strlen(longest)) {
                strcpy(longest, words[k]);
            }
            if (strlen(words[k]) < strlen(shortest)) {
                strcpy(shortest, words[k]);
            }
            k++;
        } while (k < word_count);
        
        printf("Longest word: %s\n", longest);
        printf("Shortest word: %s\n", shortest);
    }
    
    return 0;
}