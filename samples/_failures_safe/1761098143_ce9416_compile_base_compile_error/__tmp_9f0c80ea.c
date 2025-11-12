//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: string_processing
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
    int total_characters;
    int total_words;
    char processed_text[MAX_TEXT_LEN];
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_characters = 0;
    analyzer->total_words = 0;
    memset(analyzer->processed_text, 0, sizeof(analyzer->processed_text));
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(analyzer->words[i].word, 0, sizeof(analyzer->words[i].word));
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].is_capitalized = 0;
    }
}

int is_valid_char(char c) {
    return isalpha(c) || isspace(c) || c == '.' || c == ',' || c == '!' || c == '?';
}

int add_word(struct TextAnalyzer *analyzer, const char *word, int length) {
    if (length <= 0 || length >= MAX_WORD_LEN || analyzer->word_count >= MAX_WORDS) {
        return 0;
    }

    char temp_word[MAX_WORD_LEN];
    strncpy(temp_word, word, length);
    temp_word[length] = '\0';

    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, temp_word) == 0) {
            analyzer->words[i].count++;
            return 1;
        }
    }

    if (analyzer->word_count < MAX_WORDS) {
        strncpy(analyzer->words[analyzer->word_count].word, temp_word, MAX_WORD_LEN - 1);
        analyzer->words[analyzer->word_count].word[MAX_WORD_LEN - 1] = '\0';
        analyzer->words[analyzer->word_count].count = 1;
        analyzer->words[analyzer->word_count].length = length;
        analyzer->words[analyzer->word_count].is_capitalized = isupper(temp_word[0]) ? 1 : 0;
        analyzer->word_count++;
        return 1;
    }

    return 0;
}

int process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL || strlen(text) == 0 || strlen(text) >= MAX_TEXT_LEN) {
        return 0;
    }

    init_analyzer(analyzer);
    strncpy(analyzer->processed_text, text, MAX_TEXT_LEN - 1);
    analyzer->processed_text[MAX_TEXT_LEN - 1] = '\0';
    analyzer->total_characters = strlen(text);

    char buffer[MAX_WORD_LEN];
    int buf_index = 0;
    int in_word = 0;

    for (int i = 0; text[i] != '\0' && i < MAX_TEXT_LEN; i++) {
        if (!is_valid_char(text[i])) {
            continue;
        }

        if (isalpha(text[i])) {
            if (buf_index < MAX_WORD_LEN - 1) {
                buffer[buf_index++] = text[i];
            }
            in_word = 1;
        } else if (in_word) {
            buffer[buf_index] = '\0';
            if (buf_index > 0) {
                add_word(analyzer, buffer, buf_index);
                analyzer->total_words++;
            }
            buf_index = 0;
            in_word = 0;
        }
    }

    if (in_word && buf_index > 0) {
        buffer[buf_index] = '\0';
        add_word(analyzer, buffer, buf_index);
        analyzer->total_words++;
    }

    return 1;
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total characters: %d\n", analyzer->total_characters);
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->word_count);
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
    char input_text[MAX_TEXT_LEN];

    printf("Enter text to analyze (max %d characters):\n", MAX_TEXT_LEN - 1);
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }

    size_t len = strlen(input_text);
    if (len > 0 && input_text[len - 1] == '\n') {
        input_text