//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000
#define MAX_FILENAME 256

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
    uint32_t line_number;
};

struct LogStats {
    uint32_t total_entries;
    uint32_t error_count;
    uint32_t warning_count;
    uint32_t info_count;
    uint32_t debug_count;
    char earliest_timestamp[32];
    char latest_timestamp[32];
};

struct LogAnalyzer {
    struct LogEntry entries[MAX_ENTRIES];
    struct LogStats stats;
    uint32_t entry_count;
};

void initialize_analyzer(struct LogAnalyzer *analyzer) {
    if (!analyzer) return;
    memset(analyzer, 0, sizeof(struct LogAnalyzer));
    analyzer->stats.earliest_timestamp[0] = '\0';
    analyzer->stats.latest_timestamp[0] = '\0';
}

int parse_timestamp(const char *timestamp, struct tm *tm) {
    if (!timestamp || !tm) return 0;
    return strptime(timestamp, "%Y-%m-%d %H:%M:%S", tm) != NULL;
}

int compare_timestamps(const char *ts1, const char *ts2) {
    if (!ts1 || !ts2) return 0;
    struct tm tm1 = {0}, tm2 = {0};
    if (!parse_timestamp(ts1, &tm1) || !parse_timestamp(ts2, &tm2)) {
        return 0;
    }
    time_t time1 = mktime(&tm1);
    time_t time2 = mktime(&tm2);
    if (time1 == -1 || time2 == -1) return 0;
    return (time1 < time2) ? -1 : (time1 > time2) ? 1 : 0;
}

int parse_log_line(const char *line, struct LogEntry *entry) {
    if (!line || !entry) return 0;
    
    char level[16];
    char timestamp[32];
    char message[256];
    
    int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
    if (parsed != 3) return 0;
    
    struct tm tm = {0};
    if (!parse_timestamp(timestamp, &tm)) return 0;
    
    if (strlen(level) >= sizeof(entry->level) || 
        strlen(timestamp) >= sizeof(entry->timestamp) ||
        strlen(message) >= sizeof(entry->message)) {
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

int load_log_file(struct LogAnalyzer *analyzer, const char *filename) {
    if (!analyzer || !filename) return 0;
    
    FILE *file = fopen(filename, "r");
    if (!file) return 0;
    
    char line[MAX_LINE_LENGTH];
    uint32_t line_num = 0;
    
    while (fgets(line, sizeof(line), file) && analyzer->entry_count < MAX_ENTRIES) {
        line_num++;
        line[strcspn(line, "\n")] = '\0';
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entry.line_number = line_num;
            analyzer->entries[analyzer->entry_count] = entry;
            analyzer->entry_count++;
        }
    }
    
    fclose(file);
    return 1;
}

void analyze_logs(struct LogAnalyzer *analyzer) {
    if (!analyzer || analyzer->entry_count == 0) return;
    
    memset(&analyzer->stats, 0, sizeof(analyzer->stats));
    analyzer->stats.total_entries = analyzer->entry_count;
    
    strncpy(analyzer->stats.earliest_timestamp, 
            analyzer->entries[0].timestamp, 
            sizeof(analyzer->stats.earliest_timestamp) - 1);
    strncpy(analyzer->stats.latest_timestamp, 
            analyzer->entries[0].timestamp, 
            sizeof(analyzer->stats.latest_timestamp) - 1);
    
    analyzer->stats.earliest_timestamp[sizeof(analyzer->stats.earliest_timestamp) - 1] = '\0';
    analyzer->stats.latest_timestamp[sizeof(analyzer->stats.latest_timestamp) - 1] = '\0';
    
    for (uint32_t i = 0; i < analyzer->entry_count; i++) {
        const struct LogEntry *entry = &analyzer->entries[i];
        
        if (strcmp(entry->level, "ERROR") == 0) analyzer->stats.error_count++;
        else if (strcmp(entry