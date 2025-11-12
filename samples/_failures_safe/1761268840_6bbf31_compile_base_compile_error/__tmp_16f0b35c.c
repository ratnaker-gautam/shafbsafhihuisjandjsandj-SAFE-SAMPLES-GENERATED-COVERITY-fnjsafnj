//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LENGTH];
    strncpy(temp, line, MAX_LINE_LENGTH - 1);
    temp[MAX_LINE_LENGTH - 1] = '\0';
    
    char* parts[3];
    char* token = strtok(temp, "|");
    int count = 0;
    
    while (token != NULL && count < 3) {
        parts[count] = token;
        count++;
        token = strtok(NULL, "|");
    }
    
    if (count != 3) return 0;
    
    strncpy(entry->timestamp, parts[0], 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, parts[1], 15);
    entry->level[15] = '\0';
    strncpy(entry->message, parts[2], 255);
    entry->message[255] = '\0';
    
    return 1;
}

int count_log_level(const LogEntry* entries, int count, const char* level) {
    int total = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            total++;
        }
    }
    return total;
}

void find_most_frequent_word(const LogEntry* entries, int count, char* result, int result_size) {
    if (entries == NULL || count <= 0 || result == NULL || result_size <= 0) {
        result[0] = '\0';
        return;
    }
    
    char words[MAX_ENTRIES][32];
    int frequencies[MAX_ENTRIES] = {0};
    int word_count = 0;
    
    for (int i = 0; i < count; i++) {
        char temp[256];
        strncpy(temp, entries[i].message, 255);
        temp[255] = '\0';
        
        char* token = strtok(temp, " \t\n\r");
        while (token != NULL) {
            if (strlen(token) > 2) {
                int found = 0;
                for (int j = 0; j < word_count; j++) {
                    if (strcmp(words[j], token) == 0) {
                        frequencies[j]++;
                        found = 1;
                        break;
                    }
                }
                if (!found && word_count < MAX_ENTRIES) {
                    strncpy(words[word_count], token, 31);
                    words[word_count][31] = '\0';
                    frequencies[word_count] = 1;
                    word_count++;
                }
            }
            token = strtok(NULL, " \t\n\r");
        }
    }
    
    int max_freq = 0;
    int max_index = -1;
    for (int i = 0; i < word_count; i++) {
        if (frequencies[i] > max_freq) {
            max_freq = frequencies[i];
            max_index = i;
        }
    }
    
    if (max_index >= 0) {
        strncpy(result, words[max_index], result_size - 1);
        result[result_size - 1] = '\0';
    } else {
        result[0] = '\0';
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (format: timestamp|level|message), one per line.\n");
    printf("Enter 'END' on a separate line to finish.\n");
    
    char line[MAX_LINE_LENGTH];
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, MAX_LINE_LENGTH, stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Skipping line.\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    clock_t start_time = clock();
    
    int info_count = count_log_level(entries, entry_count, "INFO");
    int warn_count = count_log_level(entries, entry_count, "WARN");
    int error_count = count_log_level(entries, entry_count, "ERROR");
    
    char most_frequent[32];
    find_most_frequent_word(entries, entry_count, most_frequent, 32);
    
    clock_t end_time = clock();
    double cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries processed: %d\n", entry_count);
    printf("INFO level entries: %d