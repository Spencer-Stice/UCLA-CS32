
#include <stack>
#include <iostream>
#include <string>
#include <cassert>
using namespace std;

///////////////////////

int getPrecedence(char ch)          //returns precedence of given character, where a higher integer means higher precedence
{
    switch (ch)
    {
    case('|'):
        return 1;
    case('&'):
        return 2;
    case('!'):
        return 3;
    default:
        return 0;               //should never occur
    }
}

bool getBool(char ch, const bool values[])
{
    int i = ch - '0';
    return values[i];
}

int performOperation(bool num1, bool num2, char op)
{
    switch (op)
    {
    case('&'):
        return num1 & num2;
        break;
    case('|'):
        return num1 | num2;
    case('!'):
        return !num2;
    default:
        return true;
    }
}




///////////////////////






int evaluate(string infix, const bool values[], string& postfix, bool& result)
{
    //initializations


    postfix = "";
    if (infix.length() == 0)
        return 1;
    stack<char> operatorStack;
    char ch = ' ';
    string modInfix = "";
    int numOpening = 0;
    int numClosing = 0;
    int numOperands = 0;
    int numOperators = 0;



    // modify the input infix expression and check if it is valid



    for (int i = 0; i < infix.length(); i++)        //remove spaces
    {
        ch = infix[i];
        if (ch != ' ')
            modInfix += ch;
        if (ch != '0' && ch != '1' && ch != '2' && ch != '3' && ch != '4' && ch != '5' && ch != '6' && ch != '7' && ch != '8' && ch != '9'
            && ch != '!' && ch != '&' && ch != '|' && ch != '(' && ch != ')' && ch != ' ')
            return 1;
    }
    for (int i = 0; i < modInfix.length(); i++)         //ensure same number of opening and closing parenthesis
    {
        ch = modInfix[i];
        if (ch == '(')
            numOpening++;
        if (ch == ')')
            numClosing++;
        if (i == modInfix.length() - 1 && (numOpening != numClosing))
            return 1;
    }
    for (int i = 0; i < modInfix.length(); i++)
    {
        ch = modInfix[i];
        if (modInfix.length() == 1 && (ch == '&' || ch == '|' || ch == '!' || ch == '(' || ch == ')'))    //checks if infix is only one operator or if two binary operators are next to one another
            return 1;
        if (i == modInfix.length() - 1 && (ch == '&' || ch == '|' || ch == '!' || ch == '('))
            return 1;
        if (i == 0 && (ch == '&' || ch == '|'))
            return 1;
        if (i != 0 && i < modInfix.length() - 1)
        {
            if ((ch == '&' || ch == '|')
                && (modInfix[i - 1] == '&' || modInfix[i - 1] == '|'))
            {
                return 1;
            }
            if ((ch == '&' || ch == '|')
                && (modInfix[i + 1] == '&' || modInfix[i + 1] == '|'))
            {
                return 1;
            }
        }
        if (i != 0 && i < modInfix.length() - 1)
        {
            if ((ch == '0' || ch == '1' || ch == '2' || ch == '3' || ch == '4' || ch == '5' || ch == '6' || ch == '7' || ch == '8' || ch == '9')
                && (modInfix[i - 1] != '&' && modInfix[i - 1] != '|' && modInfix[i - 1] != '!' && modInfix[i - 1] != '('))
            {
                return 1;
            }
            if ((ch == '0' || ch == '1' || ch == '2' || ch == '3' || ch == '4' || ch == '5' || ch == '6' || ch == '7' || ch == '8' || ch == '9')
                && (modInfix[i + 1] != '&' && modInfix[i + 1] != '|' && modInfix[i + 1] != ')'))
            {
                return 1;
            }
        }
        if (i == 0 && (i != modInfix.length()-1) &&
            (ch == '0' || ch == '1' || ch == '2' || ch == '3' || ch == '4' || ch == '5' || ch == '6' || ch == '7' || ch == '8' || ch == '9') &&
            (modInfix[i + 1] == '(' || modInfix[i + 1] == ')'))
            return 1;
        if (i == modInfix.length()-1 && (i != 0) &&
            (ch == '0' || ch == '1' || ch == '2' || ch == '3' || ch == '4' || ch == '5' || ch == '6' || ch == '7' || ch == '8' || ch == '9') &&
            (modInfix[i - 1] == '(' || modInfix[i - 1] == ')'))
            return 1;
        if (ch == '(' && i < modInfix.length() - 1 && modInfix[i + 1] == ')')
            return 1;
        if (ch == '(' && i == modInfix.length() - 1)
            return 1;
        if (ch == ')' && i == 0)
            return 1;
        if (ch == ')' && i < modInfix.length() - 1 && modInfix[i + 1] == '(')
            return 1;
        if (i < modInfix.length() - 1 &&
            (ch == '0' || ch == '1' || ch == '2' || ch == '3' || ch == '4' || ch == '5' || ch == '6' || ch == '7' || ch == '8' || ch == '9') &&
            (modInfix[i + 1] == '0' || modInfix[i + 1] == '1' || modInfix[i + 1] == '2' || modInfix[i + 1] == '3' || modInfix[i + 1] == '4' || modInfix[i + 1] == '5' || modInfix[i + 1] == '6' || modInfix[i + 1] == '7' || modInfix[i + 1] == '8' || modInfix[i + 1] == '9'))
            return 1;


        //convert infix to postfix





        switch (ch)
        {
        case('0'):
        case('1'):
        case('2'):
        case('3'):
        case('4'):
        case('5'):
        case('6'):
        case('7'):
        case('8'):
        case('9'):
            {
            postfix += ch;
            numOperands++;
            break;
            }
        case('('):
        {
            operatorStack.push(ch);
            break;
        }
        case(')'):
        {
            while (operatorStack.top() != '(')
            {
                postfix += operatorStack.top();
                operatorStack.pop();
            }
            operatorStack.pop();
            break;
        }
        default:
        {
            while (operatorStack.size() != 0 && operatorStack.top() != '(' && getPrecedence(ch) <= getPrecedence(operatorStack.top()))
            {
                postfix += operatorStack.top();
                operatorStack.pop();
            }
            numOperators++;
            operatorStack.push(ch);
            break;
        }
        }
    }
    while (operatorStack.size() != 0)
    {
        postfix += operatorStack.top();
        operatorStack.pop();
    }
    if (numOperands == 0)
        return 1;
    if (numOperators == 0 && numOperands != 1)
        return 1;







    //evaluate postfix expression






    stack<bool> operandStack;
    bool op1;
    bool op2;
    for (int i = 0; i < postfix.length(); i++)
    {
        char ch = postfix[i];
        if (ch == '0' || ch == '1' || ch == '2' || ch == '3' || ch == '4' || ch == '5' || ch == '6' || ch == '7' || ch == '8' || ch == '9')
            operandStack.push(getBool(ch, values));
        else if (ch != '!')
        {
            op2 = operandStack.top();
            operandStack.pop();
            op1 = operandStack.top();
            operandStack.pop();
            operandStack.push(performOperation(op1, op2, ch));
        }
        else
        {
            op2 = operandStack.top();
            operandStack.pop();
            op1 = false;
            operandStack.push(performOperation(op1, op2, ch));
        }
    }
    result = operandStack.top();
    return 0;
}



/////////////////////
int main()
{
    bool ba[10] = {
        //  0      1      2      3      4      5      6      7      8      9
          true,  true,  true,  false, false, false, true,  false, true,  false
    };
    string pf;
    bool answer;
    assert(evaluate("2!3", ba, pf, answer) == 1);
    assert(evaluate("8|", ba, pf, answer) == 1);
    assert(evaluate(" &6", ba, pf, answer) == 1);
    assert(evaluate("4 5", ba, pf, answer) == 1);
    assert(evaluate("01", ba, pf, answer) == 1);
    assert(evaluate("()", ba, pf, answer) == 1);
    assert(evaluate("4&()!5", ba, pf, answer) == 1);
    assert(evaluate("2(9|8)", ba, pf, answer) == 1);
    assert(evaluate("2(&8)", ba, pf, answer) == 1);
    assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
    assert(evaluate("x+5", ba, pf, answer) == 1);
    assert(evaluate("", ba, pf, answer) == 1);
    assert(evaluate("2|3|4", ba, pf, answer) == 0
        && pf == "23|4|" && answer);
    assert(evaluate("2|(3|4)", ba, pf, answer) == 0
        && pf == "234||" && answer);
    assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0
        && pf == "43!03&&|" && !answer);
    assert(evaluate(" 9  ", ba, pf, answer) == 0 && pf == "9" && !answer);
    ba[2] = false;
    ba[9] = true;
    assert(evaluate("((9))", ba, pf, answer) == 0 && pf == "9" && answer);
    assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" && !answer);
    cout << "Passed all tests" << endl;
}