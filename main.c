//
//  main.c
//  Project3
//
//  Created by Javier Vega on 4/21/15.
//  Copyright (c) 2015 C for Engineeers. All rights reserved.
//


/*
 The amount of taxes one has taken from one's pay is complicated.
 Everyone has 6.2% taken out for Social Security (unless you make boo coo mulah) and 1.45% taken out for Medicare. 
 Florida has no state income tax, so you don't need to worry about that.
*/

#include <stdio.h>
#include <string.h>
#define SIZE 25
#define WORKERS 30

//Worker structure definition
struct worker{
    unsigned int idNumber;  //Employee id number
    char lastName[SIZE];  //Employee last name
    char firstName[SIZE];  //Employee first name
    int hours;  //Employee hours worked.
    double rate;  //Employee's income by hour.
    double current;  //What the company pay it's employee before tax.
    
    struct{
     double federalWithhold;
     double SSEmployee;  //Social security employee.
     double medicareEmployee;
     }taxes;
    
    double netPay;
    char payrollRank[SIZE];
};

typedef struct worker Worker;  //defines a new tipe of worker

void printRecords(void);  //Function Prototype.
void addEmployee(FILE * cPtr);  //Function Prototype.
void deleteEmployee(FILE * cPtr);  //Function Prototype.
void createTextFile(FILE * rPtr);  //Function Prototype.
void updateEmployeeInfo(FILE * cPtr);  //Function Prototype.
void displayMenu();  //Function Prototype.
void getCurrent(struct worker * const employee, int, double);  //Function Prototype.
void getTaxes(Worker * const employee, double current);  //Function Prototype.
void getNetPay(Worker * const employee, double current, double sseEmp, double fedWithhold, double medEmp);
void getRank(Worker * const employee, double netPay);  //Function Prototype.

int main(void) {
    int c;
    
    FILE * cfilePtr; //Creates a new file pointer
    
    if ((cfilePtr = fopen("/Users/javiervega/Desktop/College/College(2nd Semester)/C-For Engineers/Project3/RandomAccessFile(3)/PayRollSystem.dat", "rb+")) == NULL) {
        puts("File content can not be display.");
        
    }
    else{
        
        displayMenu();
        while ((c = getchar()) != '9') {
            
            switch (c) {
                case '1':
                    puts("");
                    puts("******************************** RECORDS ********************************");
                    puts("");
                    printRecords();
                    puts("");
                    break;
                case '2':
                    puts("");
                    createTextFile(cfilePtr);
                    puts("");
                    puts("Records were successfully placed in text file.");
                    break;
                case '3':
                    addEmployee(cfilePtr);
                    rewind(cfilePtr);
                    break;
                case '4':
                    deleteEmployee(cfilePtr);
                    rewind(cfilePtr);
                    break;
                case '5':
                    updateEmployeeInfo(cfilePtr);
                    rewind(cfilePtr);
                    break;
                default:
                    puts("Incorrect Command");
                    break;
            }
            
            puts("");
            displayMenu();
            getchar();
        }
    }
    
    fclose(cfilePtr);
    return 0;
}

//Displays main menu.
void displayMenu(){
    puts("------- MAIN MENU -------");
    puts("1 - Print Employees Information");
    puts("2 - Create Text File/Update Text File");
    puts("3 - Add Employee");
    puts("4 - Delete Employee");
    puts("5 - Update Employee Information");
    puts("9 - To Exit");
    
}

//Calculates the actual gross pay
void getCurrent(Worker * const employee, int hours, double rate){
    employee->current = hours * rate;  //Calculates employee current
    
}

//Calculates the taxes.
void getTaxes(Worker * const employee, double current){
    double tax = 6.2; //Social Security
    double medTax = 1.45;  //Medicare
    double fedTax = 13.5;  //Federal Withhold
    
    employee->taxes.SSEmployee = (tax / 100) * current;  //Calc what will be deducted
    employee->taxes.federalWithhold = (fedTax / 100) * current;
    employee->taxes.medicareEmployee = (medTax / 100) * current;

}

//Calculates gross pay after taxes.
void getNetPay(Worker * const employee, double current, double sseEmp, double fedWithhold, double medEmp){
    
    employee->netPay = current - (sseEmp + fedWithhold + medEmp);
}

//Gets the employee's ranking
void getRank(Worker * const employee, double rate){
    //Entry level lower than $13 Junior 13 to 32 Senior 32 up
    
    const char entryLevel[] = "Entry";
    const char juniorLevel[] = "Junior";
    const char seniorLevel[] = "Senior";
    int castedRate;
    
    castedRate = (int)rate;  //Casts current rate in order to test for rank.
    
    //Determine of employye is a senior base on their salarie
    if (castedRate >= 30) {
        strncpy(employee->payrollRank, seniorLevel, SIZE - 1);
        employee->payrollRank[SIZE - 1] = '\0';
    }
    
    //Determine of employye is a junior base on their salarie
    else if(castedRate < 30 && castedRate > 13){
        strncpy(employee->payrollRank, juniorLevel, SIZE - 1);
        employee->payrollRank[SIZE - 1] = '\0';
    }
    
    //Determine of employye is a entry  base on their salarie
    else if (castedRate  <= 13) {
        strncpy(employee->payrollRank, entryLevel, SIZE - 1);
        employee->payrollRank[SIZE - 1] = '\0';
    }
    
}

//Prints the record that are in the file
void printRecords(void){
    
    FILE * cfPtr;  //Creates a file
    
    struct worker employee = {0, "", "", 0.0, 0, 0.0, 0.0, 0.0, 0.0, 0.0, ""};  //Struct employee
    long int result;
    double totalTax;
    
    if ((cfPtr = fopen("/Users/javiervega/Desktop/College/College(2nd Semester)/C-For Engineers/Project3/RandomAccessFile(3)/PayRollSystem.dat", "rb")) == NULL){
        puts("File could not be open");
    }
    else{
        
        while (!feof(cfPtr)) {
            result = fread(&employee, sizeof(struct worker), 1, cfPtr);
            
            if (result != 0 && employee.idNumber != 0){
                //Add Deductions
                totalTax = employee.taxes.federalWithhold + employee.taxes.medicareEmployee + employee.taxes.SSEmployee;
                
                printf("%3s%-30s%-20s%s\n", "","ID NUMBER", "RANK", "EMPLOYEE");
                printf("%3s%-30d%-20s%-s, %s\n", "", employee.idNumber, employee.payrollRank, employee.lastName, employee.firstName);
                printf("\n%3s%-30s%-10s%-10s%-10s\n", "", "EARNINGS AND HOURS", "HOURS", "RATE", "CURRENT");
                printf("%3s%-30s%-10d%-10.2f%-10.2f\n", "", "Straigh Time", employee.hours, employee.rate, employee.current);
                puts("--------------------------------------------------------------------");
                printf("%3s%-50s\n", "", "TAXES");
                printf("%3s%-50s- %.2f\n", "", "Federal Withhold", employee.taxes.federalWithhold);
                printf("%3s%-50s- %.2f\n", "", "Social Security Employee", employee.taxes.SSEmployee);
                printf("%3s%-50s- %.2f\n", "", "Medicare Employee", employee.taxes.medicareEmployee);
                puts("--------------------------------------------------------------------");
                printf("%3s%-50s- %.2f\n\n", "", "Total Taxes", totalTax);
                printf("%3s%-52s%.2f\n\n", "", "Net Pay", employee.netPay);
                puts("*************************************************************************\n");
            }
        }
        fclose(cfPtr);
        
    }
}

//Creates a text file containg the list of employees.
void createTextFile(FILE * rPtr){
    FILE * writePtr;
    long int result;
    double totalTax;
    
    struct worker employee = {0, "", "", 0.0, 0, 0.0, 0.0, 0.0, 0.0, 0.0, ""};
    
    if ((writePtr = fopen("//Users/javiervega/Desktop/College/College(2nd Semester)/C-For Engineers/Project3/Rec.txt", "w")) == NULL){
        puts("File could not be open.");
    }
    else{
        rewind(rPtr);
        while (!feof(rPtr)) {
            
            result = fread(&employee, sizeof(struct worker), 1, rPtr);
            
            if (result != 0 && employee.idNumber != 0) {
                totalTax = employee.taxes.federalWithhold + employee.taxes.medicareEmployee + employee.taxes.SSEmployee;
                
                fprintf(writePtr, "%3s%-30s%-20s%s\n", "","ID NUMBER", "RANK", "EMPLOYEE");
                fprintf(writePtr, "%3s%-30d%-20s%-s, %s\n", "", employee.idNumber, employee.payrollRank,
                        employee.lastName, employee.firstName);
                fprintf(writePtr, "\n%3s%-30s%-10s%-10s%-10s\n", "", "EARNINGS AND HOURS", "HOURS", "RATE",
                        "CURRENT");
                fprintf(writePtr, "%3s%-30s%-10d%-10.2f%-10.2f\n", "", "Straigh Time", employee.hours,
                        employee.rate, employee.current);
                fprintf(writePtr, "--------------------------------------------------------------------\n");
                fprintf(writePtr, "%3s%-50s\n", "", "TAXES");
                fprintf(writePtr, "%3s%-50s- %.2f\n", "", "Federal Withhold", employee.taxes.federalWithhold);
                fprintf(writePtr, "%3s%-50s- %.2f\n", "", "Social Security Employee",employee.taxes.SSEmployee);
                fprintf(writePtr, "%3s%-50s- %.2f\n", "", "Medicare Employee", employee.taxes.medicareEmployee);
                fprintf(writePtr, "--------------------------------------------------------------------\n");
                fprintf(writePtr, "%3s%-50s- %.2f\n\n", "", "Total Taxes", totalTax);
                fprintf(writePtr, "%3s%-50s%.2f\n\n", "", "Net Pay", employee.netPay);
                fprintf(writePtr, "**********************************************************************\n\n");
                
            }
        }
        fclose(writePtr);
    }
    
    
}

//Add a new employee to the list.
void addEmployee(FILE * cPtr){
    struct worker employee = {0, "", "", 0.0, 0, 0.0, 0.0, 0.0, 0.0, 0.0, ""};
    
    int idNum;
    int choice = 0;
    
    do{
        //Gets the Account Number of the Employee.
        puts("");
        puts("Enter New Employee ID Number ");
        printf(">> ");
        scanf("%d", &idNum);
    
        fseek(cPtr, (idNum - 1) * sizeof(struct worker), SEEK_SET);  //moves the pointer to the correct location.
        fread(&employee, sizeof(struct worker), 1, cPtr);  //Read record from file.
    
        if(employee.idNumber != 0){
            printf("Employee with id number %d already exist\n", employee.idNumber);
        }
        else{
            //Gets the Name of the Employee.
            puts("Enter Employee Name: (Last Name, Fist Name)");
            printf(">> ");
            scanf("%24s %24s", employee.lastName, employee.firstName);
        
            puts("");
            
            //Gets the Employee houly rate.
            puts("Enter Employee Hourly Rate");
            printf(">> ");
            scanf("%lf", &employee.rate);
        
            puts("");
            
            //Gets the Employee hours worked.
            puts("Enter Hours Worked");
            printf(">> ");
            scanf("%d", &employee.hours);
        
            employee.idNumber = idNum;
        
            getCurrent(&employee, employee.hours, employee.rate);
            getTaxes(&employee, employee.current);
            getNetPay(&employee, employee.current, employee.taxes.SSEmployee, employee.taxes.federalWithhold, employee.taxes.medicareEmployee);
            getRank(&employee, employee.rate);
            
            fseek(cPtr, (employee.idNumber - 1) * sizeof(struct worker), SEEK_SET);  //moves the pointer to the correct location
            fwrite(&employee, sizeof(struct worker), 1, cPtr);  //writes to the file
        
        }
        
        //Gets the choice.
        puts("");
        puts("1 - Add another employee");
        puts("2 - Main Menu");
        printf(">> ");
        scanf("%d", &choice);
    }while (choice == 1);
}

//Deletes employee from the list
void deleteEmployee(FILE * cPtr){
    
    struct worker employee = {0, "", "", 0.0, 0, 0.0, 0.0, 0.0, 0.0, 0.0, ""};
    struct worker blanckEmployee = {0, "", "", 0.0, 0, 0.0, 0.0, 0.0, 0.0, 0.0, ""};
    unsigned int idNumber;
    int choice = 0;
    
    do{
        puts("Enter Employee Id Number to delete ");
        scanf("%d", &idNumber);
    
        fseek(cPtr, (idNumber - 1) * sizeof(struct worker), SEEK_SET);
        fread(&employee, sizeof(struct worker), 1, cPtr);
    
        if (employee.idNumber == 0 ) {
            printf("The Employee with id number %d does not exist.\n", idNumber);
        }
        else{
        
            fseek(cPtr, (idNumber - 1) * sizeof(struct worker), SEEK_SET);
            fwrite(&blanckEmployee, sizeof(struct worker), 1, cPtr);
        
        }
        
        puts("");
        puts("1 - Delete another employee");
        puts("2 - Main Menu");
        printf(">> ");
        scanf("%d", &choice);
    }while (choice == 1);
}

//Updates employees information
void updateEmployeeInfo(FILE * cPtr){
    struct worker employee = {0, "", "", 0.0, 0, 0.0, 0.0, 0.0, 0.0, 0.0, ""};
    double totalTax;
    
    int idNum;
    int choice = 0;  //Initialize choice.
    int upChoice = 0;  //Initialize update choice.
    int hours;
    double rate;
    char last[SIZE];
    char first[SIZE];
    
    do {
        puts("");
        puts("Enter id number of employee");
        printf(">> ");
        scanf("%d", &idNum);
    
        fseek(cPtr, (idNum - 1) * sizeof(struct worker), SEEK_SET);
        fread(&employee, sizeof(struct worker), 1, cPtr);
    
        if (employee.idNumber == 0) {
            printf("Employee with id number [%d] has no information.\n", idNum);
        }
        else{
            totalTax = employee.taxes.federalWithhold + employee.taxes.medicareEmployee + employee.taxes.SSEmployee;
            puts("");
            printf("********************************** RECORD *******************************\n");
            printf("%3s%-30s%-20s%s\n", "","ID NUMBER", "RANK", "EMPLOYEE");
            printf("%3s%-30d%-20s%-s, %s\n", "", employee.idNumber, employee.payrollRank, employee.lastName, employee.firstName);
            printf("\n%3s%-30s%-10s%-10s%-10s\n", "", "EARNINGS AND HOURS", "HOURS", "RATE", "CURRENT");
            printf("%3s%-30s%-10d%-10.2f%-10.2f\n", "", "Straigh Time", employee.hours, employee.rate, employee.current);
            puts("--------------------------------------------------------------------");
            printf("%3s%-50s\n", "", "TAXES");
            printf("%3s%-50s- %.2f\n", "", "Federal Withhold", employee.taxes.federalWithhold);
            printf("%3s%-50s- %.2f\n", "", "Social Security Employee", employee.taxes.SSEmployee);
            printf("%3s%-50s- %.2f\n", "", "Medicare Employee", employee.taxes.medicareEmployee);
            puts("--------------------------------------------------------------------");
            printf("%3s%-50s- %.2f\n\n", "", "Total Taxes", totalTax);
            printf("%3s%-52s%.2f\n\n", "", "Net Pay", employee.netPay);
            puts("*************************************************************************\n");
            
            puts("1 - Update hours worked");
            puts("2 - Update hourly rate");
            puts("3 - Correct employee name");
            scanf("%d", &upChoice);
            
            if (upChoice == 1) {
                puts("Enter hours worked");
                printf(">> ");
                scanf("%d", &hours);
                employee.hours = hours;
                
            }
            else if (upChoice == 2){
                puts("Enter new hourly rate");
                printf(">> ");
                scanf("%lf", &rate);
                employee.rate = rate;
            }
            else if (upChoice == 3){
                puts("Please enter the employee's name carefully (Last, First)");
                printf(">> ");
                scanf("%24s %24s", last, first);
                
                strncpy(employee.lastName, last, SIZE - 1);
                employee.lastName[SIZE - 1] = '\0';
                
                strncpy(employee.firstName, first, SIZE - 1);
                employee.firstName[SIZE - 1] = '\0';
            }
            
            getCurrent(&employee, employee.hours, employee.rate);
            getTaxes(&employee, employee.current);
            getNetPay(&employee, employee.current, employee.taxes.SSEmployee, employee.taxes.federalWithhold, employee.taxes.medicareEmployee);
            getRank(&employee, employee.rate);
            
            fseek(cPtr, (idNum - 1) * sizeof(struct worker), SEEK_SET);
            fwrite(&employee, sizeof(struct worker), 1, cPtr);
            
        }
        
        puts("Update Completed...");
        
        totalTax = employee.taxes.federalWithhold + employee.taxes.medicareEmployee + employee.taxes.SSEmployee;
        
        puts("");
        printf("********************************** RECORD *******************************\n");
        printf("%3s%-30s%-20s%s\n", "","ID NUMBER", "RANK", "EMPLOYEE");
        printf("%3s%-30d%-20s%-s, %s\n", "", employee.idNumber, employee.payrollRank, employee.lastName, employee.firstName);
        printf("\n%3s%-30s%-10s%-10s%-10s\n", "", "EARNINGS AND HOURS", "HOURS", "RATE", "CURRENT");
        printf("%3s%-30s%-10d%-10.2f%-10.2f\n", "", "Straigh Time", employee.hours, employee.rate, employee.current);
        puts("--------------------------------------------------------------------");
        printf("%3s%-50s\n", "", "TAXES");
        printf("%3s%-50s- %.2f\n", "", "Federal Withhold", employee.taxes.federalWithhold);
        printf("%3s%-50s- %.2f\n", "", "Social Security Employee", employee.taxes.SSEmployee);
        printf("%3s%-50s- %.2f\n", "", "Medicare Employee", employee.taxes.medicareEmployee);
        puts("--------------------------------------------------------------------");
        printf("%3s%-50s- %.2f\n\n", "", "Total Taxes", totalTax);
        printf("%3s%-52s%.2f\n\n", "", "Net Pay", employee.netPay);
        puts("*************************************************************************\n");
        
        puts("");
        puts("1 - Update another employee");
        puts("2 - Main Menu");
        printf(">> ");
        scanf("%d", &choice);
    } while (choice == 1);
}

