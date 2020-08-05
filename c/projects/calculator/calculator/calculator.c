/***************************************
* Name: Jeli Kogmansky                 *
* Date: 07/01/20                       *
* Exercise - Calculator                *
****************************************/

#include <assert.h> /* assert */
#include <stdlib.h> /* malloc */
#include <math.h> /* pow */
#include <string.h> /* strlen */

#include "calculator.h" /* calc funcs */
#include "stack.h" /* stack funcs */

#define ASCII_SIZE (256)
#define LEFT (1)
#define RIGHT (2)
#define NUM_OF_BRACKETS (3)

typedef double (*action_func_t)(double, double, int*);
typedef int (*EventHandler_t)(char **runner, stack_t *num_stack,
			 stack_t *oper_stack, int *status);
typedef int (*SymbolHandler_t)(char **runner, stack_t *num_stack,
			 stack_t *oper_stack, int *status, unsigned char symbol);

enum states {WAIT_NUM, WAIT_OPER, NUM_OF_STATES};
enum oper_symbols {ERROR, OPER, BRACKETS, NUM_OF_SYMBOLS};
enum status {SUCCESS, MALLOC_FAIL, WRONG_INPUT, DIVIDE_BY_ZERO, NEGATIVE_ROOT, 
			 NUM_OF_ERRORS};

EventHandler_t StateMachine[NUM_OF_STATES];
SymbolHandler_t HandleSymbol[NUM_OF_SYMBOLS];
action_func_t action_func_lut[ASCII_SIZE];
int precedence_lut[ASCII_SIZE];
int associativity_lut[ASCII_SIZE];
int brackets_lut[ASCII_SIZE];
int oper_symbols_lut[ASCII_SIZE];
char brackets_match[NUM_OF_BRACKETS];

/* --- Static Functions ---------------------------------------------------- */

static int InitLuts();
static void InitActionFuncLut();
static void InitPrecedenceLut();
static void InitStateMachine();
static void InitAssociativity();
static void InitBracketsLut();
static void InitHandleSymbolLut();
static void InitOperSymbolsLut();
static void InitBracketsMatch();

static double AddAction(double num1, double num2, int *status);
static double DivideAction(double num1, double num2, int *status);
static double SubstractAction(double num1, double num2, int *status);
static double MultiplyAction(double num1, double num2, int *status);
static double PowerAction(double num1, double num2, int *status);
static double ErrorAction(double num1, double num2, int *status);
static void Fold(stack_t *num_stack, stack_t *oper_stack, int *status);

static int GetNum(char **runner, stack_t *num_stack, stack_t *oper_stack, 
				  int *status);
static int GetOper(char **runner, stack_t *num_stack, stack_t *oper_stack, 
			       int *status);
static int HandleError(char **runner, stack_t *num_stack, stack_t *oper_stack, 
			           int *status, unsigned char symbol);
static int HandleOper(char **runner, stack_t *num_stack, stack_t *oper_stack, 
			          int *status, unsigned char symbol);
static int HandleBrackets(char **runner, stack_t *num_stack, stack_t *oper_stack, 
			              int *status, unsigned char symbol);

/* ---  Functions ------------------ --------------------------------------- */

double Calc(char *expression, int *status)
{
	double result = SUCCESS;
	char **runner = &expression;
	char dummy = '#';
	int next_state = WAIT_NUM;
	stack_t *num_stack = NULL, *oper_stack = NULL;
	size_t exp_len = strlen(expression);
	
	num_stack = StackCreate(exp_len, sizeof(double));
	if (NULL == num_stack)
	{
		*status = MALLOC_FAIL;
		return 0;
	}
	
	oper_stack = StackCreate(exp_len, sizeof(char));
	if (NULL == oper_stack)
	{
		*status = MALLOC_FAIL;
		StackDestroy(num_stack);
		return 0;
	}

	StackPush(oper_stack, &dummy);
	
	InitLuts();
		
	while (('\0' != **runner) && (SUCCESS == *status))
	{
		next_state = (StateMachine[next_state](runner, num_stack, oper_stack,
					  status));
	}
	
	while ((StackSize(oper_stack) > 1) && (SUCCESS == *status))
	{
		Fold(num_stack, oper_stack, status);
	}

	result = *(double *)StackPeek(num_stack);

	StackDestroy(num_stack);
	StackDestroy(oper_stack);	
		
	return result;
}

/* ---  Init Functions --------------------------------------------------- */

static int InitLuts()
{
	InitStateMachine();
	InitActionFuncLut();
	InitPrecedenceLut();
	InitAssociativity();
	InitBracketsLut();
	InitHandleSymbolLut();
	InitOperSymbolsLut();
	InitBracketsMatch();

	return 0;
}

static void InitActionFuncLut()
{
	int i = 0;
	
	for (i = 0; i < ASCII_SIZE; ++i)
	{
		action_func_lut[i] = ErrorAction;
	}

	action_func_lut['+'] = AddAction;
	action_func_lut['-'] = SubstractAction;
	action_func_lut['*'] = MultiplyAction;
	action_func_lut['/'] = DivideAction;
	action_func_lut['^'] = PowerAction;
}

static void InitPrecedenceLut()
{
	int i = 0;
	
	for (i = 0; i < ASCII_SIZE; ++i)
	{
		precedence_lut[i] = 0;
	}

	precedence_lut['#'] = 0;
	precedence_lut['('] = 1;
	precedence_lut['['] = 1;
	precedence_lut['{'] = 1;
	precedence_lut['}'] = 1;
	precedence_lut[')'] = 1;
	precedence_lut[']'] = 1;
	precedence_lut['+'] = 2;
	precedence_lut['-'] = 2;
	precedence_lut['*'] = 3;
	precedence_lut['/'] = 3;
	precedence_lut['^'] = 4;
}

static void InitAssociativity()
{
	int i = 0;
	
	for (i = 0; i < ASCII_SIZE; ++i)
	{
		associativity_lut[i] = ERROR;
	}

	associativity_lut['+'] = LEFT;
	associativity_lut['-'] = LEFT;
	associativity_lut['*'] = LEFT;
	associativity_lut['/'] = LEFT;
	associativity_lut['^'] = RIGHT;
}

static void InitBracketsLut()
{
	int i = 0;
	
	for (i = 0; i < ASCII_SIZE; ++i)
	{
		brackets_lut[i] = WRONG_INPUT;
	}

	brackets_lut['('] = 0;
	brackets_lut['{'] = 0;
	brackets_lut['['] = 0;
}

static void InitOperSymbolsLut()
{
	int i = 0;

	for (i = 0; i < ASCII_SIZE; ++i)
	{
		oper_symbols_lut[i] = ERROR;
	}

	oper_symbols_lut['('] = ERROR;
	oper_symbols_lut['['] = ERROR;
	oper_symbols_lut['{'] = ERROR;
	oper_symbols_lut['}'] = BRACKETS;
	oper_symbols_lut[')'] = BRACKETS;
	oper_symbols_lut[']'] = BRACKETS;
	oper_symbols_lut['+'] = OPER;
	oper_symbols_lut['-'] = OPER;
	oper_symbols_lut['*'] = OPER;
	oper_symbols_lut['/'] = OPER;
	oper_symbols_lut['^'] = OPER;
}

static void InitBracketsMatch()
{
	brackets_match[')'] = '(';
	brackets_match['}'] = '{';
	brackets_match[']'] = '[';
}

static void InitHandleSymbolLut()
{
	HandleSymbol[ERROR] = HandleError;
	HandleSymbol[OPER] = HandleOper;
	HandleSymbol[BRACKETS] = HandleBrackets;
}

static void InitStateMachine()
{
	StateMachine[WAIT_NUM]	= GetNum;
	StateMachine[WAIT_OPER] = GetOper;
}

/* ---  Handle Functions --------------------------------------------------- */

static int HandleError(char **runner, stack_t *num_stack, stack_t *oper_stack, 
					   int *status, unsigned char symbol)
{
	(void)runner;
	(void)num_stack;
	(void)oper_stack;
	(void)symbol;

	*status = WRONG_INPUT;

	return WAIT_OPER;
}

static int HandleOper(char **runner, stack_t *num_stack, stack_t *oper_stack, 
			 		  int *status, unsigned char symbol)
{
	unsigned char oper = *(char *)StackPeek(oper_stack);

	(void)runner;

	while (SUCCESS == *status && precedence_lut[symbol] <= precedence_lut[oper]
		   && LEFT == associativity_lut[symbol])
	{
		Fold(num_stack, oper_stack, status);
		oper = *(char *)StackPeek(oper_stack);
	}

	StackPush(oper_stack, &symbol);

	return WAIT_NUM;
}

static int HandleBrackets(char **runner, stack_t *num_stack, stack_t *oper_stack, 
						  int *status, unsigned char symbol)
{
	unsigned char oper = *(char *)StackPeek(oper_stack);

	(void)runner;

	while ((SUCCESS == *status) && (oper != brackets_match[symbol]))
	{
		Fold(num_stack, oper_stack, status);
		oper = *(char *)StackPeek(oper_stack);
	}

	StackPop(oper_stack);

	return WAIT_OPER;
}

/* ---  States ------------------------------------------------------------- */

static int GetNum(char **runner, stack_t *num_stack, stack_t *oper_stack,
				  int *status)
{
	double num = 0;
	char *runner_place = *runner;
	unsigned char oper = 0;

	num = GetNumConvertor(*runner, runner);

	if (*runner == runner_place)
	{
		oper = (unsigned char)**runner;
		*status = brackets_lut[oper];
		*runner += 1;
		StackPush(oper_stack, &oper);
		
		return WAIT_NUM; 
	}

	StackPush(num_stack, &num);

	return WAIT_OPER;
}

static int GetOper(char **runner, stack_t *num_stack, stack_t *oper_stack,
				   int *status)
{
	unsigned char symbol = 0;
	int state = 0;

	symbol = GetOperatorConvertor(runner);
	state = oper_symbols_lut[symbol];

	return HandleSymbol[state](runner, num_stack, oper_stack, status, symbol);
}

/* ---  Fold --------------------------------------------------------------- */

static void Fold(stack_t *num_stack, stack_t *oper_stack, int *status)
{
	double result = 0;
	double num1 = 0, num2 = 0;
	unsigned char oper = 0;

	if (2 > StackSize(num_stack))
	{
		*status = WRONG_INPUT;
		return;
	}
	
	num1 = *(double *)StackPeek(num_stack);
	StackPop(num_stack);
	num2 = *(double *)StackPeek(num_stack);
	StackPop(num_stack);
	oper = *(char *)StackPeek(oper_stack);
	StackPop(oper_stack);

	result = action_func_lut[oper](num2, num1, status);
	StackPush(num_stack, &result);
}

/* ---  Action Functions --------------------------------------------------- */

static double AddAction(double num1, double num2, int *status)
{
	(void)status;

	return (num1 + num2);
}

static double DivideAction(double num1, double num2, int *status)
{
	if (0 == num2)
	{
		*status = DIVIDE_BY_ZERO;
		return 0;
	}

	return (num1 / num2);
}

static double SubstractAction(double num1, double num2, int *status)
{
	(void)status;

	return (num1 - num2);
}

static double MultiplyAction(double num1, double num2, int *status)
{
	(void)status;

	return (num1 * num2);
}

static double PowerAction(double num1, double num2, int *status)
{
	if (0 > num1 && num2 < 1 && num2 > -1 && 0 != num2)
	{
		*status = NEGATIVE_ROOT;
	}

	(void)status;

	return pow(num1, num2);
}

static double ErrorAction(double num1, double num2, int *status)
{
	*status = WRONG_INPUT;
	
	(void)num1;
	(void)num2;
	
	return 0;
}

