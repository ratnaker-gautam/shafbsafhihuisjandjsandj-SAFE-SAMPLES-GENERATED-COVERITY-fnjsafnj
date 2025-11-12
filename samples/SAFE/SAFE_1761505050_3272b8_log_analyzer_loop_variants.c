//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 100

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LEN - 30];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN - 1) return 0;
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    int i = 0;
    while (temp[i] != '\0' && i < 19) {
        if (temp[i] == ' ') break;
        entry->timestamp[i] = temp[i];
        i++;
    }
    entry->timestamp[i] = '\0';
    
    while (temp[i] == ' ' && temp[i] != '\0') i++;
    
    int j = 0;
    while (temp[i] != '\0' && temp[i] != ' ' && j < 9) {
        entry->level[j] = temp[i];
        i++;
        j++;
    }
    entry->level[j] = '\0';
    
    while (temp[i] == ' ' && temp[i] != '\0') i++;
    
    j = 0;
    while (temp[i] != '\0' && j < MAX_LINE_LEN - 31) {
        entry->message[j] = temp[i];
        i++;
        j++;
    }
    entry->message[j] = '\0';
    
    return 1;
}

int count_log_level(const struct LogEntry* entries, int count, const char* level) {
    int total = 0;
    int idx = 0;
    do {
        if (strcmp(entries[idx].level, level) == 0) {
            total++;
        }
        idx++;
    } while (idx < count);
    return total;
}

void print_entries_by_level(const struct LogEntry* entries, int count, const char* level) {
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            found = 1;
        }
    }
    if (!found) {
        printf("No %s entries found.\n", level);
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char input[MAX_LINE_LEN];
    
    printf("Enter log entries (empty line to finish):\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(input, MAX_LINE_LEN, stdin) == NULL) break;
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (input[0] == '\0') break;
        
        if (parse_log_line(input, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Use: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    
    const char* levels[] = {"INFO", "WARN", "ERROR"};
    int level_count = sizeof(levels) / sizeof(levels[0]);
    
    int k = 0;
    while (k < level_count) {
        int count = count_log_level(entries, entry_count, levels[k]);
        printf("%s: %d\n", levels[k], count);
        k++;
    }
    
    printf("\nEnter level to filter (INFO/WARN/ERROR): ");
    if (fgets(input, MAX_LINE_LEN, stdin) != NULL) {
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        int valid_level = 0;
        for (int m = 0; m < level_count; m++) {
            if (strcmp(input, levels[m]) == 0) {
                valid_level = 1;
                break;
            }
        }
        
        if (valid_level) {
            printf("\nEntries with level %s:\n", input);
            print_entries_by_level(entries, entry_count, input);
        } else {
            printf("Invalid level specified.\n");
        }
    }
    
    return 0;
}