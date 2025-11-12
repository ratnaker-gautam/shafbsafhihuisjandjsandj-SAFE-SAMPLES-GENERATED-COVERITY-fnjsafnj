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

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

char *strdup_safe(const char *s) {
    if (s == NULL) return NULL;
    size_t len = strlen(s);
    if (len == 0) return NULL;
    char *copy = malloc(len + 1);
    if (copy == NULL) return NULL;
    strcpy(copy, s);
    return copy;
}

int compare_words(const void *a, const void *b) {
    const struct WordCount *wa = (const struct WordCount *)a;
    const struct WordCount *wb = (const struct WordCount *)b;
    return strcmp(wa->word, wb->word);
}

void cleanup_word_counts(struct WordCount *words, int count) {
    for (int i = 0; i < count; i++) {
        if (words[i].word != NULL) {
            free(words[i].word);
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char buffer[MAX_INPUT_LEN + 1];
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text for word frequency analysis: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    for (int i = 0; i < MAX_WORDS; i++) {
        words[i].word = NULL;
        words[i].count = 0;
    }
    
    char *pos = input;
    while (*pos != '\0' && word_count < MAX_WORDS) {
        while (*pos != '\0' && !is_valid_char(*pos)) {
            pos++;
        }
        
        if (*pos == '\0') break;
        
        char *word_start = pos;
        while (*pos != '\0' && is_valid_char(*pos)) {
            pos++;
        }
        
        size_t word_len = pos - word_start;
        if (word_len >= sizeof(buffer)) {
            continue;
        }
        
        strncpy(buffer, word_start, word_len);
        buffer[word_len] = '\0';
        
        for (char *p = buffer; *p; p++) {
            *p = tolower((unsigned char)*p);
        }
        
        int found = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i].word, buffer) == 0) {
                words[i].count++;
                found = 1;
                break;
            }
        }
        
        if (!found) {
            char *word_copy = strdup_safe(buffer);
            if (word_copy == NULL) {
                cleanup_word_counts(words, word_count);
                fprintf(stderr, "Memory allocation failed\n");
                return 1;
            }
            words[word_count].word = word_copy;
            words[word_count].count = 1;
            word_count++;
        }
    }
    
    qsort(words, word_count, sizeof(struct WordCount), compare_words);
    
    printf("\nWord frequencies (alphabetical order):\n");
    for (int i = 0; i < word_count; i++) {
        if (words[i].word != NULL) {
            printf("%s: %d\n", words[i].word, words[i].count);
        }
    }
    
    cleanup_word_counts(words, word_count);
    return 0;
}