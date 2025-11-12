//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp_line, line);
    
    char* token = strtok(temp_line, " ");
    if (token == NULL) return 0;
    if (strlen(token) >= sizeof(entry->timestamp)) return 0;
    strcpy(entry->timestamp, token);
    
    token = strtok(NULL, " ");
    if (token == NULL) return 0;
    if (strlen(token) >= sizeof(entry->level)) return 0;
    strcpy(entry->level, token);
    
    token = strtok(NULL, "\n");
    if (token == NULL) return 0;
    if (strlen(token) >= sizeof(entry->message)) return 0;
    strcpy(entry->message, token);
    
    return 1;
}

int count_entries_by_level(struct LogEntry* entries, int count, const char* level) {
    int total = 0;
    int i = 0;
    while (i < count) {
        if (strcmp(entries[i].level, level) == 0) {
            total++;
        }
        i++;
    }
    return total;
}

void print_level_summary(struct LogEntry* entries, int count) {
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int level_count = sizeof(levels) / sizeof(levels[0]);
    
    printf("Log Level Summary:\n");
    int j = 0;
    do {
        int count_level = count_entries_by_level(entries, count, levels[j]);
        printf("%s: %d\n", levels[j], count_level);
        j++;
    } while (j < level_count);
}

int find_most_frequent_level(struct LogEntry* entries, int count) {
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int level_count = sizeof(levels) / sizeof(levels[0]);
    int counts[4] = {0};
    
    int k = 0;
    for (; k < count; k++) {
        int m = 0;
        while (m < level_count) {
            if (strcmp(entries[k].level, levels[m]) == 0) {
                counts[m]++;
                break;
            }
            m++;
        }
    }
    
    int max_index = 0;
    int n = 1;
    for (; n < level_count; n++) {
        if (counts[n] > counts[max_index]) {
            max_index = n;
        }
    }
    
    return max_index;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    char line[MAX_LINE_LEN];
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE), empty line to finish:\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strlen(line) == 0) break;
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Use: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    printf("\nParsed %d log entries:\n", entry_count);
    int i = 0;
    while (i < entry_count) {
        printf("%d: %s %s %s\n", i + 1, entries[i].timestamp, entries[i].level, entries[i].message);
        i++;
    }
    
    printf("\n");
    print_level_summary(entries, entry_count);
    
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int most_frequent = find_most_frequent_level(entries, entry_count);
    printf("\nMost frequent log level: %s\n", levels[most_frequent]);
    
    printf("\nSearch for entries by level: ");
    char search_level[16];
    if (fgets(search_level, sizeof(search_level), stdin) != NULL) {
        size_t len = strlen(search_level);
        if (len > 0 && search_level[len - 1] == '\n') {
            search_level[len - 1] = '\0';
        }
        
        printf("Entries with level '%s':\n", search_level);
        int found = 0;
        int j = 0;
        for (; j < entry_count; j++) {
            if (strcmp(entries[j].level, search_level) == 0) {
                printf("- %s %s\n", entries[j].timestamp, entries[j].message);