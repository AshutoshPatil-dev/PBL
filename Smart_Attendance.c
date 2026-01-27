#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Student {
    int roll;
    char name[50];
    char div[10];
    char course[20];
};

void addStudent();
void viewStudents();
void markAttendance();
void viewAttendance();
void studentReport();
int studentExists(int roll);

char* getDate() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    static char date[20];
    sprintf(date, "%02d-%02d-%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    return date;
}

int main() {
    int ch;

    while (1) {
        printf("\n==============================\n");
        printf("   SMART ATTENDANCE SYSTEM\n");
        printf("==============================\n");
        printf("1. Add Student\n");
        printf("2. View Students\n");
        printf("3. Mark Attendance\n");
        printf("4. View Attendance Records\n");
        printf("5. Individual Student Report\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1: addStudent(); break;
            case 2: viewStudents(); break;
            case 3: markAttendance(); break;
            case 4: viewAttendance(); break;
            case 5: studentReport(); break;
            case 6: printf("Exiting...\n"); exit(0);
            default: printf("Invalid choice.\n");
        }
    }

    return 0;
}

void addStudent() {
    struct Student s;
    FILE *fp = fopen("students.txt", "a");

    printf("\nEnter Roll Number: ");
    scanf("%d", &s.roll);

    if (studentExists(s.roll)) {
        printf("Student with this roll number already exists!\n");
        fclose(fp);
        return;
    }

    printf("Enter Name: ");
    scanf("%s", s.name);

    printf("Enter Division: ");
    scanf("%s", s.div);

    printf("Enter Course: ");
    scanf("%s", s.course);

    fprintf(fp, "%d %s %s %s\n", s.roll, s.name, s.div, s.course);
    fclose(fp);

    printf("Student added successfully!\n");
}

void viewStudents() {
    struct Student s;
    FILE *fp = fopen("students.txt", "r");

    if (!fp) {
        printf("No student records found!\n");
        return;
    }

    printf("\n--- Student List ---\n");
    while (fscanf(fp, "%d %s %s %s", &s.roll, s.name, s.div, s.course) != EOF) {
        printf("Roll: %d | Name: %s | Div: %s | Course: %s\n",
               s.roll, s.name, s.div, s.course);
    }

    fclose(fp);
}

void markAttendance() {
    FILE *fp = fopen("attendance.txt", "a");
    FILE *fs = fopen("students.txt", "r");

    if (!fs) {
        printf("No students available. Add students first.\n");
        return;
    }

    struct Student s;
    char status;
    char* date = getDate();

    printf("\n--- Mark Attendance for %s ---\n", date);

    while (fscanf(fs, "%d %s %s %s", &s.roll, s.name, s.div, s.course) != EOF) {
        printf("Roll %d (%s): Present(P) / Absent(A): ", s.roll, s.name);
        scanf(" %c", &status);
        fprintf(fp, "%d %s %c\n", s.roll, date, status);
    }

    fclose(fp);
    fclose(fs);
    printf("Attendance saved!\n");
}

void viewAttendance() {
    FILE *fp = fopen("attendance.txt", "r");

    if (!fp) {
        printf("No attendance records found!\n");
        return;
    }

    int roll;
    char date[20], status;

    printf("\n--- Attendance Records ---\n");
    while (fscanf(fp, "%d %s %c", &roll, date, &status) != EOF) {
        printf("Roll: %d | Date: %s | Status: %c\n", roll, date, status);
    }

    fclose(fp);
}

void studentReport() {
    int roll;
    printf("\nEnter Roll Number: ");
    scanf("%d", &roll);

    if (!studentExists(roll)) {
        printf("Student not found!\n");
        return;
    }

    FILE *fp = fopen("attendance.txt", "r");

    int r;
    char date[20], status;
    int present = 0, total = 0;

    while (fscanf(fp, "%d %s %c", &r, date, &status) != EOF) {
        if (r == roll) {
            total++;
            if (status == 'P') present++;
        }
    }

    fclose(fp);

    if (total == 0) {
        printf("No attendance marked for this student yet.\n");
        return;
    }

    float percent = (present * 100.0) / total;

    printf("\n--- Attendance Report ---\n");
    printf("Roll: %d\n", roll);
    printf("Total Classes: %d\n", total);
    printf("Present: %d\n", present);
    printf("Absent: %d\n", total - present);
    printf("Percentage: %.2f%%\n", percent);

    if (percent < 75)
        printf("Status: DETAINED\n");
    else
        printf("Status: ALLOWED\n");
}

int studentExists(int roll) {
    struct Student s;
    FILE *fp = fopen("students.txt", "r");
    if (!fp) return 0;

    while (fscanf(fp, "%d %s %s %s", &s.roll, s.name, s.div, s.course) != EOF) {
        if (s.roll == roll) {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}
