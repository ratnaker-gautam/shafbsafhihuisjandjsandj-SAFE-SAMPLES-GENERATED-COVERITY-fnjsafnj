//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
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

struct LogStats {
    int total_entries;
    int error_count;
    int warning_count;
    int info_count;
    char most_frequent_level[16];
    int max_severity;
};

int parse_severity(const char* level) {
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
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

int read_log_entries(struct LogEntry* entries, int max_entries) {
    int count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish.\n");
    
    while (count < max_entries && fgets(line, sizeof(line), stdin) != NULL) {
        if (strncmp(line, "END", 3) == 0) break;
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed != 3) {
            printf("Invalid format. Skipping line.\n");
            continue;
        }
        
        if (!validate_timestamp(timestamp)) {
            printf("Invalid timestamp format. Skipping line.\n");
            continue;
        }
        
        int severity = parse_severity(level);
        if (severity == 0) {
            printf("Unknown log level. Skipping line.\n");
            continue;
        }
        
        strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp) - 1);
        strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
        strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
        entries[count].severity = severity;
        count++;
    }
    
    return count;
}

void analyze_logs(const struct LogEntry* entries, int count, struct LogStats* stats) {
    if (count <= 0 || entries == NULL || stats == NULL) return;
    
    memset(stats, 0, sizeof(struct LogStats));
    stats->total_entries = count;
    stats->max_severity = -1;
    
    int error_count = 0, warning_count = 0, info_count = 0;
    
    for (int i = 0; i < count; i++) {
        switch (entries[i].severity) {
            case 3: error_count++; break;
            case 2: warning_count++; break;
            case 1: info_count++; break;
        }
        
        if (entries[i].severity > stats->max_severity) {
            stats->max_severity = entries[i].severity;
        }
    }
    
    stats->error_count = error_count;
    stats->warning_count = warning_count;
    stats->info_count = info_count;
    
    if (error_count >= warning_count && error_count >= info_count) {
        strcpy(stats->most_frequent_level, "ERROR");
    } else if (warning_count >= error_count && warning_count >= info_count) {
        strcpy(stats->most_frequent_level, "WARNING");
    } else {
        strcpy(stats->most_frequent_level, "INFO");
    }
}

void display_stats(const struct LogStats* stats) {
    if (stats == NULL) return;
    
    printf("\n=== Log Analysis Results ===\n");
    printf("Total entries: %d\n", stats->total_entries);
    printf("ERROR entries: %d\n", stats->error_count);
    printf("WARNING entries: %d\n", stats->warning_count);
    printf("INFO entries: %d\n", stats->info_count);
    printf("Most frequent level: %s\n", stats->most_frequent_level);
    
    const char* max_severity_str = "UNKNOWN";
    switch (stats->max_severity) {
        case 3: max_severity_str = "ERROR"; break;
        case 2: max_severity_str = "WARNING"; break;
        case 1: max_severity_str = "INFO"; break;
        default