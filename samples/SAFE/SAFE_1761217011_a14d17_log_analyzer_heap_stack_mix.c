//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
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

int is_valid_timestamp(const char* timestamp) {
    if (strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int read_log_file(const char* filename, struct LogEntry* entries, int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) return -1;
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) && count < max_entries) {
        if (strlen(line) >= sizeof(line) - 1) continue;
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed != 3) continue;
        
        if (!is_valid_timestamp(timestamp)) continue;
        
        int severity = parse_log_level(level);
        if (severity == -1) continue;
        
        strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp) - 1);
        strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
        strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
        entries[count].severity = severity;
        count++;
    }
    
    fclose(file);
    return count;
}

void analyze_logs(struct LogEntry* entries, int count) {
    if (count <= 0) return;
    
    int severity_count[5] = {0};
    char* severity_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 0 && entries[i].severity < 5) {
            severity_count[entries[i].severity]++;
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("Breakdown by severity:\n");
    for (int i = 0; i < 5; i++) {
        printf("  %s: %d (%.1f%%)\n", severity_names[i], severity_count[i], 
               count > 0 ? (severity_count[i] * 100.0 / count) : 0.0);
    }
    
    int error_critical = severity_count[3] + severity_count[4];
    if (error_critical > 0) {
        printf("\nHigh severity entries (ERROR/CRITICAL): %d\n", error_critical);
        printf("Recent high severity entries:\n");
        int printed = 0;
        for (int i = count - 1; i >= 0 && printed < 3; i--) {
            if (entries[i].severity >= 3) {
                printf("  [%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
                printed++;
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    struct LogEntry* entries = malloc(MAX_ENTRIES * sizeof(struct LogEntry));
    if (!entries) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    int entry_count = read_log_file(argv[1], entries, MAX_ENTRIES);
    if (entry_count < 0) {
        printf("Error reading log file: %s\n", argv[1]);
        free(entries);
        return 1;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found\n");
        free(entries);
        return 0;
    }
    
    analyze_logs(entries, entry_count);
    free(entries);
    
    return 0;
}