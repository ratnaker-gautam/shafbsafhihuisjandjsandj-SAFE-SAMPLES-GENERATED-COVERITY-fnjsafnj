//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
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

int count_entries_by_level(struct LogEntry* entries, int count, const char* level) {
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

void print_level_summary(struct LogEntry* entries, int count) {
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int level_counts[4] = {0};
    
    int i;
    for (i = 0; i < 4; i++) {
        int j = 0;
        do {
            if (strcmp(entries[j].level, levels[i]) == 0) {
                level_counts[i]++;
            }
            j++;
        } while (j < count);
    }
    
    printf("Log Level Summary:\n");
    int k;
    for (k = 0; k < 4; k++) {
        printf("%s: %d\n", levels[k], level_counts[k]);
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (timestamp level message). Enter 'END' to finish:\n");
    
    char line[MAX_LINE_LEN];
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
        } else {
            printf("Invalid log format. Use: YYYY-MM-DD HH:MM:SS LEVEL message\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    printf("\nAnalyzing %d log entries...\n\n", entry_count);
    
    print_level_summary(entries, entry_count);
    
    printf("\nSearch for entries by level: ");
    char search_level[16];
    if (fgets(search_level, sizeof(search_level), stdin) != NULL) {
        search_level[strcspn(search_level, "\n")] = 0;
        
        int found_count = 0;
        int idx = 0;
        while (idx < entry_count) {
            if (strcmp(entries[idx].level, search_level) == 0) {
                if (found_count == 0) {
                    printf("\nEntries with level '%s':\n", search_level);
                }
                printf("%s %s %s\n", entries[idx].timestamp, entries[idx].level, entries[idx].message);
                found_count++;
            }
            idx++;
        }
        
        if (found_count == 0) {
            printf("No entries found with level '%s'\n", search_level);
        } else {
            printf("Total: %d entries\n", found_count);
        }
    }
    
    return 0;
}