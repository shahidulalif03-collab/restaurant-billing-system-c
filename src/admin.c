#include "admin.h"
#include "console.h"
#include "main.h"
#include "menu.h"
#include "report.h"
#include <conio.h>
#include <stdio.h>
#include <windows.h>

int ycod;

typedef struct {
    char username[50];
    char password[50];
} Admin;

Admin owner = {"Sohan", "Sohan252"};

int countInvalidPass = 0;
/*-------------------------------
        Admin Login
--------------------------------*/
void adminLogin(void) {
    char username[50];
    char password[50];
    char conformation;

    Admin admin;

    system("cls");

    gotoxy(30, 2);
    printf("============================================================");

    gotoxy(50, 4);
    printf("ADMIN LOGIN PANEL");

    gotoxy(30, 6);
    printf("============================================================");

    gotoxy(30, 8);
    printf("Do you want to login? (y/n): ");

    conformation = selectionCharInput();

    if(conformation == 'y' || conformation == 'Y') {
        while(1) {
            system("cls");

            gotoxy(30, 8);
            printf("                              ");
            gotoxy(30, 2);
            printf(
                "============================================================");

            gotoxy(50, 4);
            printf("ADMIN LOGIN PANEL");

            gotoxy(30, 6);
            printf(
                "============================================================");
            gotoxy(39, 7);
            printf("┌──────────────────────────────────────┐");
            gotoxy(39, 8);
            printf("│ 🔒 Restricted Area: Authorized Only  │");
            gotoxy(39, 9);
            printf("└──────────────────────────────────────┘");

            gotoxy(36, 12);
            printf("👤 Enter Username : ");
            scanf("%49s", username);
            while(getchar() != '\n')
                ;
            gotoxy(36, 13);
            printf("🔑 Enter Password : ");
            getMaskedPassword(password);

            FILE *fp = fopen("data/admin.dat", "rb");

            if(strcmp(username, owner.username) == 0 &&
               strcmp(password, owner.password) == 0) {
                adminCreate();
                return;
            }

            if(fp == NULL) {
                system("cls");
                gotoxy(36, 10);
                printf("Create Admin First to Enter\n");
                gotoxy(36, 11);
                printf("👉 Press [ANY KEY] to Return");

                getch();
                adminLogin();
                return;
            }

            int found = 0;

            while(fread(&admin, sizeof(Admin), 1, fp) == 1) {
                if(strcmp(username, admin.username) == 0 &&
                   strcmp(password, admin.password) == 0) {
                    found = 1;
                    break;
                }
            }

            fclose(fp);

            if(found) {
                countInvalidPass = 0;

                gotoxy(34, 15);
                printf("┌─────────────────────────────────────────────────┐");
                gotoxy(34, 16);
                printf("│                                                 │");

                gotoxy(40, 17);
                printf("🔑 Verifying Credentials ...");

                gotoxy(40, 18);
                printf("[✔] Login Successful! Welcome Admin.");

                gotoxy(34, 19);
                printf("│                                                 │");

                gotoxy(34, 20);
                printf("└─────────────────────────────────────────────────┘");
                gotoxy(36, 21);
                printf("👉 Press ANY KEY to enter...");
                getch();
                adminMenu();
                return;
            } else {
                countInvalidPass++;

                if(countInvalidPass == 3) {
                    system("cls");

                    gotoxy(30, 8);
                    printf("┌──────────────────────────────────────────────"
                           "──┐");
                    gotoxy(30, 9);
                    printf("│                                              "
                           "  │");

                    gotoxy(35, 10);
                    printf("🔒 SECURITY ALERT: ACCESS DENIED");

                    gotoxy(31, 11);
                    printf("    [❌] Too many failed login attempts!");
                    gotoxy(31, 12);
                    printf("     Your temporary access has been blocked.");

                    gotoxy(31, 13);
                    printf("💡 Tip: Please check your Caps Lock and try "
                           "again.");

                    gotoxy(30, 14);
                    printf("│                                              "
                           "  │");

                    gotoxy(30, 15);
                    printf("└──────────────────────────────────────────────"
                           "──┘");
                    gotoxy(33, 16);
                    printf("👉 Press [ANY KEY] to return to Home Screen...");

                    getch();

                    countInvalidPass = 0;
                    showWelcomeScreen();
                    return;

                } else {
                    gotoxy(30, 12);
                    printf("┌──────────────────────────────────────────────"
                           "──┐");
                    gotoxy(30, 13);
                    printf("│                                              "
                           "  │");

                    gotoxy(39, 14);
                    printf("🔑 Verifying Credentials ...");

                    gotoxy(39, 16);
                    printf("[❌] LOGIN FAILED!");
                    gotoxy(39, 17);
                    printf("⚠️  Invalid Username or Password.");

                    gotoxy(39, 19);
                    printf("⏳ Remaining Attempts: [ %d / 3 ]",
                           3 - countInvalidPass);

                    gotoxy(30, 20);
                    printf("│                                              "
                           "  │");

                    gotoxy(30, 21);
                    printf("└──────────────────────────────────────────────"
                           "──┘");
                    gotoxy(34, 22);
                    printf("👉 Press [ANY KEY] to re-enter credentials...");

                    getch();
                    continue;
                }
            }
        }
    } else if(conformation == 'n' || conformation == 'N') {
        showWelcomeScreen();
    } else {
        gotoxy(30, 10);
        printf("⚠️  Invalid Selection");
        gotoxy(30, 11);
        printf("👉 Press ANY KEY to try again...");
        getch();
        adminLogin();
    }
}

/*-------------------------------
        Admin Menu
--------------------------------*/
void adminMenu() {
    system("cls");

    int selectionNum;

    gotoxy(30, 2);
    printf("============================================================");

    gotoxy(51, 4);
    printf("ADMIN DASHBOARD");

    gotoxy(30, 6);
    printf("============================================================");

    gotoxy(41, 7);
    printf("┌──────────────────────────────────────┐");
    gotoxy(41, 8);
    printf("│   ⚙️  CONTROL & MANAGEMENT PANEL     │");
    gotoxy(41, 9);
    printf("└──────────────────────────────────────┘");

    gotoxy(42, 11);
    printf("[1] 📋 View All Food Items");
    gotoxy(42, 12);
    printf("[2] 📝 View Total Sales / Bills");
    gotoxy(42, 13);
    printf("[3] 📋 View Active Orders");
    gotoxy(42, 14);
    printf("[4] 📋 View Customer Review");
    gotoxy(42, 15);
    printf("[5] 🔙 Logout & Return");

    gotoxy(30, 16);
    printf("============================================================");

    gotoxy(36, 18);
    printf("➔ Select Option (1-5): ");

    ycord = 18;

    selectionNumInput(&selectionNum, ycord, adminMenu);

    switch(selectionNum) {
    case 1:
        viewAllFood();
        break;

    case 2:
        generateSalesReport();
        break;
    case 3:
        viewAllOrders();
        break;

    case 4:
        viewUserReview();
        break;

    case 5:
        showWelcomeScreen();
        break;

    default:
        gotoxy(30, 22 + ycod);
        printf("⚠️  Invalid Selection");
        gotoxy(30, 23 + ycod);
        printf("👉 Press ANY KEY to try again...");
        getch();
        adminMenu();
    }
}

/*-------------------------------
        Password Input with (*)
--------------------------------*/
void getMaskedPassword(char pass[]) {
    int ch;
    int i = 0;

    while(1) {
        ch = getch();

        // Enter
        if(ch == '\n' || ch == '\r')
            break;

        // Backspace
        if((ch == 127 || ch == 8) && i > 0) {
            i--;
            printf("\b \b");
            fflush(stdout);
        } else if(i < 49) {
            pass[i++] = (char)ch;
            printf("*");
            fflush(stdout);
        }
    }

    pass[i] = '\0';
}

/*-------------------------------
        Admin Create
--------------------------------*/
void adminCreate(void) {
    char username[50];
    char password[50];
    char confirm_password[50];
    Admin admin;

    while(1) {
        system("cls");

        gotoxy(30, 2);
        printf("============================================================");

        gotoxy(46, 4);
        printf("ADMIN CREDENTIAL SETUP");

        gotoxy(30, 6);
        printf("============================================================");

        gotoxy(36, 12);
        printf("👤 Enter New Username : ");
        scanf("%49s", username);
        while(getchar() != '\n')
            ;

        gotoxy(36, 13);
        printf("🔑 Enter New Password : ");
        getMaskedPassword(password);

        gotoxy(36, 14);
        printf("🔑 Confirm Password   : ");
        getMaskedPassword(confirm_password);

        if(strcmp(password, confirm_password) != 0) {
            gotoxy(36, 16);
            printf("⚠️  Passwords do not match. Press ANY KEY to retry...");
            getch();
            continue;
        }

        strcpy(admin.username, username);
        strcpy(admin.password, password);

        FILE *fp = fopen("data/admin.dat", "ab");
        if(fp == NULL) {
            gotoxy(36, 16);
            printf("⚠️  Could not create admin data file.");
            gotoxy(36, 17);
            printf("👉 Press ANY KEY to return...");
            getch();
            return;
        }

        fwrite(&admin, sizeof(Admin), 1, fp);
        fclose(fp);

        gotoxy(36, 16);
        printf("[✔] Admin credentials created successfully.");
        gotoxy(36, 17);
        printf("👉 Press ANY KEY to go to Admin Login...");
        getch();
        showWelcomeScreen();
        return;
    }
}