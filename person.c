#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "include/person.h"


int main(int argc, char **argv)
{    
    database_t *database;

    person_t *person = NULL;
    person_t **personList = NULL;
  
    database = sqlite3_connect(DATABASE);

    database_create(database);
    database_populate(database);

    person = database_fetchone_by_id(database, 1);

   if (person != NULL) {
        printf("#################### fetch by id\n");
        printf("person id: %d\n", person->id);
        printf("person first_name: %s\n", person->first_name);
        printf("person last_name: %s\n", person->last_name);
        printf("person age: %d\n", person->age);
    } else {
        printf("Register not found\n");
    }

  database_fetchall(database);
  
 /*for (int i=0; i < database->rowCount; i++) {
        printf("#################### fetch all\n");
        printf("person id: %d\n", personList[i]->id);
        printf("person first_name: %s\n", personList[i]->first_name);
        printf("person last_name: %s\n", personList[i]->last_name);
        printf("person age: %d\n", personList[i]->age);
  }

/*    printf("#################### fetch by id\n");
    printf("person id: %d\n", person->id);
    printf("person first_name: %s\n", person->first_name);
    printf("person last_name: %s\n", person->last_name);
    printf("person age: %d\n", person->age);*/
    
    //printf("last_name1 %s", person->last_name);
    //person->last_name = "gen";
    //printf("last_name2 %s", person->last_name);
/*    if (database_update(database, person)){
        printf("update was successfully\n");
    } else {
        printf("something wrong in update function\n");
    }*/

/*    person = database_fetchone_by_id(database, 1, person);

    if (person != NULL) {
        printf("#################### fetch by id\n");
        printf("person id: %d\n", person->id);
        printf("person first_name: %s\n", person->first_name);
        printf("person last_name: %s\n", person->last_name);
        printf("person age: %d\n", person->age);
    } else {
        printf("Register not found\n");
    }    

    if (database_delete_by_id(database, person->id)) {
        printf("register was deleted with successfully\n");
    } else {
        printf("something wrong in delete function\n");
    }

    if (database_fetchone_by_id(database, 1, person) != NULL) {
    printf("Register found\n");
    } else {
        printf("Register not found\n");
    } */

    free(person);
    database_close(database);
        
    return 0;
}
