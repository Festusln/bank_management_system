#ifndef DATA
#define DATA
#include<stdio.h>



typedef enum AccountType
{
    INAVALID_ACCOUNT = -1,
    SAVINGS_ACCOUNT = 1,
    CURRENT_ACCOUNT
}AccountType;

typedef struct AccountDetails
{
    int id;
    char firstName[20];
    char lastName[20];
    char middleName[20];
    AccountType customerAccountType;
    double depositAmount;
    double withdrawalAmount;
    double currentBalance;
    char accntDetails[500];
}AccountDetails;


short initDB(void);
short createDBSchema (void);
short insertIntoDB(AccountDetails *trans);
short readFromDB (AccountDetails *trans);


#endif