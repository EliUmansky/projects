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

/* ------------------------------------------------- function declarations ----*/

obj_t *CreateObject(class_t *class);
void AnimalCtor(animal_t *this);
void AnimalCtorInt(animal_t *this);
void DogCtor(dog_t *this, void *param);
void CatCtor(cat_t *this);
void CatCtorColour(cat_t *this, char *colour);
void LegendCtor(legend_t *this);
char *objToString (obj_t *this);
int objHashCode (obj_t *this);
char *objGetClass (obj_t *this);
void objFinalize (obj_t *this);
int objEquals (void *object);
obj_t *objClone (obj_t *this);
void ShowCounter();
void LegendSayHello(legend_t *this);
void DogSayHello(dog_t *this);
void AnimalSayHello(animal_t *this);
void AnimalFinalize(animal_t *this);
void DogFinalize(dog_t *this);
void CatFinalize(cat_t *this);
void LegendFinalize(void *this);
char *AnimalToString(animal_t *this);
char *DogToString(dog_t *this);
char *CatToString(cat_t *this);
char *LegendToString(legend_t *this);
int GetNumMasters (animal_t *this);
void foo(animal_t *animal);

/* ----------------------------------------------- static block functions  ----*/

static void AnimalStaticBlock1();
static void AnimalStaticBlock2();
static void DogStaticBlock();
static void CatStaticBlock();
static void LegendStaticBlock();

/* ---------------------------------------------------------------- vfuncs ----*/

vfunc_t ObjectVTable[] = {(vfunc_t)&objClone, (vfunc_t)&objEquals, &objFinalize,
                          (vfunc_t)&objGetClass, (vfunc_t)&objHashCode,
                          (vfunc_t)&objToString
                         };
vfunc_t animalVTable[] = {(vfunc_t)&objClone, (vfunc_t)&objEquals, &AnimalFinalize,
                          (vfunc_t)&objGetClass, (vfunc_t)&objHashCode,
                          (vfunc_t)&AnimalToString, &AnimalSayHello, (vfunc_t)&GetNumMasters
                         };
vfunc_t dogVTable[] = {(vfunc_t)&objClone, (vfunc_t)&objEquals, &AnimalFinalize,
                        (vfunc_t)&objGetClass, (vfunc_t)&objHashCode,
                        (vfunc_t)&DogToString, &DogSayHello, (vfunc_t)&GetNumMasters
                      };
vfunc_t catVTable[] = {(vfunc_t)&objClone, (vfunc_t)&objEquals, &CatFinalize,
                       (vfunc_t)&objGetClass, (vfunc_t)&objHashCode, (vfunc_t)&CatToString,
                       &AnimalSayHello, (vfunc_t)&GetNumMasters
                      };
vfunc_t legendVTable[] = {(vfunc_t)&objClone, (vfunc_t)&objEquals, &LegendFinalize,
                          (vfunc_t)&objGetClass, (vfunc_t)&objHashCode, (vfunc_t)&LegendToString,
                          &LegendSayHello, (vfunc_t)&GetNumMasters
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

obj_t *object_to_compare;
int counter;

obj_t *CreateObject(class_t *class)
{
    obj_t *object = (obj_t*)calloc(1, class->size);
    object->metadata = class;

    return object;
}

void AnimalCtor(animal_t *this)
{
    AnimalStaticBlock1();
    AnimalStaticBlock2();

    printf("Instance initialization block Animal\n");

    this->num_legs = 5;
    this->num_masters = 1;
    this->ID = ++counter;

    printf("Animal Ctor\n");
    AnimalSayHello(this);
    ShowCounter();
    printf("%s", AnimalToString(this));
    printf("%s", objToString(this));
}

void AnimalCtorInt(animal_t *this, int num_masters)
{
    AnimalStaticBlock1();
    AnimalStaticBlock2();

    printf("Instance initialization block Animal\n");

    this->num_legs = 5;
    this->num_masters = 1;
    this->ID = ++counter;

    printf("Animal Ctor int\n");
    this->num_masters = num_masters;
}

void DogCtor(dog_t *this)
{
    int super_param = 2;

    AnimalStaticBlock1();
    AnimalStaticBlock2();

    DogStaticBlock();

    AnimalCtorInt((animal_t*)this, &super_param);

    printf("Instance initialization block Dog\n");
    printf("Dog Ctor\n");

    this->num_legs = 4;
}

void CatCtor(cat_t *this, void *param)
{
    char *color = "black";

    AnimalStaticBlock1();
    AnimalStaticBlock2()
    CatStaticBlock();

    CatCtorColour(this, color);
    printf("Cat Ctor\n");
    this->num_masters = 2;

}

void CatCtorColour(cat_t *this, char *colour)
{
    AnimalStaticBlock1();
    AnimalStaticBlock2()
    CatStaticBlock();

    this->num_masters = 5;

    strcpy(this->colors, colour);
    printf("Cat Ctor with color: %s\n", this->colors);
}

void LegendCtor(legend_t *this)
{
    AnimalStaticBlock1();
    AnimalStaticBlock2()
    CatStaticBlock();
    LegendStaticBlock();

    CatCtor((cat_t*)this);

    printf("Legendary Ctor\n");
}

static void AnimalStaticBlock1()
{
    static int block_counter = 1;

    if (block_counter)
    {
        --block_counter;
        printf("Static block Animal 1\n");
    }
}

static void AnimalStaticBlock2()
{
    static int block_counter = 1;

    if (block_counter)
    {
        --block_counter;
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

void AnimalSayHello(animal_t *this)
{
    printf("Animal Hello\n");
    printf("I have %d legs\n", this->num_legs);
}

void DogSayHello(dog_t *this)
{
    printf("Dog Hello\n");
    printf("I have %d legs\n", this->num_legs);
}

void LegendSayHello(legend_t *this)
{
    (void)this;
    printf("Legendary Hello\n");
}

void AnimalShowCounter()
{
    printf("%d\n", counter);
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

char *objGetClass (obj_t *this)
{
    char *cls = "java.lang.Object";
    (void)this;

    return cls;
}

int objHashCode (obj_t *this)
{
    return (int)this;
}

char *objToString (obj_t *this)
{
    char str[50];
    sprintf(str, "Object@%lu", (size_t)this);

    return str;
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
    char str[30];
    sprintf(str, "Animal with ID: %d\n", this->ID);

    return str;
}
char *DogToString(void *this)
{
    char str[30];
    sprintf(str, "Dog with ID: %d\n", ((animal_t*)this)->ID);

    return str;
}
char *CatToString(void *this)
{
    char str[30];
    sprintf(str, "Cat with ID: %d\n", ((animal_t*)this)->ID);

    return str;
}
char *LegendToString(void *this)
{
    char str[30];
    sprintf(str, "Legendary Animal with ID: %d\n", ((animal_t*)this)->ID);

    return str;
}
void foo(animal_t *animal)
{
	printf("%s", ((str_func_t)((*((obj_t*)animal)->metadata->vfuncTable)[TOSTRING]))(animal));
}

int main()
{
  /*  char *white = "white";
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
*/
    return 0;
}
