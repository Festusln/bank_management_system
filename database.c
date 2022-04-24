#include<string.h>
#include<stdio.h>

#include "data.h"
#define DB_NAME "main.db"


sqlite3 *dbptr; //database pointer



short initDB(void)  //function to open database
{
    int rc = sqlite3_open(DB_NAME, &dbptr);

    if (rc != SQLITE_OK)
    {
        printf("Unable to open DB: %s\n", sqlite3_errmsg(dbptr));
        sqlite3_close(dbptr);
        return -1;
    }

    return 0;
}

short createDBSchema (void)
{
    char *errMsg = 0;
    int rc = -1;
    char accntNumber[11] = {'\0'};
    char accntInfoJson[100] = {'\0'};

    if (initDB())
    {
        return -1;
    }

   char * sql = sqlite3_mprintf("CREATE TABLE IF NOT EXISTS Account(accntNumber INT, accntInfoJson TEXT NOT NULL);");

    // printf("%s\n", sql);

    rc = sqlite3_exec(dbptr, sql, NULL, 0, &errMsg);

    if (rc != SQLITE_OK)
    {
        printf("Unable to create database: %s\n", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(dbptr);
        return -1;
    }
    else
    // printf("Database created successfully\n");

    sqlite3_close(dbptr);
    return 0;
}


short createJSON (AccountDetails *trans)
{
     //Creating CJSON for the account details (first name, last name, middle name and current balance)
        cJSON *details = NULL;
        
        details = cJSON_CreateObject();

        if (!details || details == NULL)
        {
            return -1;
        }
        char *body;
        cJSON_AddItemToObject(details, "accountID", cJSON_CreateNumber(trans->id));
        cJSON_AddItemToObject(details, "firstName", cJSON_CreateString(trans->firstName));
        cJSON_AddItemToObject(details, "lastName", cJSON_CreateString(trans->lastName));
        cJSON_AddItemToObject(details, "middleName", cJSON_CreateString(trans->middleName));
        cJSON_AddItemToObject(details, "accountType", cJSON_CreateNumber(trans->customerAccountType));
        cJSON_AddItemToObject(details, "currentBalance", cJSON_CreateNumber(trans->currentBalance));
        body = cJSON_PrintUnformatted(details);
        if (!body)
        {
            cJSON_Delete(details);
            return -1;
        }

        strncpy(trans->accntDetails, body, sizeof(trans->accntDetails)); //puttint data entered into trans->accntDetails in json format

        // printf("%s\n", body); //Printing the formed cJSON file

        return 0;
}

short insertIntoDB(AccountDetails *trans)
{
        if (createJSON(trans))
        {
            return -1;
        }

         if (initDB())
        {
            return -1;
        }

        char *errMsg = 0;
        int rc = -1;
        sqlite3_stmt *res = 0;

        char *sql = sqlite3_mprintf("INSERT INTO Account VALUES('%d', '%q');", trans->id, trans->accntDetails);
        rc = sqlite3_exec(dbptr, sql, NULL, 0, &errMsg);


        if (rc != SQLITE_OK)
        {
            printf("error: %s\n", errMsg);
            sqlite3_free(errMsg);
            sqlite3_close(dbptr);
            return -1;
        }

           
        return 0;
}

short parseJSON (AccountDetails *trans)
{       
        
        cJSON *json = NULL;
        cJSON *accountID = NULL;
        cJSON *firstName = NULL;
        cJSON *lastName = NULL;
        cJSON *middleName = NULL;
        cJSON *accountType = NULL;
        cJSON *currentbalance = NULL;
        const char * body;
        body = strchr(trans->accntDetails, '{');
        
        if (body == NULL)
        {
            return -1;
        }
        

        // printf("the body is %s\n", body);



        json = cJSON_Parse(body);
        if(!json || cJSON_IsNull(json))
        {
             printf("Unable to parse\n");
            cJSON_Delete(json);
            return -1;
        }

        accountID = cJSON_GetObjectItemCaseSensitive(json, "accountID");
        if (!accountID || !cJSON_IsNumber(accountID))
        {
            printf("Unable to parse ID\n");
            cJSON_Delete(accountID);
            return -1;
        }
        trans->id = accountID->valueint;


        firstName = cJSON_GetObjectItemCaseSensitive(json, "firstName");
        if (!firstName || !cJSON_IsString(firstName))
        {
            printf("Unable to parse first name\n");
            cJSON_Delete(firstName);
            return -1;
        }
        strncpy(trans->firstName, firstName->valuestring, sizeof(trans->firstName)-1);


        lastName = cJSON_GetObjectItemCaseSensitive(json, "lastName");
        if (!lastName || !cJSON_IsString(lastName))
        {
            printf("Unable to parse last name");
            cJSON_Delete(lastName);
            return -1;
        }
        strncpy(trans->lastName, lastName->valuestring, sizeof(trans->lastName)-1);

        middleName = cJSON_GetObjectItemCaseSensitive(json, "middleName");
        if (!middleName || !cJSON_IsString(middleName))
        {
            printf("Unable to parse middle name");
            cJSON_Delete(middleName);
            return -1;
        }
        strncpy(trans->middleName, middleName->valuestring, sizeof(trans->middleName)-1);



        accountType = cJSON_GetObjectItemCaseSensitive(json, "accountType");
        if (!accountType || !cJSON_IsNumber(accountType))
        {
            printf("Unable to parse accountType\n");
            cJSON_Delete(accountType);
            return -1;
        }
            trans->customerAccountType = accountType->valueint;

        
        currentbalance = cJSON_GetObjectItemCaseSensitive(json, "currentBalance");
        if (!currentbalance || !cJSON_IsNumber(currentbalance))
        {
            printf("Unable to parse balance\n");
            cJSON_Delete(currentbalance);
            return -1;
        }
            trans->currentBalance = currentbalance->valuedouble;

    return 0;
}   


short selectRecordByAccountID (AccountDetails *trans)
{
    sqlite3_stmt *res = 0;
    int rc = -1;
    int id = 0;
    char buffer [1024] = {'\0'};

    memset(trans, 0x00, sizeof(AccountDetails));

    printf("Enter your account ID: ");
    scanf("%d", &id);
    if (initDB())
    {
        printf("Unable to open database\n");
        return -1;
    }
    char *sql = sqlite3_mprintf("SELECT accntNumber, accntInfojson FROM Account WHERE accntNumber = ('%d');", id);
    rc = sqlite3_prepare_v2(dbptr, sql, -1, &res, 0);
    if (rc == SQLITE_OK)
    {
        sqlite3_bind_int(res, 1, 3);
    }else{
        printf("Failed to execute statement: %s\n", sqlite3_errmsg(dbptr));
    }

    int step = sqlite3_step(res);

    if (step == SQLITE_ROW) 
    {
    
    strncpy(trans->accntDetails, sqlite3_column_text(res, 1), sizeof(trans->accntDetails));
    if (!&trans->accntDetails[1])
    {
        printf("The enered account number does not exist\n");
        return -1;
    }

    // printf("Account details in json format is:%s\n", trans->accntDetails);

    }

   if (parseJSON (trans))
   {
       return -1;
   }
   printf("\nAccount Name: %s, %s %s\n", trans->lastName, trans->firstName, trans->middleName);



    return 0;
}


short updateRecord (AccountDetails *trans)
{
    int rc = -1;
    char *errMsg = 0;

    if (createJSON(trans))
    {
        return -1;
    }

    char *sql = sqlite3_mprintf("UPDATE Account SET accntinfojson = '%q' WHERE accntNumber = '%d';", trans->accntDetails, trans->id);
    rc = sqlite3_exec(dbptr, sql, NULL, 0, &errMsg);


    if (rc != SQLITE_OK)
    {
        printf("error: %s\n", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(dbptr);
        return -1;
    }



    return 0;
}

short deleteRecord (AccountDetails *trans)
{
    char *errMsg = 0;
    memset(trans, 0x00, sizeof(AccountDetails));
    
    if (selectRecordByAccountID(trans))
    {
        return -1;
    }

    char *sql = sqlite3_mprintf("DELETE FROM Account WHERE accntNumber = '%d';", trans->id);
    int rc = sqlite3_exec (dbptr, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK)
    {
        printf("SQL statement error: %s\n", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(dbptr);
        return -1;
    }
    printf("Account deleted successfully\n");

    return 0;
}
