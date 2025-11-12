//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
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
    int first_pos;
} WordInfo;

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_';
}

int find_or_add_word(WordInfo words[], int *word_count, const char *word, int position) {
    for (int i = 0; i < *word_count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            words[i].count++;
            return i;
        }
    }
    
    if (*word_count >= MAX_WORDS) {
        return -1;
    }
    
    strncpy(words[*word_count].word, word, MAX_WORD_LEN - 1);
    words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
    words[*word_count].count = 1;
    words[*word_count].first_pos = position;
    (*word_count)++;
    return *word_count - 1;
}

int compare_words(const void *a, const void *b) {
    const WordInfo *wa = (const WordInfo *)a;
    const WordInfo *wb = (const WordInfo *)b;
    
    if (wa->count != wb->count) {
        return wb->count - wa->count;
    }
    return wa->first_pos - wb->first_pos;
}

void process_text(const char *text) {
    WordInfo words[MAX_WORDS];
    int word_count = 0;
    char current_word[MAX_WORD_LEN];
    int word_pos = 0;
    int in_word = 0;
    int position = 0;
    
    for (int i = 0; text[i] != '\0' && i < MAX_INPUT_LEN; i++) {
        if (is_word_char(text[i])) {
            if (!in_word) {
                in_word = 1;
                word_pos = 0;
            }
            if (word_pos < MAX_WORD_LEN - 1) {
                current_word[word_pos++] = tolower((unsigned char)text[i]);
            }
        } else {
            if (in_word) {
                current_word[word_pos] = '\0';
                if (word_pos > 0) {
                    find_or_add_word(words, &word_count, current_word, position++);
                }
                in_word = 0;
            }
        }
    }
    
    if (in_word && word_pos > 0) {
        current_word[word_pos] = '\0';
        find_or_add_word(words, &word_count, current_word, position++);
    }
    
    qsort(words, word_count, sizeof(WordInfo), compare_words);
    
    printf("Word frequency analysis:\n");
    printf("%-20s %-8s %s\n", "Word", "Count", "First Position");
    printf("----------------------------------------\n");
    
    for (int i = 0; i < word_count; i++) {
        if (words[i].count > 0) {
            printf("%-20s %-8d %d\n", words[i].word, words[i].count, words[i].first_pos);
        }
    }
    
    printf("\nTotal unique words: %d\n", word_count);
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text for word frequency analysis (max %d characters):\n", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return 1;
    }
    
    if (len >= MAX_INPUT_LEN) {
        printf("Warning: Input truncated to %d characters\n", MAX_INPUT_LEN);
        input[MAX_INPUT_LEN] = '\0';
    }
    
    process_text(input);
    
    return 0;
}