/** @file stack.h
 *  @brief Stack Data structure.
 *
 *
 *  @author Antonios Kalattas
 */

#include<stdbool.h>
#include<stdlib.h>
#include<stdio.h>

#ifndef STACK_H
#define STACK_H

/** 
 * @struct Node
 * @brief Represents a node is a stack.
 * 
 * Stores the value and the next node.
 * 
 */
typedef struct node{
    int data;
    struct node *next;
}Node;

/** 
 * @struct Stack.
 * @brief Represents an estire stack.
 * 
 * Stores the size, and the top node.
 * 
 */
typedef struct{
    int size;
    Node *top;
}Stack;

/** @brief Initialization of a stack.
 * 
 * It will initialise the stack fields. Top filed will be set to null.
 * And the size will be set to 0.
 * 
 * @param Stack The stack pointer.
 * 
 * @return Void.
 */
void initStack(Stack *s);

/** @brief Pushes the new number into the stack.
 * 
 * It will create new node, with the given numeric value. Next it will
 * set it as the new head. Also it will incrasse the size by one.
 * 
 * @param Stack The stack.
 * @param int The new integer to enter in stack.
 * 
 * @return void.
 */
void push(Stack *s, int a);

/** @brief Pops the top element for the stack.
 * 
 * It will pop the top elemnt of the stack. It will check if its empty or not before 
 * procedding the popping. It will return the numeric value of the pop element.
 * 
 * @param Stack the stack.
 * 
 * @return int. The numberic value of the popped value.
 */
int pop(Stack *s);

/** @brief Checks is the stack is empty or not.
 * 
 * It will return true if the size of the stack is empty. False if the size of stack is not 0.
 * 
 * @param Stack The stack.
 * 
 * @return bool.
 */
bool isEmpty(Stack *s);

/** @brief Used to empty a stack.
 * 
 *  The method will pop every element in the stack.
 * @param Stack the stack.
 * @return Void.
 */
void makeEmpty(Stack *s);

/** @brief it will create a reverse order stack of the given stack. 
 * 
 * @param Stac Pointer to the stack.
 * @return Stack pointer of the new reversed order.
 */
Stack * flipStack(Stack *);

/** @brief Get the top value of the stack.
 * 
 * This method just the retunr the top element inside the stack.
 * The one that if we do stack.pop() will be popped.
 * 
 * @param Stack The stack.
 * @return top value of stack.
 */
int top(Stack *s);

#endif // STACK_H