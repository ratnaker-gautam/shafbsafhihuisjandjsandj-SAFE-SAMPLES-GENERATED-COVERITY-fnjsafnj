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
    if (!entries || !level || count <= 0) return 0;
    
    int level_count = 0;
    int i = 0;
    
    do {
        if (strcmp(entries[i].level, level) == 0) {
            level_count++;
        }
        i++;
    } while (i < count);
    
    return level_count;
}

void print_log_summary(const struct LogEntry* entries, int count) {
    if (!entries || count <= 0) return;
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int level_count = sizeof(levels) / sizeof(levels[0]);
    int j;
    
    for (j = 0; j < level_count; j++) {
        int count_val = count_log_level(entries, count, levels[j]);
        printf("%s: %d\n", levels[j], count_val);
    }
    
    printf("\nRecent entries:\n");
    int display_count = count > 5 ? 5 : count;
    int k = count - display_count;
    
    while (k < count) {
        printf("[%s] %s: %s\n", entries[k].timestamp, entries[k].level, entries[k].message);
        k++;
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    const char* sample_logs[] = {
        "2024-01-15 10:30:00|INFO|System started successfully",
        "2024-01-15 10:31:15|DEBUG|Initializing modules",
        "2024-01-15 10:32:30|WARN|High memory usage detected",
        "2024-01-15 10:33:45|INFO|User login: john_doe",
        "2024-01-15 10:35:00|ERROR|Database connection failed",
        "2024-01-15 10:36:15|INFO|Retrying connection",
        "2024-01-15 10:37:30|DEBUG|Connection parameters: timeout=30",
        "2024-01-15 10:38:45|INFO|Connection established",
        "2024-01-15 10:40:00|WARN|Slow query detected",
        "2024-01-15 10:41:15|INFO|Operation completed"
    };
    
    int log_count = sizeof(sample_logs) / sizeof(sample_logs[0]);
    int i;
    
    for (i = 0; i < log_count; i++) {
        if (entry_count >= MAX_ENTRIES) break;
        
        if (parse_log_line(sample_logs[i], &entries[entry_count])) {
            entry_count++;
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    print_log_summary(entries, entry_count);
    
    printf("\nSearching for ERROR entries:\n");
    int found_errors = 0;
    int idx;
    
    for (idx = 0; idx < entry_count; idx++) {
        if (strcmp(entries[idx].level, "ERROR") == 0) {
            printf("Found error: %s - %s\n", entries[idx].timestamp, entries[idx].message);
            found_errors++;
        }
    }
    
    if (found_errors == 0) {
        printf("No ERROR entries found.\n");
    }
    
    return 0;
}