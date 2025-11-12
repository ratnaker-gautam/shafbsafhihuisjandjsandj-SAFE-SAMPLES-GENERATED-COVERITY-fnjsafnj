//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_LINE_LEN 256
#define MAX_WORD_LEN 64
#define MAX_WORDS 1000

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
    size_t text_len = strlen(text);
    int word_start = -1;
    
    for (size_t i = 0; i <= text_len; i++) {
        char current = text[i];
        
        if (is_valid_char(current) && word_start == -1) {
            word_start = (int)i;
        } else if ((!is_valid_char(current) || current == '\0') && word_start != -1) {
            int word_len = (int)i - word_start;
            if (word_len > 0 && word_len < MAX_WORD_LEN) {
                char temp_word[MAX_WORD_LEN];
                strncpy(temp_word, text + word_start, (size_t)word_len);
                temp_word[word_len] = '\0';
                
                for (int j = 0; j < word_len; j++) {
                    temp_word[j] = (char)tolower((unsigned char)temp_word[j]);
                }
                
                int found = -1;
                for (int j = 0; j < word_count; j++) {
                    if (strcmp(words[j].word, temp_word) == 0) {
                        found = j;
                        break;
                    }
                }
                
                if (found >= 0) {
                    if (words[found].count < INT_MAX) {
                        words[found].count++;
                    }
                } else if (word_count < max_words) {
                    strncpy(words[word_count].word, temp_word, MAX_WORD_LEN - 1);
                    words[word_count].word[MAX_WORD_LEN - 1] = '\0';
                    words[word_count].count = 1;
                    word_count++;
                }
            }
            word_start = -1;
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

void print_word_frequencies(struct WordCount words[], int count) {
    if (words == NULL || count <= 0) {
        return;
    }
    
    printf("Word frequencies (sorted by count, then alphabetically):\n");
    printf("%-20s %s\n", "Word", "Count");
    printf("%-20s %s\n", "----", "-----");
    
    for (int i = 0; i < count; i++) {
        if (words[i].count > 0) {
            printf("%-20s %d\n", words[i].word, words[i].count);
        }
    }
}

int read_input_text(char *buffer, int max_len) {
    if (buffer == NULL || max_len <= 0) {
        return 0;
    }
    
    printf("Enter text (max %d characters):\n", max_len - 1);
    
    if (fgets(buffer, max_len, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    return (int)len;
}

int main(void) {
    char input_text[MAX_LINE_LEN];
    struct WordCount words[MAX_WORDS];
    
    int text_len = read_input_text(input_text, MAX_LINE_LEN);
    if (text_len <= 0) {
        printf("No input received.\n");
        return 1;
    }
    
    if (text_len >= MAX_LINE_LEN - 1) {
        printf("Warning: Input may have been truncated.\n");
    }
    
    int word_count = extract_words(input_text, words, MAX_WORDS);
    if (word_count <= 0) {
        printf("No valid words found in the input.\n");
        return 1;
    }
    
    sort_words(words, word_count);
    print_word_frequencies(words, word_count);
    
    return 0;
}