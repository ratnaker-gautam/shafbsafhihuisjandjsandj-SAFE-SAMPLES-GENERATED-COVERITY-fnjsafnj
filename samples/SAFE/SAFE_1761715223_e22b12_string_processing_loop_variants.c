//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 1024

int main() {
    char input[MAX_INPUT];
    char processed[MAX_INPUT];
    int counts[26] = {0};
    
    printf("Enter text to process: ");
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
    
    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if (isalpha((unsigned char)input[i])) {
            char c = tolower((unsigned char)input[i]);
            processed[j++] = c;
            if (c >= 'a' && c <= 'z') {
                counts[c - 'a']++;
            }
        }
    }
    processed[j] = '\0';
    
    printf("Processed text (letters only, lowercase): %s\n", processed);
    
    printf("Letter frequencies:\n");
    for (int i = 0; i < 26; i++) {
        if (counts[i] > 0) {
            printf("%c: %d\n", 'a' + i, counts[i]);
        }
    }
    
    char reversed[MAX_INPUT];
    size_t proc_len = strlen(processed);
    if (proc_len > 0) {
        size_t k = 0;
        size_t m = proc_len - 1;
        while (m < proc_len) {
            reversed[k++] = processed[m];
            if (m == 0) break;
            m--;
        }
        reversed[k] = '\0';
        printf("Reversed processed text: %s\n", reversed);
    }
    
    int vowel_count = 0;
    int consonant_count = 0;
    const char *vowels = "aeiou";
    size_t idx = 0;
    do {
        if (idx >= proc_len) break;
        char c = processed[idx];
        int is_vowel = 0;
        for (const char *v = vowels; *v; v++) {
            if (c == *v) {
                is_vowel = 1;
                break;
            }
        }
        if (is_vowel) {
            vowel_count++;
        } else if (isalpha((unsigned char)c)) {
            consonant_count++;
        }
        idx++;
    } while (idx < proc_len);
    
    printf("Vowels: %d, Consonants: %d\n", vowel_count, consonant_count);
    
    char longest_word[MAX_INPUT] = "";
    char current_word[MAX_INPUT];
    size_t word_start = 0;
    size_t max_len = 0;
    
    for (size_t pos = 0; pos <= proc_len; pos++) {
        if (pos == proc_len || processed[pos] == ' ') {
            size_t word_len = pos - word_start;
            if (word_len > 0 && word_len < sizeof(current_word)) {
                strncpy(current_word, processed + word_start, word_len);
                current_word[word_len] = '\0';
                
                if (word_len > max_len) {
                    max_len = word_len;
                    strncpy(longest_word, current_word, sizeof(longest_word) - 1);
                    longest_word[sizeof(longest_word) - 1] = '\0';
                }
            }
            word_start = pos + 1;
        }
    }
    
    if (max_len > 0) {
        printf("Longest word: %s (length: %zu)\n", longest_word, max_len);
    } else {
        printf("No words found\n");
    }
    
    return 0;
}