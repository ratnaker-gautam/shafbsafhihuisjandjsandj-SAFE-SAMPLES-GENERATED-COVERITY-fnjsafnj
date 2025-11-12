//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define IS_TERMINATOR(c) ((c) == '.' || (c) == '!' || (c) == '?')

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int safe_strlen(const char *str, size_t max_len) {
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

int is_valid_input(const char *str, size_t max_len) {
    if (str == NULL) return 0;
    size_t len = safe_strlen(str, max_len);
    if (len == 0 || len >= max_len) return 0;
    return 1;
}

int split_into_words(const char *input, WordCount words[], int max_words) {
    if (!is_valid_input(input, MAX_INPUT_LEN) || words == NULL || max_words <= 0) {
        return -1;
    }

    int word_count = 0;
    const char *p = input;
    
    while (*p != '\0' && word_count < max_words) {
        while (IS_WHITESPACE(*p)) p++;
        
        if (*p == '\0') break;
        
        const char *start = p;
        while (*p != '\0' && !IS_WHITESPACE(*p) && !IS_TERMINATOR(*p)) {
            p++;
        }
        
        size_t word_len = p - start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            strncpy(words[word_count].word, start, word_len);
            words[word_count].word[word_len] = '\0';
            words[word_count].count = 1;
            word_count++;
        }
        
        if (*p != '\0') p++;
    }
    
    return word_count;
}

int find_word_index(WordCount words[], int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void count_word_frequencies(WordCount words[], int *word_count) {
    if (words == NULL || word_count == NULL || *word_count <= 0) {
        return;
    }

    int unique_count = 0;
    
    for (int i = 0; i < *word_count; i++) {
        int found_index = find_word_index(words, unique_count, words[i].word);
        
        if (found_index == -1) {
            if (unique_count < i) {
                words[unique_count] = words[i];
            }
            unique_count++;
        } else {
            words[found_index].count++;
        }
    }
    
    *word_count = unique_count;
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    
    for (char *p = str; *p != '\0'; p++) {
        *p = tolower(*p);
    }
}

int compare_word_counts(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    
    if (wb->count != wa->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    
    printf("Enter text to analyze (max %d characters): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = safe_strlen(input, sizeof(input));
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (!is_valid_input(input, sizeof(input))) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    to_lowercase(input);
    
    int word_count = split_into_words(input, words, MAX_WORDS);
    if (word_count <= 0) {
        fprintf(stderr, "No words found in input\n");
        return EXIT_FAILURE;
    }
    
    count_word_frequencies(words, &word_count);
    
    qsort(words, word_count, sizeof(WordCount), compare_word_counts);
    
    printf("\nWord frequencies (sorted by count):\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    return EXIT_SUCCESS;
}