#define DATABASE "database.db"
#define TRUE 1;
#define FALSE 0;


typedef struct {
    int id;
    char first_name[50];
    char last_name[50];
    int age;
} person_t;


typedef struct {
    sqlite3 *db;
    sqlite3_stmt *res;
    char *zErrMsg;   
    int rowCount;
} database_t;


char *strcasestr(const char *haystack, const char *needle);

static database_t *sqlite3_connect(const char *filename);
static int database_execute(database_t *database, const char *sql);
static void database_create(database_t *database);
static void database_populate(database_t *database);
static person_t *database_fetchone_by_id(database_t *database, int id);
static person_t *database_fetchall(database_t *database);
static int database_update(database_t *database, person_t *person);
static int database_delete_by_id(database_t *database, int id);
static void database_close(database_t *database);
static void database_set_row_count(database_t *database);


static database_t *sqlite3_connect(const char *filename)
{
    database_t *database; 
    database = calloc(1, sizeof(database_t));

    if (database == NULL) {
        return NULL;
    }

    database->db = NULL;
    database->res = NULL;
    database->zErrMsg = 0;
    database->rowCount = 0;

    if (sqlite3_open(filename, &(database)->db) != SQLITE_OK) {
        fprintf(stderr, "Can't open database %s\n", sqlite3_errmsg(database->db));
        return NULL;
    }

    return database;
}


static void database_create(database_t *database)
{
    char *sql;

    sql = "CREATE TABLE IF NOT EXISTS person " \
          "(id INTEGER NOT NULL PRIMARY KEY ASC," \
          "first_name TEXT(50) NOT NULL," \
          "last_name TEXT(50) NOT NULL," \
          "age INTEGER NOT NULL);";           
    database_execute(database, sql);        
}


static void database_populate(database_t *database)
{
    char *sql;

    sql = "INSERT OR IGNORE INTO person VALUES (" \
          "1, 'john', 'armless', 30);";  

    database_execute(database, sql);
    sql = "INSERT OR IGNORE INTO person VALUES (" \
          "2, 'jorge', 'vargas', 40);";  
    database_execute(database, sql);

}


static int database_execute(database_t *database, const char *sql)
{

    if (strcasestr(sql, "SELECT") &&
        !strcasestr(sql, "CREATE") &&
        !strcasestr(sql, "INSERT") &&
        !strcasestr(sql, "UPDATE") && 
        !strcasestr(sql, "DELETE")) {    

        /* the old prepared statement object will be destroyed */
        sqlite3_finalize(database->res);
        /* the new prepared statement object will be created */
        return sqlite3_prepare_v2(database->db, sql, -1, &(database)->res, 0);
    }

    return sqlite3_exec(database->db, sql, NULL, 0, &(database)->zErrMsg);
}


static person_t *database_fetchone_by_id(database_t *database, int id)
{
    char *sql = malloc(64);

    sprintf(sql, "SELECT * FROM person WHERE id=%d;", id);
    database_execute(database, sql);      

    person_t *person = NULL;

    while (sqlite3_step(database->res) != SQLITE_DONE) {  

        person = calloc(1, sizeof(person_t));

        person->id = sqlite3_column_int(database->res, 0);
        strcpy(person->first_name, (const char *restrict) sqlite3_column_text(database->res, 1));
        strcpy(person->last_name, (const char *restrict) sqlite3_column_text(database->res, 2));
        person->age = sqlite3_column_int(database->res, 3);
        free(sql);
        return person;
    }
    free(sql);
    return person;
}


static person_t *database_fetchall(database_t *database)
{
    char *sql;

    sql = "SELECT * FROM person;";
    database_execute(database, sql);   

    database_set_row_count(database);

    person_t *person = malloc(database->rowCount * sizeof(person_t));

    while (sqlite3_step(database->res) != SQLITE_DONE) 
    {  
        person->id = sqlite3_column_int(database->res, 0);
        strcpy(person->first_name, (const char *restrict) sqlite3_column_text(database->res, 1));
        strcpy(person->last_name, (const char *restrict) sqlite3_column_text(database->res, 2));
        person->age = sqlite3_column_int(database->res, 3);

	person++;
    }

  

    return person - (database->rowCount);
}

static void database_set_row_count(database_t *database)
{
    
    sqlite3_reset(database->res); 
    database->rowCount = 0;
    while (sqlite3_step(database->res) != SQLITE_DONE) 
    {
      database->rowCount++;
    }

    sqlite3_reset(database->res); 
}

static int database_update(database_t *database, person_t *person)
{
    char *sql = malloc(256);

    sprintf(sql, "UPDATE person SET " \
                 "first_name='%s', " \
                 "last_name='%s', " \
                 "age=%d WHERE id=%d;", person->first_name, 
                                        person->last_name,  
                                        person->age,
                                        person->id);
    
    // printf("sql: %s\n", sql);
    
    if (database_execute(database, sql) != 0) {
        fprintf(stderr, "database_update: %s\n", database->zErrMsg);
        free(sql);
        return FALSE;
    }

    free(sql);
    return TRUE;
}


static int database_delete_by_id(database_t *database, int id)
{
    char *sql = malloc(64);

    sprintf(sql, "DELETE FROM person WHERE id=%d;", id);

    if (database_execute(database, sql) != 0) {
        fprintf(stderr, "database_delete_by_id %s\n", database->zErrMsg);
        free(sql);
        return FALSE;
    }

    free(sql);
    return TRUE;
}


static void database_close(database_t *database)
{
    sqlite3_finalize(database->res);
    sqlite3_close(database->db);    
    free(database);
}

