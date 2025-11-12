//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum LogLevel {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_CRITICAL
};

struct LogEntry {
    char timestamp[20];
    enum LogLevel level;
    char message[256];
};

int parse_log_level(const char* level_str) {
    if (strcmp(level_str, "DEBUG") == 0) return LOG_DEBUG;
    if (strcmp(level_str, "INFO") == 0) return LOG_INFO;
    if (strcmp(level_str, "WARNING") == 0) return LOG_WARNING;
    if (strcmp(level_str, "ERROR") == 0) return LOG_ERROR;
    if (strcmp(level_str, "CRITICAL") == 0) return LOG_CRITICAL;
    return -1;
}

const char* level_to_string(enum LogLevel level) {
    switch (level) {
        case LOG_DEBUG: return "DEBUG";
        case LOG_INFO: return "INFO";
        case LOG_WARNING: return "WARNING";
        case LOG_ERROR: return "ERROR";
        case LOG_CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

int is_valid_timestamp(const char* timestamp) {
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

int read_log_entry(FILE* file, struct LogEntry* entry) {
    char line[512];
    char level_str[16];
    
    if (fgets(line, sizeof(line), file) == NULL) return 0;
    
    if (sscanf(line, "%19s %15s %255[^\n]", 
               entry->timestamp, level_str, entry->message) != 3) {
        return 0;
    }
    
    if (!is_valid_timestamp(entry->timestamp)) return 0;
    
    int level = parse_log_level(level_str);
    if (level == -1) return 0;
    
    entry->level = level;
    return 1;
}

void analyze_log_level(enum LogLevel level, int* counts) {
    switch (level) {
        case LOG_DEBUG:
            counts[0]++;
            break;
        case LOG_INFO:
            counts[1]++;
            break;
        case LOG_WARNING:
            counts[2]++;
            break;
        case LOG_ERROR:
            counts[3]++;
            break;
        case LOG_CRITICAL:
            counts[4]++;
            break;
        default:
            break;
    }
}

void print_analysis(int* counts, int total) {
    printf("\nLog Analysis Summary:\n");
    printf("====================\n");
    printf("Total entries: %d\n", total);
    printf("DEBUG:    %d\n", counts[0]);
    printf("INFO:     %d\n", counts[1]);
    printf("WARNING:  %d\n", counts[2]);
    printf("ERROR:    %d\n", counts[3]);
    printf("CRITICAL: %d\n", counts[4]);
    
    if (counts[4] > 0) {
        printf("ALERT: Critical errors detected!\n");
    } else if (counts[3] > 5) {
        printf("WARNING: High number of errors detected\n");
    }
}

int main(void) {
    char filename[256];
    FILE* file;
    struct LogEntry entry;
    int counts[5] = {0};
    int total_entries = 0;
    int valid_entries = 0;
    
    printf("Enter log file name: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        fprintf(stderr, "Error reading filename\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len-1] == '\n') {
        filename[len-1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        fprintf(stderr, "Filename cannot be empty\n");
        return 1;
    }
    
    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return 1;
    }
    
    printf("Processing log file...\n");
    
    while (!feof(file)) {
        total_entries++;
        if (read_log_entry(file, &entry)) {
            valid_entries++;
            analyze_log_level(entry.level, counts);
            
            switch (entry.level) {
                case LOG_CRITICAL:
                    printf("CRITICAL: %s - %s\n", entry.timestamp, entry.message);
                    break;
                case LOG_ERROR:
                    printf("ERROR: %s - %s\n", entry.timestamp, entry.message);
                    break;
                default:
                    break;
            }
        } else if (!feof(file)) {
            printf("S