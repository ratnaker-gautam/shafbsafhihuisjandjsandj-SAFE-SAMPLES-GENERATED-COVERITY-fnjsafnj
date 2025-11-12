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
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LEN];
    strncpy(temp_line, line, MAX_LINE_LEN - 1);
    temp_line[MAX_LINE_LEN - 1] = '\0';
    
    char* token = strtok(temp_line, " ");
    if (!token) return 0;
    
    strncpy(entry->timestamp, token, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    token = strtok(NULL, " ");
    if (!token) return 0;
    
    strncpy(entry->level, token, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    token = strtok(NULL, "\n");
    if (!token) return 0;
    
    strncpy(entry->message, token, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

int count_level_occurrences(struct LogEntry* entries, int count, const char* level) {
    int occurrences = 0;
    int i = 0;
    
    while (i < count) {
        if (strcmp(entries[i].level, level) == 0) {
            occurrences++;
        }
        i++;
    }
    
    return occurrences;
}

void print_entries_by_level(struct LogEntry* entries, int count, const char* level) {
    printf("Entries with level '%s':\n", level);
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE). Enter 'END' to finish:\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) {
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
            printf("Invalid log format. Use: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    printf("\nLog Analysis Summary:\n");
    printf("Total entries: %d\n", entry_count);
    
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int level_count = sizeof(levels) / sizeof(levels[0]);
    
    int j = 0;
    do {
        int count = count_level_occurrences(entries, entry_count, levels[j]);
        printf("%s: %d\n", levels[j], count);
        j++;
    } while (j < level_count);
    
    printf("\n");
    
    char search_level[16];
    printf("Enter level to search for: ");
    if (fgets(search_level, sizeof(search_level), stdin)) {
        search_level[strcspn(search_level, "\n")] = '\0';
        
        int found = 0;
        for (int k = 0; k < level_count; k++) {
            if (strcmp(levels[k], search_level) == 0) {
                found = 1;
                break;
            }
        }
        
        if (found) {
            print_entries_by_level(entries, entry_count, search_level);
        } else {
            printf("Unknown level: %s\n", search_level);
        }
    }
    
    return 0;
}