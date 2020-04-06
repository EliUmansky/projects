#include <stdio.h> /* printf */
#include <stdlib.h> /* calloc */
#include <string.h> /* strcmp */

#define FUNC_DATA ((func_data_t*)func_data)
#define STR_SIZE (40)
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

typedef struct Class
{
    char *name;
    size_t size;
    struct Class *super;
    vfunc_t (*vfuncTable)[];
} class_t;

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

/* ------------------------------------------------- function declarations ----*/

obj_t *CreateObject(class_t *class);
void AnimalCtor(animal_t *this);
void AnimalCtorNum(animal_t *this, int num_masters);
void DogCtor(dog_t *this);
void CatCtor(cat_t *this);
void CatCtorColour(cat_t *this, char *colour);
void LegendCtor(legend_t *this);
char *objToString (obj_t *this);
int objHashCode (obj_t *this);
char *objGetClass (class_t *this);
void objFinalize (obj_t *this);
int objEquals (obj_t *object);
obj_t *objClone (obj_t *this);
void AnimalShowCounter();
void LegendSayHello(void *this);
void DogSayHello(void *this);
void AnimalSayHello(void *this);
void AnimalFinalize(animal_t *this);
void DogFinalize(dog_t *this);
void CatFinalize(cat_t *this);
void LegendFinalize(legend_t *this);
char *AnimalToString(animal_t *this);
char *DogToString(dog_t *this);
char *CatToString(cat_t *this);
char *LegendToString(legend_t *this);
int GetNumMasters (animal_t *this);
void foo(animal_t *animal);

/* ----------------------------------------------- static block functions  ----*/

static void AnimalStaticBlock();
static void DogStaticBlock();
static void CatStaticBlock();
static void LegendStaticBlock();

/* ---------------------------------------------------------------- vfuncs ----*/

vfunc_t ObjectVTable[] = {(vfunc_t)&objClone, (vfunc_t)&objEquals, (vfunc_t)&objFinalize,
                          (vfunc_t)&objGetClass, (vfunc_t)&objHashCode,
                          (vfunc_t)&objToString
                         };
vfunc_t animalVTable[] = {(vfunc_t)&objClone, (vfunc_t)&objEquals, (vfunc_t)&AnimalFinalize,
                          (vfunc_t)&objGetClass, (vfunc_t)&objHashCode,
                          (vfunc_t)&AnimalToString, (vfunc_t)&AnimalSayHello, (vfunc_t)&GetNumMasters
                         };
vfunc_t dogVTable[] = {(vfunc_t)&objClone, (vfunc_t)&objEquals, (vfunc_t)&AnimalFinalize,
                        (vfunc_t)&objGetClass, (vfunc_t)&objHashCode,
                        (vfunc_t)&DogToString, (vfunc_t)&DogSayHello, (vfunc_t)&GetNumMasters
                      };
vfunc_t catVTable[] = {(vfunc_t)&objClone, (vfunc_t)&objEquals, (vfunc_t)&CatFinalize,
                       (vfunc_t)&objGetClass, (vfunc_t)&objHashCode, (vfunc_t)&CatToString,
                       (vfunc_t)&AnimalSayHello, (vfunc_t)&GetNumMasters
                      };
vfunc_t legendVTable[] = {(vfunc_t)&objClone, (vfunc_t)&objEquals, (vfunc_t)&LegendFinalize,
                          (vfunc_t)&objGetClass, (vfunc_t)&objHashCode, (vfunc_t)&LegendToString,
                          (vfunc_t)&LegendSayHello, (vfunc_t)&GetNumMasters
                         };

/* ------------------------------------------------------------- meta data ----*/

class_t object_meta = {"Object", sizeof(obj_t), NULL, &ObjectVTable};
class_t animal_meta = {"Animal", sizeof(animal_t), &object_meta, &animalVTable};
class_t dog_meta = {"Dog", sizeof(dog_t), &animal_meta, &dogVTable};
class_t cat_meta = {"Cat", sizeof(cat_t), &animal_meta, &catVTable};
class_t legend_meta = {"LegendaryAnimal", sizeof(legend_t), &cat_meta, &legendVTable};

/* -------------------------------------------------------- functions data ----*/

typedef char* (*str_func_t) (animal_t*);

obj_t *object_to_compare;
char to_string[STR_SIZE];
char get_class[STR_SIZE];
int animal_counter;

obj_t *CreateObject(class_t *class)
{
    obj_t *object = (obj_t*)calloc(1, class->size);
    object->metadata = class;

    return object;
}

void AnimalCtor(animal_t *this)
{
    AnimalStaticBlock();

    printf("Instance initialization block Animal\n");

    this->num_legs = 5;
    this->num_masters = 1;
    this->ID = ++animal_counter;

    printf("Animal Ctor\n");
    (*((obj_t*)this)->metadata->vfuncTable)[SAYHELLO](this);
    AnimalShowCounter();
    printf("%s\n", AnimalToString(this));
    printf("%s\n", objToString((obj_t*)this));
}

void AnimalCtorNum(animal_t *this, int num_masters)
{
    AnimalStaticBlock();

    printf("Instance initialization block Animal\n");

    this->num_legs = 5;
    this->num_masters = 1;
    this->ID = ++animal_counter;

    printf("Animal Ctor int\n");
    this->num_masters = num_masters;
}

void DogCtor(dog_t *this)
{
    AnimalStaticBlock();
    DogStaticBlock();

    AnimalCtorNum((animal_t*)this, 2);

    printf("Instance initialization block Dog\n");
    printf("Dog Ctor\n");

    this->num_legs = 4;
}

void CatCtor(cat_t *this)
{
    CatCtorColour(this, "black");
    printf("Cat Ctor\n");
    this->num_masters = 2;
}

void CatCtorColour(cat_t *this, char *colour)
{
    AnimalStaticBlock();
    CatStaticBlock();

    AnimalCtor((animal_t*)this);

    this->num_masters = 5;
    strcpy(this->colors, colour);
    printf("Cat Ctor with color: %s\n", this->colors);
}

void LegendCtor(legend_t *this)
{
    AnimalStaticBlock();
    CatStaticBlock();
    LegendStaticBlock();

    CatCtor((cat_t*)this);

    printf("Legendary Ctor\n");
}

static void AnimalStaticBlock()
{
    static int block_counter = 1;

    if (block_counter)
    {
        --block_counter;
        printf("Static block Animal 1\n");
        printf("Static block Animal 2\n");

    }
}

static void DogStaticBlock()
{
    static int block_counter = 1;

    if (block_counter)
    {
        --block_counter;
        printf("Static block Dog\n");
    }
}

static void CatStaticBlock()
{
    static int block_counter = 1;

    if (block_counter)
    {
        --block_counter;
        printf("Static block Cat\n");
    }
}

static void LegendStaticBlock()
{
    static int block_counter = 1;

    if (block_counter)
    {
        --block_counter;
        printf("Static block Legendary Animal\n");
    }
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

void AnimalShowCounter()
{
    printf("%d\n", animal_counter);
}

obj_t *objClone (obj_t *this)
{
    return CreateObject(this->metadata);
}

int objEquals (obj_t *object)
{
    if (NULL == object_to_compare)
    {
        return 0;
    }

    return (!strcmp(object_to_compare->metadata->name, object->metadata->name));
}

char *objGetClass (class_t *this)
{
    memset(get_class, '\0', STR_SIZE);
    strcpy(get_class, "class ");
    strcat(get_class, this->name);

    return get_class;
}

int objHashCode (obj_t *this)
{
    return (int)this;
}

char *objToString (obj_t *this)
{
    memset(to_string, '\0', STR_SIZE);
    sprintf(to_string, "%s@%x", this->metadata->name, (size_t)this);

    return to_string;
}

int GetNumMasters (animal_t *this)
{
    return this->num_masters;
}

void objFinalize (obj_t *this)
{
    printf("finalize Object\n");
    free(this);
}

void AnimalFinalize(animal_t *this)
{
    printf("finalize Animal with ID: %d\n", this->ID);
    objFinalize((obj_t*)this);
}

void DogFinalize(dog_t *this)
{
    printf("finalize Dog with ID: %d\n", ((animal_t*)this)->ID);
    AnimalFinalize((animal_t*)this);
}

void CatFinalize(cat_t *this)
{
    printf("finalize Cat with ID: %d\n", ((animal_t*)this)->ID);
    AnimalFinalize((animal_t*)this);
}

void LegendFinalize(legend_t *this)
{
    printf("finalize Legendary Animal with ID: %d\n", ((animal_t*)this)->ID);
    CatFinalize((cat_t*)this);
}

char *AnimalToString(animal_t *this)
{
    memset(to_string, '\0', STR_SIZE);
    sprintf(to_string, "%s with ID: %d", ((obj_t*)this)->metadata->name, this->ID);

    return to_string;
}

char *DogToString(dog_t *this)
{
    memset(to_string, '\0', STR_SIZE);
    sprintf(to_string, "Dog with ID: %d", ((animal_t*)this)->ID);

    return to_string;
}

char *CatToString(cat_t *this)
{
    memset(to_string, '\0', STR_SIZE);
    sprintf(to_string, "Cat with ID: %d", ((animal_t*)this)->ID);

    return to_string;
}

char *LegendToString(legend_t *this)
{
    memset(to_string, '\0', STR_SIZE);
    sprintf(to_string, "Legendary Animal with ID: %d", ((animal_t*)this)->ID);

    return to_string;
}

void foo(animal_t *animal)
{
	printf("%s\n", ((str_func_t)((*((obj_t*)animal)->metadata->vfuncTable)[TOSTRING]))(animal));
}

int main()
{
    char *white = "white";
    int i = 0;
    obj_t *obj, *animal, *dog, *cat, *la;
    obj_t* array[5] = {0};

    obj = CreateObject(&object_meta);
    animal = CreateObject(&animal_meta);
    AnimalCtor((animal_t*)animal);
    dog = CreateObject(&dog_meta);
    DogCtor((dog_t*)dog);
    cat = CreateObject(&cat_meta);
    CatCtor((cat_t*)cat);
    la = CreateObject(&legend_meta);
    LegendCtor((legend_t*)la);

    AnimalShowCounter();

    printf("%d\n", ((animal_t*)animal)->ID);
	printf("%d\n", ((animal_t*)dog)->ID);
	printf("%d\n", ((animal_t*)cat)->ID);
	printf("%d\n", ((animal_t*)la)->ID);

	array[0] = CreateObject(&dog_meta);
	DogCtor((dog_t*)array[0]);
	array[1] = CreateObject(&cat_meta);
	CatCtor((cat_t*)array[1]);
	array[2] = CreateObject(&cat_meta);
	CatCtorColour((cat_t*)array[2], white);
	array[3] = CreateObject(&legend_meta);
	LegendCtor((legend_t*)array[3]);
	array[4] = CreateObject(&animal_meta);
	AnimalCtor((animal_t*)array[4]);

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
