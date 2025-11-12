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
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - LEVEL_LEN - 3];
} LogEntry;

#define IS_VALID_LEVEL(lvl) (strcmp(lvl, "INFO") == 0 || strcmp(lvl, "WARNING") == 0 || strcmp(lvl, "ERROR") == 0)

int parse_timestamp(const char* ts) {
    struct tm tm = {0};
    return strptime(ts, "%Y-%m-%d %H:%M:%S", &tm) != NULL;
}

int read_log_entries(LogEntry entries[], int max_entries, FILE* file) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (count < max_entries && fgets(line, sizeof(line), file)) {
        char ts[TIMESTAMP_LEN], lvl[LEVEL_LEN], msg[MAX_LINE_LEN];
        
        if (sscanf(line, "%19[^]]] %15[^]]] %[^\n]", ts, lvl, msg) == 3) {
            if (parse_timestamp(ts) && IS_VALID_LEVEL(lvl)) {
                strncpy(entries[count].timestamp, ts, TIMESTAMP_LEN - 1);
                entries[count].timestamp[TIMESTAMP_LEN - 1] = '\0';
                strncpy(entries[count].level, lvl, LEVEL_LEN - 1);
                entries[count].level[LEVEL_LEN - 1] = '\0';
                strncpy(entries[count].message, msg, sizeof(entries[count].message) - 1);
                entries[count].message[sizeof(entries[count].message) - 1] = '\0';
                count++;
            }
        }
    }
    
    return count;
}

void analyze_logs(LogEntry entries[], int count) {
    int info_count = 0, warning_count = 0, error_count = 0;
    char earliest[TIMESTAMP_LEN] = "";
    char latest[TIMESTAMP_LEN] = "";
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        else if (strcmp(entries[i].level, "WARNING") == 0) warning_count++;
        else if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
        
        if (i == 0) {
            strcpy(earliest, entries[i].timestamp);
            strcpy(latest, entries[i].timestamp);
        } else {
            if (strcmp(entries[i].timestamp, earliest) < 0) {
                strcpy(earliest, entries[i].timestamp);
            }
            if (strcmp(entries[i].timestamp, latest) > 0) {
                strcpy(latest, entries[i].timestamp);
            }
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("INFO: %d\n", info_count);
    printf("WARNING: %d\n", warning_count);
    printf("ERROR: %d\n", error_count);
    printf("Time range: %s to %s\n", earliest, latest);
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    char filename[256];
    
    printf("Enter log filename: ");
    if (scanf("%255s", filename) != 1) {
        fprintf(stderr, "Error reading filename\n");
        return 1;
    }
    
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return 1;
    }
    
    int count = read_log_entries(entries, MAX_ENTRIES, file);
    fclose(file);
    
    if (count == 0) {
        printf("No valid log entries found\n");
        return 1;
    }
    
    analyze_logs(entries, count);
    
    return 0;
}