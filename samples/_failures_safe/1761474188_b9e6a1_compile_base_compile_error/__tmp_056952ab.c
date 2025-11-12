//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH - 30];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp, line);
    
    if (sscanf(temp, "%19s %9s", entry->timestamp, entry->level) != 2) {
        return 0;
    }
    
    const char* msg_start = strchr(line, ' ');
    if (msg_start == NULL) return 0;
    msg_start = strchr(msg_start + 1, ' ');
    if (msg_start == NULL) return 0;
    msg_start++;
    
    size_t msg_len = strlen(msg_start);
    if (msg_len >= sizeof(entry->message)) return 0;
    strcpy(entry->message, msg_start);
    
    for (int i = 0; entry->level[i]; i++) {
        entry->level[i] = toupper(entry->level[i]);
    }
    
    return 1;
}

int read_log_file(struct LogEntry entries[], int max_entries) {
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish.\n");
    
    while (count < max_entries) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) break;
        if (strlen(line) == 0) continue;
        
        if (parse_log_line(line, &entries[count])) {
            count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    return count;
}

void analyze_log_levels(const struct LogEntry entries[], int count) {
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    int other_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[i].level, "INFO") == 0) {
            info_count++;
        } else {
            other_count++;
        }
    }
    
    printf("\nLog Level Analysis:\n");
    printf("ERROR: %d\n", error_count);
    printf("WARNING: %d\n", warning_count);
    printf("INFO: %d\n", info_count);
    printf("OTHER: %d\n", other_count);
}

void search_messages(const struct LogEntry entries[], int count, const char* keyword) {
    if (keyword == NULL || strlen(keyword) == 0) return;
    
    printf("\nMessages containing '%s':\n", keyword);
    int found = 0;
    
    for (int i = 0; i < count; i++) {
        if (strstr(entries[i].message, keyword) != NULL) {
            printf("[%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
            found++;
        }
    }
    
    if (found == 0) {
        printf("No messages found containing '%s'\n", keyword);
    }
}

void display_recent_entries(const struct LogEntry entries[], int count, int num_entries) {
    if (num_entries <= 0) return;
    if (num_entries > count) num_entries = count;
    
    printf("\nLast %d log entries:\n", num_entries);
    
    int start = count - num_entries;
    if (start < 0) start = 0;
    
    for (int i = start; i < count; i++) {
        printf("[%s] %s: %s\n", entries[i].timestamp, entries[i].level, entries[i].message);
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Log Analyzer\n");
    printf("============\n");
    
    entry_count = read_log_file(entries, MAX_ENTRIES);
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    printf("\nSuccessfully read %d log entries.\n", entry_count);
    
    analyze_log_levels(entries, entry_count);
    
    char search_term[100];
    printf("\nEnter search term (or press Enter to skip): ");
    if (fgets(search_term, sizeof(search_term), stdin) != NULL) {
        search_term[strcspn(search_term, "\n")] = 0;
        if (strlen(search_term) > 0) {
            search_messages(entries, entry_count, search_term);