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
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
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
        printf("Word %d: \"%s\" (length: %d)\n", idx + 1, words[idx], length);
        
        int vowel_count = 0;
        int consonant_count = 0;
        char *ptr = words[idx];
        while (*ptr) {
            char c = tolower(*ptr);
            if (c >= 'a' && c <= 'z') {
                if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                    vowel_count++;
                } else {
                    consonant_count++;
                }
            }
            ptr++;
        }
        printf("  Vowels: %d, Consonants: %d\n", vowel_count, consonant_count);
    }
    
    int total_chars = 0;
    int k = 0;
    do {
        if (k < word_count) {
            total_chars += strlen(words[k]);
        }
        k++;
    } while (k < word_count);
    
    printf("Total characters in all words: %d\n", total_chars);
    
    char search_word[MAX_INPUT];
    printf("Enter a word to search for: ");
    if (fgets(search_word, sizeof(search_word), stdin) == NULL) {
        fprintf(stderr, "Error reading search word\n");
        return 1;
    }
    
    len = strlen(search_word);
    if (len > 0 && search_word[len - 1] == '\n') {
        search_word[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty search word\n");
        return 0;
    }
    
    int found = 0;
    int pos = 0;
    for (pos = 0; pos < word_count; pos++) {
        if (strcmp(words[pos], search_word) == 0) {
            found = 1;
            break;
        }
    }
    
    if (found) {
        printf("Word \"%s\" found at position %d\n", search_word, pos + 1);
    } else {
        printf("Word \"%s\" not found\n", search_word);
    }
    
    return 0;
}