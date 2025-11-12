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
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LEN];
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
    char* parts[3];
    int part_count = 0;
    char* token = strtok(temp, "|");
    
    while (token != NULL && part_count < 3) {
        parts[part_count++] = token;
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

void print_entries_with_level(struct LogEntry* entries, int count, const char* level) {
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, level) == 0) {
            printf("%s | %s | %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            found = 1;
        }
    }
    if (!found) {
        printf("No entries found for level: %s\n", level);
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
            printf("Invalid format. Use: timestamp|level|message\n");
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
    
    int j = 0;
    do {
        int count = count_entries_by_level(entries, entry_count, levels[j]);
        printf("%s: %d\n", levels[j], count);
        j++;
    } while (j < level_count);
    
    printf("\nEnter level to filter (INFO/WARN/ERROR): ");
    char filter_level[16];
    if (fgets(filter_level, 16, stdin) != NULL) {
        size_t len = strlen(filter_level);
        if (len > 0 && filter_level[len - 1] == '\n') {
            filter_level[len - 1] = '\0';
        }
        
        int valid_level = 0;
        int k = 0;
        while (k < level_count) {
            if (strcmp(filter_level, levels[k]) == 0) {
                valid_level = 1;
                break;
            }
            k++;
        }
        
        if (valid_level) {
            printf("\nEntries with level %s:\n", filter_level);
            print_entries_with_level(entries, entry_count, filter_level);
        } else {
            printf("Invalid level specified.\n");
        }
    }
    
    return 0;
}