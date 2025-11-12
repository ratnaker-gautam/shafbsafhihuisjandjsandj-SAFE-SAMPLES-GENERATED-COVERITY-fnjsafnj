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

#define IS_VALID_LEVEL(lvl) (strcmp(lvl, "INFO") == 0 || strcmp(lvl, "WARN") == 0 || strcmp(lvl, "ERROR") == 0)

int parse_timestamp(const char* ts) {
    struct tm tm = {0};
    return strptime(ts, "%Y-%m-%d %H:%M:%S", &tm) != NULL;
}

int read_log_entries(const char* filename, LogEntry entries[], int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) return -1;
    
    int count = 0;
    char line[MAX_LINE_LEN];
    
    while (count < max_entries && fgets(line, sizeof(line), file)) {
        char ts[TIMESTAMP_LEN], lvl[LEVEL_LEN], msg[MAX_LINE_LEN];
        
        if (sscanf(line, "%19[^|]|%9[^|]|%255[^\n]", ts, lvl, msg) == 3) {
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
    
    fclose(file);
    return count;
}

#define COUNT_LEVEL(entries, count, target) do { \
    int c = 0; \
    for (int i = 0; i < count; i++) { \
        if (strcmp(entries[i].level, target) == 0) c++; \
    } \
    printf("%s: %d\n", target, c); \
} while(0)

void generate_summary(LogEntry entries[], int count) {
    if (count <= 0) {
        printf("No valid log entries found.\n");
        return;
    }
    
    printf("Log Summary:\n");
    printf("Total entries: %d\n", count);
    printf("Entries by level:\n");
    COUNT_LEVEL(entries, count, "INFO");
    COUNT_LEVEL(entries, count, "WARN");
    COUNT_LEVEL(entries, count, "ERROR");
    
    printf("\nFirst 5 entries:\n");
    int display_count = count < 5 ? count : 5;
    for (int i = 0; i < display_count; i++) {
        printf("%s | %s | %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
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
    
    generate_summary(entries, count);
    return 0;
}