//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 256
#define MAX_ENTRIES 100

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH - 30];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strncpy(temp, line, MAX_LINE_LENGTH - 1);
    temp[MAX_LINE_LENGTH - 1] = '\0';
    
    int i = 0;
    while (temp[i] != '\0' && i < 19) {
        if (temp[i] == ' ') break;
        entry->timestamp[i] = temp[i];
        i++;
    }
    entry->timestamp[i] = '\0';
    
    while (temp[i] == ' ' && temp[i] != '\0') i++;
    
    int j = 0;
    while (temp[i] != '\0' && temp[i] != ' ' && j < 9) {
        entry->level[j] = temp[i];
        i++;
        j++;
    }
    entry->level[j] = '\0';
    
    while (temp[i] == ' ' && temp[i] != '\0') i++;
    
    j = 0;
    while (temp[i] != '\0' && j < MAX_LINE_LENGTH - 31) {
        entry->message[j] = temp[i];
        i++;
        j++;
    }
    entry->message[j] = '\0';
    
    return 1;
}

int count_log_level(const struct LogEntry* entries, int count, const char* level) {
    int total = 0;
    int k = 0;
    do {
        if (strcmp(entries[k].level, level) == 0) {
            total++;
        }
        k++;
    } while (k < count);
    return total;
}

void print_statistics(const struct LogEntry* entries, int count) {
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int num_levels = 4;
    
    int m;
    for (m = 0; m < num_levels; m++) {
        int cnt = count_log_level(entries, count, levels[m]);
        printf("%s: %d\n", levels[m], cnt);
    }
}

int main() {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Enter log entries (empty line to finish):\n");
    
    char line[MAX_LINE_LENGTH];
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, MAX_LINE_LENGTH, stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (line[0] == '\0') break;
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format. Expected: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    print_statistics(entries, entry_count);
    
    printf("\nSearch for entries by level: ");
    char search_level[10];
    if (fgets(search_level, sizeof(search_level), stdin) != NULL) {
        size_t slen = strlen(search_level);
        if (slen > 0 && search_level[slen - 1] == '\n') {
            search_level[slen - 1] = '\0';
        }
        
        printf("Matching entries:\n");
        int found = 0;
        int n = 0;
        while (n < entry_count) {
            if (strcmp(entries[n].level, search_level) == 0) {
                printf("%s %s %s\n", entries[n].timestamp, entries[n].level, entries[n].message);
                found = 1;
            }
            n++;
        }
        
        if (!found) {
            printf("No entries found for level: %s\n", search_level);
        }
    }
    
    return 0;
}