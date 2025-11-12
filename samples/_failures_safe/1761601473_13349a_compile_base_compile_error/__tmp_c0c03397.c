//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct WordStats {
    char word[64];
    int length;
    int vowel_count;
    int consonant_count;
};

struct TextAnalyzer {
    struct WordStats *words;
    int word_count;
    int capacity;
    char text[1024];
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->words = NULL;
    analyzer->word_count = 0;
    analyzer->capacity = 0;
    analyzer->text[0] = '\0';
}

void free_analyzer(struct TextAnalyzer *analyzer) {
    if (analyzer->words != NULL) {
        free(analyzer->words);
        analyzer->words = NULL;
    }
    analyzer->word_count = 0;
    analyzer->capacity = 0;
}

int is_vowel(char c) {
    c = tolower(c);
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

int is_consonant(char c) {
    c = tolower(c);
    return isalpha(c) && !is_vowel(c);
}

int resize_words_array(struct TextAnalyzer *analyzer, int new_capacity) {
    if (new_capacity <= analyzer->capacity) return 1;
    struct WordStats *new_words = realloc(analyzer->words, new_capacity * sizeof(struct WordStats));
    if (new_words == NULL) return 0;
    analyzer->words = new_words;
    analyzer->capacity = new_capacity;
    return 1;
}

int add_word(struct TextAnalyzer *analyzer, const char *word, int length) {
    if (length <= 0 || length >= 64) return 0;
    if (analyzer->word_count >= analyzer->capacity) {
        int new_capacity = analyzer->capacity == 0 ? 8 : analyzer->capacity * 2;
        if (!resize_words_array(analyzer, new_capacity)) return 0;
    }
    struct WordStats *ws = &analyzer->words[analyzer->word_count];
    strncpy(ws->word, word, length);
    ws->word[length] = '\0';
    ws->length = length;
    ws->vowel_count = 0;
    ws->consonant_count = 0;
    for (int i = 0; i < length; i++) {
        if (is_vowel(ws->word[i])) ws->vowel_count++;
        else if (is_consonant(ws->word[i])) ws->consonant_count++;
    }
    analyzer->word_count++;
    return 1;
}

int parse_text(struct TextAnalyzer *analyzer, const char *input) {
    if (input == NULL) return 0;
    size_t input_len = strlen(input);
    if (input_len >= sizeof(analyzer->text)) return 0;
    strncpy(analyzer->text, input, sizeof(analyzer->text) - 1);
    analyzer->text[sizeof(analyzer->text) - 1] = '\0';
    free_analyzer(analyzer);
    int in_word = 0;
    char current_word[64];
    int word_len = 0;
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        if (isalpha(c)) {
            if (!in_word) {
                in_word = 1;
                word_len = 0;
            }
            if (word_len < 63) {
                current_word[word_len++] = c;
            }
        } else {
            if (in_word) {
                if (word_len > 0) {
                    if (!add_word(analyzer, current_word, word_len)) return 0;
                }
                in_word = 0;
                word_len = 0;
            }
        }
    }
    return 1;
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text analysis results:\n");
    printf("Input text: %s\n", analyzer->text);
    printf("Total words: %d\n", analyzer->word_count);
    printf("\nWord breakdown:\n");
    for (int i = 0; i < analyzer->word_count; i++) {
        const struct WordStats *ws = &analyzer->words[i];
        printf("Word %d: '%s' (length: %d, vowels: %d, consonants: %d)\n",
               i + 1, ws->word, ws->length, ws->vowel_count, ws->consonant_count);
    }
    int total_vowels = 0;
    int total_consonants = 0;
    for (int i = 0; i < analyzer->word_count; i++) {
        total_vowels += analyzer->words[i].vowel_count;
        total_consonants += analyzer->words[i].consonant_count;
    }
    printf("\nTotal vowels: %d\n", total_vowels);
    printf("Total consonants: %d\n", total_consonants);
}

int main(void) {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    char input[1024];
    printf("Enter text to analyze: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        free_analyzer(&analyzer);
        return 1;
    }
    size_t len = strlen