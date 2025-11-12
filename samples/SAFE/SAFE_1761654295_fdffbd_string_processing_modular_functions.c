//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

struct WordCount {
    char word[64];
    int count;
};

int is_word_char(char c) {
    return isalnum(c) || c == '\'' || c == '-';
}

int extract_words(const char *text, char words[][64], int max_words) {
    if (text == NULL || words == NULL || max_words <= 0) {
        return 0;
    }
    
    int word_count = 0;
    int i = 0;
    int len = strlen(text);
    
    while (i < len && word_count < max_words) {
        while (i < len && !is_word_char(text[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        int start = i;
        while (i < len && is_word_char(text[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len > 0 && word_len < 64) {
            strncpy(words[word_count], text + start, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    return word_count;
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower(str[i]);
    }
}

int count_unique_words(char words[][64], int word_count, struct WordCount *word_counts, int max_unique) {
    if (words == NULL || word_counts == NULL || max_unique <= 0) {
        return 0;
    }
    
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        char temp[64];
        strncpy(temp, words[i], sizeof(temp) - 1);
        temp[sizeof(temp) - 1] = '\0';
        to_lowercase(temp);
        
        int found = 0;
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(word_counts[j].word, temp) == 0) {
                word_counts[j].count++;
                found = 1;
                break;
            }
        }
        
        if (!found && unique_count < max_unique) {
            strncpy(word_counts[unique_count].word, temp, sizeof(word_counts[unique_count].word) - 1);
            word_counts[unique_count].word[sizeof(word_counts[unique_count].word) - 1] = '\0';
            word_counts[unique_count].count = 1;
            unique_count++;
        }
    }
    
    return unique_count;
}

void sort_word_counts(struct WordCount *word_counts, int count) {
    if (word_counts == NULL || count <= 1) return;
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (word_counts[j].count < word_counts[j + 1].count) {
                struct WordCount temp = word_counts[j];
                word_counts[j] = word_counts[j + 1];
                word_counts[j + 1] = temp;
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][64];
    struct WordCount word_counts[MAX_WORDS];
    
    printf("Enter text for word frequency analysis: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
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
        printf("No words found in the input.\n");
        return 1;
    }
    
    int unique_count = count_unique_words(words, word_count, word_counts, MAX_WORDS);
    
    sort_word_counts(word_counts, unique_count);
    
    printf("\nWord frequency analysis:\n");
    printf("Total words: %d\n", word_count);
    printf("Unique words: %d\n\n", unique_count);
    
    int display_count = (unique_count > 10) ? 10 : unique_count;
    for (int i = 0; i < display_count; i++) {
        printf("%-20s: %d\n", word_counts[i].word, word_counts[i].count);
    }
    
    if (unique_count > 10) {
        printf("... and %d more words\n", unique_count - 10);
    }
    
    return 0;
}