//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 10000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
} LogEntry;

int parse_log_level(const char* level) {
    if (strcmp(level, "DEBUG") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 2;
    if (strcmp(level, "WARNING") == 0) return 3;
    if (strcmp(level, "ERROR") == 0) return 4;
    if (strcmp(level, "CRITICAL") == 0) return 5;
    return 0;
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

void analyze_log_patterns(LogEntry* entries, int count) {
    int level_counts[6] = {0};
    int hour_distribution[24] = {0};
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity > 0 && entries[i].severity < 6) {
            level_counts[entries[i].severity]++;
        }
        
        if (strlen(entries[i].timestamp) >= 13) {
            int hour = (entries[i].timestamp[11] - '0') * 10 + 
                      (entries[i].timestamp[12] - '0');
            if (hour >= 0 && hour < 24) {
                hour_distribution[hour]++;
            }
        }
    }
    
    printf("Log Level Distribution:\n");
    const char* levels[] = {"", "DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    for (int i = 1; i <= 5; i++) {
        printf("%s: %d\n", levels[i], level_counts[i]);
    }
    
    printf("\nHourly Distribution:\n");
    for (int i = 0; i < 24; i++) {
        printf("%02d:00 - %d logs\n", i, hour_distribution[i]);
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL Message):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\0')) {
            break;
        }
        
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) == 0) continue;
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        
        if (parsed < 3) {
            printf("Invalid format. Skipping line: %s\n", line);
            continue;
        }
        
        if (!validate_timestamp(timestamp)) {
            printf("Invalid timestamp format. Skipping: %s\n", timestamp);
            continue;
        }
        
        int severity = parse_log_level(level);
        if (severity == 0) {
            printf("Unknown log level. Skipping: %s\n", level);
            continue;
        }
        
        if (strlen(message) == 0) {
            printf("Empty message. Skipping.\n");
            continue;
        }
        
        strncpy(entries[entry_count].timestamp, timestamp, sizeof(entries[entry_count].timestamp) - 1);
        strncpy(entries[entry_count].level, level, sizeof(entries[entry_count].level) - 1);
        strncpy(entries[entry_count].message, message, sizeof(entries[entry_count].message) - 1);
        entries[entry_count].severity = severity;
        entry_count++;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    printf("\nAnalyzing %d log entries...\n\n", entry_count);
    analyze_log_patterns(entries, entry_count);
    
    int error_count = 0;
    int warning_count = 0;
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].severity >= 4