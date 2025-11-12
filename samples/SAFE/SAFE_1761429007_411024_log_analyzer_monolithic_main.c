//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH - 30];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (strlen(line) < 20) return 0;
    
    int pos = 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (line[i] != '-') return 0;
        } else if (i == 10) {
            if (line[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (line[i] != ':') return 0;
        } else {
            if (!isdigit(line[i])) return 0;
        }
        entry->timestamp[pos++] = line[i];
    }
    entry->timestamp[pos] = '\0';
    
    if (line[19] != ' ') return 0;
    
    int level_start = 20;
    int level_end = level_start;
    while (level_end < strlen(line) && line[level_end] != ' ' && level_end - level_start < 9) {
        entry->level[level_end - level_start] = line[level_end];
        level_end++;
    }
    entry->level[level_end - level_start] = '\0';
    
    if (strcmp(entry->level, "INFO") != 0 && 
        strcmp(entry->level, "WARN") != 0 && 
        strcmp(entry->level, "ERROR") != 0) {
        return 0;
    }
    
    if (level_end >= strlen(line) || line[level_end] != ' ') return 0;
    
    int msg_start = level_end + 1;
    if (msg_start >= strlen(line)) return 0;
    
    int msg_len = strlen(line) - msg_start;
    if (msg_len >= MAX_LINE_LENGTH - 30) return 0;
    
    strncpy(entry->message, line + msg_start, msg_len);
    entry->message[msg_len] = '\0';
    
    return 1;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    int info_count = 0, warn_count = 0, error_count = 0;
    
    char line[MAX_LINE_LENGTH];
    
    printf("Log Analyzer - Enter log lines (empty line to finish):\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strlen(line) == 0) break;
        
        if (parse_log_line(line, &entries[entry_count])) {
            if (strcmp(entries[entry_count].level, "INFO") == 0) info_count++;
            else if (strcmp(entries[entry_count].level, "WARN") == 0) warn_count++;
            else if (strcmp(entries[entry_count].level, "ERROR") == 0) error_count++;
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    printf("INFO: %d\n", info_count);
    printf("WARN: %d\n", warn_count);
    printf("ERROR: %d\n", error_count);
    
    if (entry_count > 0) {
        printf("\nRecent entries:\n");
        int display_count = entry_count > 5 ? 5 : entry_count;
        for (int i = entry_count - display_count; i < entry_count; i++) {
            printf("[%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
    
    return 0;
}