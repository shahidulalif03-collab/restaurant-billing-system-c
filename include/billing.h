#ifndef BILLING_H
#define BILLING_H

void showThankYouScreen(void);
void userReview(void);
void viewUserReview(void);
void confirmOrder(float subTotal);
void saveOrder(float grandTotal);
void displayBill(char item[][50], int qty[], float price[], int count,
                 float subTotal, float vat, float discount, float grandTotal);
float generateBill(float subTotal, float *vat, float *discount);

#endif