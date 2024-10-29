/** @file parseFormula.c
 *  @brief Calculates total protons and expands chamical formulas..
 *
 *
 *  @author Antonios Kalattas
 */

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
#include<string.h>

#include"stack.h"
#include"calcProtons.h"

bool parenthesisCheck(char *);

int findNumber(Stack *);

void inputHandler(Stack *, Stack *, Stack *, char *);

int charToInt(char);

void flush(Stack *, int);

void flushWithDestination(Stack *, Stack *);

void removePar(Stack *);

int findTotalProtons(Stack *s, PeriodicTable *pt);

void expandFormula(char *, PeriodicTable *, char *, bool);

int main(int argc, char* argv[]){
    //Input arguments
    char *periodicFile = NULL;   // name of periodic table file.
    char *option = NULL;         // type of option: -v, -ext, -pn.
    char *inputFile = NULL;      // input file.
    char *outputFile=NULL;          // output file. Used only if the option is -ext and -pn.
    //checks for null strings
    if(argv[1]!=NULL)
        periodicFile=argv[1];
    else{
        perror("periodic file is not given.");
        return EXIT_FAILURE;
    }
    if(argv[2]!=NULL)
        option=argv[2];
    else{
        perror("option is not given.");
        return EXIT_FAILURE;
    }
    if(argv[3]!=NULL)
        inputFile=argv[3];
    else{
        perror("input file is not given.");
        return EXIT_FAILURE;
    }
    if(strcmp(option,"-v")!=0){      // check that option is not -v.
        if(argv[4]!=NULL)
            outputFile = argv[4];
        else{
            perror("output file not given!");
            return EXIT_FAILURE;
        }

    }
    

    //Periodic Table.
    PeriodicTable *pt = (PeriodicTable *)malloc(sizeof(PeriodicTable)); // allocate space for periodic table.
    if(pt==NULL){                                                       // check if malloc worked.
        perror("Faild to allocate space for periodic table!\n");
        return EXIT_FAILURE;
    }

    readPeriodicTable(periodicFile,pt);                                 // call the method to read it.

    if(strcmp(option,"-v")==0){                                         // if the option is -v.
        printf("Verify balanced parentheses in %s\n",inputFile);
        if(parenthesisCheck(inputFile))                                 // call the parentheses check.
            printf("Parentheses are balanced for all chemical formulas");
        freePeriodicTable(pt);
        return 0;
    }
    if(strcmp(option,"-ext")==0){                                       // if the option is -ext.
        printf("Compute extended version of formulas in %s\n", inputFile);
        expandFormula(inputFile, pt, outputFile, true);                 // call the expandFormula method.
        printf("Writing formulas to %s\n", outputFile);
        freePeriodicTable(pt);
        return 0;
    }
//
    if(strcmp(option,"-pn")==0){                                        // if the option is -pn.
        printf("Compute total proton number of formulas in %s\n", inputFile);
        printf("%s, %s",outputFile,outputFile);
        expandFormula(inputFile, pt, outputFile, false);                // call the expandFormula method.
        printf("Writing formulas to %s", outputFile);
        freePeriodicTable(pt);
        return 0;
    }
    return 0;
}

/** @brief Checks if the parentheses in the readFile are correct.
 *
 * It will scan the file and will check the parantheses.
 * On the first line that will detect a fault will stop
 * and print the line that the fault accured.
 *
 * @param char pointer to file name.
 * @return void.
 */
bool parenthesisCheck(char *file){
    bool return_flag=true;                          // true if everything is correct. False if there is atleast one problem.
    FILE *new_fp=NULL;                              // file pointer to the input file.
    new_fp = fopen(file,"r");                       // open the file
    if(new_fp==NULL){                               // check if it was oppend.
        perror("No Such File!\n");
        return EXIT_FAILURE;
    }
    Stack *par = (Stack *)malloc(sizeof(Stack));    // create a stack where we store the parentheses.
    if(!par){
        perror("faild to allocate space");
        fclose(new_fp);
        return false;
    }
    initStack(par);
    char parenth;                                       // used to sotre the character and check if it is ( or  ).
    int line=1;                                     // used to know the line where a fault may accure.
    while((parenth=fgetc(new_fp))!=EOF){
        if(parenth==10){                            // if we change the line.
            if(!isEmpty(par)){                      // if the line is change and the stack is not empty, its a fault.
                printf("Parentheses NOT balanced in line: %d!\n", line);
                return_flag=false;
            }
            makeEmpty(par);                         // else make it empty.
            line++;                                 // increase the line counter.
        }
        if(parenth=='('){                               // if the par==(.
            push(par,40);                           // push it into the stack.
        }
        if(parenth==')'){                               // if ), pop the top element of the stack and check if it is (.
            if(isEmpty(par)){                                // if the top was NULL the that means we close a parenthesis without oppening it.
                printf("Parentheses NOT balanced in line: %d!\n", line);
                return_flag=false;
            }
            else 
                pop(par); 
        }
    }
    fclose(new_fp);                                 // close the file.
    free(par);
    return return_flag;                             // return the return_flag.
}

/** @brief This function will calculate the number that was push in the stack. for example 1, 2 will become 12.
 *  @param Stack Stack that stores the numbers.
 * @return Int
 */
int findNumber(Stack *numbers_stack){
    if(isEmpty(numbers_stack)){
        return 1;
    }
    else{
        int num=0;
        int c=0;
        while(!isEmpty(numbers_stack)){
            num+=(pop(numbers_stack)*pow(10,c));
            c++;
        }
        return num;
    }
}

/** @brief Convert a number char to int.
 *
 * @param char the char digit.
 * @return Numeric Value(int).
 */
int charToInt(char c){
    return(c - '0');      //conver the char into int by subtracting the char 0.
}

/** @brief Used in -exp or -np modes where we need to fully expand the chemical.
 * 
 * We basically fully expand the chemical. For example if we have H2, it will produce the HH.
 * There are 3 mainly used stacks. The aux, the fullyExpand, and the numbers stack.
 * When a new elements, is going in first its being stoed in the AUX stack. It will stay there until a
 * new element comes in. The mechanism workes like that:
 * > Read the first letter.
 * > Keep reading until a new UPPER case later, or a parenthesis comes in.
 * > If its a number then convert it into an int and push it into the numbers stack.
 * > Just before the new elements comes in the numbers stack will be emptyed, so If I have the stack 1,2,3 it will be poped out and become 123.
 * > Pop the current state of the AUX stack n-times and push it into the fully expanded-stack.
 * > Same thing when a close-parenthesis comes in. It will keep popping and pushing unti a ( is in the top position of the aux stack.
 */
void expandFormula(char *inputFile, PeriodicTable *pt, char *outputFile, bool mode){
    FILE *fp=NULL;                                               // File pointer used to read data.
    FILE *fp_out=NULL;                                           // FIle pointer used to write data.
    fp_out = fopen(outputFile,"w");                         // initialise the output pointer.
    if(fp_out==NULL){                                       // check if it worked.
        perror("Cannot create file with this name");        
        return;
    }
    fp = fopen(inputFile,"r");                              // point it to the givven file.
    if(fp==NULL){                                           // check if it worked.
        perror("Cannot open file to read!");
        return;
    }
    Stack *full_expanded = (Stack *)malloc(sizeof(Stack));  // stack to hold the fully expanded version.
    Stack *numbers= (Stack *)malloc(sizeof(Stack));         // create a stack to save the numbers.
    Stack *aux= (Stack *)malloc(sizeof(Stack));             // create an auxilary stack to store the input.
    initStack(numbers);                                     // initialise numbers stack.
    initStack(aux);                                         // initialise aux stack.
    initStack(full_expanded);                               // initialise the full_expanded stack.
    char input;                                             // used to store the last character.
    bool ff=false;                                           // boolean to check incase if its the first character reading.
    while((input=fgetc(fp))!=EOF){                          // scan until the end-of-file.
        if(!ff){                                            // if it's the first time we are reading a character.
            push(aux,(int)input);                           // push it into the aux stack.
        }
        else{
            inputHandler(aux,full_expanded,numbers,&input); // if its not the first time then we send the character in the input handler, with the numbers stack, the aux and the fuly expanded.
            if(input==10){                                  // if line is changed.
                Stack *outputStack =(Stack *)malloc(sizeof(Stack)); // output stack used for outputing in the correct orded in the file.
                if(!outputStack){
                    perror("faild to locate space");
                    return;
                }
                initStack(outputStack);
                Stack *flippedStack = flipStack(full_expanded);     // Flipped stack where the fliped state of the fully expanded stack will be stored.
                if(!flippedStack){
                    perror("faild to locate space");
                    return ;
                }
                while(!isEmpty(flippedStack)){                      // untl the flipped is empty either push its contains in the outputStack, or print it in the .txt file.
                    if(mode==0){
                        push(outputStack,pop(flippedStack));
                    }
                    else{
                        fprintf(fp_out,"%c",top(flippedStack));
                        pop(flippedStack);
                    }
                }
                if(mode==0){                                    // if the mode is -pn
                    while(!isEmpty(outputStack))                // until the outputStack is empty, push it contains into the flippedStack.
                        push(flippedStack,pop(outputStack));
                    fprintf(fp_out,"%d\n",findTotalProtons(flippedStack, pt));//then calculate the total number of protons, and printi it into the .txt file.
                }
                if(mode!=0){                                    // if mode is -ext, make a new line (\n).
                    fprintf(fp_out,"\n");
                }
                makeEmpty(full_expanded);                       // empty the full_expanded stack.
                makeEmpty(aux);                                 // empty the aux stack.
                makeEmpty(outputStack);
                free(outputStack);                              // deallocate the memory from the output stack.
                makeEmpty(flippedStack);
                free(flippedStack);                             // deallocate the memory from the flipped stack.    
            }
        }
    ff=true;
    }
    makeEmpty(full_expanded);
    free(full_expanded);
    makeEmpty(aux);
    free(aux);
    makeEmpty(numbers);
    free(numbers);
    fclose(fp);
    fclose(fp_out);
}

/** @brief Handles the input.
 *
 *  @param Stack aux stack.
 *  @param Stack The stack where the fully expanded version is being stored.
 *  @param Stack The stack that hold the nubmers.
 *  @param char The inputcharacter.
 *
 *  @return void.
 */
void inputHandler(Stack *aux, Stack *full, Stack *numbers, char *input){
    static bool parentheses=false;               // create a static bool variable to hold the status of parentheses.
    static int parentheses_active=0;             // counter that stores the number of active parenhesis. Used when we hae nested.
    //input cases:
    //if the parenthesis status is false. Every new uppercase means new element.
    if(!parentheses){
        if(*input==10){
            push(aux,findNumber(numbers));
            flushWithDestination(full,aux);
            removePar(full);
            return;
        }
        removePar(aux);
        if((*input>='A' && *input<='Z' )|| *input=='('){          // if char is Uppercase. So new element. So expand the other that it's in the stack and push it into full.
            push(aux,findNumber(numbers));                      // get the number that is in the stack numbers. and push it into the stack aux so you can expand it.
            flushWithDestination(full, aux);                    // expand the aux stack and save it in the full stack .
            push(aux,*input);                                   // push the new input in the aux.
        }
        if(*input=='('){                                        //when the input is ( we need to handle the other characters defferently.
            //pop(aux);                                           // because we pushed ( from the previous if statement pop it.
            parentheses=true;                                   // if input is ( make the status true.
            parentheses_active++;                               // increase the number of active parentheses.
            return;
        }
        if(*input>='0' && *input<='9'){                         // if char is a number.
            push(numbers,charToInt(*input));
            return;
        }
        if(*input>='a' && *input<='z'){                         // if the char is lower case.
            push(aux, *input);
            return;
        }
        return;
    }

     // if the parenthesis status is true.
    if(parentheses){
        if(*input==10){
            push(aux,findNumber(numbers));
            flushWithDestination(full,aux);
            removePar(full);
            return;
        }
        if((*input>='A' && *input<='Z' )|| *input=='(' || *input==')'){         // if char is Uppercase. So new element. So expand the other that it's in the stack and push it into full.
            push(aux,findNumber(numbers));      // get the number that is in the stack numbers. and push it into the stack aux so you can expand it.
            flush(aux, parentheses_active);                         // expand the aux stack.
            push(aux,*input);                   // push the new character include ().
            if(*input=='(')
                parentheses_active++;
            if(*input==')')
                parentheses_active--;
        }
        if(*input>='0' && *input<='9'){         // if char is a number.
            push(numbers,charToInt(*input));
            return;
        }
        if(*input>='a' && *input<='z'){         // if the char is lower case.
            push(aux, *input);
            return;
        }
         if(*input==')'){                             //when the input is ) we need to handle the other characters defferently.
            if(parentheses_active==0)                // check if the nubmer is of active is 0 make the status false again.
                parentheses=false;
            return;
        }
        return;
    }
}


/** @brief expads the contains of the aux stack.
 *
 * @param Stack aux stack.
 *
 * @return void
 */
void flush(Stack *aux, int pars){
    Stack *s1 = (Stack *)malloc(sizeof(Stack));
    Stack *s2 = (Stack *)malloc(sizeof(Stack));
    initStack(s1);
    initStack(s2);
    int i=pop(aux);                                 // pop the top element witch is a number so you know how many times to expand.
    if(top(aux)==')'){
        pop(aux);
        while(!isEmpty(aux)){
            if(top(aux)=='('){                               // empty the aux in s1 unil you find (.
                break;
            }
            int j=pop(aux);
            push(s1,j);
        }
    }
    else{
        while(!isEmpty(aux)){
            if(top(aux)>='a' && top(aux)<='z')
                push(s1,pop(aux));
            else
                break;
        }
        push(s1,pop(aux));
    }
    for(int k=0;k<2*i;k++){                         // copy the elements from s1 to s2 and aux back.
        if(k%2==0){                                 // if counter is even, empty the elements to s2 and aux.
            while(!isEmpty(s1)){
                int k=pop(s1);
                push(aux,k);
                push(s2,k);
            }
        }
        else{                                       // if counter is odd, now empty the elements of s2 to s1.
            while(!isEmpty(s2))
                push(s1,pop(s2));
        }
    }
    makeEmpty(s1);
    makeEmpty(s2);
    free(s1);
    free(s2);
    return;
}

/** @brief expands the source stack and flushes it into destination.
 *
 * @param Stack destination stack.
 * @param Stack source stack.
 *
 * @return void.
 */
void flushWithDestination(Stack *destination, Stack *source){
    Stack *s1 = (Stack *)malloc(sizeof(Stack));         //Stack used to pushi elements between.
    Stack *s2 = (Stack *)malloc(sizeof(Stack));         //Stack used to pushi elements between.
    if(!s1){
        perror("Faild to allocate space");
        return;
    }
    if(!s2){
        perror("Faild to allocate space");
        return;
    }
    initStack(s1);
    initStack(s2);
    int i=pop(source);                                  // pop the top element, witch is a number and use it to iterate.
    while(!isEmpty(source)){                            // empty the stack to the s1.
        int j=pop(source);
        push(s1,j);
    }
    for(int k=0;k<2*i;k++){                             // keep muving the elements from s1 to s2 and aux, with the name order while multipling it.
        if(k%2==0){                                     // if the counter is even, empty the s1 in destination stack and s2.
            while(!isEmpty(s1)){
                int k=pop(s1);
                push(destination,k);
                push(s2,k);
            }

        }
        else{                                           // if counter is odd, empty the s2 in s1.
            while(!isEmpty(s2))
                push(s1,pop(s2));
        }
    }
    makeEmpty(s1);
    makeEmpty(s2);
    free(s1);
    free(s2);
    return;
}


/** @brief Used to remove all parentheses from the chemical element.
 *
 * @param Stack aux stack.
 * @return void
 */
void removePar(Stack *aux){
    Stack *s1 = (Stack *)malloc(sizeof(Stack));     //Stack to copy all the elements except of the ().
    if(!s1){
        perror("Faild to allocate space\n");
        return;
    }
    initStack(s1);
    while(!isEmpty(aux)){                           // pop the stack and check every character if its a ( or not ).
        int j = pop(aux);
        if(j!='(' && j!=')' && j!=0)
            push(s1,j);
    }
    while(!isEmpty(s1)){                            // copy it back to aux with same order.
        push(aux,pop(s1));
    }
    makeEmpty(s1);
    free(s1);
    return;
}

/**
 * @brief It will calculate the total protons using the calcProtons
 * 
 * It will empty the fully expandend stack and calculate the total protons.
 * .
 * @param Stack Pointer to the fully expanded stack.
 * @param PeriodicTable Pointer to the periodic table.
 * 
 * @return Integer, total number of protons.
 */
int findTotalProtons(Stack *flippedStack, PeriodicTable *pt){
   int total=0;                             // store the total number of protons.
    while(!isEmpty(flippedStack)){          // empty the stack.
        char element_name[4]={0};           // variable to store the full length of an element.
        element_name[0]=pop(flippedStack);  // pick the top element of the flip
        int i=1;                            
        while(!isEmpty(flippedStack)&&(char)top(flippedStack)>='a'){ // check if there is more to an element. In case its an element with a lower case letter Ci, Al etc.
            char e = (char)pop(flippedStack);                        // if true add it to the element_name.
            element_name[i]=e;
            i++;
        }
        char *s1 = element_name;                                    
        total+=calcProtons(s1,pt);                                  // call the calcProtons() in calcProtons.h
    }

   return total;                                                    // return the total number of protons.
}
