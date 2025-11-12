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
    char* severity_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 0 && entries[i].severity < 5) {
            severity_count[entries[i].severity]++;
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("=====================\n");
    printf("Total entries: %d\n", count);
    
    int idx = 0;
    while (idx < 5) {
        printf("%-8s: %d entries\n", severity_names[idx], severity_count[idx]);
        idx++;
    }
    
    int highest_severity = -1;
    int j = 0;
    do {
        if (j < count && entries[j].severity > highest_severity) {
            highest_severity = entries[j].severity;
        }
        j++;
    } while (j < count);
    
    if (highest_severity >= 0) {
        printf("Highest severity level: %s\n", severity_names[highest_severity]);
    }
    
    printf("\nRecent entries:\n");
    int display_count = (count < 5) ? count : 5;
    for (int k = count - display_count; k < count; k++) {
        printf("%s %s %s\n", entries[k].timestamp, entries[k].level, entries[k].message);
    }
}

int main() {
    struct LogEntry entries[MAX_ENTRIES];
    
    printf("Log File Analyzer\n");
    printf("Enter log filename: ");
    
    char filename[256];
    if (!fgets(filename, sizeof(filename), stdin)) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        printf("No filename provided\n");
        return 1;
    }
    
    int entry_count = read_log_file(filename, entries, MAX_ENTRIES);
    if (entry_count < 0) {
        printf("Error: Could not open or read file '%s'\n", filename);
        return 1;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found in file\n");
        return 0;
    }
    
    analyze_logs(entries, entry_count);
    
    return 0;
}