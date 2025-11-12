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
    
    while (count < max_entries && fgets(line, sizeof(line), file)) {
        char ts[32], lvl[16], msg[256];
        if (sscanf(line, "%31s %15s %255[^\n]", ts, lvl, msg) == 3) {
            strncpy(entries[count].timestamp, ts, sizeof(entries[count].timestamp) - 1);
            strncpy(entries[count].level, lvl, sizeof(entries[count].level) - 1);
            strncpy(entries[count].message, msg, sizeof(entries[count].message) - 1);
            entries[count].severity = parse_severity(lvl);
            entries[count].timestamp[31] = '\0';
            entries[count].level[15] = '\0';
            entries[count].message[255] = '\0';
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
    int j;
    for (j = 0; j < 6; j++) {
        printf("%s: %d\n", levels[j], severity_counts[j]);
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
}

void filter_by_severity(struct LogEntry* entries, int count, int min_severity) {
    printf("\nEntries with severity >= %d:\n", min_severity);
    
    int found = 0;
    int i;
    for (i = 0; i < count; i++) {
        if (entries[i].severity >= min_severity) {
            printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No entries found.\n");
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    char filename[256];
    
    printf("Enter log file name: ");
    if (scanf("%255s", filename) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    int entry_count = read_log_file(filename, entries, MAX_ENTRIES);
    if (entry_count < 0) {
        printf("Error: Could not open file '%s'\n", filename);
        return 1;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 0;
    }
    
    analyze_logs(entries, entry_count);
    
    int severity;
    printf("\nEnter minimum severity to filter (1-5): ");
    if (scanf("%d", &severity) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    if (severity < 1 || severity > 5) {
        printf("Severity must be between 1 and 5.\n");
        return 1;
    }
    
    filter_by_severity(entries, entry_count, severity);
    
    return 0;
}