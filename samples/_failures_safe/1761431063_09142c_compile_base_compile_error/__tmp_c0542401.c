//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
};

int parse_log_level(const char* level) {
    if (strcmp(level, "DEBUG") == 0) return 0;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "CRITICAL") == 0) return 4;
    return -1;
}

int validate_timestamp(const char* timestamp) {
    if (strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 10) {
            if (timestamp[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int read_log_entries(struct LogEntry* entries, int max_entries) {
    int count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish.\n");
    
    while (count < max_entries) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        struct LogEntry entry;
        char level_buf[16];
        char message_buf[256];
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", 
                           entry.timestamp, level_buf, message_buf);
        
        if (parsed != 3) {
            printf("Invalid format. Skipping.\n");
            continue;
        }
        
        if (!validate_timestamp(entry.timestamp)) {
            printf("Invalid timestamp format. Skipping.\n");
            continue;
        }
        
        strncpy(entry.level, level_buf, sizeof(entry.level) - 1);
        entry.level[sizeof(entry.level) - 1] = '\0';
        
        strncpy(entry.message, message_buf, sizeof(entry.message) - 1);
        entry.message[sizeof(entry.message) - 1] = '\0';
        
        entry.severity = parse_log_level(entry.level);
        if (entry.severity == -1) {
            printf("Unknown log level. Skipping.\n");
            continue;
        }
        
        entries[count] = entry;
        count++;
    }
    
    return count;
}

void analyze_logs(struct LogEntry* entries, int count) {
    if (count == 0) {
        printf("No log entries to analyze.\n");
        return;
    }
    
    int level_counts[5] = {0};
    char* level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 0 && entries[i].severity < 5) {
            level_counts[entries[i].severity]++;
        }
    }
    
    printf("\nLog Analysis Summary:\n");
    printf("=====================\n");
    printf("Total entries: %d\n", count);
    
    for (int i = 0; i < 5; i++) {
        printf("%-8s: %d (%.1f%%)\n", 
               level_names[i], 
               level_counts[i],
               count > 0 ? (level_counts[i] * 100.0) / count : 0.0);
    }
    
    int error_count = level_counts[3] + level_counts[4];
    if (error_count > 0) {
        printf("\nError entries (ERROR + CRITICAL): %d\n", error_count);
        printf("Error entries:\n");
        for (int i = 0; i < count; i++) {
            if (entries[i].severity >= 3) {
                printf("  %s %s: %s\n", 
                       entries[i].timestamp, 
                       entries[i].level, 
                       entries[i].message);
            }
        }
    }
}

int main(void) {
    struct LogEntry* entries = malloc(MAX_ENTRIES * sizeof(struct LogEntry));
    if (entries == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    analyze_logs(entries, count);