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

int count_log_level(const struct LogEntry* entries, int count, const char* level) {
    if (!entries || !level || count <= 0) return 0;
    
    int level_count = 0;
    int i = 0;
    while (i < count) {
        if (strcmp(entries[i].level, level) == 0) {
            level_count++;
        }
        i++;
    }
    return level_count;
}

void print_log_summary(const struct LogEntry* entries, int count) {
    if (!entries || count <= 0) return;
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int num_levels = sizeof(levels) / sizeof(levels[0]);
    
    int j;
    for (j = 0; j < num_levels; j++) {
        int count_level = count_log_level(entries, count, levels[j]);
        printf("%s: %d\n", levels[j], count_level);
    }
    
    printf("\nRecent entries:\n");
    int display_count = (count > 5) ? 5 : count;
    int k = count - display_count;
    do {
        if (k >= 0 && k < count) {
            printf("%s [%s] %s\n", entries[k].timestamp, entries[k].level, entries[k].message);
        }
        k++;
    } while (k < count);
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (format: timestamp level message):\n");
    printf("Type 'END' on a separate line to finish input.\n");
    
    char line[MAX_LINE_LEN];
    int line_number = 0;
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) {
            break;
        }
        
        line_number++;
        
        if (strlen(line) > 0 && line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        if (strlen(line) >= MAX_LINE_LEN - 1) {
            printf("Line %d too long, skipping\n", line_number);
            continue;
        }
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
        } else {
            printf("Failed to parse line %d: %s\n", line_number, line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    print_log_summary(entries, entry_count);
    
    return 0;
}