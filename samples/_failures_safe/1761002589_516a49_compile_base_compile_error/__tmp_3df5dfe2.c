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

int read_log_file(struct LogEntry entries[], int max_entries) {
    char filename[256];
    printf("Enter log filename: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        return 0;
    }
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Cannot open file %s\n", filename);
        return 0;
    }
    
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (count < max_entries && fgets(line, sizeof(line), file) != NULL) {
        char timestamp[32] = {0};
        char level[16] = {0};
        char message[256] = {0};
        
        if (sscanf(line, "%31s %15s %255[^\n]", timestamp, level, message) == 3) {
            if (strlen(timestamp) > 0 && strlen(level) > 0 && strlen(message) > 0) {
                strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp) - 1);
                strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
                strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
                entries[count].timestamp[sizeof(entries[count].timestamp) - 1] = '\0';
                entries[count].level[sizeof(entries[count].level) - 1] = '\0';
                entries[count].message[sizeof(entries[count].message) - 1] = '\0';
                entries[count].severity = parse_severity(level);
                count++;
            }
        }
    }
    
    fclose(file);
    return count;
}

void analyze_logs(struct LogEntry entries[], int count) {
    if (count <= 0) {
        printf("No log entries to analyze.\n");
        return;
    }
    
    int severity_count[5] = {0};
    int i = 0;
    
    do {
        if (entries[i].severity >= 0 && entries[i].severity < 5) {
            severity_count[entries[i].severity]++;
        }
        i++;
    } while (i < count);
    
    printf("\nLog Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    
    const char* levels[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    int j;
    for (j = 0; j < 5; j++) {
        printf("%-8s: %d entries\n", levels[j], severity_count[j]);
    }
    
    int error_count = severity_count[3] + severity_count[4];
    printf("Total errors: %d\n", error_count);
    
    if (error_count > 0) {
        printf("\nError entries:\n");
        int k = 0;
        while (k < count) {
            if (entries[k].severity >= 3) {
                printf("%s %s: %s\n", entries[k].timestamp, entries[k].level, entries[k].message);
            }
            k++;
        }
    }
}

void find_pattern(struct LogEntry entries[], int count) {
    char pattern[128];
    printf("Enter search pattern: ");
    if (fgets(pattern, sizeof(pattern), stdin) == NULL) {
        return;
    }
    
    size_t len = strlen(pattern);
    if (len > 0 && pattern[len - 1] == '\n') {
        pattern[len - 1] = '\0';
    }
    
    if (strlen(pattern) == 0) {
        return;
    }
    
    printf("Matching entries:\n");
    int found = 0;
    int i;
    for (i = 0; i < count; i++) {
        if (strstr(entries[i].message, pattern) != NULL) {
            printf("%s %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No entries matching pattern '%s'\n", pattern);
    }
}

int main() {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count =