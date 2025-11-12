//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

struct WordCount {
    char *word;
    int count;
};

char *to_lower_case(const char *str) {
    if (str == NULL) return NULL;
    
    size_t len = strlen(str);
    if (len == 0) return NULL;
    
    char *result = malloc(len + 1);
    if (result == NULL) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        result[i] = tolower((unsigned char)str[i]);
    }
    result[len] = '\0';
    return result;
}

int is_word_char(char c) {
    return isalpha((unsigned char)c);
}

char **extract_words(const char *text, int *word_count) {
    if (text == NULL || word_count == NULL) return NULL;
    
    char **words = malloc(MAX_WORDS * sizeof(char *));
    if (words == NULL) return NULL;
    
    *word_count = 0;
    size_t len = strlen(text);
    if (len == 0) return words;
    
    size_t start = 0;
    size_t i = 0;
    
    while (i < len && *word_count < MAX_WORDS) {
        while (i < len && !is_word_char(text[i])) {
            i++;
        }
        if (i >= len) break;
        
        start = i;
        while (i < len && is_word_char(text[i])) {
            i++;
        }
        
        size_t word_len = i - start;
        if (word_len > 0) {
            char *word = malloc(word_len + 1);
            if (word == NULL) break;
            
            strncpy(word, text + start, word_len);
            word[word_len] = '\0';
            
            char *lower_word = to_lower_case(word);
            free(word);
            if (lower_word == NULL) continue;
            
            words[*word_count] = lower_word;
            (*word_count)++;
        }
    }
    
    return words;
}

int find_word_index(struct WordCount *word_counts, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(word_counts[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int compare_word_counts(const void *a, const void *b) {
    const struct WordCount *wa = (const struct WordCount *)a;
    const struct WordCount *wb = (const struct WordCount *)b;
    
    if (wb->count != wa->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text to analyze (max %d characters): ", MAX_INPUT_LEN);
    
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
        printf("No input provided\n");
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int word_count = 0;
    char **words = extract_words(input, &word_count);
    
    if (words == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        free(words);
        return 0;
    }
    
    struct WordCount *word_counts = malloc(word_count * sizeof(struct WordCount));
    if (word_counts == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        for (int i = 0; i < word_count; i++) {
            free(words[i]);
        }
        free(words);
        return 1;
    }
    
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        int index = find_word_index(word_counts, unique_count, words[i]);
        if (index == -1) {
            word_counts[unique_count].word = words[i];
            word_counts[unique_count].count = 1;
            unique_count++;
        } else {
            word_counts[index].count++;
            free(words[i]);
        }
    }
    
    qsort(word_counts, unique_count, sizeof(struct WordCount), compare_word_counts);
    
    printf("\nWord frequency analysis:\n");
    printf("------------------------\n");
    for (int i = 0; i < unique_count; i++) {
        printf("%-15s: %d\n", word_counts[i].word, word_counts[i].count);
    }
    
    for (int i = 0; i < unique_count; i++) {
        free(word_counts[i].word);
    }
    free(word_counts);
    free(words);
    return 0;
}