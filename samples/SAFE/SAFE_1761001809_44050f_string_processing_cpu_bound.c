//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 4096
#define MAX_WORDS 1000
#define MAX_WORD_LEN 100

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
    int length;
} WordInfo;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

int extract_words(const char *text, WordInfo words[], int max_words) {
    int word_count = 0;
    int i = 0;
    int len = strlen(text);
    
    while (i < len && word_count < max_words) {
        while (i < len && !is_valid_char(text[i])) {
            i++;
        }
        
        if (i >= len) break;
        
        int start = i;
        while (i < len && is_valid_char(text[i]) && (i - start) < MAX_WORD_LEN - 1) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            strncpy(words[word_count].word, text + start, word_len);
            words[word_count].word[word_len] = '\0';
            words[word_count].length = word_len;
            words[word_count].count = 1;
            word_count++;
        }
    }
    
    return word_count;
}

void to_lower_case(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int find_word_index(WordInfo words[], int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void process_word_frequencies(WordInfo words[], int *word_count) {
    for (int i = 0; i < *word_count; i++) {
        to_lower_case(words[i].word);
    }
    
    for (int i = 0; i < *word_count; i++) {
        for (int j = i + 1; j < *word_count; j++) {
            if (strcmp(words[i].word, words[j].word) == 0) {
                words[i].count++;
                for (int k = j; k < *word_count - 1; k++) {
                    words[k] = words[k + 1];
                }
                (*word_count)--;
                j--;
            }
        }
    }
}

int compare_words(const void *a, const void *b) {
    const WordInfo *wa = (const WordInfo *)a;
    const WordInfo *wb = (const WordInfo *)b;
    
    if (wa->count != wb->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordInfo words[MAX_WORDS];
    
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
        return 1;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int word_count = extract_words(input, words, MAX_WORDS);
    
    if (word_count == 0) {
        printf("No valid words found in input\n");
        return 1;
    }
    
    process_word_frequencies(words, &word_count);
    
    qsort(words, word_count, sizeof(WordInfo), compare_words);
    
    printf("\nWord frequencies (sorted by count, then alphabetically):\n");
    printf("%-20s %-10s %-10s\n", "Word", "Count", "Length");
    printf("----------------------------------------\n");
    
    for (int i = 0; i < word_count; i++) {
        if (words[i].count > 0) {
            printf("%-20s %-10d %-10d\n", words[i].word, words[i].count, words[i].length);
        }
    }
    
    int total_words = 0;
    for (int i = 0; i < word_count; i++) {
        total_words += words[i].count;
    }
    
    printf("\nTotal unique words: %d\n", word_count);
    printf("Total word occurrences: %d\n", total_words);
    
    return 0;
}