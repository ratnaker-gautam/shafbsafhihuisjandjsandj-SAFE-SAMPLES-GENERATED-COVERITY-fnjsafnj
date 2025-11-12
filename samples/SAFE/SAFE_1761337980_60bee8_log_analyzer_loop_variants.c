//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char level[16];
    char timestamp[32];
    char message[256];
    
    int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (parsed != 3) return 0;
    
    if (strlen(timestamp) > 31 || strlen(level) > 15 || strlen(message) > 255) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

int count_entries_by_level(struct LogEntry* entries, int count, const char* level) {
    if (!entries || !level || count <= 0) return 0;
    
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

void print_entries_by_level(struct LogEntry* entries, int count, const char* level) {
    if (!entries || !level || count <= 0) return;
    
    printf("Entries with level '%s':\n", level);
    int found = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No entries found for level '%s'\n", level);
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int level_count = sizeof(levels) / sizeof(levels[0]);
    
    int j = 0;
    do {
        int count = count_entries_by_level(entries, entry_count, levels[j]);
        printf("%s: %d entries\n", levels[j], count);
        j++;
    } while (j < level_count);
    
    printf("\n");
    
    char search_level[16];
    printf("Enter level to view details (INFO, WARN, ERROR, DEBUG): ");
    if (fgets(search_level, sizeof(search_level), stdin)) {
        search_level[strcspn(search_level, "\n")] = 0;
        
        int valid_level = 0;
        for (int k = 0; k < level_count; k++) {
            if (strcmp(search_level, levels[k]) == 0) {
                valid_level = 1;
                break;
            }
        }
        
        if (valid_level) {
            print_entries_by_level(entries, entry_count, search_level);
        } else {
            printf("Invalid level: %s\n", search_level);
        }
    }
    
    return 0;
}