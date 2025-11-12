//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 1000
#define MAX_LINE_LEN 256

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int length;
    int is_capitalized;
};

struct TextProcessor {
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_words;
    int unique_words;
    int longest_word_length;
};

struct TextAnalyzer {
    struct TextProcessor processor;
    char input_text[MAX_LINE_LEN];
    char processed_text[MAX_LINE_LEN];
    int text_length;
    int line_count;
};

void initialize_analyzer(struct TextAnalyzer *analyzer) {
    memset(analyzer, 0, sizeof(struct TextAnalyzer));
    analyzer->processor.word_count = 0;
    analyzer->processor.total_words = 0;
    analyzer->processor.unique_words = 0;
    analyzer->processor.longest_word_length = 0;
}

int is_valid_character(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\n' || c == '\t' || c == '.' || c == ',' || c == '!' || c == '?';
}

int sanitize_input(struct TextAnalyzer *analyzer, const char *input) {
    if (input == NULL) return 0;
    
    size_t input_len = strlen(input);
    if (input_len == 0 || input_len >= MAX_LINE_LEN) return 0;
    
    int j = 0;
    for (size_t i = 0; i < input_len; i++) {
        if (is_valid_character(input[i])) {
            analyzer->input_text[j++] = input[i];
        }
        if (j >= MAX_LINE_LEN - 1) break;
    }
    analyzer->input_text[j] = '\0';
    analyzer->text_length = j;
    return j > 0;
}

void process_word(struct TextAnalyzer *analyzer, const char *word, int word_len) {
    if (word_len <= 0 || word_len >= MAX_WORD_LEN) return;
    if (analyzer->processor.word_count >= MAX_WORDS) return;
    
    char normalized_word[MAX_WORD_LEN];
    int is_cap = isupper((unsigned char)word[0]) ? 1 : 0;
    
    for (int i = 0; i < word_len; i++) {
        normalized_word[i] = tolower((unsigned char)word[i]);
    }
    normalized_word[word_len] = '\0';
    
    int found = 0;
    for (int i = 0; i < analyzer->processor.word_count; i++) {
        if (strcmp(analyzer->processor.words[i].word, normalized_word) == 0) {
            analyzer->processor.words[i].count++;
            found = 1;
            break;
        }
    }
    
    if (!found) {
        struct WordStats *ws = &analyzer->processor.words[analyzer->processor.word_count];
        strncpy(ws->word, normalized_word, MAX_WORD_LEN - 1);
        ws->word[MAX_WORD_LEN - 1] = '\0';
        ws->count = 1;
        ws->length = word_len;
        ws->is_capitalized = is_cap;
        analyzer->processor.word_count++;
        analyzer->processor.unique_words++;
        
        if (word_len > analyzer->processor.longest_word_length) {
            analyzer->processor.longest_word_length = word_len;
        }
    }
    
    analyzer->processor.total_words++;
}

void tokenize_text(struct TextAnalyzer *analyzer) {
    char buffer[MAX_LINE_LEN];
    strncpy(buffer, analyzer->input_text, MAX_LINE_LEN - 1);
    buffer[MAX_LINE_LEN - 1] = '\0';
    
    char *token = strtok(buffer, " \t\n.,!?");
    while (token != NULL && analyzer->processor.word_count < MAX_WORDS) {
        int token_len = strlen(token);
        if (token_len > 0 && token_len < MAX_WORD_LEN) {
            process_word(analyzer, token, token_len);
        }
        token = strtok(NULL, " \t\n.,!?");
    }
}

void generate_report(struct TextAnalyzer *analyzer) {
    printf("Text Analysis Report:\n");
    printf("Total words: %d\n", analyzer->processor.total_words);
    printf("Unique words: %d\n", analyzer->processor.unique_words);
    printf("Longest word length: %d\n", analyzer->processor.longest_word_length);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < analyzer->processor.word_count; i++) {
        if (analyzer->processor.words[i].count > 0) {
            printf("%s: %d\n", analyzer->processor.words[i].word, 
                   analyzer->processor.words[i].count);
        }
    }
}

int main(void) {
    struct TextAnalyzer analyzer;
    initialize_analyzer(&analyzer);
    
    char input_buffer[MAX_LINE_LEN];
    printf("Enter text to analyze (max %d characters): ", MAX_LINE_LEN - 1);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input.\n");
        return