//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LOG_LEVELS 4

#define IS_VALID_LEVEL(level) \
    (strcmp(level, "INFO") == 0 || strcmp(level, "WARN") == 0 || \
     strcmp(level, "ERROR") == 0 || strcmp(level, "DEBUG") == 0)

#define PARSE_TIMESTAMP(ts, tm) \
    (strptime(ts, "%Y-%m-%d %H:%M:%S", tm) != NULL)

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level[8];
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

int read_log_file(const char* filename, LogEntry entries[], int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) return -1;
    
    int count = 0;
    char line[MAX_LINE_LEN];
    
    while (count < max_entries && fgets(line, sizeof(line), file)) {
        char ts[TIMESTAMP_LEN], level[8], message[MAX_LINE_LEN];
        
        if (sscanf(line, "%19s %7s %255[^\n]", ts, level, message) == 3) {
            if (IS_VALID_LEVEL(level)) {
                strncpy(entries[count].timestamp, ts, TIMESTAMP_LEN - 1);
                entries[count].timestamp[TIMESTAMP_LEN - 1] = '\0';
                strncpy(entries[count].level, level, 7);
                entries[count].level[7] = '\0';
                strncpy(entries[count].message, message, MAX_LINE_LEN - TIMESTAMP_LEN - 11);
                entries[count].message[MAX_LINE_LEN - TIMESTAMP_LEN - 11] = '\0';
                count++;
            }
        }
    }
    
    fclose(file);
    return count;
}

void analyze_logs(LogEntry entries[], int count) {
    int level_counts[LOG_LEVELS] = {0};
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < LOG_LEVELS; j++) {
            if (strcmp(entries[i].level, levels[j]) == 0) {
                level_counts[j]++;
                break;
            }
        }
    }
    
    printf("Log Level Summary:\n");
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d\n", levels[i], level_counts[i]);
    }
    
    if (count > 0) {
        struct tm first_tm = {0}, last_tm = {0};
        PARSE_TIMESTAMP(entries[0].timestamp, &first_tm);
        PARSE_TIMESTAMP(entries[count-1].timestamp, &last_tm);
        
        time_t first = mktime(&first_tm);
        time_t last = mktime(&last_tm);
        
        if (first != -1 && last != -1) {
            double duration = difftime(last, first);
            printf("Time range: %.0f seconds\n", duration);
        }
    }
}

int main(void) {
    char filename[64];
    printf("Enter log filename: ");
    if (scanf("%63s", filename) != 1) {
        fprintf(stderr, "Invalid filename\n");
        return 1;
    }
    
    LogEntry entries[MAX_ENTRIES];
    int count = read_log_file(filename, entries, MAX_ENTRIES);
    
    if (count < 0) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return 1;
    }
    
    if (count == 0) {
        printf("No valid log entries found\n");
        return 0;
    }
    
    printf("Read %d log entries\n", count);
    analyze_logs(entries, count);
    
    return 0;
}