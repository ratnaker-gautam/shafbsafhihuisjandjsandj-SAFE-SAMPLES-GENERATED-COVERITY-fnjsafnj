//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[MAX_LINE_LEN - 48];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    size_t len = strlen(line);
    if (len < 20 || len >= MAX_LINE_LEN) return 0;
    
    int scanned = sscanf(line, "%31s %15s", entry->timestamp, entry->level);
    if (scanned != 2) return 0;
    
    const char* msg_start = strchr(line, ']');
    if (msg_start == NULL) return 0;
    msg_start += 2;
    
    if (msg_start >= line + len) return 0;
    
    size_t msg_len = len - (msg_start - line);
    if (msg_len >= sizeof(entry->message)) return 0;
    
    strncpy(entry->message, msg_start, msg_len);
    entry->message[msg_len] = '\0';
    
    return 1;
}

int is_valid_level(const char* level) {
    const char* valid_levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    for (int i = 0; i < 4; i++) {
        if (strcmp(level, valid_levels[i]) == 0) return 1;
    }
    return 0;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Log Analyzer - Enter log lines (format: [TIMESTAMP] LEVEL Message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (is_valid_level(entry.level)) {
                entries[entry_count] = entry;
                entry_count++;
                printf("Added: %s %s\n", entry.timestamp, entry.level);
            } else {
                printf("Invalid log level: %s\n", entry.level);
            }
        } else {
            printf("Invalid log format\n");
        }
    }
    
    printf("\nAnalysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    
    int level_counts[4] = {0};
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    
    for (int i = 0; i < entry_count; i++) {
        for (int j = 0; j < 4; j++) {
            if (strcmp(entries[i].level, levels[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
    }
    
    printf("Level distribution:\n");
    for (int i = 0; i < 4; i++) {
        printf("%s: %d\n", levels[i], level_counts[i]);
    }
    
    if (entry_count > 0) {
        printf("\nRecent entries:\n");
        int show_count = entry_count > 5 ? 5 : entry_count;
        for (int i = entry_count - show_count; i < entry_count; i++) {
            printf("%s %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
    
    return 0;
}