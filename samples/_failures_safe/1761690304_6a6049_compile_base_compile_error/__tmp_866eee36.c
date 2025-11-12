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
    char message[MAX_LINE_LENGTH - 30];
};

struct LogStats {
    int total_entries;
    int error_count;
    int warning_count;
    int info_count;
    char earliest[20];
    char latest[20];
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count;
    struct LogStats stats;
};

void init_analyzer(struct LogAnalyzer *analyzer) {
    analyzer->entry_count = 0;
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    memset(analyzer->stats.earliest, 0, sizeof(analyzer->stats.earliest));
    memset(analyzer->stats.latest, 0, sizeof(analyzer->stats.latest));
}

int parse_timestamp(const char *str, struct tm *tm) {
    return sscanf(str, "%4d-%2d-%2d %2d:%2d:%2d",
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int compare_timestamps(const char *ts1, const char *ts2) {
    struct tm tm1 = {0}, tm2 = {0};
    if (!parse_timestamp(ts1, &tm1) || !parse_timestamp(ts2, &tm2)) {
        return 0;
    }
    time_t t1 = mktime(&tm1);
    time_t t2 = mktime(&tm2);
    if (t1 == -1 || t2 == -1) return 0;
    return (t1 < t2) ? -1 : (t1 > t2) ? 1 : 0;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    
    char level[10];
    char timestamp[20];
    char message[MAX_LINE_LENGTH - 30];
    
    int result = sscanf(line, "%19s %9s %[^\n]", timestamp, level, message);
    if (result != 3) return 0;
    
    struct tm tm_temp;
    if (!parse_timestamp(timestamp, &tm_temp)) return 0;
    
    if (strcmp(level, "ERROR") != 0 && 
        strcmp(level, "WARNING") != 0 && 
        strcmp(level, "INFO") != 0) {
        return 0;
    }
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    entry->level[sizeof(entry->level) - 1] = '\0';
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void update_stats(struct LogAnalyzer *analyzer, const struct LogEntry *entry) {
    analyzer->stats.total_entries++;
    
    if (strcmp(entry->level, "ERROR") == 0) {
        analyzer->stats.error_count++;
    } else if (strcmp(entry->level, "WARNING") == 0) {
        analyzer->stats.warning_count++;
    } else if (strcmp(entry->level, "INFO") == 0) {
        analyzer->stats.info_count++;
    }
    
    if (analyzer->stats.total_entries == 1) {
        strncpy(analyzer->stats.earliest, entry->timestamp, sizeof(analyzer->stats.earliest) - 1);
        strncpy(analyzer->stats.latest, entry->timestamp, sizeof(analyzer->stats.latest) - 1);
    } else {
        if (compare_timestamps(entry->timestamp, analyzer->stats.earliest) < 0) {
            strncpy(analyzer->stats.earliest, entry->timestamp, sizeof(analyzer->stats.earliest) - 1);
        }
        if (compare_timestamps(entry->timestamp, analyzer->stats.latest) > 0) {
            strncpy(analyzer->stats.latest, entry->timestamp, sizeof(analyzer->stats.latest) - 1);
        }
    }
}

int load_log_file(struct LogAnalyzer *analyzer) {
    char filename[256];
    printf("Enter log filename: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) return 0;
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) return 0;
    
    FILE *file = fopen(filename, "