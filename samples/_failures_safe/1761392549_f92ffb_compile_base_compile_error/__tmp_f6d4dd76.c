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
    char earliest[32];
    char latest[32];
};

int parse_timestamp(const char* ts, struct tm* tm) {
    return sscanf(ts, "%d-%d-%d %d:%d:%d", 
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int compare_timestamps(const char* ts1, const char* ts2) {
    struct tm tm1 = {0}, tm2 = {0};
    if (!parse_timestamp(ts1, &tm1) || !parse_timestamp(ts2, &tm2)) {
        return 0;
    }
    time_t t1 = mktime(&tm1);
    time_t t2 = mktime(&tm2);
    if (t1 == -1 || t2 == -1) return 0;
    return (t1 < t2) ? -1 : (t1 > t2) ? 1 : 0;
}

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    
    char level[16];
    char timestamp[32];
    char message[256];
    
    int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (parsed != 3) return 0;
    
    struct tm tm = {0};
    if (!parse_timestamp(timestamp, &tm)) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    if (strcmp(level, "ERROR") == 0) entry->severity = 3;
    else if (strcmp(level, "WARNING") == 0) entry->severity = 2;
    else if (strcmp(level, "INFO") == 0) entry->severity = 1;
    else return 0;
    
    return 1;
}

void analyze_logs(struct LogEntry* entries, int count, struct LogStats* stats) {
    stats->total_entries = count;
    stats->error_count = 0;
    stats->warning_count = 0;
    stats->info_count = 0;
    
    if (count == 0) {
        stats->earliest[0] = '\0';
        stats->latest[0] = '\0';
        return;
    }
    
    strncpy(stats->earliest, entries[0].timestamp, sizeof(stats->earliest) - 1);
    stats->earliest[sizeof(stats->earliest) - 1] = '\0';
    strncpy(stats->latest, entries[0].timestamp, sizeof(stats->latest) - 1);
    stats->latest[sizeof(stats->latest) - 1] = '\0';
    
    for (int i = 0; i < count; i++) {
        switch (entries[i].severity) {
            case 3: stats->error_count++; break;
            case 2: stats->warning_count++; break;
            case 1: stats->info_count++; break;
        }
        
        if (compare_timestamps(entries[i].timestamp, stats->earliest) < 0) {
            strncpy(stats->earliest, entries[i].timestamp, sizeof(stats->earliest) - 1);
            stats->earliest[sizeof(stats->earliest) - 1] = '\0';
        }
        if (compare_timestamps(entries[i].timestamp, stats->latest) > 0) {
            strncpy(stats->latest, entries[i].timestamp, sizeof(stats->latest) - 1);
            stats->latest[sizeof(stats->latest) - 1] = '\0';
        }
    }
}

void print_stats(const struct LogStats* stats) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", stats->total_entries);
    printf("Errors: %d\n", stats->error_count);
    printf("Warnings: %d\n", stats->warning_count);
    printf("Info: %d\n", stats->info_count);
    printf("Time range: %s to %s\n", stats->earliest, stats->latest);
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries