//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 100
#define MAX_WORD_LEN 50

struct WordInfo {
    char word[MAX_WORD_LEN];
    int count;
    int length;
    int is_capitalized;
};

struct TextAnalyzer {
    struct WordInfo words[MAX_WORDS];
    int word_count;
    int total_characters;
    int total_words;
    int unique_words;
};

void initialize_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_characters = 0;
    analyzer->total_words = 0;
    analyzer->unique_words = 0;
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

int add_word(struct TextAnalyzer *analyzer, const char *word, int is_capitalized) {
    if (analyzer->word_count >= MAX_WORDS) {
        return 0;
    }
    
    int word_len = strlen(word);
    if (word_len >= MAX_WORD_LEN || word_len == 0) {
        return 0;
    }
    
    int existing_index = find_word_index(analyzer, word);
    if (existing_index != -1) {
        analyzer->words[existing_index].count++;
        return 1;
    }
    
    strncpy(analyzer->words[analyzer->word_count].word, word, MAX_WORD_LEN - 1);
    analyzer->words[analyzer->word_count].word[MAX_WORD_LEN - 1] = '\0';
    analyzer->words[analyzer->word_count].count = 1;
    analyzer->words[analyzer->word_count].length = word_len;
    analyzer->words[analyzer->word_count].is_capitalized = is_capitalized;
    analyzer->word_count++;
    analyzer->unique_words++;
    
    return 1;
}

int process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) {
        return 0;
    }
    
    char buffer[MAX_WORD_LEN];
    int buffer_index = 0;
    int text_len = strlen(text);
    
    if (text_len > 1000) {
        return 0;
    }
    
    for (int i = 0; i <= text_len; i++) {
        char c = text[i];
        
        if (!is_valid_char(c) && c != '\0') {
            return 0;
        }
        
        if (isalpha(c)) {
            if (buffer_index < MAX_WORD_LEN - 1) {
                buffer[buffer_index++] = c;
                analyzer->total_characters++;
            } else {
                return 0;
            }
        } else if (buffer_index > 0) {
            buffer[buffer_index] = '\0';
            
            int is_capitalized = isupper(buffer[0]) ? 1 : 0;
            for (int j = 0; j < buffer_index; j++) {
                buffer[j] = tolower(buffer[j]);
            }
            
            if (!add_word(analyzer, buffer, is_capitalized)) {
                return 0;
            }
            
            analyzer->total_words++;
            buffer_index = 0;
        }
    }
    
    return 1;
}

void print_analysis(struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Total characters: %d\n", analyzer->total_characters);
    printf("Unique words: %d\n", analyzer->unique_words);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        printf("'%s': %d occurrences (length: %d, %s)\n",
               analyzer->words[i].word,
               analyzer->words[i].count,
               analyzer->words[i].length,
               analyzer->words[i].is_capitalized ? "capitalized" : "lowercase");
    }
}

int main() {
    struct TextAnalyzer analyzer;
    initialize_analyzer(&analyzer);
    
    char input_text[1001];
    printf("Enter text to analyze (max 1000 characters):\n");
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input_text);
    if (len > 0 && input_text[len - 1] == '\n') {
        input_text[len - 1] = '\0';
    }
    
    if (strlen(input_text) == 0) {
        printf("No input provided.\n");
        return 1;
    }