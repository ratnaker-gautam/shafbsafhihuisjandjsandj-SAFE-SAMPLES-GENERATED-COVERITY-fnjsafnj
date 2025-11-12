//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LEN - 30];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp_line[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN - 1) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= 19 || strlen(level) >= 9 || strlen(message) >= MAX_LINE_LEN - 30) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    
    return 1;
}

void analyze_log_levels(struct LogEntry entries[], int count) {
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    int i = 0;
    while (i < count) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        }
        i++;
    }
    
    printf("Log Level Summary:\n");
    printf("ERROR: %d\n", error_count);
    printf("WARNING: %d\n", warning_count);
    printf("INFO: %d\n", info_count);
}

void find_recent_entries(struct LogEntry entries[], int count, int hours) {
    if (hours <= 0) return;
    
    time_t now;
    time(&now);
    time_t cutoff = now - (hours * 3600);
    
    printf("Entries from last %d hours:\n", hours);
    
    int found = 0;
    for (int i = 0; i < count; i++) {
        struct tm entry_time;
        if (strptime(entries[i].timestamp, "%Y-%m-%dT%H:%M:%S", &entry_time) != NULL) {
            time_t entry_timestamp = mktime(&entry_time);
            if (entry_timestamp != -1 && entry_timestamp >= cutoff) {
                printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
                found = 1;
            }
        }
    }
    
    if (!found) {
        printf("No recent entries found.\n");
    }
}

void search_messages(struct LogEntry entries[], int count, const char* keyword) {
    if (keyword == NULL || strlen(keyword) == 0) return;
    
    printf("Entries containing '%s':\n", keyword);
    
    int found = 0;
    int i = 0;
    do {
        if (strstr(entries[i].message, keyword) != NULL) {
            printf("%s %s %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            found = 1;
        }
        i++;
    } while (i < count);
    
    if (!found) {
        printf("No matches found.\n");
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (empty line to finish):\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        if (line[0] == '\n' || line[0] == '\0') break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    printf("\nAnalysis Results:\n");
    printf("================\n\n");
    
    analyze_log_levels(entries, entry_count);
    printf("\n");
    
    find_recent_entries(entries, entry_count, 24);
    printf("\n");
    
    char search_term[50];
    printf("Enter search term: ");
    if (fgets(search_term, sizeof(search_term), stdin) != NULL) {
        search_term[strcspn(search_term, "\n")] = '\0';
        if (strlen(search_term) > 0) {
            search_messages(entries, entry_count, search_term);
        }
    }
    
    return 0;
}