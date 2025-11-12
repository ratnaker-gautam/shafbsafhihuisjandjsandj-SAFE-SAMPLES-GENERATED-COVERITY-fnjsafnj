//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[256];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*);

void count_entries(LogEntry* entry) {
    static int count = 0;
    count++;
    printf("Processed %d entries\n", count);
}

void show_errors(LogEntry* entry) {
    if (strcmp(entry->level, "ERROR") == 0) {
        printf("ERROR: %s - %s\n", entry->timestamp, entry->message);
    }
}

void show_warnings(LogEntry* entry) {
    if (strcmp(entry->level, "WARN") == 0) {
        printf("WARNING: %s - %s\n", entry->timestamp, entry->message);
    }
}

void parse_log_line(const char* line, LogEntry* entry) {
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) {
        return;
    }
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';

    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");

    if (timestamp && level && message) {
        strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
        entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
        strncpy(entry->level, level, sizeof(entry->level) - 1);
        entry->level[sizeof(entry->level) - 1] = '\0';
        strncpy(entry->message, message, sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\0';
    }
}

int read_log_file(const char* filename, LogEntry entries[], int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    int count = 0;

    while (fgets(line, sizeof(line), file) && count < max_entries) {
        if (strlen(line) > 0) {
            line[strcspn(line, "\n")] = '\0';
            parse_log_line(line, &entries[count]);
            count++;
        }
    }

    fclose(file);
    return count;
}

void process_logs(LogEntry entries[], int count, LogProcessor processor) {
    if (!processor || count <= 0) {
        return;
    }

    for (int i = 0; i < count; i++) {
        processor(&entries[i]);
    }
}

int main() {
    LogEntry entries[MAX_ENTRIES];
    int entry_count;
    
    printf("Enter log filename: ");
    char filename[256];
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    filename[strcspn(filename, "\n")] = '\0';

    if (strlen(filename) == 0) {
        printf("Error: Empty filename\n");
        return 1;
    }

    entry_count = read_log_file(filename, entries, MAX_ENTRIES);
    if (entry_count == 0) {
        printf("No log entries found or file error\n");
        return 1;
    }

    printf("\nChoose processing mode:\n");
    printf("1. Count entries\n");
    printf("2. Show errors\n");
    printf("3. Show warnings\n");
    printf("Enter choice: ");

    char choice_str[16];
    if (fgets(choice_str, sizeof(choice_str), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }

    int choice = atoi(choice_str);
    LogProcessor processors[] = {count_entries, show_errors, show_warnings};
    
    if (choice >= 1 && choice <= 3) {
        process_logs(entries, entry_count, processors[choice - 1]);
    } else {
        printf("Invalid choice\n");
        return 1;
    }

    return 0;
}