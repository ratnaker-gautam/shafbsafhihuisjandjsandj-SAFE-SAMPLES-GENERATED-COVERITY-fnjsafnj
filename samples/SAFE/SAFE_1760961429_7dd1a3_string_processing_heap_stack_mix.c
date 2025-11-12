//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
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
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

char **extract_words(const char *text, int *word_count) {
    if (text == NULL || word_count == NULL) return NULL;
    
    char **words = malloc(MAX_WORDS * sizeof(char *));
    if (words == NULL) return NULL;
    
    char *copy = malloc(strlen(text) + 1);
    if (copy == NULL) {
        free(words);
        return NULL;
    }
    strcpy(copy, text);
    
    *word_count = 0;
    char *token = strtok(copy, " \t\n\r");
    
    while (token != NULL && *word_count < MAX_WORDS) {
        size_t len = strlen(token);
        if (len > 0) {
            int valid = 1;
            for (size_t i = 0; i < len; i++) {
                if (!is_word_char(token[i])) {
                    valid = 0;
                    break;
                }
            }
            
            if (valid) {
                char *word = to_lower_case(token);
                if (word != NULL) {
                    words[*word_count] = word;
                    (*word_count)++;
                }
            }
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    free(copy);
    return words;
}

int compare_words(const void *a, const void *b) {
    const struct WordCount *wa = (const struct WordCount *)a;
    const struct WordCount *wb = (const struct WordCount *)b;
    return strcmp(wa->word, wb->word);
}

void process_word_frequency(char **words, int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    struct WordCount *freq = malloc(word_count * sizeof(struct WordCount));
    if (freq == NULL) return;
    
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        int found = 0;
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(words[i], freq[j].word) == 0) {
                freq[j].count++;
                found = 1;
                break;
            }
        }
        
        if (!found) {
            freq[unique_count].word = malloc(strlen(words[i]) + 1);
            if (freq[unique_count].word != NULL) {
                strcpy(freq[unique_count].word, words[i]);
                freq[unique_count].count = 1;
                unique_count++;
            }
        }
    }
    
    qsort(freq, unique_count, sizeof(struct WordCount), compare_words);
    
    printf("Word frequency analysis:\n");
    for (int i = 0; i < unique_count; i++) {
        printf("%s: %d\n", freq[i].word, freq[i].count);
        free(freq[i].word);
    }
    
    free(freq);
}

int main(void) {
    char input[MAX_INPUT_LEN];
    
    printf("Enter text for word frequency analysis: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    int word_count = 0;
    char **words = extract_words(input, &word_count);
    
    if (words == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found in input\n");
        for (int i = 0; i < word_count; i++) {
            free(words[i]);
        }
        free(words);
        return 1;
    }
    
    process_word_frequency(words, word_count);
    
    for (int i = 0; i < word_count; i++) {
        free(words[i]);
    }
    free(words);
    
    return 0;
}