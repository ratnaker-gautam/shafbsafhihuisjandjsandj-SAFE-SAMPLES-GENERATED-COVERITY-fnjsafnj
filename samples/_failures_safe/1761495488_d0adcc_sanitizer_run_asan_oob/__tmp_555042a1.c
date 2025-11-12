//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LEVEL_LEN 10

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    char level[LEVEL_LEN];
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - LEVEL_LEN - 3];
} LogEntry;

#define VALID_LEVEL(level) (strcmp(level, "INFO") == 0 || strcmp(level, "WARN") == 0 || strcmp(level, "ERROR") == 0)

int parse_timestamp(const char* ts) {
    struct tm tm = {0};
    return strptime(ts, "%Y-%m-%d %H:%M:%S", &tm) != NULL;
}

int read_log_entries(LogEntry entries[], int max_entries) {
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (count < max_entries && fgets(line, sizeof(line), stdin)) {
        if (strlen(line) == 0 || line[0] == '\n') continue;
        
        char timestamp[TIMESTAMP_LEN];
        char level[LEVEL_LEN];
        char message[MAX_LINE_LEN - TIMESTAMP_LEN - LEVEL_LEN - 3];
        
        int scanned = sscanf(line, "%19s %9s %255[^\n]", timestamp, level, message);
        if (scanned != 3) continue;
        
        if (!parse_timestamp(timestamp)) continue;
        if (!VALID_LEVEL(level)) continue;
        if (strlen(message) >= sizeof(entries[0].message)) continue;
        
        strncpy(entries[count].timestamp, timestamp, sizeof(entries[0].timestamp) - 1);
        strncpy(entries[count].level, level, sizeof(entries[0].level) - 1);
        strncpy(entries[count].message, message, sizeof(entries[0].message) - 1);
        
        entries[count].timestamp[sizeof(entries[0].timestamp) - 1] = '\0';
        entries[count].level[sizeof(entries[0].level) - 1] = '\0';
        entries[count].message[sizeof(entries[0].message) - 1] = '\0';
        
        count++;
    }
    
    return count;
}

#define COUNT_LEVEL(entries, count, target_level, result) \
    do { \
        result = 0; \
        for (int i = 0; i < count; i++) { \
            if (strcmp(entries[i].level, target_level) == 0) { \
                result++; \
            } \
        } \
    } while(0)

#define FIND_LONGEST_MESSAGE(entries, count, result_index) \
    do { \
        result_index = -1; \
        size_t max_len = 0; \
        for (int i = 0; i < count; i++) { \
            size_t len = strlen(entries[i].message); \
            if (len > max_len) { \
                max_len = len; \
                result_index = i; \
            } \
        } \
    } while(0)

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message):\n");
    printf("Press Ctrl+D to finish input.\n");
    
    int count = read_log_entries(entries, MAX_ENTRIES);
    
    if (count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries: %d\n", count);
    
    int info_count, warn_count, error_count;
    COUNT_LEVEL(entries, count, "INFO", info_count);
    COUNT_LEVEL(entries, count, "WARN", warn_count);
    COUNT_LEVEL(entries, count, "ERROR", error_count);
    
    printf("INFO entries: %d\n", info_count);
    printf("WARN entries: %d\n", warn_count);
    printf("ERROR entries: %d\n", error_count);
    
    int longest_idx;
    FIND_LONGEST_MESSAGE(entries, count, longest_idx);
    
    if (longest_idx >= 0) {
        printf("Longest message (%zu chars): %s\n", 
               strlen(entries[longest_idx].message), entries[longest_idx].message);
    }
    
    printf("\nRecent entries (last 5):\n");
    int start = count > 5 ? count - 5 : 0;
    for (int i = start; i < count; i++) {
        printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
    
    return 0;
}