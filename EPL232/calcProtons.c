#include "calcProtons.h"

int calcProtons(char *s1, PeriodicTable *pt){

    int total=0;                             // Sum of protons. 
    for(int i=0;i<pt->size;i++){             
        if(strcmp(s1,pt->element[i].name)==0){ // if the element has the same name as the perdio table at position i.
            total+=pt->element[i].protons;     // add the number of its protons to the total.
            break;
        }
    }
    return total;                             // return the total number.
}
/** 
 * @brief reads the periodic table from a given .txt file.
 * 
 * It will read the periodic table and store in the PeriodicTable struct.
 * 
 * @param char Pointer to the filename.
 * @param PeriodicTable Pointer to the periodicTable struct.
 * 
 * @return void.
*/
void readPeriodicTable(char *fileName, PeriodicTable *pt){
    pt->size=118;
    FILE *fp=NULL;                                                                       // create the file pointer.
    fp = fopen(fileName, "r");                                                      // open the file.
    if(fp==NULL){                                                                   //check if opened succsefully.
        perror("Cannot open file in readPeriodicTable()");
        return;
    }
    for(int i=0;i<118;i++){                                                         // read the 118 elements from the given file.
        char str[4];                                                                // create a tmp string to store the chemical name.
        fscanf(fp,"%3s", str);                                                    // read until you find a space.
        pt->element[i].name = (char *)malloc(strlen(str)+1); // +1 for \0 character. //allocate space for the name in the element struct +1 for \0.
        if(pt->element[i].name==NULL){                                               // check if allocated succesfully.
            perror("Memmory allocation for periodic table element faild.");
            fclose(fp);
            return;
        }
        strcpy(pt->element[i].name,str);                                             // copy from the tmp string to the element name.
        fscanf(fp,"%d",&pt->element[i].protons);                                      // last get the number of protons of the element.
    }
    fclose(fp);                                                                     // since we are done with the file reading close it.
}


/** @brief Free ups the allocated memory for the periodic Table.
 * 
 * @param PeriodicTable Pointer to PeriodicTable.
 * @return Void.
 */
void freePeriodicTable(PeriodicTable *pt){
    for(int i=0;i<118;i++){
        free(pt->element[i].name);
    }
    free(pt);
}
//Driver for calcProtons.c
/** @brief Main method to test functionality of the calcProtons.c
 * 
 * It will take one file from the arguemnts and try to read the elements.
 * It will store it in a staticly created Periodic Table, but later free it.
 * 
 * To run the main(): 
 * compile using: gcc -DDEBUG -o periodic_driver calcProtons.c 
 * run using:     ./periodic_driver periodicTable.txt  
 * @param argv argument, for the periodic table file.
 * @return EXIT_SUCCESS, if all are okay, EXIT_FAILURE if something whent wrong.
 */
#ifdef DEBUG
int main(int argc, char* argv[]) {
    PeriodicTable *pt=(PeriodicTable *)malloc(sizeof(PeriodicTable));
    if(!pt){
        perror("Failed to allocate memory PeriodicTable");
        return EXIT_FAILURE;
    }
    if(argv[1]==NULL){
        perror("file in arguments is null\n");
        return EXIT_FAILURE;
    }
    printf("Reading periodic table\n");
    readPeriodicTable(argv[1],pt);
    printf("Testing calcProtons:\n");
    char *test_elements[]={"H", "He", "O"};
    for(int i=0;i<3;i++) {
        int protons=calcProtons(test_elements[i], pt);
        if(protons>0){
            printf("Element %s has %d protons.\n",test_elements[i],protons);
        }else{
            printf("Element %s not found in the periodic table.\n", test_elements[i]);
        }
    }
    freePeriodicTable(pt);
    return EXIT_SUCCESS;
}
#endif
