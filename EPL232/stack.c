#include "stack.h"

/** @brief Initialization of a stack.
 * 
 * It will initialise the stack fields. Top filed will be set to null.
 * And the size will be set to 0.
 * 
 * @param Stack The stack.
 * 
 * @return Void.
 */

void initStack(Stack *s){
    s->top=NULL;
    s->size=0;
}

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
void push(Stack *s, int a){
    Node *newN=(Node*)malloc(sizeof(Node)); //create a new Node dynamically.
    if(newN==NULL){                         // chekc if the dinamic allocaiton worked.
        perror("Memory allocation fail! \n");
        return;
    }
    newN->data=a;                           // set the new node value to the givven one.
    newN->next=s->top;                      // set the new node next filed to the top value.
    s->top=newN;                            // set the new node to be the new head.
    s->size++;                              // increase the size by one.
    return;
}

/** @brief Pops the top element for the stack.
 * 
 * It will pop the top elemnt of the stack. It will check if its empty or not before 
 * procedding the popping. It will return the numeric value of the pop element.
 * 
 * @param Stack the stack.
 * 
 * @return int. The numberic value of the popped value.
 */
int pop(Stack *s){
    if(isEmpty(s)){                      // check if the stack is empty.
        printf("Stack is empty\n");
        return 0;
    }
    Node *p=s->top;                     // create a pointer to the current top element. Used for free().
    int return_value=p->data;           // get the nummeric value of the top element that is about to be topped.
    s->top=s->top->next;                // set the new head to the currect next of the head.
    s->size--;                          // decrease the size by one.
    free(p);                            // free the previous head.
    return return_value;                // return the previous head numeric value.
}

/** @brief Checks is the stack is empty or not.
 * 
 * It will return true if the size of the stack is empty. False if the size of stack is not 0.
 * 
 * @param Stack The stack.
 * 
 * @return bool.
 */
bool isEmpty(Stack *s){
    return s->size==0;
}
/** @brief Used to empty a stack.
 * 
 *  The method will pop every element in the stack.
 * @param Stack the stack.
 * @return Void.
 */
void makeEmpty(Stack *s){
    if(!s)
        return;
    while(!isEmpty(s)){
        pop(s);
    }
}

/** @brief it will create a reverse order stack of the given stack. 
 * 
 * @param Stac Pointer to the stack.
 * @return Stack pointer of the new reversed order.
 */
Stack * flipStack(Stack *s){
    Stack *newS = (Stack *)malloc(sizeof(Stack));
    initStack(newS);
    while(!isEmpty(s)){
        push(newS,pop(s));
    }
    return newS;
}

/** @brief Get the top value of the stack.
 * 
 * This method just the retunr the top element inside the stack.
 * The one that if we do stack.pop() will be popped.
 * 
 * @param Stack The stack.
 * @return top value of stack.
 */
int top(Stack *s){      
    if (isEmpty(s)){                        // check if the stack is empy
        perror("Stack is empty\n"); // if empty output this message.
        return 0;
    }
    return s->top->data;                    // if not empty return the top element.
}

/** @brief Driver for the stack.c
 * 
 * It will test every method of this file.
 * It will run push, pop, top, isEmpty, makeEmpty, flipStack.
 * 
 * @return EXIT_SUCCESS, if eveything is working.
 */
#ifdef DEBUG
int main(void){
    printf("STACK DEBUG main()");
    Stack s;
    initStack(&s);
    printf("Stack is initialised.\n");

    printf("Is the stack empty?\n");
    if(isEmpty(&s))
        printf("Yes\n");
    else
        printf("No\n");
    printf("Pushinh elements\n");
    push(&s, 10);
    push(&s, 20);
    push(&s, 30);
    printf("Pushed 10, 20, 30 in the stack.\n");
    printf("Top is: %d\n",top(&s)); // Expecting: 30

    printf("Popped: %d\n", pop(&s));  // Expecting: 30
    printf("Popped: %d\n", pop(&s));  // Expecting: 20
    printf("Top element after pops: %d\n", top(&s));  // Expecting: 10
    printf("Emptied the stack.\n");
    makeEmpty(&s);
    printf("Is the stack empty?\n");
    if(isEmpty(&s))
        printf("Yes\n");
    else
        printf("No\n");
    
    push(&s, 5);
    push(&s, 15);
    push(&s, 25);
    Stack *flipped = flipStack(&s);
    printf("Created flipped\n");

    printf("Popped from flipped: %d\n", pop(flipped)); // Expecting: 5
    printf("Popped from flippe: %d\n", pop(flipped)); // Expecting: 15
    printf("Popped from flipped: %d\n", pop(flipped)); // Expecting: 25

    makeEmpty(flipped);
    free(flipped);
    
    printf("Is the stack empty?\n");
    if(isEmpty(&s))
        printf("Yes\n");
    else
        printf("No\n");
    return EXIT_SUCCESS;
}
#endif
