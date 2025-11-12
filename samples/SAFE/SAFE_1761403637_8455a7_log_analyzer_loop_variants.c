//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp_line, line);
    
    char* token = strtok(temp_line, " ");
    if (token == NULL) return 0;
    if (strlen(token) >= sizeof(entry->timestamp)) return 0;
    strcpy(entry->timestamp, token);
    
    token = strtok(NULL, " ");
    if (token == NULL) return 0;
    if (strlen(token) >= sizeof(entry->level)) return 0;
    strcpy(entry->level, token);
    
    token = strtok(NULL, "\n");
    if (token == NULL) return 0;
    if (strlen(token) >= sizeof(entry->message)) return 0;
    strcpy(entry->message, token);
    
    return 1;
}

int count_log_level(const struct LogEntry* entries, int count, const char* level) {
    int total = 0;
    int i = 0;
    while (i < count) {
        if (strcmp(entries[i].level, level) == 0) {
            total++;
        }
        i++;
    }
    return total;
}

void print_entries_by_level(const struct LogEntry* entries, int count, const char* level) {
    printf("Entries with level '%s':\n", level);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            found = 1;
        }
    }
    if (!found) {
        printf("No entries found.\n");
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Log Analyzer - Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    char line[MAX_LINE_LEN];
    int line_count = 0;
    
    do {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line_count++;
        if (line_count > MAX_ENTRIES) {
            printf("Maximum number of entries reached.\n");
            break;
        }
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
        
    } while (line_count < MAX_ENTRIES);
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 0;
    }
    
    printf("\nAnalysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int level_count = sizeof(levels) / sizeof(levels[0]);
    
    int i = 0;
    while (i < level_count) {
        int count = count_log_level(entries, entry_count, levels[i]);
        printf("%s: %d\n", levels[i], count);
        i++;
    }
    
    printf("\nEnter level to filter by (INFO/WARN/ERROR/DEBUG): ");
    char filter_level[16];
    if (fgets(filter_level, sizeof(filter_level), stdin) != NULL) {
        size_t len = strlen(filter_level);
        if (len > 0 && filter_level[len - 1] == '\n') {
            filter_level[len - 1] = '\0';
        }
        
        int valid_level = 0;
        for (int j = 0; j < level_count; j++) {
            if (strcmp(filter_level, levels[j]) == 0) {
                valid_level = 1;
                break;
            }
        }
        
        if (valid_level) {
            print_entries_by_level(entries, entry_count, filter_level);
        } else {
            printf("Invalid level specified.\n");
        }
    }
    
    return 0;
}