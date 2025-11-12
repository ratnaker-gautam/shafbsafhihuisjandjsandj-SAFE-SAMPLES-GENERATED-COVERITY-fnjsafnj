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
        char ts[TIMESTAMP_LEN], level[8], msg[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
        
        if (sscanf(line, "%19s %7s %255[^\n]", ts, level, msg) == 3) {
            if (IS_VALID_LEVEL(level)) {
                strncpy(entries[count].timestamp, ts, TIMESTAMP_LEN - 1);
                entries[count].timestamp[TIMESTAMP_LEN - 1] = '\0';
                strncpy(entries[count].level, level, 7);
                entries[count].level[7] = '\0';
                strncpy(entries[count].message, msg, sizeof(entries[count].message) - 1);
                entries[count].message[sizeof(entries[count].message) - 1] = '\0';
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
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d (%.1f%%)\n", levels[i], level_counts[i], 
               count > 0 ? (level_counts[i] * 100.0 / count) : 0.0);
    }
    
    if (count > 0) {
        struct tm start_tm = {0}, end_tm = {0};
        PARSE_TIMESTAMP(entries[0].timestamp, &start_tm);
        PARSE_TIMESTAMP(entries[count-1].timestamp, &end_tm);
        
        time_t start_time = mktime(&start_tm);
        time_t end_time = mktime(&end_tm);
        
        if (start_time != -1 && end_time != -1) {
            double duration = difftime(end_time, start_time);
            printf("Time range: %.0f seconds\n", duration);
            printf("Entries per second: %.2f\n", count / (duration > 0 ? duration : 1));
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    LogEntry entries[MAX_ENTRIES];
    int count = read_log_file(argv[1], entries, MAX_ENTRIES);
    
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