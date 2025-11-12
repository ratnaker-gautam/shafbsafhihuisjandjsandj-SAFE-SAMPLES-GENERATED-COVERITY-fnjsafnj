//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 1000
#define MAX_WORDS 500

struct WordCount {
    char word[50];
    int count;
};

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_';
}

void to_lower_case(char *str) {
    if (str == NULL) return;
    
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int find_word_index(struct WordCount words[], int count, const char *word) {
    if (word == NULL) return -1;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void sort_words(struct WordCount words[], int count) {
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

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text for word frequency analysis: ");
    
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
    
    if (input_len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char buffer[MAX_INPUT_LEN + 1];
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    to_lower_case(buffer);
    
    char *current = buffer;
    while (*current != '\0' && word_count < MAX_WORDS) {
        while (*current != '\0' && !is_word_char(*current)) {
            current++;
        }
        
        if (*current == '\0') {
            break;
        }
        
        char *word_start = current;
        while (*current != '\0' && is_word_char(*current)) {
            current++;
        }
        
        size_t word_len = current - word_start;
        if (word_len >= sizeof(words[0].word)) {
            word_len = sizeof(words[0].word) - 1;
        }
        
        char temp_word[sizeof(words[0].word)];
        strncpy(temp_word, word_start, word_len);
        temp_word[word_len] = '\0';
        
        int existing_index = find_word_index(words, word_count, temp_word);
        if (existing_index != -1) {
            if (words[existing_index].count < INT_MAX) {
                words[existing_index].count++;
            }
        } else {
            strncpy(words[word_count].word, temp_word, sizeof(words[word_count].word) - 1);
            words[word_count].word[sizeof(words[word_count].word) - 1] = '\0';
            words[word_count].count = 1;
            word_count++;
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    sort_words(words, word_count);
    
    printf("\nWord frequency analysis (top 20):\n");
    printf("%-20s %s\n", "Word", "Count");
    printf("%-20s %s\n", "----", "-----");
    
    int display_count = word_count > 20 ? 20 : word_count;
    for (int i = 0; i < display_count; i++) {
        printf("%-20s %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}