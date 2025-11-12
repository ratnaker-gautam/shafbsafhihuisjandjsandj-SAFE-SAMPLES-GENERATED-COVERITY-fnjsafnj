//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 30
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define IS_TERMINATOR(c) ((c) == '.' || (c) == '!' || (c) == '?')

typedef struct {
    char text[MAX_WORD_LEN + 1];
    int length;
} Word;

int safe_strlen(const char *str, size_t max_len) {
    if (str == NULL) return 0;
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return (int)len;
}

int validate_input(const char *input) {
    if (input == NULL) return 0;
    int len = safe_strlen(input, MAX_INPUT_LEN);
    if (len >= MAX_INPUT_LEN) return 0;
    if (len == 0) return 0;
    return 1;
}

int extract_words(const char *input, Word *words, int max_words) {
    if (input == NULL || words == NULL) return 0;
    
    int word_count = 0;
    int pos = 0;
    int input_len = safe_strlen(input, MAX_INPUT_LEN);
    
    while (pos < input_len && word_count < max_words) {
        while (pos < input_len && IS_WHITESPACE(input[pos])) {
            pos++;
        }
        
        if (pos >= input_len) break;
        
        int word_start = pos;
        int word_len = 0;
        
        while (pos < input_len && !IS_WHITESPACE(input[pos]) && 
               word_len < MAX_WORD_LEN) {
            word_len++;
            pos++;
        }
        
        if (word_len > 0 && word_len <= MAX_WORD_LEN) {
            int copy_len = word_len;
            if (copy_len > MAX_WORD_LEN) copy_len = MAX_WORD_LEN;
            
            for (int i = 0; i < copy_len; i++) {
                words[word_count].text[i] = input[word_start + i];
            }
            words[word_count].text[copy_len] = '\0';
            words[word_count].length = copy_len;
            word_count++;
        }
    }
    
    return word_count;
}

void capitalize_words(Word *words, int count) {
    if (words == NULL) return;
    
    for (int i = 0; i < count; i++) {
        if (words[i].length > 0) {
            words[i].text[0] = (char)toupper((unsigned char)words[i].text[0]);
        }
    }
}

void reverse_words(Word *words, int count) {
    if (words == NULL) return;
    
    for (int i = 0; i < count / 2; i++) {
        Word temp = words[i];
        words[i] = words[count - i - 1];
        words[count - i - 1] = temp;
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    Word words[MAX_WORDS];
    
    printf("Enter a sentence (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    int input_len = safe_strlen(input, sizeof(input));
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (!validate_input(input)) {
        printf("Invalid input: too long or empty\n");
        return 1;
    }
    
    int word_count = extract_words(input, words, MAX_WORDS);
    if (word_count == 0) {
        printf("No valid words found\n");
        return 1;
    }
    
    printf("Original words (%d): ", word_count);
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i].text);
    }
    printf("\n");
    
    capitalize_words(words, word_count);
    printf("Capitalized: ");
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i].text);
    }
    printf("\n");
    
    reverse_words(words, word_count);
    printf("Reversed: ");
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i].text);
    }
    printf("\n");
    
    return 0;
}