//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum LogLevel {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_DEBUG,
    LOG_UNKNOWN
};

struct LogEntry {
    char timestamp[20];
    enum LogLevel level;
    char message[256];
};

int parse_log_level(const char* level_str) {
    if (strcmp(level_str, "INFO") == 0) return LOG_INFO;
    if (strcmp(level_str, "WARNING") == 0) return LOG_WARNING;
    if (strcmp(level_str, "ERROR") == 0) return LOG_ERROR;
    if (strcmp(level_str, "DEBUG") == 0) return LOG_DEBUG;
    return LOG_UNKNOWN;
}

const char* level_to_string(enum LogLevel level) {
    switch (level) {
        case LOG_INFO: return "INFO";
        case LOG_WARNING: return "WARNING";
        case LOG_ERROR: return "ERROR";
        case LOG_DEBUG: return "DEBUG";
        default: return "UNKNOWN";
    }
}

int validate_timestamp(const char* timestamp) {
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
    
    if (!validate_timestamp(entry->timestamp)) return 0;
    
    entry->level = parse_log_level(level_str);
    return 1;
}

void analyze_log_level(enum LogLevel level, int* counts) {
    switch (level) {
        case LOG_INFO:
            counts[0]++;
            break;
        case LOG_WARNING:
            counts[1]++;
            break;
        case LOG_ERROR:
            counts[2]++;
            break;
        case LOG_DEBUG:
            counts[3]++;
            break;
        case LOG_UNKNOWN:
            counts[4]++;
            break;
    }
}

void print_analysis(int* counts) {
    printf("Log Analysis Results:\n");
    printf("====================\n");
    printf("INFO:    %d entries\n", counts[0]);
    printf("WARNING: %d entries\n", counts[1]);
    printf("ERROR:   %d entries\n", counts[2]);
    printf("DEBUG:   %d entries\n", counts[3]);
    printf("UNKNOWN: %d entries\n", counts[4]);
    printf("====================\n");
}

int main(void) {
    char filename[256];
    FILE* file;
    struct LogEntry entry;
    int counts[5] = {0};
    int total_entries = 0;
    
    printf("Enter log file name: ");
    if (scanf("%255s", filename) != 1) {
        fprintf(stderr, "Error reading filename\n");
        return 1;
    }
    
    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return 1;
    }
    
    while (read_log_entry(file, &entry)) {
        analyze_log_level(entry.level, counts);
        total_entries++;
    }
    
    if (ferror(file)) {
        fprintf(stderr, "Error reading file\n");
        fclose(file);
        return 1;
    }
    
    fclose(file);
    
    if (total_entries == 0) {
        printf("No valid log entries found\n");
        return 1;
    }
    
    print_analysis(counts);
    printf("Total entries processed: %d\n", total_entries);
    
    return 0;
}