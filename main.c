#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxStudents 40
#define maxIdNumberLength 10
#define maxQuizes 15
#define maxNameLength 25
#define red "\033[31m"
#define green "\033[32m"
#define white "\033[0m"

typedef struct Students
{
    char name[maxNameLength];
    char idNumber[maxIdNumberLength];
    int passedQuizes;
    int quizes[maxQuizes];
    int activities;
    int exam;
    float gwa;
    char remark[8];
} Students;

void checkAdminFileExist();
void createNewAccount();
void landingPage(int *selected);
void checkChoices(int *selected);
void addStudent();
void removeStudent();
void editStudentInfo();
void menu(int *choiceMenu);
void checkAttendanceFileExist();
void recordAttendance();
void viewAttendance();
void addQuiz();
void removeQuiz();
void viewQuizzes();
void addOrUpdateScore();
void computeGWA();
void generateReports();
int login(int *verified, int *exitLandingLoop);
void checkStudentRecords();
void viewStudentList();
void checkQuizzesFileExist();
void addOrUpdateExamScore();
void viewExamScore();
void checkGradingFileExist();
void editGradingSystem();

typedef enum start
{
    NONE,
    LOGIN,
    CREATE,
    EXIT
} Start;

typedef enum choice
{
    none,
    ADD,
    REMOVE,
    EDIT,
    VIEW,
    ATT,
    VATT,
    ADDQ,
    REMQ,
    VIEQ,
    AUSQ,
    VIES,
    AUES,
    GWA,
    GR,
    EGS,
    STOP
} Choice;

Start land;
Choice options;

float gradingQuizPercent = 40.0f;
float gradingExamPercent = 60.0f;

int main()
{
    printf("\033[33mStarting Application...\033[0m\n");

    checkAdminFileExist();
    checkStudentRecords();
    checkAttendanceFileExist();
    checkQuizzesFileExist();
    checkGradingFileExist();

    int choiceLanding = NONE;
    int exitLandingLoop = 0;
    Students student;
    int verified = 0;
    do
    {
        landingPage(&choiceLanding);
        switch (choiceLanding)
        {
        case LOGIN:
            login(&verified, &exitLandingLoop);
            break;
        case CREATE:
            createNewAccount();
            break;
        case EXIT:
            printf("EXITING PROGRAM...\n");
            return 0;
        default:
            printf("Invalid Option\n");
            break;
        }
    } while (!exitLandingLoop);

    if (verified)
    {
        int choiceMenu = none;
        do
        {
            menu(&choiceMenu);
            checkChoices(&choiceMenu);
        } while (choiceMenu != STOP);
    }
    printf("\nExited the app...\n");

    return 0;
}

void landingPage(int *choiceLanding)
{
    printf("\n=========WELCOME TO ACADEX==========\n\n");
    printf("1) LOG-IN\n");
    printf("2) CREATE NEW ACCOUNT\n");
    printf("3) EXIT PROGRAM\n\n");
    printf("SELECT: ");
    scanf("%d", choiceLanding);
    printf("\n");
}

void checkAdminFileExist()
{
    FILE *usercredentials = fopen("Admin.txt", "r");
    if (usercredentials == NULL)
    {
        printf(red "ERROR  : Admin File Doesn't Exist!\n" white);
    }
    else
    {
        printf(green "SUCCESS: Admin File Found!\n" white);
        fclose(usercredentials);
    }
}

void checkAttendanceFileExist()
{
    FILE *attendanceRecords = fopen("Attendance.txt", "r");
    if (attendanceRecords == NULL)
    {
        printf(red "ERROR  : Attendance File Doesn't Exist!\n" white);
    }
    else
    {
        printf(green "SUCCESS: Attendance File Found!\n" white);
        fclose(attendanceRecords);
    }
}

int login(int *verified, int *exitLandingLoop)
{
    char adminUsername[maxNameLength];
    char adminPassword[maxNameLength];
    char username[maxNameLength];
    char password[maxNameLength];

    FILE *usercredential = fopen("Admin.txt", "r");
    if (!usercredential)
    {
        printf(red "ERROR: Admin file not found! Cannot Login, No Existing User! Create An Account to Continue\n" white);
        return 0;
    }

    printf("Enter Username: ");
    scanf(" %[^\n]", username);
    printf("Enter Password: ");
    scanf(" %[^\n]", password);

    if (fscanf(usercredential, "%[^|]|%s", adminUsername, adminPassword) < 2)
    {
        fclose(usercredential);
        printf(red "ERROR: Admin file malformed.\n" white);
        return 0;
    }
    fclose(usercredential);

    if (strcmp(adminUsername, username) != 0 || strcmp(adminPassword, password) != 0)
    {
        printf(red "ERROR: Credentials don't match!\n" white);
        return 0;
    }

    printf(green "SUCCESS: Credentials Match!\n\n" white);
    *verified = 1;
    *exitLandingLoop = 1;
    return 1;
}

void createNewAccount()
{
    FILE *usercredential = fopen("Admin.txt", "w");
    if (usercredential == NULL)
    {
        printf(red "ERROR: Failed To Create Admin File!\n" white);
        return;
    }
    else
    {
        printf(green "SUCCESS: Admin File Created\n" white);
    }

    char username[maxNameLength];
    char password[maxNameLength];

    printf("Please Enter Username: ");
    scanf(" %[^\n]", username);
    printf("Please Enter Password: ");
    scanf(" %[^\n]", password);

    fprintf(usercredential, "%s|%s\n", username, password);
    fclose(usercredential);

    printf(green "SUCCESS: New account created successfully.\n" white);
}

void checkChoices(int *choiceMenu)
{
    switch (*choiceMenu)
    {
    case ADD:
        addStudent();
        break;
    case REMOVE:
        removeStudent();
        break;
    case EDIT:
        editStudentInfo();
        break;
    case VIEW:
        viewStudentList();
        break;
    case ATT:
        recordAttendance();
        break;
    case VATT:
        viewAttendance();
        break;
    case ADDQ:
        addQuiz();
        break;
    case REMQ:
        removeQuiz();
        break;
    case VIEQ:
        viewQuizzes();
        break;
    case AUSQ:
        addOrUpdateScore();
        break;
    case VIES:
        viewExamScore();
        break;
    case AUES:
        addOrUpdateExamScore();
        break;
    case GWA:
        computeGWA();
        break;
    case STOP:
        *choiceMenu = STOP;
        printf("Exiting App...");
        break;
    case GR:
        generateReports();
        break;
    case EGS:
        editGradingSystem();
        break;
    default:
        printf("Invalid Option! Please Select A Valid One\n");
        break;
    }
}

void addStudent()
{
    FILE *studentRecords = fopen("studentRecords.txt", "a");
    if (studentRecords == NULL)
    {
        printf(red "Error: Could not open studentRecords.txt for appending.\n" white);
        return;
    }

    char name[maxNameLength];
    char id[maxIdNumberLength];

    printf("Enter Student Name     : ");
    scanf(" %[^\n]", name);
    printf("Enter Student ID Number: ");
    scanf(" %[^\n]", id);

    fprintf(studentRecords, "%s|%s\n", name, id);
    fclose(studentRecords);

    printf(green "Student added successfully.\n" white);
}

void viewStudentList()
{
    FILE *studentRecordsFile = fopen("studentRecords.txt", "r");
    if (studentRecordsFile == NULL)
    {
        printf(red "ERROR  : Student Records Doesn't Exist! Add A Student To Create One!\n" white);
        return;
    }

    char readLine[1000];
    char studentName[maxNameLength];
    char studentId[maxIdNumberLength];

    printf("-----------------------------------------------------------\n");
    printf("| %-42s | %-10s |\n", "NAME", "ID NUMBER");
    printf("-----------------------------------------------------------\n");

    while (fgets(readLine, sizeof(readLine), studentRecordsFile))
    {
        int len = strlen(readLine);
        while (len > 0 && (readLine[len - 1] == '\n' || readLine[len - 1] == '\r'))
        {
            readLine[len - 1] = '\0';
            len--;
        }

        int i = 0, j = 0, k = 0;
        studentName[0] = '\0';
        studentId[0] = '\0';

        while (readLine[k] != '\0' && readLine[k] != '|')
        {
            if (i < maxNameLength - 1)
                studentName[i++] = readLine[k];
            k++;
        }
        studentName[i] = '\0';

        if (readLine[k] == '|')
        {
            k++;
            while (readLine[k] != '\0')
            {
                if (j < maxIdNumberLength - 1)
                    studentId[j++] = readLine[k];
                k++;
            }
            studentId[j] = '\0';
        }

        printf("| %-42s | %-10s |\n", studentName, studentId);
        printf("-----------------------------------------------------------\n");
    }

    fclose(studentRecordsFile);
}

void checkStudentRecords()
{
    FILE *studentRecordsFile = fopen("studentRecords.txt", "r");
    if (studentRecordsFile == NULL)
    {
        printf(red "ERROR  : Student Records Doesn't\n" white);
        return;
    }
    else
    {
        printf(green "SUCCESS: Student Records Found\n" white);
    }

    char readLine[1000];
    int studentCount = 0;

    while (fgets(readLine, sizeof(readLine), studentRecordsFile))
    {
        int len = strlen(readLine);
        while (len > 0 && (readLine[len - 1] == '\n' || readLine[len - 1] == '\r'))
        {
            readLine[len - 1] = '\0';
            len--;
        }

        if (strlen(readLine) > 0)
            studentCount++;
    }

    fclose(studentRecordsFile);
}

void removeStudent()
{
    FILE *originalRecord = fopen("studentRecords.txt", "r");
    if (originalRecord == NULL)
    {
        printf(red "ERROR  : Student Records Doesn't Exist!\n" white);
        return;
    }

    FILE *temporaryRecord = fopen("temporaryRecord.txt", "w");
    if (temporaryRecord == NULL)
    {
        printf(red "ERROR: Could Not Create Temporary Record\n" white);
        fclose(originalRecord);
        return;
    }

    char readOriginalRecord[1000];
    char removeStudentName[maxNameLength];

    printf("Name of Student: ");
    scanf(" %[^\n]", removeStudentName);

    int isRemoved = 0;

    while (fgets(readOriginalRecord, sizeof(readOriginalRecord), originalRecord))
    {
        int len = strlen(readOriginalRecord);
        while (len > 0 && (readOriginalRecord[len - 1] == '\n' || readOriginalRecord[len - 1] == '\r'))
        {
            readOriginalRecord[len - 1] = '\0';
            len--;
        }

        char extractedName[maxNameLength];
        char extractedId[maxIdNumberLength];
        int i = 0, j = 0, k = 0;

        extractedName[0] = '\0';
        extractedId[0] = '\0';

        while (readOriginalRecord[k] != '\0' && readOriginalRecord[k] != '|')
        {
            if (i < maxNameLength - 1)
            {
                extractedName[i] = readOriginalRecord[k];
                i++;
            }
            k++;
        }
        extractedName[i] = '\0';

        if (readOriginalRecord[k] == '|')
        {
            k++;
            while (readOriginalRecord[k] != '\0')
            {
                if (j < maxIdNumberLength - 1)
                {
                    extractedId[j] = readOriginalRecord[k];
                    j++;
                }
                k++;
            }
            extractedId[j] = '\0';
        }
        else
        {
            extractedId[0] = '\0';
        }

        if (strcmp(extractedName, removeStudentName) == 0)
        {
            isRemoved = 1;
            continue;
        }
        else
        {
            if (extractedId[0] != '\0')
                fprintf(temporaryRecord, "%s|%s\n", extractedName, extractedId);
            else
                fprintf(temporaryRecord, "%s\n", extractedName);
        }
    }

    fclose(originalRecord);
    fclose(temporaryRecord);

    if (isRemoved)
    {
        remove("studentRecords.txt");
        rename("temporaryRecord.txt", "studentRecords.txt");
        printf(green "Student \"%s\" removed successfully.\n" white, removeStudentName);
    }
    else
    {
        remove("temporaryRecord.txt");
        printf(red "Student \"%s\" Doesn't Exist.\n" white, removeStudentName);
    }
}

void editStudentInfo()
{
    FILE *originalStudentRecord = fopen("studentRecords.txt", "r");
    FILE *temporaryStudentRecord = fopen("temporaryRecord.txt", "w");

    char readStudentRecordLine[1000];
    char searchStudentName[maxNameLength];
    char extractedStudentName[maxNameLength];
    char extractedStudentId[maxIdNumberLength];
    char updatedStudentName[maxNameLength];
    char updatedStudentId[maxIdNumberLength];

    int userEditChoice;
    int nameCharIndex;
    int idCharIndex;
    int lineCharIndex;

    int isStudentFound = 0;

    if (originalStudentRecord == NULL)
    {
        printf("ERROR: Student Records Doesn't Exist.\n");
        if (temporaryStudentRecord)
        {
            fclose(temporaryStudentRecord);
        }
        return;
    }

    if (temporaryStudentRecord == NULL)
    {
        printf("ERROR: Temporary Record Doesn't Exist\n");
        fclose(originalStudentRecord);
        return;
    }

    printf("Name Of Student: ");
    scanf(" %[^\n]", searchStudentName);

    while (fgets(readStudentRecordLine, sizeof(readStudentRecordLine), originalStudentRecord))
    {
        int len = strlen(readStudentRecordLine);
        while (len > 0 && (readStudentRecordLine[len - 1] == '\n' || readStudentRecordLine[len - 1] == '\r'))
        {
            readStudentRecordLine[len - 1] = '\0';
            len--;
        }

        extractedStudentName[0] = '\0';
        extractedStudentId[0] = '\0';
        nameCharIndex = 0;
        idCharIndex = 0;
        lineCharIndex = 0;

        while (readStudentRecordLine[lineCharIndex] != '\0' && readStudentRecordLine[lineCharIndex] != '|')
        {
            if (nameCharIndex < maxNameLength - 1)
            {
                extractedStudentName[nameCharIndex] = readStudentRecordLine[lineCharIndex];
                nameCharIndex++;
            }
            lineCharIndex++;
        }
        extractedStudentName[nameCharIndex] = '\0';

        if (readStudentRecordLine[lineCharIndex] == '|')
        {
            lineCharIndex++;
            while (readStudentRecordLine[lineCharIndex] != '\0')
            {
                if (idCharIndex < maxIdNumberLength - 1)
                {
                    extractedStudentId[idCharIndex] = readStudentRecordLine[lineCharIndex];
                    idCharIndex++;
                }
                lineCharIndex++;
            }
            extractedStudentId[idCharIndex] = '\0';
        }
        else
        {
            extractedStudentId[0] = '\0';
        }

        if (isStudentFound == 0 && strcmp(extractedStudentName, searchStudentName) == 0)
        {

            printf("Student found: %s | %s\n", extractedStudentName, extractedStudentId);

            printf("\nWhat do you want to edit?\n\n");
            printf("1) Name\n");
            printf("2) ID Number\n");
            printf("3) Both Name and ID Number\n\n");
            printf("Enter your choice: ");
            scanf(" %d", &userEditChoice);

            if (userEditChoice == 1)
            {
                printf("Enter new student name: ");
                scanf(" %[^\n]", updatedStudentName);
                fprintf(temporaryStudentRecord, "%s|%s\n", updatedStudentName, extractedStudentId);
                isStudentFound = 1;
            }
            else if (userEditChoice == 2)
            {
                printf("Enter new student ID number: ");
                scanf(" %s", updatedStudentId);
                fprintf(temporaryStudentRecord, "%s|%s\n", extractedStudentName, updatedStudentId);
                isStudentFound = 1;
            }
            else if (userEditChoice == 3)
            {
                printf("Enter new student name: ");
                scanf(" %[^\n]", updatedStudentName);
                printf("Enter new student ID number: ");
                scanf(" %s", updatedStudentId);
                fprintf(temporaryStudentRecord, "%s|%s\n", updatedStudentName, updatedStudentId);
                isStudentFound = 1;
            }
            else
            {
                printf("Invalid option. Keeping original record.\n");
                if (extractedStudentId[0] != '\0')
                {
                    fprintf(temporaryStudentRecord, "%s|%s\n", extractedStudentName, extractedStudentId);
                }
                else
                {
                    fprintf(temporaryStudentRecord, "%s\n", extractedStudentName);
                }
            }
        }
        else
        {
            if (extractedStudentId[0] != '\0')
            {
                fprintf(temporaryStudentRecord, "%s|%s\n", extractedStudentName, extractedStudentId);
            }
            else
            {
                fprintf(temporaryStudentRecord, "%s\n", extractedStudentName);
            }
        }
    }

    fclose(originalStudentRecord);
    fclose(temporaryStudentRecord);

    if (isStudentFound)
    {
        remove("studentRecords.txt");
        rename("temporaryRecord.txt", "studentRecords.txt");
        printf("\nStudent record updated successfully.\n");
    }
    else
    {
        remove("temporaryRecord.txt");
        printf("\nStudent \"%s\" not found.\n", searchStudentName);
    }
}

void menu(int *choiceMenu)
{
    printf("============== MENU =============\n\n");
    printf("1) Add New Student\n");
    printf("2) Remove Student\n");
    printf("3) Edit Student Info\n");
    printf("4) View Student List\n");
    printf("5) Record Attendance\n");
    printf("6) View Attendance\n");
    printf("7) Add Quiz\n");
    printf("8) Remove Quiz\n");
    printf("9) View Quizes\n");
    printf("10) Add/Update Quiz Score\n");
    printf("11) View Exam Score\n");
    printf("12) Add/Update Exam Score\n");
    printf("13) Compute GWA\n");
    printf("14) Generate Reports\n");
    printf("15) Edit Grading System (quiz%% / exam%%)\n");
    printf("16) Exit Program\n\n");
    printf("SELECT: ");
    scanf("%d", choiceMenu);
}
void recordAttendance()
{
    FILE *studentFile = fopen("studentRecords.txt", "r");
    if (studentFile == NULL)
    {
        printf(red "ERROR: Student Records Doesn't Exist!\n" white);
        studentFile = fopen("studentRecords.txt", "w");
        if (studentFile != NULL)
            printf(green "SUCCESS: Created File Student Records\n" white);
        fclose(studentFile);
        studentFile = fopen("studentRecords.txt", "r");
        if (studentFile == NULL)
            return;
    }
    else
    {
        printf(green "SUCCESS: Found Student Records\n" white);
    }

    FILE *attendanceFile = fopen("Attendance.txt", "a");
    if (attendanceFile == NULL)
    {
        printf(red "ERROR: Attendance File Doesn't Exist!\n" white);
        attendanceFile = fopen("Attendance.txt", "w");
        if (attendanceFile != NULL)
            printf(green "SUCCESS: Created Attendance File\n" white);
        fclose(attendanceFile);
        attendanceFile = fopen("Attendance.txt", "a");
        if (attendanceFile == NULL)
            return;
    }
    else
    {
        printf(green "SUCCESS: Found Attendance File\n" white);
    }

    printf("\n");

    int day, month, year;
    printf("Day: ");
    scanf("%d", &day);
    printf("Month: ");
    scanf("%d", &month);
    printf("Year: ");
    scanf("%d", &year);

    printf("\nInput 0 = Absent, 1 = Present, 2 = Late, 3 = Excused\n\n");

    char readLine[1000];
    char studentName[maxNameLength];
    int status;

    while (fgets(readLine, sizeof(readLine), studentFile))
    {
        int len = strlen(readLine);
        while (len > 0 && (readLine[len - 1] == '\n' || readLine[len - 1] == '\r'))
        {
            readLine[len - 1] = '\0';
            len--;
        }

        int i = 0;
        while (readLine[i] != '\0' && readLine[i] != '|')
        {
            studentName[i] = readLine[i];
            i++;
        }
        studentName[i] = '\0';

        do
        {
            printf("name: %s: ", studentName);
            if (scanf("%d", &status) != 1)
            {
                while (getchar() != '\n')
                    ;
                status = -1;
            }
            if (status < 0 || status > 3)
                printf("Invalid input! Enter 0, 1, 2, or 3.\n");
        } while (status < 0 || status > 3);

        fprintf(attendanceFile, "%s|%02d-%02d-%02d|%d\n", studentName, day, month, year, status);
    }

    fclose(studentFile);
    fclose(attendanceFile);

    printf(green "\nAttendance recorded successfully for all students.\n" white);
}

void viewAttendance()
{
    FILE *attendanceFile = fopen("Attendance.txt", "r");
    if (attendanceFile == NULL)
    {
        printf(red "ERROR: Attendance file not found.\n" white);
        return;
    }

    int day, month, year;
    printf("Enter date to view attendance:\n");
    printf("Day: ");
    scanf("%d", &day);
    printf("Month: ");
    scanf("%d", &month);
    printf("Year: ");
    scanf("%d", &year);

    char targetDate[11];
    snprintf(targetDate, sizeof(targetDate), "%02d-%02d-%02d", day, month, year);

    char readLine[256];
    char studentName[maxNameLength];
    int status;

    printf("\n======= ATTENDANCE FOR %s ========\n", targetDate);
    printf("| %-25s | %-10s |\n", "NAME", "STATUS");
    printf("--------------------------------------------\n");

    while (fgets(readLine, sizeof(readLine), attendanceFile))
    {
        int len = strlen(readLine);
        while (len > 0 && (readLine[len - 1] == '\n' || readLine[len - 1] == '\r'))
        {
            readLine[len - 1] = '\0';
            len--;
        }

        int i = 0, j = 0, k = 0;
        studentName[0] = '\0';
        char recordDate[11];
        recordDate[0] = '\0';
        status = -1;

        while (readLine[k] != '\0' && readLine[k] != '|')
            studentName[i++] = readLine[k++];
        studentName[i] = '\0';

        if (readLine[k] == '|')
            k++;
        while (readLine[k] != '\0' && readLine[k] != '|')
            recordDate[j++] = readLine[k++];
        recordDate[j] = '\0';

        if (readLine[k] == '|')
            k++;
        if (readLine[k] != '\0')
            status = atoi(&readLine[k]);

        if (strcmp(recordDate, targetDate) == 0)
        {
            char statusText[10];
            switch (status)
            {
            case 0:
                strcpy(statusText, "Absent");
                break;
            case 1:
                strcpy(statusText, "Present");
                break;
            case 2:
                strcpy(statusText, "Late");
                break;
            case 3:
                strcpy(statusText, "Excused");
                break;
            default:
                strcpy(statusText, "Unknown");
                break;
            }

            printf("| %-25s | %-10s |\n", studentName, statusText);
        }
    }

    printf("--------------------------------------------\n\n");

    fclose(attendanceFile);
}

void addQuiz()
{
    FILE *quizFile = fopen("Quizzes.txt", "a");
    if (!quizFile)
    {
        printf(red "Error: Could not open Quizzes.txt\n" white);
        return;
    }

    char quizName[50];
    int points;

    printf("Enter quiz name/number: ");
    scanf(" %[^\n]", quizName);
    printf("Enter total points for this quiz: ");
    scanf("%d", &points);

    fprintf(quizFile, "%s|%d\n", quizName, points);
    fclose(quizFile);

    printf(green "Quiz added successfully.\n" white);
}

void addOrUpdateScore()
{
    FILE *studentFile = fopen("studentRecords.txt", "r");
    FILE *quizFile = fopen("Quizzes.txt", "r");
    if (!studentFile || !quizFile)
    {
        if (studentFile)
            fclose(studentFile);
        if (quizFile)
            fclose(quizFile);
        printf(red "Error: studentRecords.txt or Quizzes.txt not found.\n" white);
        return;
    }

    char studentName[maxNameLength];
    char quizName[50];
    int score;

    printf("Enter student's full name: ");
    scanf(" %[^\n]", studentName);
    printf("Enter quiz name: ");
    scanf(" %[^\n]", quizName);
    printf("Enter score (integer): ");
    scanf("%d", &score);

    int studentFound = 0;
    {
        char line[1000];
        rewind(studentFile);
        while (fgets(line, sizeof(line), studentFile))
        {
            int ln = strlen(line);
            while (ln > 0 && (line[ln - 1] == '\n' || line[ln - 1] == '\r'))
                line[--ln] = '\0';

            char nameOnly[maxNameLength];
            sscanf(line, "%[^|]", nameOnly);
            if (strcmp(nameOnly, studentName) == 0)
            {
                studentFound = 1;
                break;
            }
        }
    }

    if (!studentFound)
    {
        printf(red "Error: Student \"%s\" not found in studentRecords.txt\n" white, studentName);
        fclose(studentFile);
        fclose(quizFile);
        return;
    }

    int quizFound = 0;
    {
        char line[256];
        rewind(quizFile);
        while (fgets(line, sizeof(line), quizFile))
        {
            int ln = strlen(line);
            while (ln > 0 && (line[ln - 1] == '\n' || line[ln - 1] == '\r'))
                line[--ln] = '\0';

            char qname[50];
            int pts;
            if (sscanf(line, "%[^|]|%d", qname, &pts) >= 1)
            {
                if (strcmp(qname, quizName) == 0)
                {
                    quizFound = 1;
                    break;
                }
            }
        }
    }

    if (!quizFound)
    {
        printf(red "Error: Quiz \"%s\" not found in Quizzes.txt\n" white, quizName);
        fclose(studentFile);
        fclose(quizFile);
        return;
    }

    fclose(studentFile);
    fclose(quizFile);

    FILE *scoreFile = fopen("QuizScores.txt", "r");
    FILE *tempFile = fopen("tempScores.txt", "w");
    if (!tempFile)
    {
        if (scoreFile)
            fclose(scoreFile);
        printf(red "Error: Could not open temporary file for writing.\n" white);
        return;
    }

    char readLine[512];
    int updated = 0;

    if (scoreFile)
    {
        while (fgets(readLine, sizeof(readLine), scoreFile))
        {
            char lineStudent[maxNameLength];
            char lineQuiz[50];
            int lineScore;
            int scanned = sscanf(readLine, "%[^|]|%[^|]|%d", lineStudent, lineQuiz, &lineScore);

            if (scanned == 3 && strcmp(lineStudent, studentName) == 0 && strcmp(lineQuiz, quizName) == 0)
            {
                fprintf(tempFile, "%s|%s|%d\n", studentName, quizName, score);
                updated = 1;
            }
            else
            {
                fputs(readLine, tempFile);
            }
        }
        fclose(scoreFile);
    }

    if (!updated)
    {
        fprintf(tempFile, "%s|%s|%d\n", studentName, quizName, score);
    }

    fclose(tempFile);

    remove("QuizScores.txt");
    if (rename("tempScores.txt", "QuizScores.txt") != 0)
    {
        printf(red "Error: Could not rename temp file to QuizScores.txt\n" white);
        remove("tempScores.txt");
        return;
    }

    printf(green "Score for \"%s\" on \"%s\" recorded/updated to %d.\n" white, studentName, quizName, score);
}

void viewQuizzes()
{
    FILE *quizFile = fopen("Quizzes.txt", "r");
    if (!quizFile)
    {
        printf(red "ERROR: Quizzes.txt not found.\n" white);
        return;
    }

    char readLine[256];
    char quizName[50];
    int points;

    printf("\n-------------------------------------------------\n");
    printf("                   LIST OF QUIZZES              \n");
    printf("-------------------------------------------------\n");
    printf("| %-28s | %-15s |\n", "QUIZ NAME", "POINTS");
    printf("-------------------------------------------------\n");

    while (fgets(readLine, sizeof(readLine), quizFile))
    {
        int ln = strlen(readLine);
        while (ln > 0 && (readLine[ln - 1] == '\n' || readLine[ln - 1] == '\r'))
            readLine[--ln] = '\0';
        if (sscanf(readLine, "%[^|]|%d", quizName, &points) >= 1)
            printf("| %-28s | %-15d |\n", quizName, points);
    }

    printf("-------------------------------------------------\n\n");

    fclose(quizFile);
}

void removeQuiz()
{
    FILE *quizFile = fopen("Quizzes.txt", "r");
    FILE *tempFile = fopen("temporaryQuizzes.txt", "w");
    if (quizFile == NULL || tempFile == NULL)
    {
        printf(red "Error: Could not open necessary files.\n" white);
        if (quizFile)
            fclose(quizFile);
        if (tempFile)
            fclose(tempFile);
        return;
    }

    char readLine[256];
    char lastLine[256] = "";
    int firstLine = 1;

    while (fgets(readLine, sizeof(readLine), quizFile))
    {
        if (!firstLine)
            fprintf(tempFile, "%s", lastLine);
        strcpy(lastLine, readLine);
        firstLine = 0;
    }

    fclose(quizFile);
    fclose(tempFile);

    remove("Quizzes.txt");
    rename("temporaryQuizzes.txt", "Quizzes.txt");

    printf(green "Latest quiz removed successfully for all students.\n" white);
}

void computeGWA()
{
    FILE *studentRecordsFile = fopen("studentRecords.txt", "r");
    if (studentRecordsFile == NULL)
    {
        printf(red "Error: Could not open studentRecords.txt\n" white);
        return;
    }

    char readLine[1000];
    char studentName[maxNameLength];
    char studentId[maxIdNumberLength];
    float gwa;

    printf("\n================ GWA REPORT ================\n");
    printf("| %-25s | %-10s | %-5s |\n", "NAME", "ID NUMBER", "GWA");
    printf("---------------------------------------------------\n");

    while (fgets(readLine, sizeof(readLine), studentRecordsFile))
    {
        int len = strlen(readLine);
        while (len > 0 && (readLine[len - 1] == '\n' || readLine[len - 1] == '\r'))
        {
            readLine[len - 1] = '\0';
            len--;
        }

        int i = 0, j = 0, k = 0;
        studentName[0] = '\0';
        studentId[0] = '\0';

        while (readLine[k] != '\0' && readLine[k] != '|')
        {
            if (i < maxNameLength - 1)
                studentName[i++] = readLine[k];
            k++;
        }
        studentName[i] = '\0';

        if (readLine[k] == '|')
        {
            k++;
            while (readLine[k] != '\0' && readLine[k] != '|')
            {
                if (j < maxIdNumberLength - 1)
                    studentId[j++] = readLine[k];
                k++;
            }
            studentId[j] = '\0';
        }

        gwa = 0.0f;

        printf("| %-25s | %-10s | %.2f |\n", studentName, studentId, gwa);
    }

    printf("---------------------------------------------------\n\n");

    fclose(studentRecordsFile);
}

void checkQuizzesFileExist()
{
    FILE *quizFile = fopen("Quizzes.txt", "r");

    if (quizFile == NULL)
    {
        printf(red "ERROR: Quizzes File Doesn't Exist!\n" white);
        quizFile = fopen("Quizzes.txt", "w");
        if (quizFile == NULL)
        {
            printf(red "ERROR: Failed to create Quizzes.txt!\n" white);
            return;
        }
        fclose(quizFile);
        printf(green "SUCCESS: Quizzes Created successfully.\n" white);
    }
    else
    {
        printf(green "SUCCESS: Quizzes File found!\n" white);
        fclose(quizFile);
    }
}

void generateReports()
{
    FILE *studentRecordsFile = fopen("studentRecords.txt", "r");
    if (studentRecordsFile == NULL)
    {
        printf(red "Error: Could not open studentRecords.txt\n" white);
        return;
    }

    int totalPossibleQuizPoints = 0;
    FILE *qfile = fopen("Quizzes.txt", "r");
    if (qfile)
    {
        char qline[256];
        char qname[64];
        int qpts;
        while (fgets(qline, sizeof(qline), qfile))
        {
            int ln = strlen(qline);
            while (ln > 0 && (qline[ln - 1] == '\n' || qline[ln - 1] == '\r'))
                qline[--ln] = '\0';
            if (sscanf(qline, "%[^|]|%d", qname, &qpts) == 2)
                totalPossibleQuizPoints += qpts;
        }
        fclose(qfile);
    }

    char readLine[1000];
    char studentName[maxNameLength];
    char studentId[maxIdNumberLength];

    printf("\n--------------------------------------------------------------------------\n");
    printf("| %-25s | %-10s | %-10s | %-6s | %-6s | %-6s |\n", "NAME", "ID NUMBER", "QUIZ TOT", "EXAM", "GWA", "REMARK");
    printf("--------------------------------------------------------------------------\n");

    while (fgets(readLine, sizeof(readLine), studentRecordsFile))
    {
        int len = strlen(readLine);
        while (len > 0 && (readLine[len - 1] == '\n' || readLine[len - 1] == '\r'))
            readLine[--len] = '\0';

        int i = 0, j = 0, k = 0;
        studentName[0] = '\0';
        studentId[0] = '\0';

        while (readLine[k] != '\0' && readLine[k] != '|')
        {
            if (i < maxNameLength - 1)
                studentName[i++] = readLine[k];
            k++;
        }
        studentName[i] = '\0';

        if (readLine[k] == '|')
        {
            k++;
            while (readLine[k] != '\0')
            {
                if (j < maxIdNumberLength - 1)
                    studentId[j++] = readLine[k];
                k++;
            }
            studentId[j] = '\0';
        }

        int totalQuizPoints = 0;
        FILE *quizScores = fopen("QuizScores.txt", "r");
        if (quizScores)
        {
            char qline[512];
            char qStudent[maxNameLength];
            char qName[64];
            int qScore;
            while (fgets(qline, sizeof(qline), quizScores))
            {
                int ql = strlen(qline);
                while (ql > 0 && (qline[ql - 1] == '\n' || qline[ql - 1] == '\r'))
                    qline[--ql] = '\0';
                if (sscanf(qline, "%[^|]|%[^|]|%d", qStudent, qName, &qScore) == 3)
                {
                    if (strcmp(qStudent, studentName) == 0)
                        totalQuizPoints += qScore;
                }
            }
            fclose(quizScores);
        }

        int examScore = 0;
        FILE *exf = fopen("Exams.txt", "r");
        if (exf)
        {
            char eline[256];
            char eStudent[maxNameLength];
            int eScore;
            while (fgets(eline, sizeof(eline), exf))
            {
                int el = strlen(eline);
                while (el > 0 && (eline[el - 1] == '\n' || eline[el - 1] == '\r'))
                    eline[--el] = '\0';
                if (sscanf(eline, "%[^|]|%d", eStudent, &eScore) == 2)
                {
                    if (strcmp(eStudent, studentName) == 0)
                    {
                        examScore = eScore;
                        break;
                    }
                }
            }
            fclose(exf);
        }

        float gwa = 0.0f;
        float quizComponent = 0.0f;
        float examComponent = 0.0f;
        if (totalPossibleQuizPoints > 0)
            quizComponent = ((float)totalQuizPoints / (float)totalPossibleQuizPoints) * gradingQuizPercent;
        examComponent = ((float)examScore) * (gradingExamPercent / 100.0f);
        gwa = quizComponent + examComponent;

        char remark[8];
        if (gwa >= 75.0f)
            strcpy(remark, "PASS");
        else
            strcpy(remark, "FAIL");

        if (strcmp(remark, "PASS") == 0)
            printf(green "| %-25s | %-10s | %-10d | %-6d | %-6.2f | %-6s |\n" white, studentName, studentId, totalQuizPoints, examScore, gwa, remark);
        else
            printf(red "| %-25s | %-10s | %-10d | %-6d | %-6.2f | %-6s |\n" white, studentName, studentId, totalQuizPoints, examScore, gwa, remark);
    }

    printf("--------------------------------------------------------------------------\n\n");

    fclose(studentRecordsFile);
}

void addOrUpdateExamScore()
{
    FILE *studentFile = fopen("studentRecords.txt", "r");
    if (!studentFile)
    {
        printf(red "Error: studentRecords.txt not found. Add students first.\n" white);
        return;
    }
    fclose(studentFile);

    char studentName[maxNameLength];
    int examScore;

    printf("Enter student's full name: ");
    scanf(" %[^\n]", studentName);
    printf("Enter exam score (integer): ");
    scanf("%d", &examScore);

    FILE *exFile = fopen("Exams.txt", "r");
    FILE *tempFile = fopen("tempExams.txt", "w");
    if (!tempFile)
    {
        if (exFile)
            fclose(exFile);
        printf(red "Error: Could not open temporary file for writing.\n" white);
        return;
    }

    char line[512];
    int updated = 0;

    if (exFile)
    {
        while (fgets(line, sizeof(line), exFile))
        {
            int ln = strlen(line);
            while (ln > 0 && (line[ln - 1] == '\n' || line[ln - 1] == '\r'))
                line[--ln] = '\0';

            char lineStudent[maxNameLength];
            int lineScore;
            int scanned = sscanf(line, "%[^|]|%d", lineStudent, &lineScore);

            if (scanned == 2 && strcmp(lineStudent, studentName) == 0)
            {
                fprintf(tempFile, "%s|%d\n", studentName, examScore);
                updated = 1;
            }
            else
            {
                fprintf(tempFile, "%s\n", line);
            }
        }
        fclose(exFile);
    }

    if (!updated)
        fprintf(tempFile, "%s|%d\n", studentName, examScore);

    fclose(tempFile);

    remove("Exams.txt");
    if (rename("tempExams.txt", "Exams.txt") != 0)
    {
        printf(red "Error: Could not save exam records (rename failed).\n" white);
        remove("tempExams.txt");
        return;
    }

    printf(green "Exam score for \"%s\" set to %d.\n" white, studentName, examScore);
}

void viewExamScore()
{
    FILE *studentsFile = fopen("studentRecords.txt", "r");
    if (studentsFile == NULL)
    {
        printf(red "ERROR: studentRecords.txt not found.\n" white);
        return;
    }

    FILE *examsFile = fopen("Exams.txt", "r");

    char readLine[1000];
    char studentName[maxNameLength];
    char studentId[maxIdNumberLength];

    printf("\n----------------------------------------------\n");
    printf("| %-40s | %-7s |\n", "NAME", "SCORE");
    printf("----------------------------------------------\n");

    while (fgets(readLine, sizeof(readLine), studentsFile))
    {
        int len = strlen(readLine);
        while (len > 0 && (readLine[len - 1] == '\n' || readLine[len - 1] == '\r'))
            readLine[--len] = '\0';

        int i = 0, j = 0, k = 0;
        studentName[0] = '\0';
        studentId[0] = '\0';

        while (readLine[k] != '\0' && readLine[k] != '|')
        {
            if (i < maxNameLength - 1)
                studentName[i++] = readLine[k];
            k++;
        }
        studentName[i] = '\0';

        if (readLine[k] == '|')
        {
            k++;
            while (readLine[k] != '\0')
            {
                if (j < maxIdNumberLength - 1)
                    studentId[j++] = readLine[k];
                k++;
            }
            studentId[j] = '\0';
        }

        int examScore = -1;
        if (examsFile != NULL)
        {
            char examLine[256];
            rewind(examsFile);
            while (fgets(examLine, sizeof(examLine), examsFile))
            {
                int el = strlen(examLine);
                while (el > 0 && (examLine[el - 1] == '\n' || examLine[el - 1] == '\r'))
                    examLine[--el] = '\0';

                char eName[maxNameLength];
                int eScore;
                if (sscanf(examLine, "%[^|]|%d", eName, &eScore) == 2)
                {
                    if (strcmp(eName, studentName) == 0)
                    {
                        examScore = eScore;
                        break;
                    }
                }
            }
        }

        if (examScore == -1)
            printf("| %-40s | %-7s |\n", studentName, "N/A");
        else
            printf("| %-40s | %-7d |\n", studentName, examScore);

        printf("----------------------------------------------\n");
    }

    if (examsFile)
        fclose(examsFile);
    fclose(studentsFile);
    printf("\n");
}

void checkGradingFileExist()
{
    FILE *gfile = fopen("Grading.txt", "r");
    if (gfile == NULL)
    {
        gradingQuizPercent = 40.0f;
        gradingExamPercent = 60.0f;
        gfile = fopen("Grading.txt", "w");
        if (gfile)
        {
            fprintf(gfile, "%.2f|%.2f\n", gradingQuizPercent, gradingExamPercent);
            fclose(gfile);
            printf(green "SUCCESS: Grading file created with defaults (Quiz=40%% Exam=60%%)\n" white);
        }
    }
    else
    {
        float q = 0.0f, e = 0.0f;
        if (fscanf(gfile, "%f|%f", &q, &e) == 2)
        {
            if (q >= 0.0f && e >= 0.0f && (q + e) > 0.0f)
            {
                gradingQuizPercent = q;
                gradingExamPercent = e;
            }
        }
        fclose(gfile);
        printf(green "SUCCESS: Grading file loaded (Quiz=%.2f%% Exam=%.2f%%)\n" white, gradingQuizPercent, gradingExamPercent);
    }
}

void editGradingSystem()
{
    float q, e;
    printf("Current: Quiz=%.2f%% Exam=%.2f%%\n", gradingQuizPercent, gradingExamPercent);
    printf("Enter new quiz percent (0-100): ");
    scanf("%f", &q);
    printf("Enter new exam percent (0-100): ");
    scanf("%f", &e);

    if (q < 0.0f || e < 0.0f || (q + e) <= 0.0f)
    {
        printf(red "Invalid grading percentages. No changes made.\n" white);
        return;
    }

    gradingQuizPercent = q;
    gradingExamPercent = e;

    FILE *gfile = fopen("Grading.txt", "w");
    if (!gfile)
    {
        printf(red "Error: Could not save grading file.\n" white);
        return;
    }
    fprintf(gfile, "%.2f|%.2f\n", gradingQuizPercent, gradingExamPercent);
    fclose(gfile);

    printf(green "Grading system updated: Quiz=%.2f%% Exam=%.2f%%\n" white, gradingQuizPercent, gradingExamPercent);
}
