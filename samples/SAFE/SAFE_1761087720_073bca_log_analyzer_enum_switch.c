//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
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

int read_log_entry(FILE* file, struct LogEntry* entry) {
    char line[512];
    char level_str[16];
    
    if (fgets(line, sizeof(line), file) == NULL) {
        return 0;
    }
    
    if (sscanf(line, "%19s %15s %255[^\n]", 
               entry->timestamp, level_str, entry->message) != 3) {
        return -1;
    }
    
    int level = parse_log_level(level_str);
    if (level == -1) {
        return -1;
    }
    
    entry->level = level;
    return 1;
}

void analyze_log_level(struct LogEntry* entries, int count, enum LogLevel target_level) {
    int matches = 0;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].level == target_level) {
            matches++;
        }
    }
    
    printf("Found %d %s entries\n", matches, level_to_string(target_level));
}

void process_log_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        return;
    }
    
    struct LogEntry entries[1000];
    int count = 0;
    
    while (count < 1000) {
        int result = read_log_entry(file, &entries[count]);
        if (result == 0) break;
        if (result == 1) count++;
    }
    
    fclose(file);
    
    if (count == 0) {
        printf("No valid log entries found\n");
        return;
    }
    
    printf("Analyzing %d log entries...\n", count);
    
    for (enum LogLevel level = LOG_DEBUG; level <= LOG_CRITICAL; level++) {
        analyze_log_level(entries, count, level);
    }
    
    int debug_count = 0;
    int info_count = 0;
    int warning_count = 0;
    int error_count = 0;
    int critical_count = 0;
    
    for (int i = 0; i < count; i++) {
        switch (entries[i].level) {
            case LOG_DEBUG:
                debug_count++;
                break;
            case LOG_INFO:
                info_count++;
                break;
            case LOG_WARNING:
                warning_count++;
                break;
            case LOG_ERROR:
                error_count++;
                break;
            case LOG_CRITICAL:
                critical_count++;
                break;
        }
    }
    
    printf("\nSummary by switch statement:\n");
    printf("DEBUG: %d\n", debug_count);
    printf("INFO: %d\n", info_count);
    printf("WARNING: %d\n", warning_count);
    printf("ERROR: %d\n", error_count);
    printf("CRITICAL: %d\n", critical_count);
}

int main() {
    char filename[256];
    
    printf("Enter log filename: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        printf("Error: Empty filename\n");
        return 1;
    }
    
    process_log_file(filename);
    return 0;
}