//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 30

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int char_count = 0;
    int vowel_count = 0;
    int consonant_count = 0;
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN);
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
        printf("Empty input string\n");
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int i = 0;
    while (i < input_len) {
        while (i < input_len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        int word_start = i;
        while (i < input_len && !isspace((unsigned char)input[i])) {
            i++;
        }
        
        int word_len = i - word_start;
        if (word_len > 0 && word_count < MAX_WORDS) {
            if (word_len > MAX_WORD_LEN) {
                word_len = MAX_WORD_LEN;
            }
            
            int j;
            for (j = 0; j < word_len; j++) {
                words[word_count][j] = input[word_start + j];
            }
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    for (int idx = 0; idx < input_len; idx++) {
        char c = tolower((unsigned char)input[idx]);
        if (isalpha((unsigned char)c)) {
            char_count++;
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                vowel_count++;
            } else {
                consonant_count++;
            }
        }
    }
    
    printf("Input analysis:\n");
    printf("Total characters: %d\n", (int)input_len);
    printf("Alphabetic characters: %d\n", char_count);
    printf("Vowels: %d\n", vowel_count);
    printf("Consonants: %d\n", consonant_count);
    printf("Words found: %d\n", word_count);
    
    if (word_count > 0) {
        printf("Words:\n");
        int k = 0;
        do {
            printf("  %d: %s\n", k + 1, words[k]);
            k++;
        } while (k < word_count);
    }
    
    printf("Character frequency:\n");
    int freq[26] = {0};
    int m;
    for (m = 0; m < input_len; m++) {
        char ch = tolower((unsigned char)input[m]);
        if (ch >= 'a' && ch <= 'z') {
            freq[ch - 'a']++;
        }
    }
    
    int n = 0;
    while (n < 26) {
        if (freq[n] > 0) {
            printf("  %c: %d\n", 'a' + n, freq[n]);
        }
        n++;
    }
    
    return 0;
}