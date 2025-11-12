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

int is_valid_char(char c) {
    return isalpha((unsigned char)c) || c == ' ';
}

int count_vowels(const char *str) {
    int count = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        char c = tolower((unsigned char)str[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            count++;
        }
    }
    return count;
}

int count_consonants(const char *str) {
    int count = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        char c = tolower((unsigned char)str[i]);
        if (isalpha((unsigned char)c) && c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u') {
            count++;
        }
    }
    return count;
}

int resize_words_array(struct TextAnalyzer *analyzer) {
    int new_capacity = analyzer->capacity == 0 ? 8 : analyzer->capacity * 2;
    struct WordStats *new_words = realloc(analyzer->words, new_capacity * sizeof(struct WordStats));
    if (new_words == NULL) {
        return 0;
    }
    analyzer->words = new_words;
    analyzer->capacity = new_capacity;
    return 1;
}

int add_word(struct TextAnalyzer *analyzer, const char *word) {
    if (analyzer->word_count >= analyzer->capacity) {
        if (!resize_words_array(analyzer)) {
            return 0;
        }
    }
    struct WordStats *ws = &analyzer->words[analyzer->word_count];
    size_t len = strlen(word);
    if (len >= sizeof(ws->word)) {
        len = sizeof(ws->word) - 1;
    }
    strncpy(ws->word, word, len);
    ws->word[len] = '\0';
    ws->length = (int)len;
    ws->vowel_count = count_vowels(ws->word);
    ws->consonant_count = count_consonants(ws->word);
    analyzer->word_count++;
    return 1;
}

int parse_text(struct TextAnalyzer *analyzer, const char *input) {
    size_t input_len = strlen(input);
    if (input_len >= sizeof(analyzer->text)) {
        return 0;
    }
    for (size_t i = 0; i < input_len; i++) {
        if (!is_valid_char(input[i])) {
            return 0;
        }
    }
    strncpy(analyzer->text, input, sizeof(analyzer->text) - 1);
    analyzer->text[sizeof(analyzer->text) - 1] = '\0';
    
    char buffer[64];
    int buf_index = 0;
    for (size_t i = 0; i <= input_len; i++) {
        if (input[i] == ' ' || input[i] == '\0') {
            if (buf_index > 0) {
                buffer[buf_index] = '\0';
                if (!add_word(analyzer, buffer)) {
                    return 0;
                }
                buf_index = 0;
            }
        } else if (isalpha((unsigned char)input[i])) {
            if (buf_index < (int)sizeof(buffer) - 1) {
                buffer[buf_index++] = input[i];
            } else {
                return 0;
            }
        }
    }
    return 1;
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text analysis for: %s\n", analyzer->text);
    printf("Total words: %d\n", analyzer->word_count);
    printf("Word details:\n");
    for (int i = 0; i < analyzer->word_count; i++) {
        const struct WordStats *ws = &analyzer->words[i];
        printf("  Word: %-12s Length: %2d Vowels: %d Consonants: %d\n", 
               ws->word, ws->length, ws->vowel_count, ws->consonant_count);
    }
}

int main() {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input[1024];
    printf("Enter text (letters and spaces only, max %zu chars): ", sizeof(input) - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        free_analyzer(&analyzer