#include "admin.h"
#include "console.h"
#include "main.h"
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h >

int ch;
int ycod = 0;
char conformation;
float total;

struct Food {
    int id;
    char date[15];
    char name[50];
    float price;
};

struct Food menu[100];
struct Food temp;
struct Food item;
int totalItems = 0;

int viewmenu(void);
void viewAllFood(void);
void addfooditem(void);
void deletefooditem(void);
void updatefooditem(void);
void saveMenu(void);
int loadMenuItems(void);
int isDuplicateID(int newID, int currentID);

int loadMenuItems(void) {
    FILE *fp = fopen("data/menu.dat", "rb");
    if(fp == NULL) {
        return 0;
    }

    long fileSize;
    if(fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        return 0;
    }
    fileSize = ftell(fp);
    rewind(fp);

    size_t recordSize = sizeof(struct Food);
    if(recordSize == 0 || fileSize % recordSize != 0) {
        fclose(fp);
        remove("data/menu.dat");
        return 0;
    }

    int count = 0;
    while(count < 100 && fread(&menu[count], recordSize, 1, fp) == 1) {
        if(menu[count].id <= 0 || menu[count].name[0] == '\0' ||
           menu[count].date[0] == '\0') {
            fclose(fp);
            remove("data/menu.dat");
            return 0;
        }
        count++;
    }

    fclose(fp);
    return count;
}

void viewAllFood(void) {
    int selectionNum;

    viewmenu();
    ycod = viewmenu();
    gotoxy(40, 13 + ycod);
    printf("[1] ➕ Add New Food Item");
    gotoxy(40, 14 + ycod);
    printf("[2] 🔄 Update Food Item");
    gotoxy(40, 15 + ycod);
    printf("[3] ❌ Remove Food Item");
    gotoxy(40, 16 + ycod);
    printf("[4] 🔙  Back");

    gotoxy(35, 18 + ycod);
    printf("➔ Select Option (1-4): ");

    selectionNumInput(&selectionNum, 18 + ycod, adminMenu);

    switch(selectionNum) {
    case 1:
        addfooditem();
        break;

    case 2:
        updatefooditem();
        break;

    case 3:
        deletefooditem();
        break;
    case 4:
        adminMenu();
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

int viewmenu(void) {
    system("cls");

    ycod = 0;
    totalItems = loadMenuItems();

    gotoxy(12, 3);
    printf("==============================================================="
           "===============================");
    gotoxy(52, 5);
    printf("MENU VIEW");
    gotoxy(12, 7);
    printf("==============================================================="
           "===============================");

    int has_items = 0;

    gotoxy(12, 8);
    printf("  #%-6s | %10s |  %-54s |  %-10s", "ID", "Update Date",
           "Food Item name", "Price");
    gotoxy(12, 9);
    printf("----------+-------------+-----------------------------------------"
           "----------------+-----------");

    for(int i = 0; i < totalItems; i++) {
        gotoxy(12, 10 + ycod);
        printf("  #%-6d | %11s | %-55s |  %-10.2f", menu[i].id, menu[i].date,
               menu[i].name, menu[i].price);
        ycod++;
        has_items = 1;
        total += menu[i].price;
    }

    gotoxy(12, 10 + ycod);
    printf("----------+-------------+-------------------------------------"
           "--------------------+-----------");

    if(!has_items) {
        system("cls");

        gotoxy(12, 3);
        printf("======================================================="
               "=======================================");
        gotoxy(50, 5);
        printf("MENU VIEW");
        gotoxy(12, 7);
        printf("======================================================="
               "=======================================");
        gotoxy(48, 8);
        printf("Menu is empty.");
        ycod = 1;
    }

    gotoxy(12, 11 + ycod);
    printf("==============================================================="
           "===============================");

    gotoxy(95, 12 + ycod);
    printf("%f", total);

    return ycod;
}

void addfooditem(void) {
    system("cls");

    gotoxy(30, 3);
    printf("============================================================");

    gotoxy(50, 5);
    printf("➕ ADD NEW FOOD ITEM");

    gotoxy(30, 7);
    printf("============================================================");

    gotoxy(12, 9);
    printf("Do you want to add an item to cart? (y/n): ");
    char confirm = selectionCharInput();

    memset(&menu[totalItems], 0, sizeof(menu[totalItems]));

    if(confirm == 'y' || confirm == 'Y') {
        gotoxy(12, 9);
        printf("                                            ");

        while(1) {
            gotoxy(32, 9);
            printf("➔ Enter ID    : ");

            if(scanf("%d", &menu[totalItems].id) == 1) {
                ch = getchar();

                if(ch == '\n') {
                    if(isDuplicateID(menu[totalItems].id, -1)) {
                        gotoxy(32, 13);
                        printf("❌ ID already exists! Enter another ID...");

                        getch();

                        gotoxy(43, 9);
                        printf("                ");
                        gotoxy(32, 13);
                        printf("                                         ");
                        continue;
                    }
                    break;
                }
            }
            while((ch = getchar()) != '\n' && ch != EOF)
                ;
            gotoxy(43, 9);
            printf("                ");
        }

        gotoxy(32, 10);
        printf("➔ Enter Name  : ");
        scanf(" %[^\n]", menu[totalItems].name);

        while(1) {
            gotoxy(32, 11);
            printf("➔ Enter Price : ");

            if(scanf("%f", &menu[totalItems].price) == 1) {
                ch = getchar();
                if(ch == '\n') {
                    break;
                }
            }

            gotoxy(48, 11);
            printf("                    ");

            while((ch = getchar()) != '\n' && ch != EOF)
                ;
        }

        gotoxy(30, 13);
        printf("------------------------------------------------------------");

        totalItems++;

        gotoxy(32, 14);
        printf("[✔] Food Item Added Successfully!");
        saveMenu();
    } else if(confirm == 'n' || confirm == 'N') {
        viewAllFood();
        return;
    } else {
        gotoxy(30, 13);
        printf("⚠️  Invalid Selection");
        gotoxy(30, 14);
        printf("👉 Press ANY KEY to try again...");
        getch();
        addfooditem();
        return;
    }
}

void deletefooditem(void) {
    system("cls");
    int id;
    int found = 0;

    viewmenu();

    gotoxy(12, 12 + ycod);
    printf("Do you want to add an item to cart? (y/n): ");

    char confirm = selectionCharInput();

    if(confirm == 'y' || confirm == 'Y') {
        gotoxy(12, 12 + ycod);
        printf("                                            ");
        gotoxy(32, 13 + ycod);
        printf("Enter Food ID : ");
        scanf("%d", &id);

        while(getchar() != '\n')
            ;

        FILE *fp = fopen("data/menu.dat", "rb");
        FILE *temp = fopen("data/temp.dat", "wb");

        if(fp == NULL || temp == NULL) {
            printf("File opening failed!\n");

            if(fp)
                fclose(fp);
            if(temp)
                fclose(temp);

            getch();
            return;
        }

        while(fread(&item, sizeof(struct Food), 1, fp) == 1) {
            if(item.id == id) {
                found = 1;
                continue;
            }
            fwrite(&item, sizeof(struct Food), 1, temp);
        }

        fclose(fp);
        fclose(temp);

        remove("data/menu.dat");
        rename("data/temp.dat", "data/menu.dat");

        if(found) {
            gotoxy(48, 13 + ycod);
            printf("[✔] Food Item Deleted Successfully!");
        } else {
            gotoxy(48, 13 + ycod);
            printf("[✘] Food ID Not Found!");
        }

        gotoxy(32, 15 + ycod);
        printf("Do you want to remove more items? (y/n): ");

        conformation = selectionCharInput();

        if(conformation == 'y' || conformation == 'Y') {
            deletefooditem();
            return;
        } else if(conformation == 'n' || conformation == 'N') {
            viewAllFood();
            return;
        } else {
            gotoxy(32, 17 + ycod);
            printf("⚠️  Invalid Selection");
            gotoxy(32, 18 + ycod);
            printf("👉 Press ANY KEY to try again...");
            getch();
            deletefooditem();
            return;
        }
    } else if(confirm == 'n' || confirm == 'N') {
        viewAllFood();
        return;
    } else {
        gotoxy(32, 17 + ycod);
        printf("⚠️  Invalid Selection");
        gotoxy(32, 18 + ycod);
        printf("👉 Press ANY KEY to try again...");
        getch();
        deletefooditem();
        return;
    }
}

void updatefooditem(void) {
    system("cls");

    viewmenu();

    struct tm *date;
    time_t t;
    t = time(NULL);
    date = localtime(&t);

    int id;
    int found = 0;

    gotoxy(30, 3);
    printf("============================================================");

    gotoxy(51, 5);
    printf("🔄 Update Food Item");

    gotoxy(30, 7);
    printf("============================================================");

    gotoxy(32, 12 + ycod);
    printf("Do you want to add an item to cart? (y/n): ");
    char confirm = selectionCharInput();

    if(confirm == 'y' || confirm == 'Y') {
        gotoxy(32, 12 + ycod);
        printf("                                                ");
        gotoxy(25, 12 + ycod);
        printf("Enter Food ID to update: ");
        scanf("%d", &id);
        while(getchar() != '\n')
            ;

        FILE *fp = fopen("data/menu.dat", "rb");
        FILE *temp = fopen("data/temp.dat", "wb");

        if(fp == NULL || temp == NULL) {
            printf("File opening failed!\n");

            if(fp)
                fclose(fp);
            if(temp)
                fclose(temp);

            getch();
            return;
        }
        int oldID;
        int newID;

        while(fread(&item, sizeof(struct Food), 1, fp) == 1) {
            if(item.id == id) {
                found = 1;

                gotoxy(25, 12 + ycod);
                printf("                                    ");
                gotoxy(25, 12 + ycod);
                printf("Previous ID    : %d", item.id);

                oldID = item.id;

                while(1) {
                    gotoxy(65, 12 + ycod);
                    printf("| New ID    : ");

                    if(scanf("%d", &newID) != 1) {
                        while(getchar() != '\n')
                            ;
                        continue;
                    }

                    while(getchar() != '\n')
                        ;

                    if(isDuplicateID(newID, oldID)) {
                        gotoxy(42, 14 + ycod);
                        printf("ID already exists!");

                        getch();

                        gotoxy(32, 14 + ycod);
                        printf("                            ");
                        gotoxy(76, 12 + ycod);
                        printf("        ");
                        continue;
                    }

                    item.id = newID;
                    break;
                }

                char dateStr[20];
                strftime(dateStr, sizeof(dateStr), "%d-%m-%Y", date);
                strcpy(item.date, dateStr);

                gotoxy(25, 14 + ycod);
                printf("Previous Name  : %s", item.name);

                gotoxy(65, 14 + ycod);
                printf("| New Name  : ");
                scanf(" %49[^\n]", item.name);

                gotoxy(25, 16 + ycod);
                printf("Previous Price : %.2f", item.price);

                while(1) {
                    gotoxy(65, 16 + ycod);
                    printf("| New Price : ");

                    if(scanf("%f", &item.price) == 1) {
                        ch = getchar();
                        if(ch == '\n') {
                            break;
                        }
                    }

                    gotoxy(79, 16 + ycod);
                    printf("                    ");

                    while((ch = getchar()) != '\n' && ch != EOF)
                        ;
                }
            }

            fwrite(&item, sizeof(struct Food), 1, temp);
        }

        fclose(fp);
        fclose(temp);

        if(found) {
            remove("data/menu.dat");
            rename("data/temp.dat", "data/menu.dat");
        } else {
            remove("data/temp.dat");
        }

        if(found == 0) {
            gotoxy(50, 12 + ycod);
            printf("Food ID not found!\n");
            gotoxy(45, 14 + ycod);
            printf("👉 Press ANY KEY to continue...");
            getch();
            updatefooditem();
            return;
        }

    } else if(confirm == 'n' || confirm == 'N') {
        viewAllFood();
        return;
    } else {
        gotoxy(32, 16 + ycod);
        printf("⚠️  Invalid Selection");
        gotoxy(32, 17 + ycod);
        printf("👉 Press ANY KEY to try again...");
        getch();
        updatefooditem();
        return;
    }

    gotoxy(45, 19 + ycod);
    printf("Food Update Successful");
    gotoxy(45, 20 + ycod);
    printf("👉 Press ANY KEY to continue...");
    getch();
    viewAllFood();
    return;
}

void saveMenu(void) {
    struct tm *date;
    time_t t;
    t = time(NULL);
    date = localtime(&t);

    FILE *fp = fopen("data/menu.dat", "ab");
    ;

    if(fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    char dateStr[20];
    strftime(dateStr, sizeof(dateStr), "%d-%m-%Y", date);
    strcpy(menu[totalItems - 1].date, dateStr);
    fwrite(&menu[totalItems - 1], sizeof(struct Food), 1, fp);

    fclose(fp);

    gotoxy(32, 15);
    printf("[✔] Menu saved successfully!");

    gotoxy(30, 16);
    printf("------------------------------------------------------------");

    gotoxy(30, 18);
    printf("👉 Press ANY KEY to continue...");
    getch();
    addfooditem();
}

int isDuplicateID(int newID, int currentID) {
    FILE *fp = fopen("data/menu.dat", "rb");
    if(fp == NULL)
        return 0;

    struct Food item;
    while(fread(&item, sizeof(struct Food), 1, fp) == 1) {
        if(item.id == newID) {
            if(currentID == -1 || item.id != currentID) {
                fclose(fp);
                return 1;
            }
        }
    }
    fclose(fp);
    return 0;
}