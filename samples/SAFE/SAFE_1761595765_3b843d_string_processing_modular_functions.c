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

int extract_words(const char *text, struct WordCount words[], int max_words) {
    if (text == NULL || words == NULL || max_words <= 0) {
        return 0;
    }
    
    int word_count = 0;
    int len = strlen(text);
    int i = 0;
    
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
        if (word_len >= 64) {
            word_len = 63;
        }
        
        char temp_word[64];
        strncpy(temp_word, text + start, word_len);
        temp_word[word_len] = '\0';
        
        for (int j = 0; j < word_len; j++) {
            temp_word[j] = tolower(temp_word[j]);
        }
        
        int found = 0;
        for (int j = 0; j < word_count; j++) {
            if (strcmp(words[j].word, temp_word) == 0) {
                words[j].count++;
                found = 1;
                break;
            }
        }
        
        if (!found) {
            strcpy(words[word_count].word, temp_word);
            words[word_count].count = 1;
            word_count++;
        }
    }
    
    return word_count;
}

void sort_words(struct WordCount words[], int count) {
    if (words == NULL || count <= 1) {
        return;
    }
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (words[j].count < words[j + 1].count || 
                (words[j].count == words[j + 1].count && strcmp(words[j].word, words[j + 1].word) > 0)) {
                struct WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

void print_word_counts(const struct WordCount words[], int count) {
    if (words == NULL || count <= 0) {
        return;
    }
    
    printf("Word frequency analysis:\n");
    printf("------------------------\n");
    
    for (int i = 0; i < count && i < 20; i++) {
        printf("%-20s: %d\n", words[i].word, words[i].count);
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
        printf("No words found in the input.\n");
        return 1;
    }
    
    sort_words(words, word_count);
    print_word_counts(words, word_count);
    
    return 0;
}