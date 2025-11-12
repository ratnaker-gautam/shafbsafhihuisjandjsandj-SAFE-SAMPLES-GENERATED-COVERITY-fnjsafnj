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
    
    char* parts[3];
    char* token = strtok(temp_line, "|");
    int part_count = 0;
    
    while (token != NULL && part_count < 3) {
        parts[part_count] = token;
        part_count++;
        token = strtok(NULL, "|");
    }
    
    if (part_count != 3) return 0;
    
    strncpy(entry->timestamp, parts[0], 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, parts[1], 15);
    entry->level[15] = '\0';
    strncpy(entry->message, parts[2], 255);
    entry->message[255] = '\0';
    
    return 1;
}

int count_log_level(const struct LogEntry* entries, int count, const char* level) {
    int total = 0;
    int i = 0;
    do {
        if (strcmp(entries[i].level, level) == 0) {
            total++;
        }
        i++;
    } while (i < count);
    return total;
}

void print_entries_by_level(const struct LogEntry* entries, int count, const char* level) {
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            printf("%s | %s | %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            found = 1;
        }
    }
    if (!found) {
        printf("No %s level entries found.\n", level);
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: timestamp|level|message), empty line to finish:\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, MAX_LINE_LEN, stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strlen(line) == 0) break;
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Use: timestamp|level|message\n");
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
    
    int i = 0;
    while (i < level_count) {
        int count = count_log_level(entries, entry_count, levels[i]);
        printf("%s entries: %d\n", levels[i], count);
        i++;
    }
    
    printf("\nEnter level to filter (INFO/WARN/ERROR): ");
    char filter_level[16];
    if (fgets(filter_level, 16, stdin) != NULL) {
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
            printf("\nEntries with level %s:\n", filter_level);
            print_entries_by_level(entries, entry_count, filter_level);
        } else {
            printf("Invalid level specified.\n");
        }
    }
    
    return 0;
}