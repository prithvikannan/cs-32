#include "Set.h"  // with ItemType being a type alias for char
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

int getPrecendenceFor(char op);
bool convertToPostfix(string infix, string& pf);
int evaluatePostfixExpression(string& postfix, const Set& trueValues, const Set& falseValues, bool& result);
int evaluate(string infix, const Set& trueValues, const Set& falseValues, string& postfix, bool& result);
bool parenthesisCheck(string infix);

int main() {
	string trueChars = "tywz";
	string falseChars = "fnx";
	Set trues;
	Set falses;
	for (int k = 0; k < trueChars.size(); k++)
		trues.insert(trueChars[k]);
	for (int k = 0; k < falseChars.size(); k++)
		falses.insert(falseChars[k]);

	string pf;
	bool answer;

	assert(evaluate("w| f", trues, falses, pf, answer) == 0 && pf == "wf|" &&  answer);
	assert(evaluate("y|", trues, falses, pf, answer) == 1);
	assert(evaluate("n t", trues, falses, pf, answer) == 1);
	assert(evaluate("nt", trues, falses, pf, answer) == 1);
	assert(evaluate("()", trues, falses, pf, answer) == 1);
	assert(evaluate("y(n|y)", trues, falses, pf, answer) == 1);
	assert(evaluate("t(&n)", trues, falses, pf, answer) == 1);
	assert(evaluate("(n&(t|7)", trues, falses, pf, answer) == 1);
	assert(evaluate("", trues, falses, pf, answer) == 1);
	assert(evaluate("f  |  !f & (t&n) ", trues, falses, pf, answer) == 0
		&& pf == "ff!tn&&|" && !answer);
	assert(evaluate(" x  ", trues, falses, pf, answer) == 0 && pf == "x" && !answer);
	trues.insert('x');
	assert(evaluate("((x))", trues, falses, pf, answer) == 3);
	falses.erase('x');
	assert(evaluate("((x))", trues, falses, pf, answer) == 0 && pf == "x"  &&  answer);
	trues.erase('w');
	assert(evaluate("w| f", trues, falses, pf, answer) == 2);
	falses.insert('w');
	assert(evaluate("w| f", trues, falses, pf, answer) == 0 && pf == "wf|" && !answer);

	assert(evaluate("(w & f))", trues, falses, pf, answer) == 1);
	assert(evaluate("((w & f)", trues, falses, pf, answer) == 1);
	assert(evaluate("!!w", trues, falses, pf, answer) == 0 && !answer);
	assert(evaluate("!!t", trues, falses, pf, answer) == 0 && pf == "t!!" &&  answer);
	assert(evaluate("(n)()", trues, falses, pf, answer) == 1);
	assert(evaluate("!", trues, falses, pf, answer) == 1);
	assert(evaluate("2", trues, falses, pf, answer) == 1);
	assert(evaluate("x!!", trues, falses, pf, answer) == 1);
	assert(evaluate("x!", trues, falses, pf, answer) == 1);
	assert(evaluate("x&!", trues, falses, pf, answer) == 1);
	assert(evaluate("!a!b!c", trues, falses, pf, answer) == 1);

	string a;
	convertToPostfix("y|!!x", a);
	cout << a << endl;

	string b;
	convertToPostfix("w|t&z", b);
	cout << b << endl;

	cout << "Passed all cases!" << endl;
}


int getPrecendenceFor(char op) {
	// returns 1,2,3 depending on the precendence of a particular operator
	switch (op) {
	case '!':
		return 3;
		break;
	case '&':
		return 2;
		break;
	case '|':
		return 1;
		break;
	default:
		return 0;
	}

}
bool convertToPostfix(string infix, string& pf) {
	// converts the infix string to postfix and saves that to pf
	string postfix = "";
	if (!parenthesisCheck(infix)) {				// checks if the number of closing parenthesis match number of opening parenthesis
		return false;
	}
	stack<char> opsstack;

	bool nextCouldBeAndOr = false;				// initialize booleans to keep track of which types the next character can be in a syntactically valid string
	bool nextCouldBeNot = true;
	bool nextCouldBeOperand = true;
	bool nextCouldBeOpen = true;
	bool nextCouldBeClosed = false;

	if (infix.size() == 0) {					// handle edge case with empty infix
		return false;
	}
	for (int i = 0; i < infix.size(); i++) {	// loop over each character in the string
		char ch = infix[i];
		switch (ch) {
		case '(':								
			if (!nextCouldBeOpen) {				// if the char is not allowed to be an open parenthesis, return false
				return false;
			}
			opsstack.push(ch);					
			break;
			nextCouldBeAndOr = false;			// reassign booleans to valid values that can follow an open parenthesis
			nextCouldBeNot = true;
			nextCouldBeOperand = true;
			nextCouldBeOpen = true;
			nextCouldBeClosed = false;
		case ')':
			if (!nextCouldBeClosed) {			// if the char is not allowed to be a closed parenthesis, return false
				return false;
			}
			while (opsstack.top() != '(') {		// find the corresponding open parenthesis
				postfix += opsstack.top();
				opsstack.pop();
			}
			opsstack.pop();
			nextCouldBeAndOr = true;			// reassign booleans to valid values that can follow a closed parenthesis
			nextCouldBeNot = false;
			nextCouldBeOperand = false;
			nextCouldBeOpen = false;
			nextCouldBeClosed = true;
			break;
		case '!':
			if (!nextCouldBeNot) {				// if the char is not allowed to be a not, return false
				return false;
			}
			while (!opsstack.empty() && (opsstack.top() != '(') 
				&& getPrecendenceFor(ch) < getPrecendenceFor(opsstack.top())) {
												// find the previous operand and append to postfix
				postfix += opsstack.top();
				opsstack.pop();
			}
			opsstack.push(ch);
			nextCouldBeAndOr = false;			// reassign booleans to valid values that can follow a not
			nextCouldBeNot = true;
			nextCouldBeOperand = true;
			nextCouldBeOpen = true;
			nextCouldBeClosed = false;
			break;
		case '&':
		case '|':
			if (!nextCouldBeAndOr) {			// if the char is not allowed to be and/or, return false
				return false;
			}
			while (!opsstack.empty() && (opsstack.top() != '(') && getPrecendenceFor(ch) <= getPrecendenceFor(opsstack.top())) {
												// find the previous operand and append to postfix
				postfix += opsstack.top();
				opsstack.pop();
			}
			opsstack.push(ch);
			nextCouldBeAndOr = false;			// reassign booleans to valid values that can follow and/or 
			nextCouldBeNot = true;
			nextCouldBeOperand = true;
			nextCouldBeOpen = true;
			nextCouldBeClosed = false;
			break;
		case ' ':								// handle spaces by not adding to postfix expression
			break;
		default:								// case operand
			if (!nextCouldBeOperand) {			// if the char is not allowed to be operand, return false
				return false;
			}
			if (!islower(ch)) {
				return false;
			}
			nextCouldBeAndOr = true;			// reassign booleans to valid values that can follow operand
			nextCouldBeNot = false;
			nextCouldBeOperand = false;
			nextCouldBeOpen = false;
			nextCouldBeClosed = true;
			postfix += ch;
			break;
		}
	}
	while (!opsstack.empty()) {					// add everything left on stack to postfix
		postfix += opsstack.top();
		opsstack.pop();
	}
	pf = postfix;
	return true;
}


int evaluatePostfixExpression(string& postfix, const Set& trueValues, const Set& falseValues, bool& result) {
	stack<bool> operands;
	for (int i = 0; i < postfix.size(); i++) {	// loop through the postfix expression
		char ch = postfix[i];
		bool op1;
		bool op2;
		switch (ch) {
		case '!':
			if (operands.size() < 1) {			// if there is not an operand for the nor, return 1
				return 1;
			}
			op1 = operands.top();				
			operands.pop();
			operands.push(!op1);				// do the not operation on the most recent item on the stack
			break;
		case '&':
			if (operands.size() < 2) {			// if there are not 2 operands for the and, return 1
				return 1;
			}
			op2 = operands.top();
			operands.pop();
			op1 = operands.top();
			operands.pop();
			operands.push(op1 && op2);			// do the and operation on the most recent 2 items on the stack
			break;
		case '|':
			if (operands.size() < 2) {			// if there are not 2 operands for the or, return 1
				return 1;
			}
			op2 = operands.top();
			operands.pop();
			op1 = operands.top();
			operands.pop();
			operands.push(op1 || op2);			// do the or operation on the most recent 2 items on the stack
			break;
		default:
			if (trueValues.contains(ch) && falseValues.contains(ch)) {
												// if an operand exists in both the true and false set, return 3
				return 3;
			}
			if (!trueValues.contains(ch) && !falseValues.contains(ch)) {
												// if an operand exists in neither the true and false set, return 2
				return 2;
			}
			operands.push(trueValues.contains(ch)); // push the value of the operand onto the stack
			break;
		}
	}
	result = operands.top();
	return 0;
}

bool parenthesisCheck(string infix) {
	// returns true if the number of parenthesis match, false otherwise
	int count = 0;
	for (int i = 0; i < infix.size(); i++) {
		if (infix[i] == '(') {
			count++;
		}
		else if (infix[i] == ')') {
			count--;
		}
		if (count < 0) {
			return false;
		}
	}
	if (count != 0) {
		return false;
	}
	return true;
}

int evaluate(string infix, const Set& trueValues, const Set& falseValues, string& postfix, bool& result) {
	// attempts to convert to postfix, if failed return 1
	if (!convertToPostfix(infix, postfix)) {	
		return 1;
	}
	// evaluates postfix, if error found return 1, if true/false issue return 2/3, otherwise return 0 
	return evaluatePostfixExpression(postfix, trueValues, falseValues, result);		
																		
}