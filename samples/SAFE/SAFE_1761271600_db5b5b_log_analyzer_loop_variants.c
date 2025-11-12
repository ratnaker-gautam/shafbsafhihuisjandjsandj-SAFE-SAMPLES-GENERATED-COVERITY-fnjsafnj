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
    if (strcmp(level, "DEBUG") == 0) return 0;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "WARNING") == 0) return 2;
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "CRITICAL") == 0) return 4;
    return -1;
}

int read_log_file(struct LogEntry entries[], int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter empty line to finish.\n");
    
    while (count < max_entries) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) == 0) {
            break;
        }
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", 
                           timestamp, level, message);
        
        if (parsed < 3) {
            printf("Invalid format. Skipping.\n");
            continue;
        }
        
        int severity = parse_severity(level);
        if (severity == -1) {
            printf("Unknown log level: %s. Skipping.\n", level);
            continue;
        }
        
        strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp) - 1);
        strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
        strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
        entries[count].severity = severity;
        
        count++;
    }
    
    return count;
}

void analyze_logs(struct LogEntry entries[], int count) {
    if (count == 0) {
        printf("No log entries to analyze.\n");
        return;
    }
    
    int severity_count[5] = {0};
    char* severity_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    
    int i = 0;
    do {
        if (entries[i].severity >= 0 && entries[i].severity < 5) {
            severity_count[entries[i].severity]++;
        }
        i++;
    } while (i < count);
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    printf("Total entries: %d\n", count);
    
    for (int j = 0; j < 5; j++) {
        printf("%s: %d\n", severity_names[j], severity_count[j]);
    }
    
    int error_critical = 0;
    for (int k = 0; k < count; k++) {
        if (entries[k].severity >= 3) {
            error_critical++;
        }
    }
    printf("ERROR+CRITICAL entries: %d\n", error_critical);
    
    printf("\nRecent entries (last 5):\n");
    int start = (count > 5) ? count - 5 : 0;
    for (int m = start; m < count; m++) {
        printf("%s [%s] %s\n", entries[m].timestamp, entries[m].level, entries[m].message);
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    
    printf("Simple Log Analyzer\n");
    printf("===================\n");
    
    int count = read_log_file(entries, MAX_ENTRIES);
    
    if (count > 0) {
        analyze_logs(entries, count);
    } else {
        printf("No valid log entries were processed.\n");
    }
    
    return 0;
}