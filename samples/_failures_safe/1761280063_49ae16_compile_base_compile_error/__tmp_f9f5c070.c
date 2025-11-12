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

#define PARSE_TIMESTAMP(ts, tm_struct) \
    (strptime(ts, "%Y-%m-%d %H:%M:%S", tm_struct) != NULL)

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level[8];
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

int validate_timestamp(const char* ts) {
    struct tm tm_struct = {0};
    return PARSE_TIMESTAMP(ts, &tm_struct);
}

int read_log_entries(LogEntry entries[], int max_entries, FILE* file) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (count < max_entries && fgets(line, sizeof(line), file)) {
        if (line[0] == '\n' || line[0] == '\0') {
            break;
        }
        char ts[TIMESTAMP_LEN], level[8], message[MAX_LINE_LEN];
        
        if (sscanf(line, "%19s %7s %255[^\n]", ts, level, message) == 3) {
            if (validate_timestamp(ts) && IS_VALID_LEVEL(level)) {
                strncpy(entries[count].timestamp, ts, TIMESTAMP_LEN - 1);
                entries[count].timestamp[TIMESTAMP_LEN - 1] = '\0';
                strncpy(entries[count].level, level, 7);
                entries[count].level[7] = '\0';
                strncpy(entries[count].message, message, MAX_LINE_LEN - TIMESTAMP_LEN - 10 - 1);
                entries[count].message[MAX_LINE_LEN - TIMESTAMP_LEN - 10 - 1] = '\0';
                count++;
            }
        }
    }
    
    return count;
}

void analyze_log_levels(const LogEntry entries[], int count, int level_counts[]) {
    for (int i = 0; i < LOG_LEVELS; i++) {
        level_counts[i] = 0;
    }
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "INFO") == 0) level_counts[0]++;
        else if (strcmp(entries[i].level, "WARN") == 0) level_counts[1]++;
        else if (strcmp(entries[i].level, "ERROR") == 0) level_counts[2]++;
        else if (strcmp(entries[i].level, "DEBUG") == 0) level_counts[3]++;
    }
}

void find_most_frequent_message(const LogEntry entries[], int count, char* most_freq_msg, int* max_count) {
    *max_count = 0;
    most_freq_msg[0] = '\0';
    
    for (int i = 0; i < count; i++) {
        int current_count = 0;
        for (int j = 0; j < count; j++) {
            if (strcmp(entries[i].message, entries[j].message) == 0) {
                current_count++;
            }
        }
        if (current_count > *max_count) {
            *max_count = current_count;
            strncpy(most_freq_msg, entries[i].message, MAX_LINE_LEN - TIMESTAMP_LEN - 10 - 1);
            most_freq_msg[MAX_LINE_LEN - TIMESTAMP_LEN - 10 - 1] = '\0';
        }
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int level_counts[LOG_LEVELS];
    char most_freq_msg[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
    int max_freq_count;
    
    printf("Enter log data (timestamp level message, one per line):\n");
    printf("Format: YYYY-MM-DD HH:MM:SS LEVEL message_text\n");
    printf("Supported levels: INFO, WARN, ERROR, DEBUG\n");
    printf("Enter empty line to finish input.\n");
    
    int entry_count = read_log_entries(entries, MAX_ENTRIES, stdin);
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    analyze_log_levels(entries, entry_count, level_counts);
    find_most_frequent_message(entries, entry_count, most_freq_msg, &max_freq_count);
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    printf("INFO entries: %d\n", level_counts[0]);
    printf("WARN entries: %d\n", level_counts[1]);
    printf("ERROR entries: %d\n", level_counts[2]);
    printf("DEBUG entries: %d\n", level_counts[3]);
    
    if (max_freq_count > 1) {