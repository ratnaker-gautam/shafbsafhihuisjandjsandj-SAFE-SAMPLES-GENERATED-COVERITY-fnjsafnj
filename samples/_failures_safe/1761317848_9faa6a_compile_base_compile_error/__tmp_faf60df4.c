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
    if (strcmp(level, "WARN") == 0) return 2;
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
        
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry entry;
        int scanned = sscanf(line, "%31s %15s %255[^\n]", 
                           entry.timestamp, entry.level, entry.message);
        
        if (scanned != 3) {
            printf("Invalid format. Skipping.\n");
            continue;
        }
        
        if (!validate_timestamp(entry.timestamp)) {
            printf("Invalid timestamp format. Skipping.\n");
            continue;
        }
        
        entry.severity = parse_severity(entry.level);
        if (entry.severity == 0) {
            printf("Unknown log level. Skipping.\n");
            continue;
        }
        
        entries[count] = entry;
        count++;
    }
    
    return count;
}

void analyze_logs(const struct LogEntry* entries, int count, struct LogStats* stats) {
    stats->total_entries = count;
    stats->error_count = 0;
    stats->warning_count = 0;
    stats->info_count = 0;
    stats->max_severity = 0;
    
    int level_counts[4] = {0};
    
    for (int i = 0; i < count; i++) {
        const struct LogEntry* entry = &entries[i];
        
        switch (entry->severity) {
            case 3: stats->error_count++; break;
            case 2: stats->warning_count++; break;
            case 1: stats->info_count++; break;
        }
        
        level_counts[entry->severity]++;
        
        if (entry->severity > stats->max_severity) {
            stats->max_severity = entry->severity;
        }
    }
    
    int max_count = 0;
    int max_level = 0;
    for (int i = 1; i <= 3; i++) {
        if (level_counts[i] > max_count) {
            max_count = level_counts[i];
            max_level = i;
        }
    }
    
    switch (max_level) {
        case 3: strcpy(stats->most_frequent_level, "ERROR"); break;
        case 2: strcpy(stats->most_frequent_level, "WARN"); break;
        case 1: strcpy(stats->most_frequent_level, "INFO"); break;
        default: strcpy(stats->most_frequent_level, "UNKNOWN"); break;
    }
}

void display_stats(const struct LogStats* stats) {
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", stats->total_entries);
    printf("ERROR entries: %d\n", stats->error_count);
    printf("WARN entries: %d\n", stats->warning_count);
    printf("INFO entries: %d\n", stats->info_count);
    printf("Most frequent level: %s\n", stats->most_frequent_level);
    
    const char* max_severity_str = "UNKNOWN";
    switch (stats->max_severity) {
        case 3: max_severity_str = "ERROR"; break;
        case 2: max_severity_str = "WARN"; break;
        case 1: max_severity_str = "INFO"; break;
    }
    printf("Highest severity: %s\n", max_severity_str);
}

int