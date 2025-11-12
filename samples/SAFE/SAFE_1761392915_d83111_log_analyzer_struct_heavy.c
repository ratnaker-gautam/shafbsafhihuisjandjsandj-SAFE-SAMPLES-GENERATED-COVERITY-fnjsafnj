//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000
#define MAX_LEVEL_LENGTH 10

struct LogEntry {
    char timestamp[20];
    char level[MAX_LEVEL_LENGTH];
    char message[256];
    int severity;
};

struct LogStats {
    int total_entries;
    int error_count;
    int warning_count;
    int info_count;
    char most_frequent_level[MAX_LEVEL_LENGTH];
    int max_severity;
};

int parse_timestamp(const char* str, struct tm* tm) {
    return sscanf(str, "%d-%d-%d %d:%d:%d", 
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int validate_level(const char* level) {
    if (strlen(level) >= MAX_LEVEL_LENGTH) return 0;
    for (int i = 0; level[i]; i++) {
        if (!isalpha(level[i])) return 0;
    }
    return 1;
}

int calculate_severity(const char* level) {
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    return 0;
}

int read_log_entries(struct LogEntry entries[], int max_count) {
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), stdin) != NULL && count < max_count) {
        if (strlen(line) >= MAX_LINE_LENGTH - 1) continue;
        
        struct LogEntry entry;
        struct tm time_struct;
        
        int parsed = sscanf(line, "%19s %9s %255[^\n]", 
                           entry.timestamp, entry.level, entry.message);
        
        if (parsed != 3) continue;
        if (!parse_timestamp(entry.timestamp, &time_struct)) continue;
        if (!validate_level(entry.level)) continue;
        
        entry.severity = calculate_severity(entry.level);
        if (entry.severity == 0) continue;
        
        entries[count] = entry;
        count++;
    }
    
    return count;
}

void analyze_logs(const struct LogEntry entries[], int count, struct LogStats* stats) {
    stats->total_entries = count;
    stats->error_count = 0;
    stats->warning_count = 0;
    stats->info_count = 0;
    stats->max_severity = 0;
    
    int level_counts[3] = {0};
    const char* level_names[] = {"INFO", "WARNING", "ERROR"};
    
    for (int i = 0; i < count; i++) {
        switch (entries[i].severity) {
            case 1: stats->info_count++; level_counts[0]++; break;
            case 2: stats->warning_count++; level_counts[1]++; break;
            case 3: stats->error_count++; level_counts[2]++; break;
        }
        
        if (entries[i].severity > stats->max_severity) {
            stats->max_severity = entries[i].severity;
        }
    }
    
    int max_count = 0;
    int max_index = 0;
    for (int i = 0; i < 3; i++) {
        if (level_counts[i] > max_count) {
            max_count = level_counts[i];
            max_index = i;
        }
    }
    
    strncpy(stats->most_frequent_level, level_names[max_index], MAX_LEVEL_LENGTH - 1);
    stats->most_frequent_level[MAX_LEVEL_LENGTH - 1] = '\0';
}

void print_stats(const struct LogStats* stats) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", stats->total_entries);
    printf("INFO entries: %d\n", stats->info_count);
    printf("WARNING entries: %d\n", stats->warning_count);
    printf("ERROR entries: %d\n", stats->error_count);
    printf("Most frequent level: %s\n", stats->most_frequent_level);
    printf("Highest severity: %d\n", stats->max_severity);
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL MESSAGE):\n");
    printf("Press Ctrl+D to finish input.\n");
    
    int entry_count = read_log_entries(entries, MAX_ENTRIES);
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    analyze_logs(entries, entry_count, &stats);
    print_stats(&stats);
    
    return 0;
}