//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LEVEL_LEN 16

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level[LEVEL_LEN];
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - LEVEL_LEN];
} LogEntry;

#define VALID_LEVEL(level) (strcmp(level, "INFO") == 0 || strcmp(level, "WARNING") == 0 || strcmp(level, "ERROR") == 0)

int parse_timestamp(const char* str) {
    struct tm tm = {0};
    return strptime(str, "%Y-%m-%d %H:%M:%S", &tm) != NULL;
}

int read_log_entries(const char* filename, LogEntry entries[], int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) return -1;
    
    int count = 0;
    char line[MAX_LINE_LEN];
    
    while (count < max_entries && fgets(line, sizeof(line), file)) {
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') line[len-1] = '\0';
        
        char timestamp[TIMESTAMP_LEN];
        char level[LEVEL_LEN];
        char message[MAX_LINE_LEN];
        
        if (sscanf(line, "%19s %15s %[^\n]", timestamp, level, message) == 3) {
            if (parse_timestamp(timestamp) && VALID_LEVEL(level)) {
                strncpy(entries[count].timestamp, timestamp, TIMESTAMP_LEN-1);
                entries[count].timestamp[TIMESTAMP_LEN-1] = '\0';
                strncpy(entries[count].level, level, LEVEL_LEN-1);
                entries[count].level[LEVEL_LEN-1] = '\0';
                strncpy(entries[count].message, message, sizeof(entries[count].message)-1);
                entries[count].message[sizeof(entries[count].message)-1] = '\0';
                count++;
            }
        }
    }
    
    fclose(file);
    return count;
}

void analyze_logs(LogEntry entries[], int count) {
    int info_count = 0, warning_count = 0, error_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARNING") == 0) warning_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("INFO: %d\n", info_count);
    printf("WARNING: %d\n", warning_count);
    printf("ERROR: %d\n", error_count);
    
    if (count > 0) {
        printf("\nRecent entries:\n");
        int start = count > 5 ? count - 5 : 0;
        for (int i = start; i < count; i++) {
            printf("%s [%s] %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    LogEntry entries[MAX_ENTRIES];
    int count = read_log_entries(argv[1], entries, MAX_ENTRIES);
    
    if (count < 0) {
        printf("Error: Could not open file '%s'\n", argv[1]);
        return 1;
    }
    
    if (count == 0) {
        printf("No valid log entries found in '%s'\n", argv[1]);
        return 1;
    }
    
    analyze_logs(entries, count);
    return 0;
}