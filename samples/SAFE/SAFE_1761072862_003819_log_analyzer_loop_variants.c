//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
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
    
    while (fgets(line, sizeof(line), file) != NULL && count < max_entries) {
        if (strlen(line) < 10) continue;
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed >= 2) {
            if (strlen(timestamp) > 0 && strlen(level) > 0) {
                strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp)-1);
                strncpy(entries[count].level, level, sizeof(entries[count].level)-1);
                if (parsed == 3) {
                    strncpy(entries[count].message, message, sizeof(entries[count].message)-1);
                }
                entries[count].severity = parse_severity(level);
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
    
    if (count <= 0) return;
    
    do {
        if (entries[i].severity >= 0 && entries[i].severity < 5) {
            severity_count[entries[i].severity]++;
            total_severity += entries[i].severity;
        }
        i++;
    } while (i < count);
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    
    const char* level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    int j;
    for (j = 0; j < 5; j++) {
        printf("%s: %d\n", level_names[j], severity_count[j]);
    }
    
    if (count > 0) {
        double avg_severity = (double)total_severity / count;
        printf("Average severity: %.2f\n", avg_severity);
    }
    
    int k = 0;
    printf("\nRecent critical entries:\n");
    while (k < count) {
        if (entries[k].severity >= 3) {
            printf("- %s %s: %s\n", entries[k].timestamp, entries[k].level, entries[k].message);
        }
        k++;
    }
}

int main() {
    struct LogEntry entries[MAX_ENTRIES];
    char filename[256];
    
    printf("Enter log file name: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len-1] == '\n') {
        filename[len-1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }
    
    int entry_count = read_log_file(filename, entries, MAX_ENTRIES);
    if (entry_count < 0) {
        printf("Error reading log file\n");
        return 1;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return 0;
    }
    
    analyze_logs(entries, entry_count);
    
    return 0;
}