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

int process_text(struct TextAnalyzer *analyzer) {
    if (analyzer->text[0] == '\0') {
        return 0;
    }

    char buffer[1024];
    if (strlen(analyzer->text) >= sizeof(buffer)) {
        return 0;
    }
    strcpy(buffer, analyzer->text);

    char *token = strtok(buffer, " ");
    while (token != NULL) {
        int valid = 1;
        for (int i = 0; token[i] != '\0'; i++) {
            if (!is_valid_char(token[i])) {
                valid = 0;
                break;
            }
        }
        if (valid && strlen(token) > 0) {
            if (analyzer->word_count >= analyzer->capacity) {
                if (!resize_words_array(analyzer)) {
                    return 0;
                }
            }
            if (strlen(token) < sizeof(analyzer->words[analyzer->word_count].word)) {
                strcpy(analyzer->words[analyzer->word_count].word, token);
                analyzer->words[analyzer->word_count].length = strlen(token);
                analyzer->words[analyzer->word_count].vowel_count = count_vowels(token);
                analyzer->words[analyzer->word_count].consonant_count = analyzer->words[analyzer->word_count].length - analyzer->words[analyzer->word_count].vowel_count;
                analyzer->word_count++;
            }
        }
        token = strtok(NULL, " ");
    }
    return 1;
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text analysis results:\n");
    printf("Total words: %d\n", analyzer->word_count);
    printf("\nWord details:\n");
    for (int i = 0; i < analyzer->word_count; i++) {
        printf("Word: %-15s Length: %2d  Vowels: %d  Consonants: %d\n",
               analyzer->words[i].word,
               analyzer->words[i].length,
               analyzer->words[i].vowel_count,
               analyzer->words[i].consonant_count);
    }
}

int main() {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);

    printf("Enter text to analyze (letters and spaces only, max 1023 chars):\n");
    if (fgets(analyzer.text, sizeof(analyzer.text), stdin) == NULL) {
        printf("Error reading input.\n");
        free_analyzer(&analyzer);
        return 1;
    }

    size_t len = strlen(analyzer.text);
    if (len > 0 && analyzer.text[len - 1] == '\n') {
        analyzer.text[len - 1] = '\0';
    }

    for (int i = 0; analyzer.text[i] != '\0'; i++) {
        if (!is_valid_char(analyzer.text[i])) {
            printf("Invalid character detected. Only letters and spaces allowed.\n");
            free_analyzer(&analyzer);
            return 1;
        }
    }

    if (!process_text(&analyzer)) {
        printf("Error processing text.\n");
        free_analyzer(&analyzer);
        return 1;
    }

    print_analysis(&analyzer);
    free_analyzer(&analyzer);
    return 0;
}