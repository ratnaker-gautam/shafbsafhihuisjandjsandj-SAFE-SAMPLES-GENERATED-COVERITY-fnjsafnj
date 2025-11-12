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

struct TextProcessor {
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_words;
    int total_chars;
    int longest_word_len;
};

void init_processor(struct TextProcessor *proc) {
    proc->word_count = 0;
    proc->total_words = 0;
    proc->total_chars = 0;
    proc->longest_word_len = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        proc->words[i].count = 0;
        proc->words[i].length = 0;
        proc->words[i].is_capitalized = 0;
        memset(proc->words[i].word, 0, MAX_WORD_LEN);
    }
}

int is_word_char(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

int find_word_index(struct TextProcessor *proc, const char *word) {
    for (int i = 0; i < proc->word_count; i++) {
        if (strcmp(proc->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int add_word(struct TextProcessor *proc, const char *word, int len, int capitalized) {
    if (len <= 0 || len >= MAX_WORD_LEN || proc->word_count >= MAX_WORDS) {
        return -1;
    }
    
    int idx = find_word_index(proc, word);
    if (idx >= 0) {
        proc->words[idx].count++;
        return idx;
    }
    
    idx = proc->word_count;
    strncpy(proc->words[idx].word, word, MAX_WORD_LEN - 1);
    proc->words[idx].word[MAX_WORD_LEN - 1] = '\0';
    proc->words[idx].count = 1;
    proc->words[idx].length = len;
    proc->words[idx].is_capitalized = capitalized;
    proc->word_count++;
    
    if (len > proc->longest_word_len) {
        proc->longest_word_len = len;
    }
    
    return idx;
}

void process_text(struct TextProcessor *proc, const char *text) {
    if (text == NULL) return;
    
    int len = strlen(text);
    if (len <= 0 || len >= MAX_TEXT_LEN) return;
    
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    int in_word = 0;
    
    for (int i = 0; i <= len; i++) {
        char c = text[i];
        
        if (is_word_char(c) && buf_pos < MAX_WORD_LEN - 1) {
            buffer[buf_pos++] = c;
            in_word = 1;
        } else if (in_word) {
            buffer[buf_pos] = '\0';
            if (buf_pos > 0) {
                int capitalized = isupper((unsigned char)buffer[0]) ? 1 : 0;
                add_word(proc, buffer, buf_pos, capitalized);
                proc->total_words++;
                proc->total_chars += buf_pos;
            }
            buf_pos = 0;
            in_word = 0;
        }
        
        if (c == '\0') break;
    }
}

void print_stats(const struct TextProcessor *proc) {
    printf("Text Statistics:\n");
    printf("Total words: %d\n", proc->total_words);
    printf("Total characters in words: %d\n", proc->total_chars);
    printf("Unique words: %d\n", proc->word_count);
    printf("Longest word length: %d\n\n", proc->longest_word_len);
    
    printf("Word frequency:\n");
    for (int i = 0; i < proc->word_count; i++) {
        printf("%-20s: count=%d, length=%d, %s\n",
               proc->words[i].word,
               proc->words[i].count,
               proc->words[i].length,
               proc->words[i].is_capitalized ? "capitalized" : "lowercase");
    }
}

int main() {
    struct TextProcessor processor;
    init_processor(&processor);
    
    char input_text[MAX_TEXT_LEN];
    printf("Enter text to analyze (max %d characters):\n", MAX_TEXT_LEN - 1);
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_text);
    if (input_len > 0 && input_text[input_len - 1] == '\n') {
        input_text[input_len - 1] = '\0';
    }
    
    if (strlen(input_text) == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    process_text(&processor, input_text);
    print_stats(&processor);
    
    return 0;
}