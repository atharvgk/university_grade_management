#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FILENAME "student_data.txt"

// Define a structure for a student
typedef struct Student
{
    char name[50];
    char srn[20];
    float gpa;
    struct Student *prev;
    struct Student *next;
} Student;

// Function to create a new student node
Student *createStudent(const char *name, const char *srn, float gpa)
{
    Student *newStudent = (Student *)malloc(sizeof(Student));
    strcpy(newStudent->name, name);
    strcpy(newStudent->srn, srn);
    newStudent->gpa = gpa;
    newStudent->prev = NULL;
    newStudent->next = NULL;
    return newStudent;
}

// Function to insert a student into the linked list(ME)
void insertStudent(Student *head, const char *name, const char *srn, float gpa)
{
    Student *newStudent = createStudent(name, srn, gpa);
    newStudent->next = head->next;
    if (head->next != NULL)
    {
        head->next->prev = newStudent;
    }
    head->next = newStudent;
    newStudent->prev = head;
}

// Function to display all students and their GPAs
void displayStudents(Student *head)
{
    if (head->next == NULL)
    {
        printf("No data to show.\n");
        return;
    }

    Student *current = head->next;
    while (current != NULL)
    {
        printf("Name: %s, SRN: %s, GPA: %.2f\n", current->name, current->srn, current->gpa);
        current = current->next;
    }
    printf("\n");
}

// Function to search for a student by SRN
Student *searchStudentBySRN(Student *head, const char *srn)
{
    Student *current = head->next;
    while (current != NULL)
    {
        if (strcmp(current->srn, srn) == 0)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Function to update student information
void updateStudentInfo(Student *head, const char *srn, const char *newName, float newGPA)
{
    Student *student = searchStudentBySRN(head, srn);
    if (student != NULL)
    {
        strcpy(student->name, newName);
        student->gpa = newGPA;
        printf("Student information updated successfully.\n");
    }
    else
    {
        printf("Student with SRN %s not found.\n");
    }
}

// Function to delete a student by SRN(ME)
void deleteStudentBySRN(Student *head, const char *srn)
{
    Student *student = searchStudentBySRN(head, srn);
    if (student != NULL)
    {
        if (student->prev != NULL)
        {
            student->prev->next = student->next;
        }
        if (student->next != NULL)
        {
            student->next->prev = student->prev;
        }
        free(student);
        printf("Student with SRN %s deleted.\n");
    }
    else
    {
        printf("Student with SRN %s not found.\n");
    }
}

// Function to display scholarship recipients within a specific GPA range
void displayScholarshipRecipientsInRange(Student *head, const char *scholarshipName, float minGPA, float maxGPA)
{
    if (head->next == NULL)
    {
        printf("No data to show.\n");
        return;
    }

    Student *current = head->next;
    while (current != NULL)
    {
        if (current->gpa >= minGPA && current->gpa < maxGPA)
        {
            printf("Name: %s GPA: %.2f  SRN: %s\n", current->name, current->gpa, current->srn);
        }
        current = current->next;
    }

    printf("\n");
}

// Function to save student data to a file
void saveStudentDataToFile(Student *head)
{
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    Student *current = head->next;
    while (current != NULL)
    {
        fprintf(file, "%s %s %.2f\n", current->name, current->srn, current->gpa);
        current = current->next;
    }

    fclose(file);
}

// Function to load student data from a file(ME)
void loadStudentDataFromFile(Student *head)
{
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    fseek(file, 0, SEEK_END);
    if (ftell(file) == 0)
    {
        fclose(file);
        return;
    }

    fseek(file, 0, SEEK_SET);

    char name[50];
    char name1[50];
    char name2[50];
    char srn[20];
    float gpa;
    if (fscanf(file, "%s %s %f\n", name, srn, &gpa) == 3)
    {
        while (fscanf(file, "%s %s %f\n", name, srn, &gpa) == 3)
        {
            insertStudent(head, name, srn, gpa);
        }
    }
    else if(fscanf(file, "%s %s %s %f\n", name1, name2, srn, &gpa) == 4)
    {
        strcat(name1, name2);
        while (fscanf(file, "%s %s %f\n", name, srn, &gpa) == 3)
        {
            insertStudent(head, name, srn, gpa);
        }
    }
    fclose(file);
}

int main()
{
    Student *head = createStudent("", "", 0.0);
    int choice;
    char name[50];
    char srn[20];
    float gpa;
    float cnrCutoff, mrdCutoff;
    int scholarshipChoice;
    int cutoffsEntered = 0;
    loadStudentDataFromFile(head);
    do
    {
        printf("University Grade Management System\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student by SRN\n");
        printf("4. Update Student Information\n");
        printf("5. Delete Student\n");
        printf("6. Scholarship\n");
        printf("7. Save Student Data to File\n");
        printf("8. Quit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter Student Name: ");
            scanf(" %49[^\n]", name);
            printf("Enter SRN: ");
            scanf("%s", srn);
            printf("Enter GPA: ");
            scanf("%f", &gpa);
            insertStudent(head, name, srn, gpa);
            break;
        case 2:
            displayStudents(head);
            break;
        case 3:
            printf("Enter the SRN to search for: ");
            scanf("%s", srn);
            Student *foundStudent = searchStudentBySRN(head, srn);
            if (foundStudent != NULL)
            {
                printf("Student found:\n");
                printf("Name: %s, SRN: %s, GPA: %.2f\n", foundStudent->name, foundStudent->srn, foundStudent->gpa);
            }
            else
            {
                printf("Student with SRN %s not found.\n");
            }
            break;
        case 4:
            printf("Enter the SRN to update: ");
            scanf("%s", srn);
            printf("Enter the new name: ");
            scanf(" %49[^\n]", name);
            printf("Enter the new GPA: ");
            scanf("%f", &gpa);
            updateStudentInfo(head, srn, name, gpa);
            break;
        case 5:
            printf("Enter the SRN to delete: ");
            scanf("%s", srn);
            deleteStudentBySRN(head, srn);
            break;
        case 6:
            printf("Enter CNR Scholarship Cutoff (GPA): ");
            scanf("%f", &cnrCutoff);
            printf("Enter MRD Scholarship Cutoff (GPA): ");
            scanf("%f", &mrdCutoff);
            printf("CNR Rao Scholarship Recipients:\n");
            displayScholarshipRecipientsInRange(head, "CNR Rao Scholarship", cnrCutoff, 10.0);
            printf("MRD Merit Scholarship Recipients:\n");
            displayScholarshipRecipientsInRange(head, "MRD Merit Scholarship", mrdCutoff, cnrCutoff);
            printf("DAC Scholarship Recipients:\n");
            displayScholarshipRecipientsInRange(head, "DAC Scholarship", 7.75, mrdCutoff);
            break;
        case 7:
            saveStudentDataToFile(head);
            printf("Student data saved to file.\n");
            break;
        case 8:
            printf("Exiting the program. Goodbye!\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 8);
    while (head->next != NULL)
    {
        Student *temp = head->next;
        head->next = head->next->next;
        free(temp);
    }

    free(head);

    return 0;
}