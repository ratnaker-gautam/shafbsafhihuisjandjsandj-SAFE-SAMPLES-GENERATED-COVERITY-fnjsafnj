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
        int is_palindrome = 1;
        size_t word_len = strlen(words[idx]);
        
        if (word_len > 0) {
            int k = 0;
            int l = word_len - 1;
            while (k < l) {
                if (tolower(words[idx][k]) != tolower(words[idx][l])) {
                    is_palindrome = 0;
                    break;
                }
                k++;
                l--;
            }
        } else {
            is_palindrome = 0;
        }
        
        printf("Word %d: \"%s\" %s a palindrome\n", 
               idx + 1, words[idx], is_palindrome ? "is" : "is not");
    }
    
    char longest_word[MAX_INPUT] = "";
    size_t max_len = 0;
    
    int m = 0;
    do {
        if (m < word_count) {
            size_t current_len = strlen(words[m]);
            if (current_len > max_len) {
                max_len = current_len;
                strncpy(longest_word, words[m], MAX_INPUT - 1);
                longest_word[MAX_INPUT - 1] = '\0';
            }
        }
        m++;
    } while (m < word_count);
    
    if (max_len > 0) {
        printf("Longest word: \"%s\" (length: %zu)\n", longest_word, max_len);
    } else {
        printf("No words found\n");
    }
    
    return 0;
}