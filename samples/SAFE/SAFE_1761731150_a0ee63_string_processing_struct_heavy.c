//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 100
#define MAX_TEXT_LEN 1024

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int length;
    int is_capitalized;
};

struct TextAnalyzer {
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_words;
    int total_chars;
    int longest_word_len;
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_words = 0;
    analyzer->total_chars = 0;
    analyzer->longest_word_len = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].is_capitalized = 0;
        analyzer->words[i].word[0] = '\0';
    }
}

int is_word_char(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

void add_word(struct TextAnalyzer *analyzer, const char *word, int len) {
    if (len <= 0 || len >= MAX_WORD_LEN || analyzer->word_count >= MAX_WORDS) {
        return;
    }
    
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strncmp(analyzer->words[i].word, word, len) == 0 && 
            analyzer->words[i].word[len] == '\0') {
            analyzer->words[i].count++;
            return;
        }
    }
    
    if (analyzer->word_count < MAX_WORDS) {
        strncpy(analyzer->words[analyzer->word_count].word, word, len);
        analyzer->words[analyzer->word_count].word[len] = '\0';
        analyzer->words[analyzer->word_count].count = 1;
        analyzer->words[analyzer->word_count].length = len;
        analyzer->words[analyzer->word_count].is_capitalized = isupper((unsigned char)word[0]);
        analyzer->word_count++;
    }
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) {
        return;
    }
    
    char buffer[MAX_TEXT_LEN];
    size_t text_len = strlen(text);
    if (text_len >= MAX_TEXT_LEN - 1) {
        text_len = MAX_TEXT_LEN - 1;
    }
    strncpy(buffer, text, text_len);
    buffer[text_len] = '\0';
    
    char *ptr = buffer;
    while (*ptr) {
        while (*ptr && !is_word_char(*ptr)) {
            ptr++;
        }
        
        if (!*ptr) break;
        
        char *word_start = ptr;
        while (*ptr && is_word_char(*ptr)) {
            ptr++;
        }
        
        int word_len = ptr - word_start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            char temp_word[MAX_WORD_LEN];
            strncpy(temp_word, word_start, word_len);
            temp_word[word_len] = '\0';
            
            add_word(analyzer, temp_word, word_len);
            analyzer->total_words++;
            analyzer->total_chars += word_len;
            
            if (word_len > analyzer->longest_word_len) {
                analyzer->longest_word_len = word_len;
            }
        }
        
        if (!*ptr) break;
        ptr++;
    }
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Total characters in words: %d\n", analyzer->total_chars);
    printf("Longest word length: %d\n", analyzer->longest_word_len);
    printf("Unique words: %d\n", analyzer->word_count);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        if (analyzer->words[i].count > 0) {
            printf("%-20s: %d occurrences\n", 
                   analyzer->words[i].word, 
                   analyzer->words[i].count);
        }
    }
}

int main() {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input_text[MAX_TEXT_LEN];
    printf("Enter text to analyze (max %d characters):\n", MAX_TEXT_LEN - 1);
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_text);
    if (len > 0 && input_text[len - 1] == '\n') {
        input_text[len - 1] = '\0';
    }
    
    if (strlen(input_text) == 0) {
        printf("No text entered.\n");
        return 1;
    }
    
    process_text(&analyzer, input_text);
    print_analysis(&analyzer);
    
    return 0;
}