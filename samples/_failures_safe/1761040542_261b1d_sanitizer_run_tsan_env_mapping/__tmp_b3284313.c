//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

int main() {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_INPUT_LEN];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter a string: ");
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
        printf("Empty input string\n");
        return 0;
    }
    
    if (len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace(input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        j = i;
        while (j < len && !isspace(input[j])) {
            j++;
        }
        
        size_t word_len = j - i;
        if (word_len >= MAX_INPUT_LEN) {
            fprintf(stderr, "Word too long\n");
            return 1;
        }
        
        for (k = 0; k < word_len; k++) {
            words[word_count][k] = input[i + k];
        }
        words[word_count][word_len] = '\0';
        word_count++;
        
        i = j;
    }
    
    printf("Words found: %d\n", word_count);
    
    for (i = 0; i < word_count; i++) {
        printf("Word %d: ", i + 1);
        
        int char_count = 0;
        char current_word[MAX_INPUT_LEN];
        strncpy(current_word, words[i], sizeof(current_word) - 1);
        current_word[sizeof(current_word) - 1] = '\0';
        
        size_t word_len = strlen(current_word);
        for (j = 0; j < word_len; j++) {
            if (isalnum(current_word[j])) {
                char_count++;
            }
        }
        printf("'%s' (length: %zu, alphanumeric chars: %d)\n", 
               current_word, word_len, char_count);
    }
    
    printf("\nProcessing words with different loop styles:\n");
    
    printf("For loop iteration:\n");
    for (i = 0; i < word_count; i++) {
        printf("  Processing: %s\n", words[i]);
    }
    
    printf("While loop iteration:\n");
    i = 0;
    while (i < word_count) {
        printf("  Processing: %s\n", words[i]);
        i++;
    }
    
    printf("Do-while loop iteration:\n");
    if (word_count > 0) {
        i = 0;
        do {
            printf("  Processing: %s\n", words[i]);
            i++;
        } while (i < word_count);
    }
    
    printf("Reverse iteration:\n");
    for (i = word_count - 1; i >= 0; i--) {
        printf("  Reverse: %s\n", words[i]);
    }
    
    printf("Character analysis:\n");
    for (i = 0; i < word_count; i++) {
        printf("  Word '%s': ", words[i]);
        int vowels = 0, consonants = 0, digits = 0;
        
        for (j = 0; words[i][j] != '\0'; j++) {
            char c = tolower(words[i][j]);
            if (c >= '0' && c <= '9') {
                digits++;
            } else if (c >= 'a' && c <= 'z') {
                if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                    vowels++;
                } else {
                    consonants++;
                }
            }
        }
        printf("vowels=%d, consonants=%d, digits=%d\n", vowels, consonants, digits);
    }
    
    return 0;
}