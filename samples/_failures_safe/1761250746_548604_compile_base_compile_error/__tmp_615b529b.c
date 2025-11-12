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

void analyze_logs(struct LogEntry* logs, int count) {
    int level_counts[5] = {0};
    time_t earliest = 0;
    time_t latest = 0;
    
    for (int i = 0; i < count; i++) {
        if (i == 0 || logs[i].timestamp < earliest) {
            earliest = logs[i].timestamp;
        }
        if (i == 0 || logs[i].timestamp > latest) {
            latest = logs[i].timestamp;
        }
        
        if (logs[i].level >= 0 && logs[i].level < 5) {
            level_counts[logs[i].level]++;
        }
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Time range: %ld seconds\n", (long)(latest - earliest));
    printf("Total entries: %d\n", count);
    
    for (enum LogLevel level = LOG_INFO; level <= LOG_UNKNOWN; level++) {
        printf("%-10s: %d\n", level_to_string(level), level_counts[level]);
    }
}

int main(void) {
    struct LogEntry logs[100];
    int log_count = 0;
    char filename[256];
    
    printf("Enter log filename: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        fprintf(stderr, "Error reading filename\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        fprintf(stderr, "Empty filename\n");
        return 1;
    }
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return 1;
    }
    
    while (log_count < 100) {
        int result = read_log_entry(file, &logs[log_count]);
        if (result == 0) {
            break;
        } else if (result > 0) {
            log_count++;
        }
    }
    
    fclose(file);
    
    if (log_count == 0) {
        printf("No valid log entries found\n");
        return 1;
    }
    
    analyze_logs(logs, log_count);
    
    printf("\nFilter by level (INFO/WARNING/ERROR/CRITICAL/ALL): ");
    char filter[32];
    if (fgets(filter, sizeof(filter), stdin) == NULL) {
        return 1;
    }
    
    len = strlen(filter);
    if (len > 0 && filter[len - 1] == '\n') {
        filter[len - 1] = '\0';
    }
    
    for (size_t i = 0; i < len; i++) {
        filter[i] = toupper(filter[i]);
    }
    
    enum LogLevel filter_level = parse_log_level(filter);
    
    printf("\nMatching entries:\n");
    int matches = 0;
    for (int i = 0; i < log_count; i++) {
        if (strcmp(filter, "ALL") == 0 || logs[i].level == filter_level) {
            char time_buf[32];
            struct tm*