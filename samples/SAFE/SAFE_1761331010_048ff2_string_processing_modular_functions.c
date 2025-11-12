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
        while (i < len && is_word_char(text[i]) && (i - start) < 63) {
            words[word_count][i - start] = tolower((unsigned char)text[i]);
            i++;
        }
        words[word_count][i - start] = '\0';
        
        if (strlen(words[word_count]) > 0) {
            word_count++;
        }
    }
    
    return word_count;
}

int count_unique_words(char words[][64], int word_count, struct WordCount *unique_words) {
    if (words == NULL || unique_words == NULL || word_count <= 0) {
        return 0;
    }
    
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        int found = 0;
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(words[i], unique_words[j].word) == 0) {
                unique_words[j].count++;
                found = 1;
                break;
            }
        }
        
        if (!found && unique_count < MAX_WORDS) {
            strncpy(unique_words[unique_count].word, words[i], 63);
            unique_words[unique_count].word[63] = '\0';
            unique_words[unique_count].count = 1;
            unique_count++;
        }
    }
    
    return unique_count;
}

void sort_word_counts(struct WordCount *unique_words, int unique_count) {
    if (unique_words == NULL || unique_count <= 1) {
        return;
    }
    
    for (int i = 0; i < unique_count - 1; i++) {
        for (int j = 0; j < unique_count - i - 1; j++) {
            if (unique_words[j].count < unique_words[j + 1].count) {
                struct WordCount temp = unique_words[j];
                unique_words[j] = unique_words[j + 1];
                unique_words[j + 1] = temp;
            }
        }
    }
}

void print_word_counts(const struct WordCount *unique_words, int unique_count) {
    if (unique_words == NULL || unique_count <= 0) {
        return;
    }
    
    printf("Word frequency analysis:\n");
    printf("------------------------\n");
    
    for (int i = 0; i < unique_count && i < 10; i++) {
        printf("%-20s: %d\n", unique_words[i].word, unique_words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][64];
    struct WordCount unique_words[MAX_WORDS];
    
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
        printf("No words found in the input.\n");
        return 1;
    }
    
    int unique_count = count_unique_words(words, word_count, unique_words);
    
    if (unique_count == 0) {
        printf("No unique words found.\n");
        return 1;
    }
    
    sort_word_counts(unique_words, unique_count);
    print_word_counts(unique_words, unique_count);
    
    return 0;
}