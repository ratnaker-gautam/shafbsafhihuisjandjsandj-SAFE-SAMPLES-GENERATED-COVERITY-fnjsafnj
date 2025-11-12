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
    return isalnum((unsigned char)c) || c == '_' || c == '-';
}

int extract_words(const char *text, char words[][64], int max_words) {
    if (text == NULL || words == NULL || max_words <= 0) {
        return 0;
    }
    
    int word_count = 0;
    int pos = 0;
    int len = strlen(text);
    
    while (pos < len && word_count < max_words) {
        while (pos < len && !is_word_char(text[pos])) {
            pos++;
        }
        
        if (pos >= len) {
            break;
        }
        
        int word_start = pos;
        while (pos < len && is_word_char(text[pos])) {
            pos++;
        }
        
        int word_len = pos - word_start;
        if (word_len > 0 && word_len < 64) {
            strncpy(words[word_count], text + word_start, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    return word_count;
}

void to_lowercase(char *str) {
    if (str == NULL) {
        return;
    }
    
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int count_unique_words(char words[][64], int word_count, struct WordCount *word_counts, int max_unique) {
    if (words == NULL || word_counts == NULL || word_count <= 0 || max_unique <= 0) {
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
    if (word_counts == NULL || count <= 1) {
        return;
    }
    
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

void print_word_counts(const struct WordCount *word_counts, int count) {
    if (word_counts == NULL || count <= 0) {
        return;
    }
    
    printf("Word frequency analysis:\n");
    printf("------------------------\n");
    
    for (int i = 0; i < count; i++) {
        printf("%-20s: %d\n", word_counts[i].word, word_counts[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][64];
    struct WordCount word_counts[MAX_WORDS];
    
    printf("Enter text for word frequency analysis (max %d characters):\n", MAX_INPUT_LEN);
    
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
        printf("No words found in the input.\n");
        return 1;
    }
    
    int unique_count = count_unique_words(words, word_count, word_counts, MAX_WORDS);
    
    if (unique_count == 0) {
        printf("No unique words found.\n");
        return 1;
    }
    
    sort_word_counts(word_counts, unique_count);
    print_word_counts(word_counts, unique_count);
    
    return 0;
}