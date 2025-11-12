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

int read_log_file(const char* filename, struct LogEntry* entries, int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) return -1;
    
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (count < max_entries && fgets(line, sizeof(line), file)) {
        char ts[32], lvl[16], msg[256];
        if (sscanf(line, "%31s %15s %255[^\n]", ts, lvl, msg) == 3) {
            int sev = parse_severity(lvl);
            if (sev >= 0) {
                strcpy(entries[count].timestamp, ts);
                strcpy(entries[count].level, lvl);
                strcpy(entries[count].message, msg);
                entries[count].severity = sev;
                count++;
            }
        }
    }
    
    fclose(file);
    return count;
}

void analyze_logs(struct LogEntry* entries, int count) {
    int severity_count[5] = {0};
    int total_severity = 0;
    int i = 0;
    
    do {
        if (i < count) {
            severity_count[entries[i].severity]++;
            total_severity += entries[i].severity;
        }
        i++;
    } while (i < count);
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    
    const char* levels[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    int j;
    for (j = 0; j < 5; j++) {
        printf("%s: %d\n", levels[j], severity_count[j]);
    }
    
    if (count > 0) {
        double avg_severity = (double)total_severity / count;
        printf("Average severity: %.2f\n", avg_severity);
    }
    
    printf("\nRecent entries:\n");
    int start = (count > 5) ? count - 5 : 0;
    int k = start;
    while (k < count) {
        printf("%s %s: %s\n", entries[k].timestamp, entries[k].level, entries[k].message);
        k++;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    struct LogEntry entries[MAX_ENTRIES];
    int count = read_log_file(argv[1], entries, MAX_ENTRIES);
    
    if (count < 0) {
        printf("Error: Could not open or read file '%s'\n", argv[1]);
        return 1;
    }
    
    if (count == 0) {
        printf("No valid log entries found in '%s'\n", argv[1]);
        return 1;
    }
    
    analyze_logs(entries, count);
    return 0;
}