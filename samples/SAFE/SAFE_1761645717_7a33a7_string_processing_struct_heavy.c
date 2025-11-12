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

void init_analyzer(struct TextAnalyzer *analyzer) {
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

int add_word(struct TextAnalyzer *analyzer, const char *word, int length) {
    if (length <= 0 || length >= MAX_WORD_LEN || analyzer->word_count >= MAX_WORDS) {
        return 0;
    }

    for (int i = 0; i < analyzer->word_count; i++) {
        if (strncmp(analyzer->words[i].word, word, length) == 0 && 
            analyzer->words[i].length == length) {
            analyzer->words[i].count++;
            return 1;
        }
    }

    if (analyzer->word_count < MAX_WORDS) {
        strncpy(analyzer->words[analyzer->word_count].word, word, length);
        analyzer->words[analyzer->word_count].word[length] = '\0';
        analyzer->words[analyzer->word_count].length = length;
        analyzer->words[analyzer->word_count].count = 1;
        analyzer->words[analyzer->word_count].is_capitalized = isupper(word[0]) ? 1 : 0;
        analyzer->word_count++;
        analyzer->unique_words++;
        return 1;
    }
    return 0;
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) return;
    
    int len = strlen(text);
    if (len <= 0) return;
    
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    int in_word = 0;
    
    for (int i = 0; i < len; i++) {
        if (!is_valid_char(text[i])) continue;
        
        if (isalpha(text[i])) {
            if (buf_pos < MAX_WORD_LEN - 1) {
                buffer[buf_pos++] = text[i];
                analyzer->total_characters++;
            }
            in_word = 1;
        } else if (in_word && buf_pos > 0) {
            buffer[buf_pos] = '\0';
            add_word(analyzer, buffer, buf_pos);
            analyzer->total_words++;
            buf_pos = 0;
            in_word = 0;
        }
    }
    
    if (in_word && buf_pos > 0) {
        buffer[buf_pos] = '\0';
        add_word(analyzer, buffer, buf_pos);
        analyzer->total_words++;
    }
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total characters: %d\n", analyzer->total_characters);
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->unique_words);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        printf("'%s': %d occurrence(s), length: %d, %s\n",
               analyzer->words[i].word,
               analyzer->words[i].count,
               analyzer->words[i].length,
               analyzer->words[i].is_capitalized ? "capitalized" : "lowercase");
    }
}

int main() {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input_buffer[1024];
    printf("Enter text to analyze (max 1023 characters):\n");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    process_text(&analyzer, input_buffer);
    print_analysis(&analyzer);
    
    return 0;
}