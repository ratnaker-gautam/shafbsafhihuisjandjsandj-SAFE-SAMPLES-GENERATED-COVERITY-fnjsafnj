//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64

struct WordCount {
    char word[MAX_WORD_LEN];
    int count;
};

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

int extract_words(const char *text, struct WordCount words[], int max_words) {
    if (text == NULL || words == NULL || max_words <= 0) {
        return 0;
    }
    
    int word_count = 0;
    int text_len = (int)strlen(text);
    int word_start = -1;
    
    for (int i = 0; i <= text_len; i++) {
        char current = (i < text_len) ? text[i] : ' ';
        
        if (is_valid_char(current)) {
            if (word_start == -1) {
                word_start = i;
            }
        } else {
            if (word_start != -1) {
                int word_len = i - word_start;
                if (word_len > 0 && word_len < MAX_WORD_LEN && word_count < max_words) {
                    strncpy(words[word_count].word, &text[word_start], word_len);
                    words[word_count].word[word_len] = '\0';
                    
                    for (int j = 0; j < word_len; j++) {
                        words[word_count].word[j] = tolower((unsigned char)words[word_count].word[j]);
                    }
                    
                    words[word_count].count = 1;
                    word_count++;
                }
                word_start = -1;
            }
        }
    }
    
    return word_count;
}

int find_word_index(struct WordCount words[], int count, const char *word) {
    if (words == NULL || word == NULL) {
        return -1;
    }
    
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void count_word_frequencies(struct WordCount words[], int *word_count) {
    if (words == NULL || word_count == NULL || *word_count <= 0) {
        return;
    }
    
    int unique_count = 0;
    
    for (int i = 0; i < *word_count; i++) {
        if (words[i].count > 0) {
            int existing_index = find_word_index(words, unique_count, words[i].word);
            
            if (existing_index == -1) {
                if (unique_count != i) {
                    strcpy(words[unique_count].word, words[i].word);
                    words[unique_count].count = 1;
                }
                unique_count++;
            } else {
                words[existing_index].count++;
            }
        }
    }
    
    *word_count = unique_count;
}

int compare_word_counts(const void *a, const void *b) {
    const struct WordCount *wa = (const struct WordCount *)a;
    const struct WordCount *wb = (const struct WordCount *)b;
    
    if (wb->count != wa->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

void sort_words_by_frequency(struct WordCount words[], int count) {
    if (words == NULL || count <= 0) {
        return;
    }
    qsort(words, count, sizeof(struct WordCount), compare_word_counts);
}

void display_word_counts(struct WordCount words[], int count) {
    if (words == NULL || count <= 0) {
        return;
    }
    
    printf("Word frequencies (sorted by count):\n");
    for (int i = 0; i < count; i++) {
        if (words[i].count > 0) {
            printf("%-20s: %d\n", words[i].word, words[i].count);
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    struct WordCount words[MAX_WORDS];
    
    printf("Enter text for word frequency analysis: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    int word_count = extract_words(input, words, MAX_WORDS);
    
    if (word_count == 0) {
        printf("No valid words found in the input.\n");
        return 1;
    }
    
    count_word_frequencies(words, &word_count);
    
    if (word_count > 0) {
        sort_words_by_frequency(words, word_count);
        display_word_counts(words, word_count);
    }
    
    return 0;
}