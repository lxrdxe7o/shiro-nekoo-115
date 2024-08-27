/*
*
* This a simple Hospital Management System written in C.
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define MAX_NAME_LENGTH 50
#define MAX_ADDRESS_LENGTH 200
#define MAX_DOCTOR_REMARK 200
#define MAX_PASSWORD_LENGTH 50
#define MAX_USER_NAME 30
#define MAX_PATIENTS 100
#define MAX_DOCTORS 10
#define BLOOD_CHAR_MAX 5
#define MAX_CONTACT_LENGTH 15
#define DAYS_IN_WEEK 7
#define SLOTS_PER_DAY 5
#define APPOINTMENT_PER_SLOT 5
#define MAX_WARDS 50

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

typedef struct {
    int wardID;
    char patientName[MAX_NAME_LENGTH];
    char doctorName[MAX_NAME_LENGTH];
} medicalWard;

typedef struct patientInfo
{
    char patientName[MAX_NAME_LENGTH];
    char patientAddress[MAX_ADDRESS_LENGTH];
    char prescription[MAX_DOCTOR_REMARK];
    char patientPass[MAX_PASSWORD_LENGTH];
    int age;
    int id;
    float height;
    float weight;
    char patientContact[MAX_CONTACT_LENGTH];
    char emergencyContact[MAX_CONTACT_LENGTH];
    char patientUserName[MAX_USER_NAME];
    char bloodType[BLOOD_CHAR_MAX];
} patientInfo;

typedef struct doctorInfo
{
    char doctorName[MAX_NAME_LENGTH];
    char doctorAddress[MAX_ADDRESS_LENGTH];
    char specialty[MAX_DOCTOR_REMARK];
    char doctorPass[MAX_PASSWORD_LENGTH];
    int id;
    int experience;
    char doctorContact[MAX_CONTACT_LENGTH];
    char doctorUserName[MAX_USER_NAME];
    int calendar[DAYS_IN_WEEK][SLOTS_PER_DAY][APPOINTMENT_PER_SLOT];
} doctorInfo;

void clear_terminal();

void getPatientInfo(patientInfo *patient);
void getInfo_patient(patientInfo patients[MAX_PATIENTS], int *lastAssignedID);
void patientAccountCreation(patientInfo *patient, int *lastAssignedID);
int searchPatient(patientInfo patients[MAX_PATIENTS], const char* name);
void deletePatient(patientInfo patients[MAX_PATIENTS], const char* name, int *lastAssignedID);
void displayPatient(const patientInfo* patient);
void displayPatientSubMenu();
int patientLogIn(const patientInfo *patients, int maxCount, const char *inputUser, const char *pass);
void editPatientInfo(patientInfo *patient);
void takeAppointment(doctorInfo *doctors, patientInfo *patients);
void appointmentFinalization(doctorInfo doctors[MAX_DOCTORS], patientInfo patients[MAX_PATIENTS]);
void displayPrescription(const patientInfo *patients);

void getDoctorInfo(doctorInfo *doctor);
void doctorAccountCreation(doctorInfo *doctor, int *lastAssignedID);
void getInfo_doctor(doctorInfo doctors[MAX_DOCTORS], int *lastAssignedID);
int searchDoctor(doctorInfo doctors[MAX_DOCTORS], const char* name);
void deleteDoctor(doctorInfo doctors[MAX_DOCTORS], const char* name, int *lastAssignedID);
void displayDoctor(const doctorInfo* doctor);
void initializeCalendar(doctorInfo *doctors);
void displayCalendar(const doctorInfo *doctors);
void updateCalendar(doctorInfo *doctors);
int docLogIn(const doctorInfo *doctors, int maxCount, const char *inputUser, const char *pass);
void docLogInPanel();
void displayDoctorSubMenu();
void editDoctorInfo(doctorInfo *doctor);
void prescribePatient(patientInfo *patients);


void displayPatientMenu();
void displayDoctorMenu();
void displayMainMenu();

void nextPage();
void notFound(char k);
void inputString(char* buffer, int length);
int inputInt();

void savePatientsToFile(const patientInfo patients[MAX_PATIENTS], const char *filename);
void loadPatientsFromFile(patientInfo patients[MAX_PATIENTS], const char *filename);

void saveDoctorsToFile(const doctorInfo doctors[MAX_DOCTORS], const char *filename);
void loadDoctorsFromFile(doctorInfo doctors[MAX_DOCTORS], const char *filename);

void saveIDs(int patientID, int doctorID, int wardID);
void loadIDs(int *patientID, int *doctorID, int *wardID);

void deleteAll(patientInfo patients[MAX_PATIENTS], doctorInfo doctors[MAX_DOCTORS], int *lastAssignedPatientID, int *lastAssignedDoctorID, int *lastAssignedWardID);

int searchWard(const medicalWard *wards, int maxCount, const char *patientName);
void addWard(medicalWard *wards, int *lastAssignedWardID, const char *patientName, const char *doctorName);
void deleteWard(medicalWard *wards, int *lastAssignedWardID, const char *patientName);
void displayWard(const medicalWard *wards, int maxCount);
void addPatientToWard(medicalWard *wards, int *lastAssignedWardID, patientInfo *patients, int maxCountPatients, doctorInfo *doctors, int maxCountDoctors);
void displayWardMenu();
void deletePatientFromWard(medicalWard *wards, int *lastAssignedWardID, patientInfo *patients, int maxCountPatients);

void loadWardFromFile(medicalWard *wards, int *lastAssignedWardID, const char *filename);
void saveWardToFile(const medicalWard *wards, int maxCount, const char *filename);
int main() {
    static int lastAssignedPatientID = 0;
    static int lastAssignedDoctorID = 0;
    static int lastAssignedWardID = 0;

    loadIDs(&lastAssignedPatientID, &lastAssignedDoctorID, &lastAssignedWardID);

    patientInfo patients[MAX_PATIENTS] = {0};
    doctorInfo doctors[MAX_DOCTORS] = {0};
    medicalWard wards[MAX_WARDS] = {0};

    loadPatientsFromFile(patients, "patients.dat");
    loadDoctorsFromFile(doctors, "doctors.dat");
    loadWardFromFile(wards, &lastAssignedWardID ,"wards.dat");

    int choice;
    while (1) {
        clear_terminal();
        displayMainMenu();
        int choice = inputInt();
        clear_terminal();

        switch (choice) {
            case 1: {
                int patientChoice;
                while (1) {
                    patientMenu:;
                    displayPatientMenu();
                    patientChoice = inputInt();
                    clear_terminal();

                    switch (patientChoice) {
                        case 1:
                            clear_terminal();
                            getInfo_patient(patients, &lastAssignedPatientID);
                            break;
                        case 2:
                        {
                            clear_terminal();
                            displayPatientMenu();
                            char searchName[MAX_NAME_LENGTH];
                            printf("\n\n");
                            printf("Enter the name of the patient to search: ");
                            inputString(searchName, sizeof(searchName));

                            int index = searchPatient(patients, searchName);
                            if (index != -1) {
                                clear_terminal();
                                printf("Patient found:\n");
                                displayPatient(&patients[index]);
                                nextPage();
                            } else {
                                clear_terminal();
                                notFound('p');
                                nextPage();
                            }
                            break;
                        }
                        case 3:
                        {
                            clear_terminal();
                            displayPatientMenu();
                            char deleteName[MAX_NAME_LENGTH];
                            printf("\n\n");
                            printf("Enter the name of the patient to delete: ");
                            inputString(deleteName, sizeof(deleteName));

                            clear_terminal();
                            deletePatient(patients, deleteName, &lastAssignedPatientID);
                            break;
                        }
                        case 4:
                        {
                            clear_terminal();
                            displayPatientMenu();
                            char displayName[MAX_NAME_LENGTH];
                            printf("\n\n");
                            printf("Enter the name of the patient to display: ");
                            inputString(displayName, sizeof(displayName));

                            int index = searchPatient(patients, displayName);
                            if (index != -1) {
                                clear_terminal();
                                displayPatient(&patients[index]);
                                nextPage();
                            } else {
                                clear_terminal();
                                notFound('p');
                                nextPage();
                            }
                            break;
                        }
                        case 5:
                        {
                            clear_terminal();

                            char inputUser[MAX_USER_NAME];
                            char pass[MAX_PASSWORD_LENGTH];

                            printf("Enter Patient Username:");
                            inputString(inputUser, MAX_USER_NAME);

                            printf("Enter password:");
                            inputString(pass, MAX_PASSWORD_LENGTH);

                            int index = patientLogIn(patients, MAX_PATIENTS, inputUser, pass);
                            if (index != -1)
                            {
                                while (1)
                                {
                                    displayPatientSubMenu();
                                    int subChoice;
                                    subChoice = inputInt();
                                    clear_terminal();

                                    switch (subChoice)
                                    {
                                    case 1:
                                        displayPatient(&patients[index]);
                                        displayPrescription(&patients[index]);
                                        break;
                                    case 2:
                                        editPatientInfo(&patients[index]);
                                        break;
                                    case 3:
                                        appointmentFinalization(doctors, patients);
                                        break;
                                    case 4:
                                        goto patientMenu;
                                    default:
                                        printf("Invalid choice. Please try again.\n");
                                    }

                                }
                                clear_terminal();
                                break;
                            }
                            else
                            {
                                clear_terminal();
                                notFound('p');
                                nextPage();
                                break;
                            }
                        }
                        case 6:
                            clear_terminal();
                            goto mainMenu;
                        default:
                            clear_terminal();
                            printf("Invalid choice. Please try again.\n");
                            nextPage();
                            break;
                    }
                }
                break;
            }
            case 2: {
                int doctorChoice;
                while (1) {
                    docMenu:;
                    displayDoctorMenu();
                    doctorChoice = inputInt();

                    switch (doctorChoice) {
                        case 1:
                            clear_terminal();
                            getInfo_doctor(doctors, &lastAssignedDoctorID);
                            break;
                        case 2:
                        {
                            clear_terminal();
                            displayDoctorMenu();
                            char searchName[MAX_NAME_LENGTH];
                            printf("\n\n");
                            printf("Enter the name of the doctor to search: ");
                            inputString(searchName, sizeof(searchName));

                            int index = searchDoctor(doctors, searchName);
                            if (index != -1) {
                                clear_terminal();
                                printf("Doctor found:\n");
                                displayDoctor(&doctors[index]);
                                nextPage();
                            } else {
                                printf("Doctor not found.\n");
                            }
                            break;
                        }
                        case 3:
                        {
                            clear_terminal();
                            displayDoctorMenu();
                            char deleteName[MAX_NAME_LENGTH];
                            printf("\n\n");
                            printf("Enter the name of the doctor to delete: ");
                            inputString(deleteName, sizeof(deleteName));

                            deleteDoctor(doctors, deleteName, &lastAssignedDoctorID);
                            break;
                        }
                        case 4:
                        {
                            clear_terminal();
                            displayDoctorMenu();
                            char displayName[MAX_NAME_LENGTH];
                            printf("\n\n");
                            printf("Enter the name of the doctor to display: ");
                            inputString(displayName, sizeof(displayName));

                            int index = searchDoctor(doctors, displayName);
                            if (index != -1) {
                                clear_terminal();
                                displayDoctor(&doctors[index]);
                                printf("\n\n");
                                displayCalendar(&doctors[index]);
                                nextPage();
                            } else {
                                printf("Doctor not found.\n");
                            }
                            break;
                        }
                        case 5:
                        {
                            clear_terminal();

                            char inputUser[MAX_USER_NAME];
                            char pass[MAX_PASSWORD_LENGTH];

                            printf("Enter Doctor Username:");
                            inputString(inputUser, MAX_USER_NAME);

                            printf("Enter password:");
                            inputString(pass, MAX_PASSWORD_LENGTH);

                            int index = docLogIn(doctors, MAX_DOCTORS, inputUser, pass);
                            if (index != -1)
                            {
                                while (1)
                                {
                                    displayDoctorSubMenu();
                                    int subChoiceDoc;
                                    subChoiceDoc = inputInt();
                                    clear_terminal();

                                    switch (subChoiceDoc)
                                    {
                                    case 1:
                                        updateCalendar(&doctors[index]);
                                        break;
                                    case 2:
                                        editDoctorInfo(&doctors[index]);
                                        break;
                                    case 3:
                                        prescribePatient(patients);

                                        break;
                                    case 4:
                                        goto docMenu;
                                    default:
                                        printf("Invalid choice. Please try again.\n");
                                    }

                                }
                                clear_terminal();
                                break;
                            }
                            else
                            {
                                clear_terminal();
                                notFound('d');
                                nextPage();
                                break;
                            }
                        }
                        case 6:
                            goto mainMenu;
                        default:
                            printf("Invalid choice. Please try again.\n");
                    }
                }
                break;
            }
            case 420:
                clear_terminal();
                deleteAll(patients, doctors, &lastAssignedPatientID, &lastAssignedDoctorID, &lastAssignedWardID);
                exit(0);
                break;
            case 3:
            {
                int wardChoice;
                while (1) {

                    displayWardMenu();
                    wardChoice = inputInt();

                    switch (wardChoice) {
                        case 1:
                            clear_terminal();
                            addPatientToWard(wards, &lastAssignedWardID, patients, MAX_PATIENTS, doctors, MAX_DOCTORS);
                            break;
                        case 2:
                        {
                            clear_terminal();
                            displayWardMenu();
                            char searchName[MAX_NAME_LENGTH];
                            printf("\n\n");
                            printf("Enter the name of the patient to search: ");
                            inputString(searchName, sizeof(searchName));

                            int index = searchWard(wards, MAX_WARDS, searchName);
                            if (index != -1) {
                                clear_terminal();
                                printf("Patient found in ward:\n");
                                displayWard(&wards[index], 1);
                                nextPage();
                            } else {
                                printf("Patient not found in any ward.\n");
                            }
                            break;
                        }
                        case 3:
                        {
                            clear_terminal();
                            displayWardMenu();

                            deletePatientFromWard(wards, &lastAssignedWardID, patients, MAX_PATIENTS);
                            break;
                        }
                        case 4:
                        {
                            clear_terminal();
                            displayWardMenu();
                            displayWard(wards, MAX_WARDS);
                            nextPage();
                            break;
                        }
                        case 5:
                            goto mainMenu;
                        default:
                            printf("Invalid choice. Please try again.\n");
                    }
                }
                break;
            }

            case 4:
                savePatientsToFile(patients, "patients.dat");
                saveDoctorsToFile(doctors, "doctors.dat");
                saveIDs(lastAssignedPatientID, lastAssignedDoctorID, lastAssignedWardID);
                saveWardToFile(wards, MAX_WARDS, "wards.dat");

                clear_terminal();
                printf("Exiting the program..\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
                mainMenu:;
        }
    }

    return 0;
}

void getPatientInfo(patientInfo *patient)
{
    printf("Input name: ");
    inputString(patient->patientName, sizeof(patient->patientName));

    printf("Input age: ");
    patient->age = inputInt();

    printf("Input height (in centimeters): ");
    patient->height = inputInt();

    printf("Input weight (in kilograms): ");
    patient->weight = inputInt();

    printf("Input blood group: ");
    inputString(patient->bloodType, sizeof(patient->bloodType));

    printf("Input patient contact number: ");
    inputString(patient->patientContact, sizeof(patient->patientContact));

    printf("Input address: ");
    inputString(patient->patientAddress, sizeof(patient->patientAddress));

    printf("Input emergency contact number: ");
    inputString(patient->emergencyContact, sizeof(patient->emergencyContact));
}


void patientAccountCreation(patientInfo *patient, int *lastAssignedID)
{
    printf("\n\n");
    patient->id = ++(*lastAssignedID);

    printf("Assigned unique ID: %d\n", patient->id);

    printf("Input patient username: ");
    inputString(patient->patientUserName, sizeof(patient->patientUserName));

    printf("Input patient password: ");
    inputString(patient->patientPass, sizeof(patient->patientPass));
}

void getInfo_patient(patientInfo patients[MAX_PATIENTS], int *lastAssignedID)
{
    int numPatients;
    printf("How many patients do you want to add (maximum %d)? ", MAX_PATIENTS);
    numPatients = inputInt();

    if ((*lastAssignedID) + numPatients > MAX_PATIENTS)
    {
        printf("You can add a maximum of %d patients.\n", MAX_PATIENTS - (*lastAssignedID));
        numPatients = MAX_PATIENTS - (*lastAssignedID);
    }

    for (int i = 0; i < numPatients; i++)
    {
        printf("\nInput details for patient %d:\n", i + 1);
        getPatientInfo(&patients[(*lastAssignedID) + i]);
        patientAccountCreation(&patients[(*lastAssignedID) + i], lastAssignedID);
    }
}

int searchPatient(patientInfo patients[MAX_PATIENTS], const char* name)
{
    for (int i = 0; i < MAX_PATIENTS; i++)
    {
        if (strcmp(patients[i].patientName, name) == 0)
        {
            return i;
        }
    }
    return -1;
}

void deletePatient(patientInfo patients[MAX_PATIENTS], const char* name, int *lastAssignedID)
{
    int index = searchPatient(patients, name);
    if (index != -1)
    {
        for (int i = index; i < MAX_PATIENTS - 1; i++)
        {
            patients[i] = patients[i + 1];
            patients[i].id = i + 1;
        }
        memset(&patients[MAX_PATIENTS - 1], 0, sizeof(patientInfo));
        (*lastAssignedID)--;
        printf("Patient '%s' deleted successfully.\n", name);
    }
    else
    {
        printf("Patient '%s' not found.\n", name);
    }
}

void getDoctorInfo(doctorInfo *doctor)
{
    printf("Input name: ");
    inputString(doctor->doctorName, sizeof(doctor->doctorName));

    printf("Input address: ");
    inputString(doctor->doctorAddress, sizeof(doctor->doctorAddress));

    printf("Input specialty: ");
    inputString(doctor->specialty, sizeof(doctor->specialty));

    printf("Input years of experience: ");
    doctor->experience = inputInt();

    printf("Input contact number: ");
    inputString(doctor->doctorContact, sizeof(doctor->doctorContact));
}

void doctorAccountCreation(doctorInfo *doctor, int *lastAssignedID)
{
    doctor->id = ++(*lastAssignedID);

    printf("Assigned unique ID: %d\n", doctor->id);

    printf("Input doctor username: ");
    inputString(doctor->doctorUserName, sizeof(doctor->doctorUserName));

    printf("Input doctor password: ");
    inputString(doctor->doctorPass, sizeof(doctor->doctorPass));
}

void getInfo_doctor(doctorInfo doctors[MAX_DOCTORS], int *lastAssignedID)
{
    int numDoctors;
    printf("How many doctors do you want to add (maximum %d)? ", MAX_DOCTORS);
    numDoctors = inputInt();

    if ((*lastAssignedID) + numDoctors > MAX_DOCTORS)
    {
        printf("You can add a maximum of %d doctors.\n", MAX_DOCTORS - (*lastAssignedID));
        numDoctors = MAX_DOCTORS - (*lastAssignedID);
    }

    for (int i = 0; i < numDoctors; i++)
    {
        printf("\nInput details for doctor %d:\n", i + 1);
        getDoctorInfo(&doctors[(*lastAssignedID) + i]);
        doctorAccountCreation(&doctors[(*lastAssignedID) + i], lastAssignedID);
        initializeCalendar(&doctors[(*lastAssignedID) + i]);
    }
}

int searchDoctor(doctorInfo doctors[MAX_DOCTORS], const char* name)
{
    for (int i = 0; i < MAX_DOCTORS; i++)
    {
        if (strcmp(doctors[i].doctorName, name) == 0)
        {
            return i;
        }
    }
    return -1;
}
void deleteDoctor(doctorInfo doctors[MAX_DOCTORS], const char* name, int *lastAssignedID)
{
    int index = searchDoctor(doctors, name);
    if (index != -1)
    {
        for (int i = index; i < MAX_DOCTORS - 1; i++)
        {
            doctors[i] = doctors[i + 1];
            doctors[i].id = i + 1;
        }
        memset(&doctors[MAX_DOCTORS - 1], 0, sizeof(doctorInfo));
        (*lastAssignedID)--;
        printf("Doctor '%s' deleted successfully.\n", name);
    }
    else
    {
        printf("Doctor '%s' not found.\n", name);
    }
}

void displayPatient(const patientInfo* patient)
{
    printf("\n          %s╔═════════════════════════════════════════════════════════════════╗%s\n", CYAN, RESET);
    printf("          %s║                      Patient Information                        ║%s\n", CYAN, RESET);
    printf("          %s╚═════════════════════════════════════════════════════════════════╝%s\n", CYAN, RESET);
    printf("          %s╔═════════════════════════════════════════════════════════════════╗%s\n", CYAN, RESET);
    printf("          %s║ Name: %-57s ║%s\n", CYAN, patient->patientName, RESET);
    printf("          %s║ Age: %-58d ║%s\n", CYAN, patient->age, RESET);
    printf("          %s║ Height: %-55.2f ║%s\n", CYAN, patient->height, RESET);
    printf("          %s║ Weight: %-55.2f ║%s\n", CYAN, patient->weight, RESET);
    printf("          %s║ Blood Type: %-52s║%s\n", CYAN, patient->bloodType, RESET);
    printf("          %s║ Contact Number: %-48s║%s\n", CYAN, patient->patientContact, RESET);
    printf("          %s║ Address: %-53s  ║%s\n", CYAN, patient->patientAddress, RESET);
    printf("          %s║ Emergency Contact: %-43s  ║%s\n", CYAN, patient->emergencyContact, RESET);
    printf("          %s║ Username: %-49s     ║%s\n", CYAN, patient->patientUserName, RESET);
    printf("          %s║ Password: %-49s     ║%s\n", CYAN, patient->patientPass, RESET);
    printf("          %s║ ID: %-57d   ║%s\n", CYAN, patient->id, RESET);
    printf("          %s╚═════════════════════════════════════════════════════════════════╝%s\n", CYAN, RESET);
}



void displayDoctor(const doctorInfo* doctor)
{
    printf("\n          %s╔═════════════════════════════════════════════════════════════════╗%s\n", GREEN, RESET);
    printf("          %s║                      Doctor Information                         ║%s\n", GREEN, RESET);
    printf("          %s╚═════════════════════════════════════════════════════════════════╝%s\n", GREEN, RESET);
    printf("          %s╔═════════════════════════════════════════════════════════════════╗%s\n", GREEN, RESET);
    printf("          %s║ Name: %-57s ║%s\n", GREEN, doctor->doctorName, RESET);
    printf("          %s║ Address: %-53s  ║%s\n", GREEN, doctor->doctorAddress, RESET);
    printf("          %s║ Specialty: %-51s  ║%s\n", GREEN, doctor->specialty, RESET);
    printf("          %s║ Experience: %-50d  ║%s\n", GREEN, doctor->experience, RESET);
    printf("          %s║ Contact Number: %-46s  ║%s\n", GREEN, doctor->doctorContact, RESET);
    printf("          %s║ Username: %-49s     ║%s\n", GREEN, doctor->doctorUserName, RESET);
    printf("          %s║ Password: %-49s     ║%s\n", GREEN, doctor->doctorPass, RESET);
    printf("          %s║ ID: %-57d   ║%s\n", GREEN, doctor->id, RESET);
    printf("          %s╚═════════════════════════════════════════════════════════════════╝%s\n", GREEN, RESET);
}

void displayPatientMenu()
{
    printf("\n                    %s╔════════════════════════════════════════╗%s\n", YELLOW, RESET);
    printf("                    %s║              Patient Panel             ║%s\n", YELLOW, RESET);
    printf("                    %s╠════════════════════════════════════════╣%s\n", YELLOW, RESET);
    printf("                    %s║ %s1.%s Add patients                        ║%s\n", YELLOW, BLUE, YELLOW, RESET);
    printf("                    %s║ %s2.%s Search patient                      ║%s\n", YELLOW, BLUE, YELLOW, RESET);
    printf("                    %s║ %s3.%s Delete patient                      ║%s\n", YELLOW, BLUE, YELLOW, RESET);
    printf("                    %s║ %s4.%s Display patient information         ║%s\n", YELLOW, BLUE, YELLOW, RESET);
    printf("                    %s║ %s5.%s Patient login                       ║%s\n", YELLOW, BLUE, YELLOW, RESET);
    printf("                    %s║ %s6.%s Back to main menu                   ║%s\n", YELLOW, BLUE, YELLOW, RESET);
    printf("                    %s╚════════════════════════════════════════╝%s\n", YELLOW, RESET);
    printf("\n");
    printf("Enter your choice: ");
}

void displayDoctorMenu()
{
    printf("\n                    %s╔════════════════════════════════════════╗%s\n", MAGENTA, RESET);
    printf("                    %s║              Doctor Panel              ║%s\n", MAGENTA, RESET);
    printf("                    %s╠════════════════════════════════════════╣%s\n", MAGENTA, RESET);
    printf("                    %s║ %s1.%s Add doctors                         ║%s\n", MAGENTA, BLUE, MAGENTA, RESET);
    printf("                    %s║ %s2.%s Search doctor                       ║%s\n", MAGENTA, BLUE, MAGENTA, RESET);
    printf("                    %s║ %s3.%s Delete doctor                       ║%s\n", MAGENTA, BLUE, MAGENTA, RESET);
    printf("                    %s║ %s4.%s Display doctor schedule             ║%s\n", MAGENTA, BLUE, MAGENTA, RESET);
    printf("                    %s║ %s5.%s Doctor login                        ║%s\n", MAGENTA, BLUE, MAGENTA, RESET);
    printf("                    %s║ %s6.%s Back to main menu                   ║%s\n", MAGENTA, BLUE, MAGENTA, RESET);
    printf("                    %s╚════════════════════════════════════════╝%s\n", MAGENTA, RESET);
    printf("\n");
    printf("Enter your choice: ");
}

void displayMainMenu()
{
    printf("\n                    %s╔════════════════════════════════════════╗%s\n", BLUE, RESET);
    printf("                    %s║                 Main Menu              ║%s\n", BLUE, RESET);
    printf("                    %s╠════════════════════════════════════════╣%s\n", BLUE, RESET);
    printf("                    %s║ %s1.%s Patient Panel                       ║%s\n", BLUE, GREEN, BLUE, RESET);
    printf("                    %s║ %s2.%s Doctor Panel                        ║%s\n", BLUE, GREEN, BLUE, RESET);
    printf("                    %s║ %s3.%s Medical Ward Panel                  ║%s\n", BLUE, GREEN, BLUE, RESET);
    printf("                    %s║ %s4.%s Exit                                ║%s\n", BLUE, GREEN, BLUE, RESET);
    printf("                    %s╚════════════════════════════════════════╝%s\n", BLUE, RESET);
    printf("\n");
    printf("Enter your choice: ");
}

void displayWardMenu()
{
    printf("\n                    %s╔════════════════════════════════════════╗%s\n", YELLOW, RESET);
    printf("                    %s║              Medical Ward Panel        ║%s\n", YELLOW, RESET);
    printf("                    %s╠════════════════════════════════════════╣%s\n", YELLOW, RESET);
    printf("                    %s║ %s1.%s Add ward                            ║%s\n", YELLOW, BLUE, YELLOW, RESET);
    printf("                    %s║ %s2.%s Search ward                         ║%s\n", YELLOW, BLUE, YELLOW, RESET);
    printf("                    %s║ %s3.%s Delete ward                         ║%s\n", YELLOW, BLUE, YELLOW, RESET);
    printf("                    %s║ %s4.%s Display ward information            ║%s\n", YELLOW, BLUE, YELLOW, RESET);
    printf("                    %s║ %s5.%s Back to main menu                   ║%s\n", YELLOW, BLUE, YELLOW, RESET);
    printf("                    %s╚════════════════════════════════════════╝%s\n", YELLOW, RESET);
    printf("\n");
    printf("Enter your choice: ");
}

void clear_terminal()
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void nextPage()
{
  char choice[10];
  next:;
  printf("Go to previous panel (yes/no) ?: ");
  inputString(choice, sizeof(choice));

  while(1)
    if(strcasecmp(choice, "yes") == 0)
        break;
    else
    {
        printf("Do you want to exit the program (yes/no) ?: ");
        inputString(choice, sizeof(choice));

        if(strcasecmp(choice, "yes") == 0)
            exit(0);
        else
            goto next;
    }
  clear_terminal();
}

void savePatientsToFile(const patientInfo patients[MAX_PATIENTS], const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error opening file for writing");
        return;
    }
    fwrite(patients, sizeof(patientInfo), MAX_PATIENTS, file);
    fclose(file);
}

void loadPatientsFromFile(patientInfo patients[MAX_PATIENTS], const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file for reading");
        return;
    }
    fread(patients, sizeof(patientInfo), MAX_PATIENTS, file);
    fclose(file);
}

void saveDoctorsToFile(const doctorInfo doctors[MAX_DOCTORS], const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error opening file for writing");
        return;
    }
    fwrite(doctors, sizeof(doctorInfo), MAX_DOCTORS, file);
    fclose(file);
}

void loadDoctorsFromFile(doctorInfo doctors[MAX_DOCTORS], const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file for reading");
        return;
    }
    fread(doctors, sizeof(doctorInfo), MAX_DOCTORS, file);
    fclose(file);
}


void initializeCalendar(doctorInfo *doctors) {
    for (int i = 0; i < DAYS_IN_WEEK; i++) {
        for (int j = 0; j < SLOTS_PER_DAY; j++) {
            for (int k = 0; k < APPOINTMENT_PER_SLOT; k++) {
                doctors->calendar[i][j][k] = 0;
            }
        }
    }
}

void displayCalendar(const doctorInfo *doctors) {
    const char *daysOfWeek[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

    printf("══════════════════════════════════════════════════════════════════════════════\n");
    printf(" Doctor %s's Weekly Calendar:                                     \n", doctors->doctorName);
    printf("══════════════════════════════════════════════════════════════════════════════\n");
    for (int i = 0; i < DAYS_IN_WEEK; i++) {
        printf("%-9s: ", daysOfWeek[i]);
        for (int j = 0; j < SLOTS_PER_DAY; j++) {
            int count = 0;
            for (int k = 0; k < APPOINTMENT_PER_SLOT; k++) {
                if (doctors->calendar[i][j][k] == 1) {
                    count++;
                }
            }
            printf("%-12s (%d appointments)", doctors->calendar[i][j] ? "[Available]" : "[Unavailable]", count);
        }
        printf("\n");
    }
}

void updateCalendar(doctorInfo *doctors) {
    int day, slot;
    printf("Enter the day you want to update (0 for Monday, 1 for Tuesday, ..., 6 for Sunday): ");
    day = inputInt();
    printf("Enter the slot you want to update (0 to 4): ");
    slot = inputInt();

    if (day < 0 || day >= DAYS_IN_WEEK || slot < 0 || slot >= SLOTS_PER_DAY) {
        printf("Invalid day or slot. Please try again.\n");
        return;
    }

    printf("Enter 1 for available or 0 for unavailable: ");
    int status;
    status = inputInt();

    if (status != 0 && status != 1) {
        printf("Invalid status. Please try again.\n");
        return;
    }

    for (int i = 0; i < APPOINTMENT_PER_SLOT; i++) {
        doctors->calendar[day][slot][i] = status;
    }
    printf("Calendar updated successfully.\n");
    printf("Updated schedule:\n");
    displayCalendar(doctors);
}

void appointmentFinalization(doctorInfo doctors[MAX_DOCTORS], patientInfo patients[MAX_PATIENTS])
{
    char patientName[MAX_NAME_LENGTH];
    printf("Enter the name of the patient: ");
    inputString(patientName, sizeof(patientName));

    int patientIndex = searchPatient(patients, patientName);
    if (patientIndex == -1) {
        printf("Patient not found.\n");
        return;
    }

    char doctorName[MAX_NAME_LENGTH];
    printf("Enter the name of the doctor: ");
    inputString(doctorName, sizeof(doctorName));

    int doctorIndex = searchDoctor(doctors, doctorName);
    if (doctorIndex == -1) {
        printf("Doctor not found.\n");
        return;
    }

    takeAppointment(&doctors[doctorIndex], &patients[patientIndex]);
}

void takeAppointment(doctorInfo *doctors, patientInfo *patients)
{
    int day, slot;
    printf("Enter the day you want to take appointment (0 for Monday, 1 for Tuesday, ..., 6 for Sunday): ");
    day = inputInt();
    printf("Enter the slot you want to take appointment (0 to 4): ");
    slot = inputInt();

    if (day < 0 || day >= DAYS_IN_WEEK || slot < 0 || slot >= SLOTS_PER_DAY) {
        printf("Invalid day or slot. Please try again.\n");
        return;
    }

    if (doctors->calendar[day][slot] == 0) {
        printf("Doctor is unavailable at this time. Please choose another slot.\n");
        return;
    }

    for (int i = 0; i < APPOINTMENT_PER_SLOT; i++) {
        if (doctors->calendar[day][slot][i] == 1) {
            doctors->calendar[day][slot][i] = 0;
            printf("Appointment taken successfully.\n");
            return;
        }
    }

    printf("No more appointments available for this slot. Please choose another slot.\n");
}



void notFound(char k)
{
    if(k == 'p')
    {
        printf("          %s╔═════════════════════════════════════════════════════════════════╗%s\n", RED, RESET);
        printf("          %s║                                                                 ║%s\n", RED, RESET);
        printf("          %s║                                                                 ║%s\n", RED, RESET);
        printf("          %s║                                                                 ║%s\n", RED, RESET);
        printf("          %s║                       PATIENT NOT FOUND                         ║%s\n", RED, RESET);
        printf("          %s║                                                                 ║%s\n", RED, RESET);
        printf("          %s║                                                                 ║%s\n", RED, RESET);
        printf("          %s║                                                                 ║%s\n", RED, RESET);
        printf("          %s╚═════════════════════════════════════════════════════════════════╝%s\n", RED, RESET);
    }
    else if(k == 'd')
    {
        printf("          %s╔═════════════════════════════════════════════════════════════════╗%s\n", RED, RESET);
        printf("          %s║                                                                 ║%s\n", RED, RESET);
        printf("          %s║                                                                 ║%s\n", RED, RESET);
        printf("          %s║                                                                 ║%s\n", RED, RESET);
        printf("          %s║                        DOCTOR NOT FOUND                         ║%s\n", RED, RESET);
        printf("          %s║                                                                 ║%s\n", RED, RESET);
        printf("          %s║                                                                 ║%s\n", RED, RESET);
        printf("          %s║                                                                 ║%s\n", RED, RESET);
        printf("          %s╚═════════════════════════════════════════════════════════════════╝%s\n", RED, RESET);

    }
}

int docLogIn(const doctorInfo *doctors, int maxCount, const char *inputUser, const char *pass)
{
    for (int i = 0; i < maxCount; i++)
    {
        if (doctors[i].id != -1 && strcmp(doctors[i].doctorUserName, inputUser) == 0 && strcmp(doctors[i].doctorPass, pass) == 0)
        {
            return i;
        }
    }
    return -1;
}

int patientLogIn(const patientInfo *patients, int maxCount, const char *inputUser, const char *pass)
{
    for (int i = 0; i < maxCount; i++)
    {
        if (patients[i].id != -1 && strcmp(patients[i].patientUserName, inputUser) == 0 && strcmp(patients[i].patientPass, pass) == 0)
        {
            return i;
        }
    }
    return -1;
}

void displayPatientSubMenu() {
    printf("\n                    %s╔════════════════════════════════════════╗%s\n", YELLOW, RESET);
    printf("                    %s║           Patient Sub-Panel            ║%s\n", YELLOW, RESET);
    printf("                    %s╠════════════════════════════════════════╣%s\n", YELLOW, RESET);
    printf("                    %s║ %s1.%s Check patient info                  ║%s\n", YELLOW, BLUE, YELLOW, RESET);
    printf("                    %s║ %s2.%s Edit patient info                   ║%s\n", YELLOW, BLUE, YELLOW, RESET);
    printf("                    %s║ %s3.%s Take doctor's appointment           ║%s\n", YELLOW, BLUE, YELLOW, RESET);
    printf("                    %s║ %s4.%s Back to patient menu                ║%s\n", YELLOW, BLUE, YELLOW, RESET);
    printf("                    %s╚════════════════════════════════════════╝%s\n", YELLOW, RESET);
    printf("\n");
    printf("Enter your choice: ");
}

void displayDoctorSubMenu() {
    printf("\n                    %s╔════════════════════════════════════════╗%s\n", YELLOW, RESET);
    printf("                    %s║           Doctor Sub-Panel             ║%s\n", YELLOW, RESET);
    printf("                    %s╠════════════════════════════════════════╣%s\n", YELLOW, RESET);
    printf("                    %s║ %s1.%s Update schedule                     ║%s\n", YELLOW, BLUE, YELLOW, RESET);
    printf("                    %s║ %s2.%s Edit information                    ║%s\n", YELLOW, BLUE, YELLOW, RESET);
    printf("                    %s║ %s3.%s Prescribe patient                   ║%s\n", YELLOW, BLUE, YELLOW, RESET);
    printf("                    %s║ %s4.%s Back to doctor menu                 ║%s\n", YELLOW, BLUE, YELLOW, RESET);
    printf("                    %s╚════════════════════════════════════════╝%s\n", YELLOW, RESET);
    printf("\n");
    printf("Enter your choice: ");
}

void prescribePatient(patientInfo *patients)
{
    char patientName[MAX_NAME_LENGTH];
    printf("Enter the name of the patient: ");
    inputString(patientName, MAX_NAME_LENGTH);

    printf("=========================================\n");

    int patientIndex = searchPatient(patients, patientName);
    if (patientIndex == -1) {
        notFound('p');
        return;
    }

    char prescription[MAX_DOCTOR_REMARK];
    printf("Enter the prescription for the patient: ");
    printf("=========================================");
    printf("\n");
    inputString(prescription, MAX_DOCTOR_REMARK);

    strcpy(patients[patientIndex].prescription, prescription);
    printf("Prescription updated successfully.\n");
}

void editPatientInfo(patientInfo *patient) {
    printf("Enter new name: ");
    inputString(patient->patientName, MAX_NAME_LENGTH);

    printf("Enter new address: ");
    inputString(patient->patientAddress, MAX_ADDRESS_LENGTH);

    printf("Enter new contact: ");
    inputString(patient->patientContact, MAX_CONTACT_LENGTH);

    printf("Enter new username: ");
    inputString(patient->patientUserName, MAX_USER_NAME);

    printf("Enter new password: ");
    inputString(patient->patientPass, MAX_PASSWORD_LENGTH);

    printf("Enter new age: ");
    patient->age = inputInt();
}

void editDoctorInfo(doctorInfo *doctor) {
    printf("Enter new name: ");
    inputString(doctor->doctorName, MAX_NAME_LENGTH);

    printf("Enter new address: ");
    inputString(doctor->doctorAddress, MAX_ADDRESS_LENGTH);

    printf("Enter new specialty: ");
    inputString(doctor->specialty, MAX_DOCTOR_REMARK);

    printf("Enter new password: ");
    inputString(doctor->doctorPass, MAX_PASSWORD_LENGTH);

    printf("Enter new contact: ");
    inputString(doctor->doctorContact, MAX_CONTACT_LENGTH);

    printf("Enter new username: ");
    inputString(doctor->doctorUserName, MAX_USER_NAME);

    printf("Enter new experience: ");
    doctor->experience = inputInt();
}

void inputString(char* buffer, int length)
{
    fgets(buffer, length, stdin);

    if ((strlen(buffer) > 0) && (buffer[strlen (buffer) - 1] == '\n'))
    {
        buffer[strlen (buffer) - 1] = '\0';
    }
}

int inputInt()
{
    int value;
    scanf("%d", &value);

    while(getchar() != '\n');

    return value;
}

void saveIDs(int patientID, int doctorID, int wardID) {
    FILE *file = fopen("ids.txt", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(file, "%d\n%d\n%d", patientID, doctorID, wardID);
    fclose(file);
}

void loadIDs(int *patientID, int *doctorID, int *wardID) {
    FILE *file = fopen("ids.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    fscanf(file, "%d\n%d\n%d", patientID, doctorID, wardID);
    fclose(file);
}

void deleteAll(patientInfo patients[MAX_PATIENTS], doctorInfo doctors[MAX_DOCTORS], int *lastAssignedPatientID, int *lastAssignedDoctorID, int *lastAssignedWardID)
{
    for (int i = 0; i < MAX_PATIENTS; i++)
    {
        memset(&patients[i], 0, sizeof(patientInfo));
    }
    for (int i = 0; i < MAX_DOCTORS; i++)
    {
        memset(&doctors[i], 0, sizeof(doctorInfo));
    }
    *lastAssignedPatientID = 0;
    *lastAssignedDoctorID = 0;

        printf("          %s╔═════════════════════════════════════════════════════════════════╗%s\n", RED, RESET);
        printf("          %s║                                                                 ║%s\n", RED, RESET);
        printf("          %s║                                                                 ║%s\n", RED, RESET);
        printf("          %s║                                                                 ║%s\n", RED, RESET);
        printf("          %s║                      Congratulations!!!                         ║%s\n", RED, RESET);
        printf("          %s║                  You just NUKED the system!!!                   ║%s\n", RED, RESET);
        printf("          %s║                                                                 ║%s\n", RED, RESET);
        printf("          %s║                                                                 ║%s\n", RED, RESET);
        printf("          %s║                                                                 ║%s\n", RED, RESET);
        printf("          %s╚═════════════════════════════════════════════════════════════════╝%s\n", RED, RESET);


    savePatientsToFile(patients, "patients.dat");
    saveDoctorsToFile(doctors, "doctors.dat");
    saveIDs(*lastAssignedPatientID, *lastAssignedDoctorID, *lastAssignedWardID);
}

void displayPrescription(const patientInfo *patient)
{
    printf("══════════════════════════════════════════════════════════════════════════════\n");
    printf(" Patient %s's Prescription:                                     \n", patient->patientName);
    printf("══════════════════════════════════════════════════════════════════════════════\n");

    printf("Prescription: %s\n", patient->prescription);
    printf("============================================\n");

}

int searchWard(const medicalWard *wards, int maxCount, const char *patientName) {
    for (int i = 0; i < maxCount; i++) {
        if (strcasecmp(wards[i].patientName, patientName) == 0) {
            return i;
        }
    }
    return -1;
}

void addWard(medicalWard *wards, int *lastAssignedWardID, const char *patientName, const char *doctorName) {
    wards[*lastAssignedWardID].wardID = *lastAssignedWardID + 1;
    strncpy(wards[*lastAssignedWardID].patientName, patientName, MAX_NAME_LENGTH);
    strncpy(wards[*lastAssignedWardID].doctorName, doctorName, MAX_NAME_LENGTH);
    (*lastAssignedWardID)++;
}

void deleteWard(medicalWard *wards, int *lastAssignedWardID, const char *patientName) {
    int index = searchWard(wards, *lastAssignedWardID, patientName);
    if (index != -1) {
        for (int i = index; i < *lastAssignedWardID; i++) {
            wards[i] = wards[i + 1];
            wards[i].wardID = i + 1;
        }
        (*lastAssignedWardID)--;
    }
}

void displayWard(const medicalWard *wards, int maxCount) {
    printf("Ward Information:\n");
    for (int i = 0; i < maxCount; i++) {
        printf("============================================\n");
        printf("Ward ID: %d\n", wards[i].wardID);
        printf("Patient Name: %s\n", wards[i].patientName);
        printf("Doctor Name: %s\n", wards[i].doctorName);
        printf("============================================\n");
    }
}

void addPatientToWard(medicalWard *wards, int *lastAssignedWardID, patientInfo *patients, int maxCountPatients, doctorInfo *doctors, int maxCountDoctors)
{
    char patientName[MAX_NAME_LENGTH];
    printf("Enter the name of the patient: ");
    inputString(patientName, MAX_NAME_LENGTH);

    int patientIndex = searchPatient(patients, patientName);
    if (patientIndex == -1) {
        notFound('p');
        return;
    }

    char doctorName[MAX_NAME_LENGTH];
    printf("Enter the name of the doctor: ");
    inputString(doctorName, MAX_NAME_LENGTH);

    int doctorIndex = searchDoctor(doctors, doctorName);
    if (doctorIndex == -1) {
        notFound('d');
        return;
    }

    addWard(wards, lastAssignedWardID, patientName, doctorName);
    printf("Patient added to ward successfully.\n");
}

void deletePatientFromWard(medicalWard *wards, int *lastAssignedWardID, patientInfo *patients, int maxCountPatients)
{
    char patientName[MAX_NAME_LENGTH];
    printf("\n");
    printf("Enter the name of the patient: ");
    inputString(patientName, MAX_NAME_LENGTH);

    int patientIndex = searchPatient(patients, patientName);
    if (patientIndex == -1) {
        notFound('p');
        return;
    }

    deleteWard(wards, lastAssignedWardID, patientName);
    printf("Patient removed from ward successfully.\n");
}

void saveWardToFile(const medicalWard *wards, int maxCount, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error opening file for writing");
        return;
    }
    fwrite(wards, sizeof(medicalWard), maxCount, file);
    fclose(file);
}

void loadWardFromFile(medicalWard *wards, int *lastAssignedWardID, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file for reading");
        return;
    }
    fread(wards, sizeof(medicalWard), MAX_WARDS, file);
    fclose(file);

    for (int i = 0; i < MAX_WARDS; i++) {
        if (wards[i].wardID == 0) {
            *lastAssignedWardID = i;
            break;
        }
    }
}
