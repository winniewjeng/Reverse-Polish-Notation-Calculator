#include "SimpleList.hpp"
#include "Token.hpp"
#include "Operand.hpp"
#include "Operator.hpp"
#include "Queue.hpp"
#include <iostream>
#include "Stack.hpp"

//evaluate the rpn expression
double Eval(Queue<Token*> postfix);
//parse the user-input string into tokens and store them in queue
Queue<Token*> toToken(string expression);
//convert the infix queue to a post-fix queue using Shunting-Yard algorithm
Queue<Token*> toPostFix(Queue<Token*> infix);
// check if eval is working
void test_eval();

using namespace std;

int main(int argc, const char * argv[]) {
    
    //    test_eval();
    
    string expression;
    cout << "Enter the operation: ";
    cin >> expression;
    
    Queue<Token*> infix = toToken(expression);
    cout << infix << endl;
    Queue<Token*> postfix = toPostFix(infix);
    cout << postfix << endl;
    return 0;
    
    /*
     attack plan:
     //get a populated queue of infix tokens
     //shunting-yard alogrithm
     //get an emptry queue of postfix tokens
     //get a stack of operator tokens
     //if it's a number, add it to the output
     //if it's an operator, check precedence
     //if precedence is correct, push it to the stack
     //if not, add it pop a stack to the postfix queue and push operator to stack
     //keep doing it until stack operator tokens has no more element
     */
}

Queue<Token*> toToken(string infix) {
    string num_str;
    string optr_str; // operator string
    Queue<Token*> input;
    
    for (int i = 0; i < infix.size(); i++) {
        // if the item in string is a number, get the number and push it to input Queue
        if (isdigit(infix[i]) || infix[i] == '.') {
            //form operand
            while (isdigit(infix[i]) || infix[i] == '.') {
                num_str += infix[i];
                i++;
            }
            double number = stod(num_str);
            input.push(new Operand(number));
            // reset the num_str for clean reading of the next number
            num_str="";
        }
        
        // if the item in string is an operator, get the operator and push it to input Queue
        if (!isdigit(infix[i])) {
            optr_str += infix[i];
            input.push(new Operator(optr_str));
            // reset the optr_str for clean reading of the next operator
            optr_str = "";
        }
        
        //trig detected...NOT YET IMPLEMENTED
        //if (isalpha(infix[i])) {}
        
    }
    return input;
}

double Eval(Queue<Token*> postfix) {
    //a stack that stores double* Operands
    Stack<double> s;
    
    while (!postfix.empty()) {
        //get the top Token* in the queue
        Token* p = postfix.top();
        //cout << p << " ";
        double LHS;
        double RHS;
        
        // if Token* is of type Operand(0), i.e. number digit, push into stack
        // if Token* is of type Operator(1), pop stack twice and do the math
        if (p->get_type() == 0) {
            // cast the Token* p to Operand*
            Operand* token_num = static_cast<Operand*>(p);
            // get the value that Operand* holds
            double num = token_num->get_num();
            // push the value into stack
            s.push(num);
        } else {
            // get two numbers from stack
            RHS = s.pop();
            LHS = s.pop();
            // do math with the numbers
            double result = static_cast<Operator*>(p)->do_math(LHS, RHS);
            // push result back into the stack
            s.push(result);
        }
        // house keeping - shrink the postfix queue
        postfix.pop();
    }
    
    return s.top();
};

Queue<Token*> toPostFix(Queue<Token*> infix) {
    // get a postfix queue
    Queue<Token*> postfix;
    // get an operator stack
    Stack<Operator*> optr;
    
    while (!infix.empty()) {
        
        Token* item = infix.pop();
        if (item->get_type() == 0) {
            // item is a number-->push item into the postfix queue
            postfix.push(item);
        }
        else {
            // if item is an operator, cast it as such
            Operator* optr_item = static_cast<Operator*>(item);
            
            if (optr.empty()) {
                // if stack is empty-->push optr_item into the stack
                optr.push(optr_item);
            } else {
                // if stack is not empty, check 1) for paranthesis, 2) for precedence
                if (optr_item->get_symb() == ")") {
                    //if optr_item is a closed paranthesis: keep poping the stack and pushing the popped item into the postfix queue until finding its counterpart
                    infix.pop(); // get rid of the ")" closed paranthesis in infix
                    while (optr.top()->get_symb() != "(") {
                        postfix.push(optr.pop());
                    }
                    optr.pop(); // get rid of the "(" opened paranthesis in stack
                } else if (optr_item->has_precedence(optr.top())) {
                    // if operator has precedence over the top operator in stack--> 1) pop stack and push the popped into postfix queue and 2) push optr_item into optr stack
                    postfix.push(optr.pop());
                    optr.push(optr_item);
                }
                else {
                    optr.push(optr_item);
                }
            }
        }
        
        if (infix.empty()) {
            while(!optr.empty()) {
                postfix.push(optr.pop());
            }
        }
    }
    
    return postfix;
}

void test_eval() {
    
    Queue<Token*> input;
    
    input.push(new Operand(4));
    input.push(new Operand(2));
    input.push(new Operator("+"));
    input.push(new Operand(5));
    input.push(new Operator("*"));
    
    cout << "Evaluate: " << endl;
    cout << input << " = " << Eval(input) << endl;
    
    Queue<Token*> input2;
    
    input2.push(new Operand(8));
    input2.push(new Operand(3));
    input2.push(new Operator("*"));
    input2.push(new Operand(6));
    input2.push(new Operator("/"));
    input2.push(new Operand(2));
    input2.push(new Operator("+"));
    
    cout << input2 << " = " << Eval(input2) << endl;
};

