//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
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
    char message[256];
    int severity;
};

struct LogStats {
    int total_entries;
    int error_count;
    int warning_count;
    int info_count;
    char most_frequent_level[10];
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
        if (strncmp(line, "END", 3) == 0 && (line[3] == '\n' || line[3] == '\0')) {
            break;
        }
        
        char timestamp[20], level[10], message[256];
        int parsed = sscanf(line, "%19s %9s %255[^\n]", timestamp, level, message);
        
        if (parsed == 3 && validate_timestamp(timestamp)) {
            struct LogEntry* entry = &entries[count];
            strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
            strncpy(entry->level, level, sizeof(entry->level) - 1);
            strncpy(entry->message, message, sizeof(entry->message) - 1);
            entry->severity = parse_severity(level);
            entries[count].timestamp[19] = '\0';
            entries[count].level[9] = '\0';
            entries[count].message[255] = '\0';
            count++;
        } else {
            printf("Invalid log format. Skipping.\n");
        }
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
        
        if (entry->severity >= 1 && entry->severity <= 3) {
            level_counts[entry->severity]++;
        }
        
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
        case 1: strcpy(stats->most_frequent_level, "INFO"); break;
        case 2: strcpy(stats->most_frequent_level, "WARN"); break;
        case 3: strcpy(stats->most_frequent_level, "ERROR"); break;
        default: strcpy(stats->most_frequent_level, "NONE"); break;
    }
}

void display_stats(const struct LogStats* stats) {
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", stats->total_entries);
    printf("INFO entries: %d\n", stats->info_count);
    printf("WARN entries: %d\n", stats->warning_count);
    printf("ERROR entries: %d\n", stats->error_count);
    printf("Most frequent level: %s\n", stats->most_frequent_level);
    printf("Highest severity: %d\n", stats->max_severity);
}

int main() {
    struct LogEntry entries[MAX