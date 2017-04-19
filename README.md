# sqlite3_crud
sqlite3 CRUD like a boss, using C language

# all prototypes
```c
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
```

# how to compile
```
user@computer:~/person$ gcc -o person person.c -Wall -lsqlite3
```

# how to use
```c
database_t *database;

person_t *person = NULL;
person_t *personList = NULL;

database = sqlite3_connect(DATABASE);

database_create(database);
database_populate(database);

person = database_fetchone_by_id(database, 1);
personList = database_fetchall(database);

strcpy(person->last_name, "jane watson");
database_update(database, person);

database_delete_by_id(database, person->id);

free(person);
free(personList);
database_close(database);
```
