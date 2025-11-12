//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 100
#define MAX_WORD_LEN 50
#define MAX_TEXT_LEN 1000

struct WordInfo {
    char word[MAX_WORD_LEN];
    int count;
    int length;
    int is_capitalized;
};

struct TextAnalyzer {
    struct WordInfo words[MAX_WORDS];
    int word_count;
    int total_words;
    int total_chars;
    int longest_word_length;
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_words = 0;
    analyzer->total_chars = 0;
    analyzer->longest_word_length = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        analyzer->words[i].word[0] = '\0';
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].is_capitalized = 0;
    }
}

int is_valid_char(char c) {
    return isalpha(c) || c == ' ' || c == '\n' || c == '\t';
}

int find_word_index(struct TextAnalyzer *analyzer, const char *word) {
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void add_word(struct TextAnalyzer *analyzer, const char *word, int is_capitalized) {
    if (analyzer->word_count >= MAX_WORDS) {
        return;
    }
    
    int index = find_word_index(analyzer, word);
    if (index != -1) {
        analyzer->words[index].count++;
        return;
    }
    
    int len = strlen(word);
    if (len >= MAX_WORD_LEN) {
        return;
    }
    
    strcpy(analyzer->words[analyzer->word_count].word, word);
    analyzer->words[analyzer->word_count].count = 1;
    analyzer->words[analyzer->word_count].length = len;
    analyzer->words[analyzer->word_count].is_capitalized = is_capitalized;
    analyzer->word_count++;
    
    if (len > analyzer->longest_word_length) {
        analyzer->longest_word_length = len;
    }
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    char buffer[MAX_TEXT_LEN];
    if (strlen(text) >= MAX_TEXT_LEN) {
        return;
    }
    strcpy(buffer, text);
    
    char *token = strtok(buffer, " \t\n");
    while (token != NULL && analyzer->total_words < MAX_WORDS) {
        int valid = 1;
        for (int i = 0; token[i] != '\0'; i++) {
            if (!isalpha(token[i])) {
                valid = 0;
                break;
            }
        }
        
        if (valid && strlen(token) > 0) {
            int is_capitalized = isupper(token[0]) ? 1 : 0;
            char lowercase_word[MAX_WORD_LEN];
            strcpy(lowercase_word, token);
            for (int i = 0; lowercase_word[i] != '\0'; i++) {
                lowercase_word[i] = tolower(lowercase_word[i]);
            }
            
            add_word(analyzer, lowercase_word, is_capitalized);
            analyzer->total_words++;
            analyzer->total_chars += strlen(token);
        }
        
        token = strtok(NULL, " \t\n");
    }
}

void print_analysis(struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Total characters: %d\n", analyzer->total_chars);
    printf("Unique words: %d\n", analyzer->word_count);
    printf("Longest word length: %d\n\n", analyzer->longest_word_length);
    
    printf("Word frequency:\n");
    for (int i = 0; i < analyzer->word_count; i++) {
        printf("%s: %d (length: %d, capitalized: %s)\n",
               analyzer->words[i].word,
               analyzer->words[i].count,
               analyzer->words[i].length,
               analyzer->words[i].is_capitalized ? "yes" : "no");
    }
}

int main() {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input_text[MAX_TEXT_LEN];
    printf("Enter text to analyze (max %d characters):\n", MAX_TEXT_LEN - 1);
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input_text);
    if (len > 0 && input_text[len - 1] == '\n') {
        input_text[len - 1] = '\0';
    }
    
    int valid_input = 1;
    for (int i = 0; input_text[i] != '\0'; i++) {
        if (!is_valid_char(input_text[i])) {
            valid_input = 0;
            break;
        }
    }
    
    if (!valid_input) {