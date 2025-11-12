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
    int is_palindrome;
};

struct TextAnalyzer {
    struct WordStats *words;
    int word_count;
    int total_vowels;
    int total_consonants;
    char longest_word[64];
    char shortest_word[64];
};

void initialize_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->words = NULL;
    analyzer->word_count = 0;
    analyzer->total_vowels = 0;
    analyzer->total_consonants = 0;
    memset(analyzer->longest_word, 0, sizeof(analyzer->longest_word));
    memset(analyzer->shortest_word, 0, sizeof(analyzer->shortest_word));
}

int is_vowel(char c) {
    c = tolower(c);
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

int is_consonant(char c) {
    c = tolower(c);
    return isalpha(c) && !is_vowel(c);
}

int check_palindrome(const char *str, int len) {
    if (len <= 1) return 1;
    int left = 0, right = len - 1;
    while (left < right) {
        if (tolower(str[left]) != tolower(str[right])) return 0;
        left++;
        right--;
    }
    return 1;
}

int process_word(struct TextAnalyzer *analyzer, const char *word, int len) {
    if (len <= 0 || len >= 64) return 0;
    
    struct WordStats *new_words = realloc(analyzer->words, (analyzer->word_count + 1) * sizeof(struct WordStats));
    if (!new_words) return 0;
    analyzer->words = new_words;
    
    struct WordStats *current = &analyzer->words[analyzer->word_count];
    strncpy(current->word, word, len);
    current->word[len] = '\0';
    current->length = len;
    current->vowel_count = 0;
    current->consonant_count = 0;
    current->is_palindrome = check_palindrome(word, len);
    
    for (int i = 0; i < len; i++) {
        if (is_vowel(current->word[i])) {
            current->vowel_count++;
            analyzer->total_vowels++;
        } else if (is_consonant(current->word[i])) {
            current->consonant_count++;
            analyzer->total_consonants++;
        }
    }
    
    if (analyzer->word_count == 0) {
        strcpy(analyzer->longest_word, current->word);
        strcpy(analyzer->shortest_word, current->word);
    } else {
        if (len > strlen(analyzer->longest_word)) {
            strcpy(analyzer->longest_word, current->word);
        }
        if (len < strlen(analyzer->shortest_word)) {
            strcpy(analyzer->shortest_word, current->word);
        }
    }
    
    analyzer->word_count++;
    return 1;
}

int parse_text(struct TextAnalyzer *analyzer, const char *text) {
    if (!text) return 0;
    
    int len = strlen(text);
    if (len <= 0 || len > 1024) return 0;
    
    char buffer[64];
    int buf_index = 0;
    
    for (int i = 0; i <= len; i++) {
        if (i < len && isalpha(text[i])) {
            if (buf_index < 63) {
                buffer[buf_index++] = text[i];
            }
        } else {
            if (buf_index > 0) {
                buffer[buf_index] = '\0';
                if (!process_word(analyzer, buffer, buf_index)) {
                    return 0;
                }
                buf_index = 0;
            }
        }
    }
    return 1;
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    if (!analyzer || analyzer->word_count == 0) {
        printf("No words to analyze.\n");
        return;
    }
    
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->word_count);
    printf("Total vowels: %d\n", analyzer->total_vowels);
    printf("Total consonants: %d\n", analyzer->total_consonants);
    printf("Longest word: %s\n", analyzer->longest_word);
    printf("Shortest word: %s\n", analyzer->shortest_word);
    
    printf("\nWord Details:\n");
    for (int i = 0; i < analyzer->word_count; i++) {
        const struct WordStats *word = &analyzer->words[i];
        printf("Word: %-12s Length: %2d Vowels: %d Consonants: %d Palindrome: %s\n",
               word->word, word->length, word->vowel_count, word->consonant_count,
               word->is_palindrome ? "Yes" : "No");
    }
}

void cleanup_analyzer(struct TextAnalyzer *analyzer) {
    if (analyzer->words) {
        free(analyzer->words);
        analyzer->words =