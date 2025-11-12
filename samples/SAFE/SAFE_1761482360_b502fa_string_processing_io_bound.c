//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

struct WordCount {
    char word[MAX_WORD_LEN];
    int count;
};

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
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
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int extract_words(const char *text, struct WordCount *words, int *word_count) {
    int len = strlen(text);
    int start = -1;
    *word_count = 0;
    
    for (int i = 0; i <= len; i++) {
        if (i < len && is_word_char(text[i])) {
            if (start == -1) {
                start = i;
            }
        } else {
            if (start != -1) {
                int word_len = i - start;
                if (word_len >= MAX_WORD_LEN) {
                    word_len = MAX_WORD_LEN - 1;
                }
                
                char temp_word[MAX_WORD_LEN];
                strncpy(temp_word, text + start, word_len);
                temp_word[word_len] = '\0';
                to_lower_case(temp_word);
                
                int existing = find_word(words, *word_count, temp_word);
                if (existing != -1) {
                    words[existing].count++;
                } else {
                    if (*word_count >= MAX_WORDS) {
                        return 0;
                    }
                    strcpy(words[*word_count].word, temp_word);
                    words[*word_count].count = 1;
                    (*word_count)++;
                }
                start = -1;
            }
        }
    }
    return 1;
}

void sort_words(struct WordCount *words, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (words[j].count < words[j + 1].count) {
                struct WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text for word frequency analysis (max %d characters):\n", MAX_INPUT_LEN - 1);
    
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
    
    if (!extract_words(input, words, &word_count)) {
        printf("Too many unique words to process.\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input.\n");
        return 1;
    }
    
    sort_words(words, word_count);
    
    printf("\nWord frequency analysis:\n");
    printf("%-20s %s\n", "Word", "Frequency");
    printf("%-20s %s\n", "----", "---------");
    
    for (int i = 0; i < word_count; i++) {
        printf("%-20s %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}