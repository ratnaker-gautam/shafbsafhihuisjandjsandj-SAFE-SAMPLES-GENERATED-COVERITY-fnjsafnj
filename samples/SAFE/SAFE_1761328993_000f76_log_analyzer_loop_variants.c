//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

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
    int total_severity = 0;
    int i = 0;
    
    while (i < count) {
        int sev = entries[i].severity;
        if (sev >= 1 && sev <= 5) {
            severity_counts[sev]++;
            total_severity += sev;
        }
        i++;
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    
    const char* levels[] = {"", "DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    int j;
    for (j = 1; j <= 5; j++) {
        printf("%s: %d\n", levels[j], severity_counts[j]);
    }
    
    if (count > 0) {
        double avg_severity = (double)total_severity / count;
        printf("Average severity: %.2f\n", avg_severity);
    }
    
    printf("\nRecent entries:\n");
    int start = (count > 5) ? count - 5 : 0;
    int k = start;
    do {
        if (k < count) {
            printf("%s %s: %s\n", entries[k].timestamp, entries[k].level, entries[k].message);
        }
        k++;
    } while (k < count && k < start + 5);
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    char filename[256];
    
    printf("Enter log filename: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }
    
    int count = read_log_file(filename, entries, MAX_ENTRIES);
    if (count < 0) {
        printf("Error reading file: %s\n", filename);
        return 1;
    }
    
    if (count == 0) {
        printf("No valid log entries found\n");
        return 0;
    }
    
    analyze_logs(entries, count);
    return 0;
}