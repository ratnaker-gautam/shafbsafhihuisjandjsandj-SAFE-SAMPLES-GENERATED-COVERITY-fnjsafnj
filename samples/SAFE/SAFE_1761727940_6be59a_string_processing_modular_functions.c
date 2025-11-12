//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LINE_LEN 256
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

struct WordCount {
    char word[MAX_WORD_LEN];
    int count;
};

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

int extract_words(const char *text, struct WordCount words[], int max_words) {
    char buffer[MAX_LINE_LEN];
    int word_count = 0;
    int i = 0;
    int len = strlen(text);
    
    if (len >= MAX_LINE_LEN) {
        return -1;
    }
    
    strncpy(buffer, text, MAX_LINE_LEN - 1);
    buffer[MAX_LINE_LEN - 1] = '\0';
    
    while (i < len && word_count < max_words) {
        while (i < len && !is_valid_char(buffer[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        int start = i;
        while (i < len && is_valid_char(buffer[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            char temp_word[MAX_WORD_LEN];
            strncpy(temp_word, &buffer[start], word_len);
            temp_word[word_len] = '\0';
            
            for (int j = 0; j < word_len; j++) {
                temp_word[j] = tolower((unsigned char)temp_word[j]);
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
                strncpy(words[word_count].word, temp_word, MAX_WORD_LEN - 1);
                words[word_count].word[MAX_WORD_LEN - 1] = '\0';
                words[word_count].count = 1;
                word_count++;
            }
        }
    }
    
    return word_count;
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

int get_input(char *buffer, int max_len) {
    if (fgets(buffer, max_len, stdin) == NULL) {
        return -1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    } else if (len == max_len - 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        return -2;
    }
    
    return 0;
}

int main(void) {
    char input[MAX_LINE_LEN];
    struct WordCount words[MAX_WORDS];
    
    printf("Enter text for word frequency analysis: ");
    
    int result = get_input(input, MAX_LINE_LEN);
    if (result != 0) {
        if (result == -1) {
            printf("Error reading input.\n");
        } else {
            printf("Input too long. Maximum %d characters allowed.\n", MAX_LINE_LEN - 1);
        }
        return 1;
    }
    
    if (strlen(input) == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    int word_count = extract_words(input, words, MAX_WORDS);
    if (word_count < 0) {
        printf("Error processing input text.\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found in input.\n");
        return 0;
    }
    
    sort_words(words, word_count);
    
    printf("\nWord frequency analysis:\n");
    printf("Word\t\tCount\n");
    printf("----\t\t-----\n");
    
    for (int i = 0; i < word_count; i++) {
        printf("%-15s%d\n", words[i].word, words[i].count);
    }
    
    return 0;
}