//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 30
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define TO_UPPER(c) (((c) >= 'a' && (c) <= 'z') ? ((c) - 'a' + 'A') : (c))
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) - 'A' + 'a') : (c))

typedef struct {
    char word[MAX_WORD_LEN + 1];
    int count;
} WordCount;

int safe_strlen(const char *str, size_t max_len) {
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

void safe_strncpy(char *dest, const char *src, size_t n) {
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
}

int safe_strcmp(const char *s1, const char *s2) {
    while (*s1 && *s2 && *s1 == *s2) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

void normalize_word(char *word) {
    int i = 0, j = 0;
    char temp[MAX_WORD_LEN + 1];
    
    while (word[i] != '\0' && j < MAX_WORD_LEN) {
        if (isalnum((unsigned char)word[i])) {
            temp[j++] = TO_LOWER(word[i]);
        }
        i++;
    }
    temp[j] = '\0';
    safe_strncpy(word, temp, MAX_WORD_LEN + 1);
}

int find_word_index(WordCount *words, int count, const char *word) {
    int i;
    for (i = 0; i < count; i++) {
        if (safe_strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int extract_words(const char *input, WordCount *words) {
    int word_count = 0;
    int i = 0;
    int len = safe_strlen(input, MAX_INPUT_LEN);
    
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && IS_WHITESPACE(input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        int start = i;
        while (i < len && !IS_WHITESPACE(input[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len > 0 && word_len <= MAX_WORD_LEN) {
            safe_strncpy(words[word_count].word, &input[start], word_len + 1);
            words[word_count].word[word_len] = '\0';
            normalize_word(words[word_count].word);
            
            if (words[word_count].word[0] != '\0') {
                int existing_index = find_word_index(words, word_count, words[word_count].word);
                if (existing_index >= 0) {
                    words[existing_index].count++;
                } else {
                    words[word_count].count = 1;
                    word_count++;
                }
            }
        }
    }
    
    return word_count;
}

void print_word_counts(WordCount *words, int count) {
    int i;
    printf("Word frequencies:\n");
    for (i = 0; i < count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    int input_len = safe_strlen(input, MAX_INPUT_LEN);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (input[0] == '\0') {
        printf("No input provided\n");
        return 1;
    }
    
    int word_count = extract_words(input, words);
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 1;
    }
    
    print_word_counts(words, word_count);
    
    return 0;
}