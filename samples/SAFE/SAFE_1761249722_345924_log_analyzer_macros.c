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

#define LEVEL_ERROR 0
#define LEVEL_WARN 1
#define LEVEL_INFO 2
#define LEVEL_DEBUG 3

#define CHECK_NULL(ptr) if ((ptr) == NULL) return EXIT_FAILURE
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) return EXIT_FAILURE

struct log_entry {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
};

const char* level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};

int parse_timestamp(const char* str, struct tm* tm) {
    return sscanf(str, "%4d-%2d-%2d %2d:%2d:%2d",
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int parse_log_level(const char* str) {
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strcmp(str, level_names[i]) == 0) {
            return i;
        }
    }
    return -1;
}

int read_log_file(const char* filename, struct log_entry entries[], int max_entries) {
    FILE* file = fopen(filename, "r");
    CHECK_NULL(file);
    
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && count < max_entries) {
        if (strlen(line) >= MAX_LINE_LEN - 1) continue;
        
        char timestamp[TIMESTAMP_LEN];
        char level_str[10];
        char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
        
        int parsed = sscanf(line, "%19s %9s %[^\n]", timestamp, level_str, message);
        if (parsed != 3) continue;
        
        struct tm time_info;
        if (!parse_timestamp(timestamp, &time_info)) continue;
        
        int level = parse_log_level(level_str);
        if (level == -1) continue;
        
        strncpy(entries[count].timestamp, timestamp, TIMESTAMP_LEN - 1);
        entries[count].timestamp[TIMESTAMP_LEN - 1] = '\0';
        entries[count].level = level;
        strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
        entries[count].message[sizeof(entries[count].message) - 1] = '\0';
        
        count++;
    }
    
    fclose(file);
    return count;
}

void analyze_logs(struct log_entry entries[], int count) {
    int level_counts[LOG_LEVELS] = {0};
    int total_entries = 0;
    
    for (int i = 0; i < count; i++) {
        CHECK_BOUNDS(entries[i].level, LOG_LEVELS);
        level_counts[entries[i].level]++;
        total_entries++;
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", total_entries);
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        double percentage = total_entries > 0 ? (level_counts[i] * 100.0) / total_entries : 0.0;
        printf("%s: %d (%.1f%%)\n", level_names[i], level_counts[i], percentage);
    }
    
    if (count > 0) {
        printf("\nFirst entry: %s %s %s\n", 
               entries[0].timestamp, level_names[entries[0].level], entries[0].message);
        printf("Last entry: %s %s %s\n", 
               entries[count-1].timestamp, level_names[entries[count-1].level], entries[count-1].message);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    struct log_entry entries[MAX_ENTRIES];
    int count = read_log_file(argv[1], entries, MAX_ENTRIES);
    
    if (count <= 0) {
        printf("No valid log entries found or error reading file.\n");
        return EXIT_FAILURE;
    }
    
    analyze_logs(entries, count);
    return EXIT_SUCCESS;
}