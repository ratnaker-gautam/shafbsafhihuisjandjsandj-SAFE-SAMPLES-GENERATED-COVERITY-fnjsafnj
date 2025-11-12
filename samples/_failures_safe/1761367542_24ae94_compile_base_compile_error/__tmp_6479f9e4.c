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

int compare_words(const void *a, const void *b) {
    const char *word1 = *(const char **)a;
    const char *word2 = *(const char **)b;
    return strcmp(word1, word2);
}

void count_word_frequencies(char **words, int word_count, struct WordCount **frequencies, int *unique_count) {
    if (words == NULL || frequencies == NULL || unique_count == NULL) return;
    
    *unique_count = 0;
    if (word_count == 0) return;
    
    char **sorted_words = malloc(word_count * sizeof(char *));
    if (sorted_words == NULL) return;
    
    for (int i = 0; i < word_count; i++) {
        sorted_words[i] = words[i];
    }
    
    qsort(sorted_words, word_count, sizeof(char *), compare_words);
    
    *frequencies = malloc(word_count * sizeof(struct WordCount));
    if (*frequencies == NULL) {
        free(sorted_words);
        return;
    }
    
    int current_count = 1;
    for (int i = 1; i <= word_count; i++) {
        if (i < word_count && strcmp(sorted_words[i], sorted_words[i-1]) == 0) {
            current_count++;
        } else {
            if (*unique_count < word_count) {
                (*frequencies)[*unique_count].word = malloc(strlen(sorted_words[i-1]) + 1);
                if ((*frequencies)[*unique_count].word != NULL) {
                    strcpy((*frequencies)[*unique_count].word, sorted_words[i-1]);
                    (*frequencies)[*unique_count].count = current_count;
                    (*unique_count)++;
                }
            }
            current_count = 1;
        }
    }
    
    free(sorted_words);
}

int compare_frequencies(const void *a, const void *b) {
    const struct WordCount *wc1 = (const struct WordCount *)a;
    const struct WordCount *wc2 = (const struct WordCount *)b;
    
    if (wc1->count != wc2->count) {
        return wc2->count - wc1->count;
    }
    return strcmp(wc1->word, wc2->word);
}

void free_words(char **words, int count) {
    if (words == NULL) return;
    for (int i = 0; i < count; i++) {
        free(words[i]);
    }
    free(words);
}

void free_frequencies(struct WordCount *frequencies, int count) {
    if (frequencies == NULL) return;
    for (int i = 0; i < count; i++) {
        free(frequencies[i].word);
    }
    free(frequencies);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text to analyze (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1]