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
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int find_word_index(struct WordCount words[], int count, const char *word) {
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
            if (strcmp(words[j].word, words[j + 1].word) > 0) {
                struct WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

int main() {
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
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return 0;
    }
    
    char temp[MAX_INPUT_LEN + 1];
    if (strlen(input) >= sizeof(temp)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    strcpy(temp, input);
    
    char *ptr = temp;
    while (*ptr != '\0' && word_count < MAX_WORDS) {
        while (*ptr != '\0' && !is_word_char(*ptr)) {
            ptr++;
        }
        
        if (*ptr == '\0') {
            break;
        }
        
        char *word_start = ptr;
        while (*ptr != '\0' && is_word_char(*ptr)) {
            ptr++;
        }
        
        char original_char = *ptr;
        *ptr = '\0';
        
        if (ptr - word_start >= 50) {
            *ptr = original_char;
            continue;
        }
        
        char word[50];
        strcpy(word, word_start);
        to_lower_case(word);
        
        int index = find_word_index(words, word_count, word);
        if (index != -1) {
            if (words[index].count < INT_MAX) {
                words[index].count++;
            }
        } else {
            if (word_count < MAX_WORDS) {
                strcpy(words[word_count].word, word);
                words[word_count].count = 1;
                word_count++;
            }
        }
        
        *ptr = original_char;
        if (*ptr != '\0') {
            ptr++;
        }
    }
    
    sort_words(words, word_count);
    
    printf("\nWord frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}