/** @file calcProtons.h
 *  @brief Calculates the number of protons.
 *
 *  @author Antonios Kalattas
 */
#include<stdlib.h>
#include<string.h>
#include<stdio.h>

#ifndef CALCPROTONS_H
#define CALCPROTONS_H

/** 
 * @struct Element
 * @brief Represents an element of the periodic table.
 * 
 * Stores the name and the number of its protons.
 * 
 */
typedef struct{
    char *name;     // name of the element.
    int protons;    // number of protons.
}Element;

/**
 * @struct PeriodicTable
 * 
 * @brief Represents the Periodic Table.
 * 
 * Stores all the elements of the periodic table in a type struct element array.
 * and the size.
 * 
 */
typedef struct{
    int size;
    Element element[118];
}PeriodicTable;

/** @brief Reads the periodic table, from the given file.
 * 
 * It will read the periodic table from the given file and will store it in the PeriodicTable 
 * struct field element. It will store seperatly every element, storing its name and the number of protons.
 * 
 * @param char* Name of the file that contains the periodic table.
 * @param PeriodicTable A pointer to a struct PeriodicTable.
 * 
 * @return Void.
 */
void readPeriodicTable(char *, PeriodicTable *);

/** @brief Will release any allocate memory.
 * 
 *  @param PerdiodicTable* pointer to periodic table.
 *  @return void.
 */
void freePeriodicTable(PeriodicTable *);

int calcProtons(char *s1, PeriodicTable *pt);

#endif
