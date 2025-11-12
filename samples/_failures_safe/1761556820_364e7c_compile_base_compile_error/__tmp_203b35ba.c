//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
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
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 12];
} LogEntry;

int read_log_file(const char* filename, LogEntry entries[], int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) return -1;
    
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) && count < max_entries) {
        if (strlen(line) >= sizeof(line) - 1) continue;
        
        char ts[TIMESTAMP_LEN], level[8], message[MAX_LINE_LEN];
        int parsed = sscanf(line, "%19s %7s %[^\n]", ts, level, message);
        
        if (parsed == 3 && IS_VALID_LEVEL(level)) {
            strncpy(entries[count].timestamp, ts, TIMESTAMP_LEN - 1);
            entries[count].timestamp[TIMESTAMP_LEN - 1] = '\0';
            strncpy(entries[count].level, level, 7);
            entries[count].level[7] = '\0';
            strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
            entries[count].message[sizeof(entries[count].message) - 1] = '\0';
            count++;
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
        printf("%s: %d\n", levels[i], level_counts[i]);
    }
    
    if (count > 0) {
        struct tm first_tm = {0}, last_tm = {0};
        if (PARSE_TIMESTAMP(entries[0].timestamp, &first_tm) && 
            PARSE_TIMESTAMP(entries[count-1].timestamp, &last_tm)) {
            time_t first = mktime(&first_tm);
            time_t last = mktime(&last_tm);
            if (first != -1 && last != -1) {
                double hours = difftime(last, first) / 3600.0;
                printf("Time span: %.2f hours\n", hours);
            }
        }
    }
}

void search_logs(LogEntry entries[], int count, const char* keyword) {
    printf("Search results for '%s':\n", keyword);
    int found = 0;
    
    for (int i = 0; i < count; i++) {
        if (strstr(entries[i].message, keyword) != NULL) {
            printf("[%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            found++;
        }
    }
    
    if (!found) {
        printf("No matches found.\n");
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    char filename[256];
    char keyword[256];
    int choice;
    
    printf("Enter log filename: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) return 1;
    filename[strcspn(filename, "\n")] = '\0';
    
    int count = read_log_file(filename, entries, MAX_ENTRIES);
    if (count <= 0) {
        printf("Error: Could not read log file or no valid entries found.\n");
        return 1;
    }
    
    printf("Loaded %d log entries.\n", count);
    
    while (1) {
        printf("\n1. Analyze logs\n2. Search logs\n3. Exit\nChoice: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input.\n");
            continue;
        }
        while (getchar() != '\n');
        
        switch (choice) {
            case 1:
                analyze_logs(entries, count);
                break;
            case 2:
                printf("Enter search keyword: ");
                if (