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
    
    while (fgets(line, sizeof(line), file) && count < max_entries) {
        if (strlen(line) < 10) continue;
        
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        int parsed = sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message);
        if (parsed == 3) {
            strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp)-1);
            strncpy(entries[count].level, level, sizeof(entries[count].level)-1);
            strncpy(entries[count].message, message, sizeof(entries[count].message)-1);
            entries[count].severity = parse_severity(level);
            count++;
        }
    }
    
    fclose(file);
    return count;
}

void analyze_logs(struct LogEntry* entries, int count) {
    int severity_counts[5] = {0};
    int total_severity = 0;
    int valid_entries = 0;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].severity >= 0) {
            severity_counts[entries[i].severity]++;
            total_severity += entries[i].severity;
            valid_entries++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries processed: %d\n", count);
    printf("Valid entries: %d\n", valid_entries);
    printf("\nSeverity Distribution:\n");
    
    const char* level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    int i = 0;
    while (i < 5) {
        printf("%-8s: %d\n", level_names[i], severity_counts[i]);
        i++;
    }
    
    if (valid_entries > 0) {
        double avg_severity = (double)total_severity / valid_entries;
        printf("\nAverage severity: %.2f\n", avg_severity);
    }
    
    printf("\nRecent Critical/Error entries:\n");
    int found = 0;
    for (int j = count - 1; j >= 0 && found < 5; j--) {
        if (entries[j].severity >= 3) {
            printf("- [%s] %s: %s\n", entries[j].timestamp, entries[j].level, entries[j].message);
            found++;
        }
    }
    
    if (found == 0) {
        printf("No critical or error entries found.\n");
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