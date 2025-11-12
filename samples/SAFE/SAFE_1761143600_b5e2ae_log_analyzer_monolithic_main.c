//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: log_analyzer
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
    char message[MAX_LINE_LEN];
};

int parse_timestamp(const char* ts_str, struct tm* tm) {
    return sscanf(ts_str, "%d-%d-%d %d:%d:%d",
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int is_valid_level(const char* level) {
    const char* valid_levels[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    for (int i = 0; i < 5; i++) {
        if (strcmp(level, valid_levels[i]) == 0) return 1;
    }
    return 0;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Log Analyzer - Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\\n");
    printf("Enter 'END' on a separate line to finish input\\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\0')) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        struct tm tm;
        int parsed = sscanf(line, "%31s %15s %1023[^\n]",
                           entry.timestamp, entry.level, entry.message);
        
        if (parsed != 3) {
            printf("Invalid format. Skipping line.\\n");
            continue;
        }
        
        if (!parse_timestamp(entry.timestamp, &tm)) {
            printf("Invalid timestamp format. Skipping line.\\n");
            continue;
        }
        
        if (!is_valid_level(entry.level)) {
            printf("Invalid log level. Skipping line.\\n");
            continue;
        }
        
        if (strlen(entry.message) >= MAX_LINE_LEN - 1) {
            printf("Message too long. Truncating.\\n");
            entry.message[MAX_LINE_LEN - 1] = '\0';
        }
        
        entries[entry_count] = entry;
        entry_count++;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\\n");
        return 0;
    }
    
    printf("\\n=== Log Analysis Results ===\\n");
    printf("Total entries: %d\\n", entry_count);
    
    int level_counts[5] = {0};
    const char* levels[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    
    for (int i = 0; i < entry_count; i++) {
        for (int j = 0; j < 5; j++) {
            if (strcmp(entries[i].level, levels[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
    }
    
    printf("\\nLevel distribution:\\n");
    for (int i = 0; i < 5; i++) {
        printf("%-8s: %d (%.1f%%)\\n", levels[i], level_counts[i],
               entry_count > 0 ? (level_counts[i] * 100.0) / entry_count : 0.0);
    }
    
    int error_count = level_counts[3] + level_counts[4];
    printf("\\nTotal errors (ERROR + CRITICAL): %d\\n", error_count);
    
    if (error_count > 0) {
        printf("\\nError entries:\\n");
        for (int i = 0; i < entry_count; i++) {
            if (strcmp(entries[i].level, "ERROR") == 0 || 
                strcmp(entries[i].level, "CRITICAL") == 0) {
                printf("[%s] %s: %s\\n", entries[i].timestamp, 
                       entries[i].level, entries[i].message);
            }
        }
    }
    
    return 0;
}