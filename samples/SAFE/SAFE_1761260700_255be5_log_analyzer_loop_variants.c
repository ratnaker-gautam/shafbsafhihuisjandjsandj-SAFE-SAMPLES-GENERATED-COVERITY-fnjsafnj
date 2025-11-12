//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
    int severity;
};

int parse_severity(const char* level) {
    if (strcmp(level, "DEBUG") == 0) return 1;
    if (strcmp(level, "INFO") == 0) return 2;
    if (strcmp(level, "WARNING") == 0) return 3;
    if (strcmp(level, "ERROR") == 0) return 4;
    if (strcmp(level, "CRITICAL") == 0) return 5;
    return 0;
}

int read_log_file(const char* filename, struct LogEntry* entries, int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) return -1;
    
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) && count < max_entries) {
        if (strlen(line) < 10) continue;
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed >= 2) {
            strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp) - 1);
            strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
            if (parsed == 3) {
                strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
            }
            entries[count].severity = parse_severity(level);
            count++;
        }
    }
    
    fclose(file);
    return count;
}

void analyze_logs(struct LogEntry* entries, int count) {
    int severity_counts[6] = {0};
    int i = 0;
    
    do {
        if (entries[i].severity >= 0 && entries[i].severity <= 5) {
            severity_counts[entries[i].severity]++;
        }
        i++;
    } while (i < count);
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    
    const char* levels[] = {"UNKNOWN", "DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    for (int j = 0; j < 6; j++) {
        printf("%-8s: %d\n", levels[j], severity_counts[j]);
    }
    
    int error_count = 0;
    int k = 0;
    while (k < count) {
        if (entries[k].severity >= 4) {
            error_count++;
        }
        k++;
    }
    
    printf("High severity entries (ERROR+): %d\n", error_count);
    
    if (count > 0) {
        struct LogEntry recent = entries[0];
        for (int m = 1; m < count; m++) {
            if (strcmp(entries[m].timestamp, recent.timestamp) > 0) {
                recent = entries[m];
            }
        }
        printf("Most recent entry: %s %s %s\n", recent.timestamp, recent.level, recent.message);
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log filename: ");
    char filename[256];
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        fprintf(stderr, "Invalid filename\n");
        return 1;
    }
    
    int entry_count = read_log_file(filename, entries, MAX_ENTRIES);
    if (entry_count < 0) {
        fprintf(stderr, "Error reading log file\n");
        return 1;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return 0;
    }
    
    analyze_logs(entries, entry_count);
    
    return 0;
}