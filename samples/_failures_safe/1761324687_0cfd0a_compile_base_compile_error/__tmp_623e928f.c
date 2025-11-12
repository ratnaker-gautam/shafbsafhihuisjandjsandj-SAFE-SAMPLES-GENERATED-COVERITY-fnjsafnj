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
    return difftime(t1, t2) > 0 ? 1 : (difftime(t1, t2) < 0 ? -1 : 0);
}

void update_stats(struct LogStats* stats, const struct LogEntry* entry) {
    stats->total_entries++;
    if (strcmp(entry->level, "ERROR") == 0) stats->error_count++;
    else if (strcmp(entry->level, "WARNING") == 0) stats->warning_count++;
    else if (strcmp(entry->level, "INFO") == 0) stats->info_count++;
    
    if (stats->total_entries == 1) {
        strncpy(stats->earliest, entry->timestamp, sizeof(stats->earliest)-1);
        strncpy(stats->latest, entry->timestamp, sizeof(stats->latest)-1);
        stats->earliest[sizeof(stats->earliest)-1] = '\0';
        stats->latest[sizeof(stats->latest)-1] = '\0';
    } else {
        if (compare_timestamps(entry->timestamp, stats->earliest) < 0) {
            strncpy(stats->earliest, entry->timestamp, sizeof(stats->earliest)-1);
            stats->earliest[sizeof(stats->earliest)-1] = '\0';
        }
        if (compare_timestamps(entry->timestamp, stats->latest) > 0) {
            strncpy(stats->latest, entry->timestamp, sizeof(stats->latest)-1);
            stats->latest[sizeof(stats->latest)-1] = '\0';
        }
    }
}

int parse_log_line(const char* line, struct LogEntry* entry) {
    char level[16];
    char timestamp[32];
    char message[256];
    
    if (sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message) != 3) {
        return 0;
    }
    
    for (char* p = level; *p; p++) *p = toupper(*p);
    
    if (strcmp(level, "ERROR") != 0 && strcmp(level, "WARNING") != 0 && strcmp(level, "INFO") != 0) {
        return 0;
    }
    
    struct tm tm = {0};
    if (!parse_timestamp(timestamp, &tm)) {
        return 0;
    }
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp)-1);
    strncpy(entry->level, level, sizeof(entry->level)-1);
    strncpy(entry->message, message, sizeof(entry->message)-1);
    entry->timestamp[sizeof(entry->timestamp)-1] = '\0';
    entry->level[sizeof(entry->level)-1] = '\0';
    entry->message[sizeof(entry->message)-1] = '\0';
    
    if (strcmp(level, "ERROR") == 0) entry->severity = 3;
    else if (strcmp(level, "WARNING") == 0) entry->severity = 2;
    else entry->severity = 1;
    
    return 1;
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
    struct LogStats stats = {0};
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish input\n");