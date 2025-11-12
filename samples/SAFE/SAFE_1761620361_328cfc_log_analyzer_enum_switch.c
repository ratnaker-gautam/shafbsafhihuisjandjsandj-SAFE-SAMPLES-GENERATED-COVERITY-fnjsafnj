//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

enum LogLevel {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_CRITICAL,
    LOG_UNKNOWN
};

struct LogEntry {
    time_t timestamp;
    enum LogLevel level;
    char message[256];
};

int parse_log_level(const char* level_str) {
    if (strcmp(level_str, "INFO") == 0) return LOG_INFO;
    if (strcmp(level_str, "WARNING") == 0) return LOG_WARNING;
    if (strcmp(level_str, "ERROR") == 0) return LOG_ERROR;
    if (strcmp(level_str, "CRITICAL") == 0) return LOG_CRITICAL;
    return LOG_UNKNOWN;
}

const char* level_to_string(enum LogLevel level) {
    switch (level) {
        case LOG_INFO: return "INFO";
        case LOG_WARNING: return "WARNING";
        case LOG_ERROR: return "ERROR";
        case LOG_CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

int read_log_entry(FILE* file, struct LogEntry* entry) {
    char line[512];
    char level_str[32];
    char date_str[32];
    char time_str[32];
    
    if (fgets(line, sizeof(line), file) == NULL) {
        return 0;
    }
    
    if (sscanf(line, "%31s %31s %31s %255[^\n]", 
               date_str, time_str, level_str, entry->message) != 4) {
        return -1;
    }
    
    char datetime[64];
    snprintf(datetime, sizeof(datetime), "%s %s", date_str, time_str);
    
    struct tm tm = {0};
    if (strptime(datetime, "%Y-%m-%d %H:%M:%S", &tm) == NULL) {
        return -1;
    }
    
    entry->timestamp = mktime(&tm);
    entry->level = parse_log_level(level_str);
    
    return 1;
}

void analyze_logs(FILE* file) {
    struct LogEntry entry;
    int counts[5] = {0};
    time_t earliest = 0, latest = 0;
    int entry_count = 0;
    
    while (read_log_entry(file, &entry) > 0) {
        if (entry_count == 0) {
            earliest = entry.timestamp;
            latest = entry.timestamp;
        } else {
            if (entry.timestamp < earliest) earliest = entry.timestamp;
            if (entry.timestamp > latest) latest = entry.timestamp;
        }
        
        if (entry.level >= 0 && entry.level < 5) {
            counts[entry.level]++;
        }
        entry_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    printf("Time range: %s to %s\n", 
           ctime(&earliest), ctime(&latest));
    
    printf("Breakdown by level:\n");
    for (int i = 0; i < 5; i++) {
        printf("  %s: %d\n", level_to_string(i), counts[i]);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Cannot open file %s\n", argv[1]);
        return 1;
    }
    
    analyze_logs(file);
    fclose(file);
    return 0;
}