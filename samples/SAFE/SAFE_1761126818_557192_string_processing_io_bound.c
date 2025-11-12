//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
        
        j = 0;
        while (i < len && !isspace((unsigned char)input[i]) && j < MAX_WORD_LEN) {
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
    
    printf("\nProcessing %d words:\n", word_count);
    
    for (i = 0; i < word_count; i++) {
        int vowel_count = 0;
        int consonant_count = 0;
        int digit_count = 0;
        int other_count = 0;
        
        for (j = 0; words[i][j] != '\0'; j++) {
            char c = words[i][j];
            if (isalpha((unsigned char)c)) {
                char lower_c = tolower((unsigned char)c);
                if (lower_c == 'a' || lower_c == 'e' || lower_c == 'i' || 
                    lower_c == 'o' || lower_c == 'u') {
                    vowel_count++;
                } else {
                    consonant_count++;
                }
            } else if (isdigit((unsigned char)c)) {
                digit_count++;
            } else {
                other_count++;
            }
        }
        
        printf("Word %d: \"%s\" (length: %zu)\n", i + 1, words[i], strlen(words[i]));
        printf("  Vowels: %d, Consonants: %d, Digits: %d, Other: %d\n", 
               vowel_count, consonant_count, digit_count, other_count);
        
        if (strlen(words[i]) > 1) {
            printf("  Reversed: ");
            for (k = strlen(words[i]) - 1; k >= 0; k--) {
                putchar(words[i][k]);
            }
            printf("\n");
        }
        
        printf("\n");
    }
    
    printf("Word frequency analysis:\n");
    for (i = 0; i < word_count; i++) {
        if (words[i][0] == '\0') {
            continue;
        }
        
        int frequency = 1;
        for (j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                frequency++;
                words[j][0] = '\0';
            }
        }
        
        if (frequency > 0) {
            printf("'%s' appears %d time(s)\n", words[i], frequency);
        }
    }
    
    return 0;
}