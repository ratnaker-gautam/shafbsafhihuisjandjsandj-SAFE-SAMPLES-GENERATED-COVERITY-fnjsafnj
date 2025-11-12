//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

struct WordCount {
    char word[64];
    int count;
};

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_';
}

int find_word(struct WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void to_lower_case(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int compare_word_count(const void *a, const void *b) {
    const struct WordCount *wa = (const struct WordCount *)a;
    const struct WordCount *wb = (const struct WordCount *)b;
    if (wa->count != wb->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text for word frequency analysis: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int pos = 0;
    while (pos < len && word_count < MAX_WORDS) {
        while (pos < len && !is_word_char(input[pos])) {
            pos++;
        }
        
        if (pos >= len) {
            break;
        }
        
        int start = pos;
        while (pos < len && is_word_char(input[pos])) {
            pos++;
        }
        
        int word_len = pos - start;
        if (word_len >= 63) {
            word_len = 62;
        }
        
        char current_word[64];
        strncpy(current_word, &input[start], word_len);
        current_word[word_len] = '\0';
        to_lower_case(current_word);
        
        int existing_index = find_word(words, word_count, current_word);
        if (existing_index != -1) {
            if (words[existing_index].count < INT_MAX) {
                words[existing_index].count++;
            }
        } else {
            strcpy(words[word_count].word, current_word);
            words[word_count].count = 1;
            word_count++;
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    qsort(words, word_count, sizeof(struct WordCount), compare_word_count);
    
    printf("\nWord frequency analysis (top 20):\n");
    printf("%-20s %s\n", "Word", "Count");
    printf("%-20s %s\n", "----", "-----");
    
    int display_count = word_count > 20 ? 20 : word_count;
    for (int i = 0; i < display_count; i++) {
        printf("%-20s %d\n", words[i].word, words[i].count);
    }
    
    if (word_count > 20) {
        printf("... and %d more words\n", word_count - 20);
    }
    
    return 0;
}