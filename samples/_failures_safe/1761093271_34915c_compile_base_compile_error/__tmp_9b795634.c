//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: string_processing
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
    int first_position;
    int last_position;
};

struct TextProcessor {
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_words;
    int unique_words;
    char processed_text[MAX_TEXT_LEN];
};

void init_processor(struct TextProcessor *proc) {
    proc->word_count = 0;
    proc->total_words = 0;
    proc->unique_words = 0;
    memset(proc->processed_text, 0, sizeof(proc->processed_text));
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(proc->words[i].word, 0, sizeof(proc->words[i].word));
        proc->words[i].count = 0;
        proc->words[i].first_position = -1;
        proc->words[i].last_position = -1;
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

int find_word_index(struct TextProcessor *proc, const char *word) {
    for (int i = 0; i < proc->word_count; i++) {
        if (strcmp(proc->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int add_word(struct TextProcessor *proc, const char *word, int position) {
    if (proc->word_count >= MAX_WORDS) return 0;
    if (strlen(word) >= MAX_WORD_LEN) return 0;
    
    int index = find_word_index(proc, word);
    if (index >= 0) {
        proc->words[index].count++;
        proc->words[index].last_position = position;
        return 1;
    }
    
    if (!safe_strncpy(proc->words[proc->word_count].word, word, MAX_WORD_LEN)) {
        return 0;
    }
    proc->words[proc->word_count].count = 1;
    proc->words[proc->word_count].first_position = position;
    proc->words[proc->word_count].last_position = position;
    proc->word_count++;
    proc->unique_words++;
    return 1;
}

int process_text(struct TextProcessor *proc, const char *input) {
    if (input == NULL) return 0;
    
    char buffer[MAX_TEXT_LEN];
    if (!safe_strncpy(buffer, input, sizeof(buffer))) return 0;
    
    char *pos = buffer;
    int word_position = 0;
    char current_word[MAX_WORD_LEN];
    int in_word = 0;
    int word_len = 0;
    
    while (*pos != '\0') {
        if (isalnum((unsigned char)*pos)) {
            if (!in_word) {
                in_word = 1;
                word_len = 0;
            }
            if (word_len < MAX_WORD_LEN - 1) {
                current_word[word_len++] = tolower((unsigned char)*pos);
            }
        } else {
            if (in_word) {
                current_word[word_len] = '\0';
                if (word_len > 0) {
                    if (!add_word(proc, current_word, word_position)) {
                        return 0;
                    }
                    proc->total_words++;
                    word_position++;
                }
                in_word = 0;
                word_len = 0;
            }
        }
        pos++;
    }
    
    if (in_word && word_len > 0) {
        current_word[word_len] = '\0';
        if (!add_word(proc, current_word, word_position)) {
            return 0;
        }
        proc->total_words++;
    }
    
    if (!safe_strncpy(proc->processed_text, buffer, sizeof(proc->processed_text))) {
        return 0;
    }
    
    return 1;
}

void print_stats(struct TextProcessor *proc) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", proc->total_words);
    printf("Unique words: %d\n", proc->unique_words);
    printf("\nWord Statistics:\n");
    
    for (int i = 0; i < proc->word_count; i++) {
        printf("Word: %-15s Count: %-3d First: %-3d Last: %-3d\n",
               proc->words[i].word,
               proc->words[i].count,
               proc->words[i].first_position,
               proc->words[i].last_position);
    }
}

int main(void) {
    struct TextProcessor processor;
    init_processor(&processor);
    
    char input_text[MAX_TEXT_LEN];
    printf("Enter text to analyze (max %d characters):\n", MAX_TEXT_LEN - 1);