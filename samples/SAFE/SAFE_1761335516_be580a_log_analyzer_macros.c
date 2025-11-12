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

int parse_timestamp(const char* str, struct tm* tm) {
    return sscanf(str, "%4d-%2d-%2d %2d:%2d:%2d",
                  &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                  &tm->tm_hour, &tm->tm_min, &tm->tm_sec) == 6;
}

int parse_log_level(const char* str) {
    if (strncmp(str, "ERROR", 5) == 0) return LEVEL_ERROR;
    if (strncmp(str, "WARN", 4) == 0) return LEVEL_WARN;
    if (strncmp(str, "INFO", 4) == 0) return LEVEL_INFO;
    if (strncmp(str, "DEBUG", 5) == 0) return LEVEL_DEBUG;
    return -1;
}

int read_log_file(const char* filename, struct log_entry entries[], int max_entries) {
    FILE* file = fopen(filename, "r");
    CHECK_NULL(file);
    
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && count < max_entries) {
        if (strlen(line) >= MAX_LINE_LEN - 1) continue;
        
        struct log_entry* entry = &entries[count];
        struct tm time_info;
        
        if (parse_timestamp(line, &time_info)) {
            strftime(entry->timestamp, sizeof(entry->timestamp), 
                    "%Y-%m-%d %H:%M:%S", &time_info);
            
            char* level_start = line + TIMESTAMP_LEN;
            while (*level_start && isspace(*level_start)) level_start++;
            
            int level = parse_log_level(level_start);
            if (level >= 0) {
                entry->level = level;
                
                char* msg_start = level_start;
                while (*msg_start && !isspace(*msg_start)) msg_start++;
                while (*msg_start && isspace(*msg_start)) msg_start++;
                
                if (*msg_start) {
                    size_t msg_len = strlen(msg_start);
                    if (msg_len > 0 && msg_start[msg_len-1] == '\n') {
                        msg_start[msg_len-1] = '\0';
                    }
                    strncpy(entry->message, msg_start, sizeof(entry->message) - 1);
                    entry->message[sizeof(entry->message) - 1] = '\0';
                    count++;
                }
            }
        }
    }
    
    fclose(file);
    return count;
}

void generate_summary(struct log_entry entries[], int count, int level_counts[]) {
    for (int i = 0; i < LOG_LEVELS; i++) {
        level_counts[i] = 0;
    }
    
    for (int i = 0; i < count; i++) {
        CHECK_BOUNDS(entries[i].level, LOG_LEVELS);
        level_counts[entries[i].level]++;
    }
}

int main(void) {
    struct log_entry entries[MAX_ENTRIES];
    int level_counts[LOG_LEVELS];
    const char* level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};
    
    printf("Enter log file name: ");
    char filename[256];
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len-1] == '\n') {
        filename[len-1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return EXIT_FAILURE;
    }
    
    int entry_count = read_log_file(filename, entries, MAX_ENTRIES);
    if (entry_count < 0) {
        printf("Error reading log file\n");
        return EXIT_FAILURE;
    }
    
    printf("Successfully read %d log entries\n\n", entry_count);
    
    generate_summary(entries, entry_count, level_counts);
    
    printf("Log Level Summary:\n");
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d\n", level_names[i], level_counts[i]);
    }
    
    printf("\nRecent entries:\n");
    int display_count = (entry_count < 5) ? entry_count : 5;
    for (int i = 0; i < display_count; i++) {
        printf("[%s] %s: %s\n", entries[i].timestamp, 
               level_names[entries[i].level], entries[i].message);
    }
    
    return EXIT_SUCCESS;
}