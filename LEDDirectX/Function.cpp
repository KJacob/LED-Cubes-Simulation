/* 
 * File:   Function.cpp
 * Author: prasad
 * 
 * Created on 16 January, 2014, 1:57 PM
 */
#include "Function.h"

const char* Function::stroperators[NUM_OPERATORS] = {
    "!", "*", "/", "%", "+", "-", "<", ">", ">=", "<=",
    "==", "!=", "&", "|"
  };

const char* Function::strfn[NUM_FUNCTIONS] =
  {
    "sin", "cos", "tan", "floor", "ceil", "round", "avg",
    "pow", "sqrt", "abs"
  };

const char Function::chparams[4] = {'t', 'x', 'y', 'z'};

const double Function::eps = 0.000001;

Function::Function() : szOperators(NUM_OPERATORS),
        szFunctions(NUM_FUNCTIONS),
        chParams(NUM_PARAMS)
{
  int i;
  /* Set the pointer to zero. */
  m_strpt = 0;

  for (i = 0; i < NUM_OPERATORS; ++i)
    szOperators[i] = stroperators[i];
  
  for (i = 0; i < NUM_FUNCTIONS; ++i)
    szFunctions[i] = strfn[i];
  
  for (i = 0; i < NUM_PARAMS; ++i)
    chParams[i] = chparams[i];
}

void
Function::getCurrentEquation(string& result)
{
	result = this->m_strfunction;
}

Function::Function(const Function& orig) {
  /* This class does not support these kinds of operations. */
}

bool
Function::nextStringToken(string& receive)
{
  receive.clear();
  
  if (m_strpt >= m_strfunction.length())
    return false;
  
  while (m_strfunction[m_strpt] == '\t' ||
          m_strfunction[m_strpt] == ' ')
  {
    m_strpt++;
    if (m_strfunction.length () == m_strpt)
      return false;
  }
  
  if (isSeparator(m_strfunction[m_strpt]))
  {
    receive.push_back (m_strfunction[m_strpt++]);
  }
  else if (isalpha(m_strfunction[m_strpt]))
  {
    /* Got an alphabet. A parameter or a function? */
    while (m_strpt < m_strfunction.length () &&
            isalpha (m_strfunction[m_strpt]))
      receive.push_back (m_strfunction[m_strpt++]);
  }
  else if (isdigit(m_strfunction[m_strpt]))
  {
    int n_dots = 0;
    
    while (m_strpt < m_strfunction.length ())
    {
      if (isdigit(m_strfunction[m_strpt]))
        receive.push_back (m_strfunction[m_strpt]);
      else if (m_strfunction[m_strpt] == '.' && n_dots < 2)
      {
        receive.push_back (m_strfunction[m_strpt]);
        n_dots++;
      }
      else break;
      
      m_strpt++;
    }
  }
  else {
    while (m_strpt < m_strfunction.length() &&
            !isalnum(m_strfunction[m_strpt]) &&
            !isSeparator(m_strfunction[m_strpt]))
      receive.push_back (m_strfunction[m_strpt++]);
  }
  
  return true;
}

bool
Function::tokenizeString (FUNCTOKEN& receive, const string& token)
{
  int result;
  
  /* What do have here? A parameter? */
  if (token.length() == 1)
  {
    if (isSeparator(token[0]))
    {
      switch (token[0])
      {
        case ',':
          receive.sepid = SEP_COMMA;
          break;
          
        case '(':
          receive.sepid = SEP_LPAREN;
          break;
          
        case ')':
          receive.sepid = SEP_RPAREN;
      }
      
      receive.type = TOKEN_SEPARATOR;
      return true;
    }
    
    result = linearSearch(token[0], chParams);
    
    if (result >= 0)
    {
      receive.type = TOKEN_PARAM;
      receive.param = Parameter(result);
      return true;
    }
  }
  
  result = linearSearch(token, szFunctions);
  if (result >= 0)
  {
    receive.type = TOKEN_FUNCTION;
    receive.fn.fnId = FunctionType(result);
    
    /* In fact, we don't know the number of arguments.*/
    receive.fn.n_args = 0;
    return true;
  }
  
  result = linearSearch(token, szOperators);
  if (result >= 0)
  {
    receive.type = TOKEN_OPERATOR;
    receive.opid = Operator(result);
    
    return true;
  }
  
  /* If it belongs to none of this category, then, perhaps, it is a number. */
  if (isdigit(token[0]))
  {
    receive.type = TOKEN_NUMBER;
    receive.val = atof(token.c_str());
    return true;
  }
  
  /* Now we have a problem. */
  return false;
}

template
<typename Type>
int
Function::linearSearch(Type item, vector<Type> array)
{
  int index;
  for (index = 0; index < array.size(); ++index)
    if (item == array[index])
      return index;
  
  /* No such element.*/
  return -1;
}

bool
Function::isSeparator(char ch)
{
  return (
          ch == ' '  ||
          ch == '\t' ||
          ch == ','  ||
          ch == ')'  || 
          ch == '('
          );
}

void
Function::setFunction(const string& str)
{
  this->m_strfunction = str;
}

/*
 * In memory of the Legend E. W. Dijkstra
 * The shunting yard algorithm.
 */
bool
Function::processFunction() throw (string)
{
  string result;
  FUNCTOKEN token, optoken;
  
  stack<FUNCTOKEN> opStack;
  
  m_outfn.clear();
  m_strpt = 0;
  
  while (nextStringToken(result))
  {
    if (!tokenizeString(token, result))
    {
      throw string("Unrecognized token: ") + result;
    }
    
    switch (token.type)
    {
      case TOKEN_NUMBER:
      case TOKEN_PARAM:
        m_outfn.push_back(token);
        break;
        
      case TOKEN_OPERATOR:
        while (!opStack.empty() &&
               opStack.top().type == TOKEN_OPERATOR && 
               opStack.top().opid <= token.opid)
        {
          m_outfn.push_back(opStack.top());
          opStack.pop();
        }
        
        opStack.push(token);
        break;
        
      case TOKEN_SEPARATOR:
        if (token.sepid == SEP_LPAREN)
          opStack.push(token);
        else if (token.sepid == SEP_RPAREN)
        {
          while (true)
          {
            if (opStack.empty())
              throw string("Mismatched parenthesis.");
            
            if (opStack.top().type == TOKEN_SEPARATOR &&
                opStack.top().sepid == SEP_LPAREN)
            {
              opStack.pop();
              
              if (opStack.size() && opStack.top().type == TOKEN_FUNCTION)
              {
                /* Take me, I'm a function. */
                opStack.top().fn.n_args++;
                m_outfn.push_back(opStack.top());
                opStack.pop();
              }
              break;
            }
            
            m_outfn.push_back(opStack.top());
            opStack.pop();
          }
        }
        else 
        {
          /* Comma. */
          while (true)
          {
            if (opStack.empty())
              throw string ("Stray Comma.");
            
            if (opStack.top().type == TOKEN_SEPARATOR &&
                opStack.top().sepid == SEP_LPAREN)
            {
              optoken = opStack.top();
              opStack.pop();
              
              if (opStack.top().type != TOKEN_FUNCTION)
                throw string ("Unexpected comma.");
              else opStack.top().fn.n_args++;
              
              opStack.push(optoken);
              break;
            }
            
            m_outfn.push_back(opStack.top());
            opStack.pop();
          }
        }
        break;
        
      case TOKEN_FUNCTION:
        opStack.push(token);
        break;
        
      default:
        /* There's no plan B because plan A can't fail. */
        ;
    }
  }
  
  while (opStack.size())
  {
    if (opStack.top().type != TOKEN_OPERATOR)
      throw string ("Incorrect syntax.");
    m_outfn.push_back(opStack.top());
    opStack.pop();
  }
  
  return true;
}

bool
Function::evalOnFunction(stack<FUNCTOKEN>& operandStack, const FUNCTOKEN& token)
{
  int i, n_args = token.fn.n_args;
  FUNCTOKEN rToken;
  double op1, op2;
  
  rToken.type = TOKEN_NUMBER;
  
  if (token.fn.fnId == FN_AVG)
  {
    if (n_args > operandStack.size())
      return false;
    
    rToken.val = 0;
    
    for (i = 0; i < n_args; ++i)
    {
      rToken.val += operandStack.top().val;
      operandStack.pop();
    }
    
    rToken.val = rToken.val / n_args;
  }
  else if (token.fn.fnId == FN_POW)
  {
    if (!getArgs(operandStack, &op2, &op1, 2))
      return false;
    
    rToken.val = pow(op1, op2);
  }
  else
  {
    if (!getArgs(operandStack, &op1, &op2, 1))
      return false;
    
    switch (token.fn.fnId)
    {
      case FN_ABS:
        rToken.val = abs(op1);
        break;
        
      case FN_CEIL:
        rToken.val = ceil(op1);
        break;
        
      case FN_COS:
        rToken.val = cos(op1);
        break;
        
      case FN_FLOOR:
        rToken.val = floor(op1);
        break;
        
      case FN_RND:
        rToken.val = floor(op1 + 0.5);
        break;
        
      case FN_SIN:
        rToken.val = sin(op1);
        break;
        
      case FN_SQRT:
        rToken.val = sqrt(op1);
        break;
        
      case FN_TAN:
        rToken.val = tan(op1);
        break;
    }
  }
  
  operandStack.push(rToken);
  return true;
}

double
Function::evaluateFunction(FUNCPARAM& param)
{
  stack<FUNCTOKEN> operandStack;
  FUNCTOKEN valtoken, token;
  list<FUNCTOKEN>::iterator it;
  
  for (it = m_outfn.begin (); it != m_outfn.end(); ++it)
  {
	token = *it;
    switch(token.type)
    {
      case TOKEN_NUMBER:
        operandStack.push(token);
        break;
        
      case TOKEN_OPERATOR:
        if (!evalOnOperator (operandStack, token))
          throw string ("Error while retrieving operator's arguments. ");
        break;
        
      case TOKEN_FUNCTION:
        if (!evalOnFunction (operandStack, token))
          throw string("Error while evaluating function.");
        break;
        
      case TOKEN_PARAM:
        getParamValue (valtoken, param, token);
        operandStack.push (valtoken);
        break;
        
      default:
        /* Errors can't happen. */
        ;
    }
  }
  
  if (operandStack.size() != 1)
    throw string ("Wrong function.");
  
  return operandStack.top().val;
}

void
Function::getParamValue(FUNCTOKEN& receive, const FUNCPARAM& param, const FUNCTOKEN& token)
{
  receive.type = TOKEN_NUMBER;
  
  switch(token.param)
  {
    case PARAM_T:
      receive.val = param.t;
      break;
      
    case PARAM_X:
      receive.val = param.x;
      break;
      
    case PARAM_Y:
      receive.val = param.y;
      break;
      
    case PARAM_Z:
      receive.val = param.z;
      break;
      
    default:
      /* No. This can't be. */
      ;
  }
}

bool
Function::evalOnOperator(stack<FUNCTOKEN>& operandStack, const FUNCTOKEN& token)
{
  double op1, op2;
  FUNCTOKEN rToken;
  
  rToken.type = TOKEN_NUMBER;
  
  /* Single operand operator. */
  if (token.opid == OP_LOG_NOT)
  {
    if (!getArgs(operandStack, &op2, &op1, 1))
      return false;
    
    if (doubleEqual(op2, 0.0))
      rToken.val = DOUBLE_TRUE;
    else rToken.val = DOUBLE_FALSE;
    
    operandStack.push(rToken);
    return true;
  }
  
  if (!getArgs(operandStack, &op2, &op1, 2))
    return false;
  
  switch(token.opid)
  {
    case OP_DIV:      
      /* Can't divide by zero. */
      if (doubleEqual(op2, 0.0))
        return false;
      
      rToken.val = op1 / op2;
      break;
      
    case OP_LOG_AND:
      if (!doubleEqual(op2, 0.0) && !doubleEqual(op1, 0.0))
        rToken.val = DOUBLE_TRUE;
      else rToken.val = DOUBLE_FALSE;
      
      break;
      
    case OP_LOG_OR:
      if (!doubleEqual(op2, 0.0) || !doubleEqual(op1, 0.0))
        rToken.val = DOUBLE_TRUE;
      else rToken.val = DOUBLE_FALSE;
      
      break;
      
    case OP_MINUS:
      rToken.val = op1 - op2;
      break;
      
    case OP_MOD:
      rToken.val = long (op1) % long (op2);
      break;
      
    case OP_MUL:
      rToken.val = op1 * op2;
      break;
      
    case OP_PLUS:
      rToken.val = op1 + op2;
      break;
      
    case OP_REL_ET:
      if (doubleEqual(op1, op2))
        rToken.val = DOUBLE_TRUE;
      else rToken.val = DOUBLE_FALSE;
      break;
      
    case OP_REL_GT:
      if (op1 > op2)
        rToken.val = DOUBLE_TRUE;
      else rToken.val = DOUBLE_FALSE;
      break;
      
    case OP_REL_GTE:
      if (op1 >= op2)
        rToken.val = DOUBLE_TRUE;
      else rToken.val = DOUBLE_FALSE;
      break;
      
    case OP_REL_LT:
      if (op1 < op2)
        rToken.val = DOUBLE_TRUE;
      else rToken.val = DOUBLE_FALSE;
      break;
      
    case OP_REL_LTE:
      if (op1 <= op2)
        rToken.val = DOUBLE_TRUE;
      else rToken.val = DOUBLE_FALSE;
      break;
      
    case OP_REL_NET:
      if (!doubleEqual(op1, op2))
        rToken.val = DOUBLE_TRUE;
      else rToken.val = DOUBLE_FALSE;
      break;
  }
  
  operandStack.push(rToken);
  return true;
}

bool
Function::getArgs (stack<FUNCTOKEN>& operandStack, double* op2, double* op1, int nArgs)
{
  if (operandStack.size() < nArgs)
    return false;
  
  if (nArgs > 0)
  {
    *op2 = operandStack.top().val;
    operandStack.pop();
  }
  
  if (nArgs > 1)
  {
    *op1 = operandStack.top().val;
    operandStack.pop();
  }
  
  return true;
}

bool
Function::doubleEqual(double a, double b) {
  double difference = a - b;
  
  return difference < eps && difference > -eps;
}

Function::~Function() {
}