/* 
 * File:   Function.h
 * Author: prasad
 *
 * Created on 16 January, 2014, 1:57 PM
 */

#pragma once

#include <string>
#include <stack>
#include <queue>
#include <list>
#include <cstring>
#include <cmath>
#include <cctype>

#define NUM_OPERATORS 14
#define NUM_FUNCTIONS 10
#define NUM_PARAMS 4

#define DOUBLE_TRUE 1.0
#define DOUBLE_FALSE 0.0

typedef double VALUE;

using namespace std;


enum FuncTokenType
{
  TOKEN_OPERATOR = 0,
  TOKEN_FUNCTION,
  TOKEN_PARAM,
  TOKEN_NUMBER,
  TOKEN_SEPARATOR
};

enum Operator
{
  OP_LOG_NOT = 0,
  OP_MUL,
  OP_DIV,
  OP_MOD,
  OP_PLUS,
  OP_MINUS,
  OP_REL_LT,
  OP_REL_GT,
  OP_REL_GTE,
  OP_REL_LTE,
  OP_REL_ET,
  OP_REL_NET,
  OP_LOG_AND,
  OP_LOG_OR
};

enum FunctionType
{
  FN_SIN = 0,
  FN_COS,
  FN_TAN,
  FN_FLOOR,
  FN_CEIL,
  FN_RND,
  FN_AVG,
  FN_POW,
  FN_SQRT,
  FN_ABS
};

enum Separator
{
  SEP_COMMA = 0,
  SEP_LPAREN,
  SEP_RPAREN
};

enum Parameter
{
  PARAM_T = 0,
  PARAM_X,
  PARAM_Y,
  PARAM_Z
};

typedef struct _FUNCTION
{
  FunctionType fnId;
  int n_args;
} FUNCTION;

/* The function parameter. */
typedef struct _FUNCPARAM
{
  double x,y, z, t;
} FUNCPARAM;

typedef struct _FUNCTOKEN
{
   FuncTokenType type;
  
  union{
    FUNCTION fn;
    Operator opid;
    Parameter param;
    Separator sepid;
    VALUE val;
  };
} FUNCTOKEN;
/**
 * Implementation of the Shunting Yard Algorithm by Esdgar Wube Dijkstra
 * 
 * Being written by Kurian Jacob.
 */

class Function {
private:
  /* The string version of the current function. */
  string m_strfunction;
  
  /* The processed function in Abstract Syntax Tree form. */
  list<FUNCTOKEN> m_outfn;
  
  /* The current pointer. */
  int m_strpt;
  
  static const char *stroperators[NUM_OPERATORS];
  
  static const char *strfn[NUM_FUNCTIONS];
  
  static const char chparams[4];
  
  static const double eps;
  
  vector<string> szOperators, szFunctions;
  vector<char> chParams;
  
public:
  Function();
  Function(const Function& orig);
  
  /* 
   * Reads a single token from the function string. The out value is in receive.
   * Return value is false if there are no more tokens to read.
   */
  bool nextStringToken (string& receive);
  bool tokenizeString (FUNCTOKEN& receive, const string& token);
  bool isSeparator (char ch);
  
  template<typename Type>
  int linearSearch (Type item, vector<Type> array);
  bool processFunction () throw(string);
  void setFunction (const string& str);
  
  double evaluateFunction (FUNCPARAM& param);
  void getParamValue (FUNCTOKEN& receive, const FUNCPARAM& param, const FUNCTOKEN& token);
  bool evalOnOperator (stack<FUNCTOKEN>& operandStack, const FUNCTOKEN& token);
  bool evalOnFunction (stack<FUNCTOKEN>& operandStack, const FUNCTOKEN& token);
  
  bool doubleEqual (double a, double b);
  bool getArgs (stack<FUNCTOKEN>& operandStack, double* op2, double* op1, int n_args);
  void getCurrentEquation (string& result);
  
  virtual ~Function();
};

