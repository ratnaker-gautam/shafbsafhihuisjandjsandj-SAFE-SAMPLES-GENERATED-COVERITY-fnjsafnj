//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 100
#define MAX_WORD_LEN 50
#define MAX_TEXT_LEN 1000

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int first_position;
};

struct TextProcessor {
    struct WordStats words[MAX_WORDS];
    int word_count;
    char processed_text[MAX_TEXT_LEN];
    int total_chars;
    int total_words;
};

void init_processor(struct TextProcessor *proc) {
    proc->word_count = 0;
    proc->processed_text[0] = '\0';
    proc->total_chars = 0;
    proc->total_words = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        proc->words[i].word[0] = '\0';
        proc->words[i].count = 0;
        proc->words[i].first_position = -1;
    }
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\t' || c == '\n';
}

int safe_strncpy(char *dest, const char *src, size_t n) {
    if (dest == NULL || src == NULL || n == 0) return 0;
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return 1;
}

int add_word(struct TextProcessor *proc, const char *word, int position) {
    if (proc->word_count >= MAX_WORDS) return 0;
    if (word == NULL || word[0] == '\0') return 0;
    
    for (int i = 0; i < proc->word_count; i++) {
        if (strcmp(proc->words[i].word, word) == 0) {
            proc->words[i].count++;
            return 1;
        }
    }
    
    if (safe_strncpy(proc->words[proc->word_count].word, word, MAX_WORD_LEN)) {
        proc->words[proc->word_count].count = 1;
        proc->words[proc->word_count].first_position = position;
        proc->word_count++;
        return 1;
    }
    return 0;
}

int process_text(struct TextProcessor *proc, const char *input) {
    if (proc == NULL || input == NULL) return 0;
    
    char buffer[MAX_TEXT_LEN];
    if (!safe_strncpy(buffer, input, MAX_TEXT_LEN)) return 0;
    
    char clean_text[MAX_TEXT_LEN];
    int clean_index = 0;
    
    for (int i = 0; buffer[i] != '\0' && clean_index < MAX_TEXT_LEN - 1; i++) {
        if (is_valid_char(buffer[i])) {
            clean_text[clean_index++] = tolower((unsigned char)buffer[i]);
        } else if (clean_index > 0 && clean_text[clean_index-1] != ' ') {
            clean_text[clean_index++] = ' ';
        }
    }
    
    if (clean_index > 0 && clean_text[clean_index-1] == ' ') {
        clean_index--;
    }
    clean_text[clean_index] = '\0';
    
    if (!safe_strncpy(proc->processed_text, clean_text, MAX_TEXT_LEN)) return 0;
    proc->total_chars = clean_index;
    
    char word[MAX_WORD_LEN];
    int word_index = 0;
    int word_position = 0;
    
    for (int i = 0; clean_text[i] != '\0' && proc->total_words < MAX_WORDS; i++) {
        if (clean_text[i] != ' ') {
            if (word_index < MAX_WORD_LEN - 1) {
                word[word_index++] = clean_text[i];
            }
        } else {
            if (word_index > 0) {
                word[word_index] = '\0';
                if (add_word(proc, word, word_position)) {
                    proc->total_words++;
                    word_position++;
                }
                word_index = 0;
            }
        }
    }
    
    if (word_index > 0) {
        word[word_index] = '\0';
        if (add_word(proc, word, word_position)) {
            proc->total_words++;
        }
    }
    
    return 1;
}

void print_stats(const struct TextProcessor *proc) {
    if (proc == NULL) return;
    
    printf("Processed text: %s\n", proc->processed_text);
    printf("Total characters: %d\n", proc->total_chars);
    printf("Total unique words: %d\n", proc->word_count);
    printf("Total words: %d\n", proc->total_words);
    printf("\nWord statistics:\n");
    
    for (int i = 0; i < proc->word_count; i++) {
        printf("Word: %-15s Count: %-3d First Position: %d\n",
               proc->words[i].word, proc->words[i].count, proc->words[i].first_position);
    }
}

int main() {
    struct TextProcessor processor;
    init_processor(&processor);
    
    char input[MAX_TEXT_LEN];
    printf("Enter text to process (max %d characters): ", MAX