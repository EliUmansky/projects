#include <stdio.h> /* printf */
#include <stdlib.h> /* calloc */
#include <string.h> /* strcmp */

#define FUNC_DATA ((func_data_t*)func_data)

/* ------------------------------------------------------------- Enums -------*/

enum func
{
    CLONE, EQUALS, FINALIZE, GETCLASS, HASHCODE, TOSTRING, SAYHELLO, GETNUMMASTERS
};

enum class_name
{
    OBJECT, ANIMAL, DOG, CAT, LEGEND
};

/* ------------------------------------------------------------ typedefs ------*/

typedef void (*vfunc_t)(void*);
typedef struct Class class_t;

struct Class
{
    char *name;
    size_t size;
    class_t *super;
    vfunc_t (*vfuncTable)[];
};

typedef struct Obj
{
    class_t *metadata;
} obj_t;

typedef struct Animal
{
    obj_t object;
    int num_legs;
    int num_masters;
    int ID;
} animal_t;

typedef struct Dog
{
    animal_t animal;
    int num_legs;
} dog_t;

typedef struct Cat
{
    animal_t animal;
    int num_masters;
    char colors[10];
} cat_t;

typedef struct LegendaryAnimal
{
    cat_t cat;
} legend_t;

typedef struct FunctionsData
{
    obj_t *object;
    obj_t *object_to_compare;
    int isEqual;
    char get_class[20];
    int hash_code;
    char to_string[30];
    int num_masters;
    int num_legs;
} func_data_t;

/* ------------------------------------------------- function declarations ----*/

obj_t *CreateObject(class_t *class, void *param);
void objToString (void *param);
void objHashCode (void *param);
void objGetClass (void *func_data);
void objFinalize (void *func_data);
void objEquals (void *object);
void objClone (void *param);
void ShowCounter(void *param);
void LegendSayHello(void *this);
void DogSayHello(void *this);
void AnimalSayHello(void *this);
void AnimalFinalize(void *this);
void DogFinalize(void *this);
void CatFinalize(void *this);
void LegendFinalize(void *this);
void AnimalToString(void *this);
void DogToString(void *this);
void CatToString(void *this);
void LegendToString(void *this);
void GetNumMasters (void *this);
void foo(animal_t *animal);

/* ------------------------------------------ static function declarations ----*/

static void Init(obj_t *object, char *class_name, void *param);
static void AnimalCtor(animal_t *this, void *param);
static void DogCtor(dog_t *this, void *param);
static void CatCtor(cat_t *this, void *param);
static void LegendCtor(legend_t *this, void *param);
static void StaticAnimal1();
static void StaticAnimal2();
static void StaticDog();
static void StaticCat();
static void StaticLegend();

/* ---------------------------------------------------------------- vfuncs ----*/

vfunc_t ObjectVTable[] = {&objClone, &objEquals, &objFinalize,
                          &objGetClass, &objHashCode, &objToString
                         };
vfunc_t animalVTable[] = {&objClone, &objEquals, &AnimalFinalize,
                          &objGetClass, &objHashCode, &AnimalToString,
                          &AnimalSayHello, &GetNumMasters
                         };
vfunc_t dogVTable[] = {&objClone, &objEquals, &AnimalFinalize, &objGetClass,
                       &objHashCode, &DogToString, &DogSayHello, &GetNumMasters
                      };
vfunc_t catVTable[] = {&objClone, &objEquals, &CatFinalize, &objGetClass,
                       &objHashCode, &CatToString, &AnimalSayHello, &GetNumMasters
                      };
vfunc_t legendVTable[] = {&objClone, &objEquals, &LegendFinalize, &objGetClass,
                          &objHashCode, &LegendToString, &LegendSayHello,
                          &GetNumMasters
                         };

/* ------------------------------------------------------------- meta data ----*/

class_t object_meta = {"Object", sizeof(obj_t), NULL, &ObjectVTable};
class_t animal_meta = {"Animal", sizeof(animal_t), &object_meta, &animalVTable};
class_t dog_meta = {"Dog", sizeof(dog_t), &animal_meta, &dogVTable};
class_t cat_meta = {"Cat", sizeof(cat_t), &animal_meta, &catVTable};
class_t legend_meta = {"LegendaryAnimal", sizeof(legend_t), &cat_meta, &legendVTable};

/* -------------------------------------------------------- functions data ----*/

func_data_t func_data[] = { {(obj_t*)&object_meta, NULL, 0, " ", 0, " ", 0, 0},
    {(obj_t*)&animal_meta, NULL, 0, " ", 0, " ", 0, 0},
    {(obj_t*)&dog_meta, NULL, 0, " ", 0, " ", 0, 0},
    {(obj_t*)&cat_meta, NULL, 0, " ", 0, " ", 0, 0},
    {(obj_t*)&legend_meta, NULL, 0, " ", 0, " ", 0, 0}
};

/* ------------------------------------------------------ global variables ----*/

int counter = 0;
int animal_counter = 1;
int dog_counter = 1;
int cat_counter = 1;
int legend_counter = 1;
int animal_ctor_count = 1;

obj_t *CreateObject(class_t *class, void *param)
{
    obj_t *object = (obj_t*)calloc(1, class->size);
    object->metadata = class;

    Init(object, class->name, param);

    return object;
}
static void Init(obj_t *object, char *class_name, void *param)
{
    if (!strcmp(class_name, "Object"))
    {
        return;
    }
    else if (!strcmp(class_name, "Animal"))
    {
        AnimalCtor((animal_t*)object, param);
    }
    else if (!strcmp(class_name, "Dog"))
    {
        DogCtor((dog_t*)object, param);
    }
    else if (!strcmp(class_name, "Cat"))
    {
        CatCtor((cat_t*)object, param);
    }
    else if (!strcmp(class_name, "LegendaryAnimal"))
    {
        LegendCtor((legend_t*)object, param);
    }
    else
    {
        printf("wrong class name");
    }
}

static void AnimalCtor(animal_t *this, void *param)
{
    if (0 < animal_counter)
    {
        --animal_counter;
        StaticAnimal1();
        StaticAnimal2();
    }

    printf("Instance initialization block Animal\n");
    this->num_legs = 5;
    this->num_masters = 1;
    this->ID = ++counter;

    if (NULL == param)
    {
        printf("Animal Ctor\n");
        (*((obj_t*)this)->metadata->vfuncTable)[SAYHELLO](this);
        ShowCounter(NULL);
        (*((obj_t*)this)->metadata->vfuncTable)[TOSTRING](this);
        printf("%s", func_data[ANIMAL].to_string);
        objToString(this);
        printf("%s", func_data[OBJECT].to_string);
    }
    else
    {
        printf("Animal Ctor int\n");
        this->num_masters = *(int*)param;
    }
}

static void DogCtor(dog_t *this, void *param)
{
    int super_param = 2;

    (void)param;

    if (0 < animal_counter)
    {
        --animal_counter;
        StaticAnimal1();
        StaticAnimal2();
    }
    if (0 < dog_counter)
    {
        --dog_counter;
        StaticDog();
    }

    AnimalCtor((animal_t*)this, &super_param);

    printf("Instance initialization block Dog\n");
    printf("Dog Ctor\n");

    this->num_legs = 4;
}

static void CatCtor(cat_t *this, void *param)
{
    char *color = "black";

    if (0 < animal_counter)
    {
        --animal_counter;
        StaticAnimal1();
        StaticAnimal2();
    }
    if (0 < cat_counter)
    {
        --cat_counter;
        StaticCat();
    }

    if (0 < animal_ctor_count)
    {
        --animal_ctor_count;
        AnimalCtor((animal_t*)this, NULL);
    }

    this->num_masters = 5;

    if (NULL == param)
    {
        CatCtor(this, color);
        printf("Cat Ctor\n");
        this->num_masters = 2;
    }
    else
    {
        strcpy(this->colors, (char*)param);
        printf("Cat Ctor with color: %s\n", this->colors);
    }

    animal_ctor_count = 1;
}

static void LegendCtor(legend_t *this, void *param)
{
    if (0 < animal_counter)
    {
        --animal_counter;
        StaticAnimal1();
        StaticAnimal2();
    }
    if (0 < cat_counter)
    {
        --cat_counter;
        StaticCat();
    }
    if (0 < legend_counter)
    {
        --legend_counter;
        StaticLegend();
    }

    CatCtor((cat_t*)this, NULL);

    printf("Legendary Ctor\n");
}
static void StaticAnimal1()
{
    printf("Static block Animal 1\n");
}
static void StaticAnimal2()
{
    printf("Static block Animal 2\n");
}
static void StaticDog()
{
    printf("Static block Dog\n");
}
static void StaticCat()
{
    printf("Static block Cat\n");
}
static void StaticLegend()
{
    printf("Static block Legendary Animal\n");
}
void AnimalSayHello(void *this)
{
    printf("Animal Hello\n");
    printf("I have %d legs\n", ((animal_t*)this)->num_legs);
}
void DogSayHello(void *this)
{
    printf("Dog Hello\n");
    printf("I have %d legs\n", ((dog_t*)this)->num_legs);
}
void LegendSayHello(void *this)
{
    (void)this;
    printf("Legendary Hello\n");
}
void ShowCounter(void *param)
{
    (void)param;
    printf("%d\n", counter);
}
void objClone (void *param)
{
    (void)param;
    printf("Clone\n");
}
void objEquals (void *object)
{
    FUNC_DATA[OBJECT].isEqual = (FUNC_DATA[OBJECT].object_to_compare == (obj_t*)object);
}
void objGetClass (void *metadata)
{
    printf("class %s\n", ((class_t*)metadata)->name);
}
void objHashCode (void *param)
{
    int hash = 5381;
    int c;

    (void)param;

    while ('\0' != (c = *(FUNC_DATA[OBJECT].object->metadata->name)++))
    {
        hash = ((hash << 5) + hash) + c;
    }

    FUNC_DATA[OBJECT].hash_code = hash;
}
void objToString (void *this)
{
    sprintf((func_data[OBJECT]).to_string, "%s@%lu\n", ((obj_t*)this)->metadata->name, (size_t)this);
}
void GetNumMasters (void *this)
{
    func_data[ANIMAL].num_masters = ((animal_t*)this)->num_masters;
}
void objFinalize (void *this)
{
    (void)func_data;
    printf("finalize Object\n");
    free(this);
}
void AnimalFinalize(void *this)
{
    printf("finalize Animal with ID: %d\n", ((animal_t*)this)->ID);
    ObjectVTable[FINALIZE](this);
}
void DogFinalize(void *this)
{
    printf("finalize Dog with ID: %d\n", ((animal_t*)this)->ID);
    animalVTable[FINALIZE](this);
}
void CatFinalize(void *this)
{
    printf("finalize Cat with ID: %d\n", ((animal_t*)this)->ID);
    animalVTable[FINALIZE](this);
}
void LegendFinalize(void *this)
{
    printf("finalize Legendary Animal with ID: %d\n", ((animal_t*)this)->ID);
    catVTable[FINALIZE](this);
}
void AnimalToString(void *this)
{
    sprintf((func_data[ANIMAL]).to_string, "Animal with ID: %d\n", ((animal_t*)this)->ID);
}
void DogToString(void *this)
{
    sprintf((func_data[DOG]).to_string, "Dog with ID: %d\n", ((animal_t*)this)->ID);
    sprintf((func_data[ANIMAL]).to_string, "Dog with ID: %d\n", ((animal_t*)this)->ID);
}
void CatToString(void *this)
{
    sprintf((func_data[CAT]).to_string, "Cat with ID: %d\n", ((animal_t*)this)->ID);
    sprintf((func_data[ANIMAL]).to_string, "Cat with ID: %d\n", ((animal_t*)this)->ID);

}
void LegendToString(void *this)
{
    sprintf((func_data[LEGEND]).to_string, "Legendary Animal with ID: %d\n", ((animal_t*)this)->ID);
    sprintf((func_data[ANIMAL]).to_string, "Legendary Animal with ID: %d\n", ((animal_t*)this)->ID);

}
void foo(animal_t *animal)
{

    (((*((obj_t*)animal)->metadata->vfuncTable)[TOSTRING]))(animal);
	printf("%s", (func_data[ANIMAL]).to_string);
}

int main()
{
    char *white = "white";
    int i = 0;
    obj_t *animal = CreateObject(&animal_meta, NULL);
    obj_t *dog = CreateObject(&dog_meta, NULL);
    obj_t *cat = CreateObject(&cat_meta, NULL);
    obj_t *la = CreateObject(&legend_meta, NULL);
    obj_t* array[5] = {0};

    ShowCounter(NULL);

    printf("%d\n", ((animal_t*)animal)->ID);
	printf("%d\n", ((animal_t*)dog)->ID);
	printf("%d\n", ((animal_t*)cat)->ID);
	printf("%d\n", ((animal_t*)la)->ID);

	array[0] = CreateObject(&dog_meta,NULL);
	array[1] = CreateObject(&cat_meta, NULL);
	array[2] = CreateObject(&cat_meta, white);
	array[3] = CreateObject(&legend_meta, NULL);
	array[4] = CreateObject(&animal_meta, NULL);

    for(i = 0; i < 5; ++i)
	{
	    (*(array[i])->metadata->vfuncTable)[SAYHELLO](array[i]);
		printf("%d\n",((animal_t*)array[i])->num_masters);
	}

	for(i = 0; i < 5; ++i)
    {
        foo((animal_t*)array[i]);
    }

    objGetClass(&cat_meta);

    for(i = 0; i < 5; ++i)
    {
        (*(array[i])->metadata->vfuncTable)[FINALIZE](array[i]);
    }

    (*animal->metadata->vfuncTable)[FINALIZE](animal);
	(*dog->metadata->vfuncTable)[FINALIZE](dog);
	(*cat->metadata->vfuncTable)[FINALIZE](cat);
	(*la->metadata->vfuncTable)[FINALIZE](la);

    return 0;
}
