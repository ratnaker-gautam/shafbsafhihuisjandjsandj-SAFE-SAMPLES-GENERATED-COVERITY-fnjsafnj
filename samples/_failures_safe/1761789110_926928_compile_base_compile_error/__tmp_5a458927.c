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

int is_valid_timestamp(const char* timestamp) {
    if (strlen(timestamp) != 19) return 0;
    if (timestamp[4] != '-' || timestamp[7] != '-' || timestamp[10] != ' ' ||
        timestamp[13] != ':' || timestamp[16] != ':') return 0;
    
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7 || i == 10 || i == 13 || i == 16) continue;
        if (!isdigit(timestamp[i])) return 0;
    }
    
    int year, month, day, hour, minute, second;
    if (sscanf(timestamp, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second) != 6)
        return 0;
    
    if (year < 2000 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    if (hour < 0 || hour > 23) return 0;
    if (minute < 0 || minute > 59) return 0;
    if (second < 0 || second > 59) return 0;
    
    return 1;
}

int is_valid_level(const char* level) {
    return strcmp(level, "ERROR") == 0 || strcmp(level, "WARNING") == 0 || 
           strcmp(level, "INFO") == 0;
}

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (!line || !entry) return 0;
    if (strlen(line) >= MAX_LINE_LENGTH) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "");
    
    if (!timestamp || !level || !message) return 0;
    if (!is_valid_timestamp(timestamp)) return 0;
    if (!is_valid_level(level)) return 0;
    
    strncpy(entry->timestamp, timestamp, 19);
    entry->timestamp[19] = '\0';
    strncpy(entry->level, level, 9);
    entry->level[9] = '\0';
    strncpy(entry->message, message, MAX_LINE_LENGTH - 30);
    entry->message[MAX_LINE_LENGTH - 31] = '\0';
    
    return 1;
}

void initialize_analyzer(struct LogAnalyzer* analyzer) {
    if (!analyzer) return;
    analyzer->entry_count = 0;
    analyzer->stats.total_entries = 0;
    analyzer->stats.error_count = 0;
    analyzer->stats.warning_count = 0;
    analyzer->stats.info_count = 0;
    memset(analyzer->stats.earliest, 0, 20);
    memset(analyzer->stats.latest, 0, 20);
}

void update_stats(struct LogAnalyzer* analyzer, const struct LogEntry* entry) {
    if (!analyzer || !entry) return;
    
    analyzer->stats.total_entries++;
    
    if (strcmp(entry->level, "ERROR") == 0) {
        analyzer->stats.error_count++;
    } else if (strcmp(entry->level, "WARNING") == 0) {
        analyzer->stats.warning_count++;
    } else if (strcmp(entry->level, "INFO") == 0) {
        analyzer->stats.info_count++;
    }
    
    if (analyzer->stats.earliest[0] == '\0' || strcmp(entry->timestamp, analyzer->stats.earliest) < 0) {
        strncpy(analyzer->stats.earliest, entry->timestamp, 19);
        analyzer->stats.earliest[19] = '\0';
    }
    
    if (analyzer->stats.latest[0] == '\0' || strcmp(entry->timestamp, analyzer->stats.latest) > 0) {
        strncpy(analyzer->stats.latest, entry->timestamp, 19);
        analyzer->stats.latest[19] = '\0';
    }
}

int add_log_entry(struct LogAnalyzer* analyzer, const char* line) {
    if (!analyzer || !line) return 0;
    if (analyzer->entry_count >= MAX_ENTRIES) return 0;
    
    struct LogEntry entry;
    if (!