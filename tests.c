/**
 * Sample test routine for executing each function at least once.
 * Copyright Brian Fraser, 2020
 */

#include "list.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

// Macro for custom testing; does exit(1) on failure.
#define CHECK(condition) do{ \
    if (!(condition)) { \
        printf("ERROR: %s (@%d): failed condition \"%s\"\n", __func__, __LINE__, #condition); \
        exit(1);\
    }\
} while(0)



// For checking the "free" function called
static int complexTestFreeCounter = 0;
static void complexTestFreeFn(void* pItem) 
{
    CHECK(pItem != NULL);
    complexTestFreeCounter++;
}

// For searching
static bool itemEquals(void* pItem, void* pArg) 
{
    return (pItem == pArg);
}

static void testComplex()
{
    // Empty list
    List* pList = List_create();
    CHECK(pList != NULL);
    CHECK(List_count(pList) == 0);

    // Add 
    int added = 41;
    CHECK(List_add(pList, &added) == 0);
    CHECK(List_count(pList) == 1);
    CHECK(List_curr(pList) == &added);

    // Insert
    int inserted = 42;
    CHECK(List_insert(pList, &inserted) == 0);
    CHECK(List_count(pList) == 2);
    CHECK(List_curr(pList) == &inserted);
    
    // Prepend
    int prepended = 43;
    CHECK(List_prepend(pList, &prepended) == 0);
    CHECK(List_count(pList) == 3);
    CHECK(List_curr(pList) == &prepended);
    
    // Append
    int appended = 44;
    CHECK(List_append(pList, &appended) == 0);
    CHECK(List_count(pList) == 4);
    CHECK(List_curr(pList) == &appended);
    
    // Next through it all (from before list)
    CHECK(List_first(pList) == &prepended);
    CHECK(List_prev(pList) == NULL);
    CHECK(List_next(pList) == &prepended);
    CHECK(List_next(pList) == &inserted);
    CHECK(List_next(pList) == &added);
    CHECK(List_next(pList) == &appended);
    CHECK(List_next(pList) == NULL);
    CHECK(List_next(pList) == NULL);

    // Prev through it all
    //   starting from past end
    CHECK(List_last(pList) == &appended);
    CHECK(List_next(pList) == NULL);
    CHECK(List_prev(pList) == &appended);
    CHECK(List_prev(pList) == &added);
    CHECK(List_prev(pList) == &inserted);
    CHECK(List_prev(pList) == &prepended);
    CHECK(List_prev(pList) == NULL);
    CHECK(List_prev(pList) == NULL);

    // Remove first
    CHECK(List_first(pList) == &prepended);
    CHECK(List_remove(pList) == &prepended);
    CHECK(List_curr(pList) == &inserted);

    // Trim last
    CHECK(List_trim(pList) == &appended);
    CHECK(List_curr(pList) == &added);

    // Free remaining 2 elements
    complexTestFreeCounter = 0;
    List_free(pList, complexTestFreeFn);
    CHECK(complexTestFreeCounter == 2);

    // Concat
    int one = 1;
    int two = 2;
    int three = 3;
    int four = 4;
    List* pList1 = List_create();
    List_add(pList1, &one);
    List_add(pList1, &two);
    List* pList2 = List_create();
    List_add(pList2, &three);
    List_add(pList2, &four);
    
    List_concat(pList1, pList2);
    CHECK(List_count(pList1) == 4);
    CHECK(List_first(pList1) == &one);
    CHECK(List_last(pList1) == &four);


    // Search
    List_first(pList1);
    CHECK(List_search(pList1, itemEquals, &two) == &two);
    CHECK(List_search(pList1, itemEquals, &two) == &two);
    CHECK(List_search(pList1, itemEquals, &one) == NULL);


    //Free remaining 4 elements

    complexTestFreeCounter = 0;
    List_free(pList, complexTestFreeFn);
    CHECK(complexTestFreeCounter == 4);
    




    //MAX LISTS = 3
    //MAX NODES = 12







    //testing trying to create more lists than there are.
    pList1 = List_create();
    CHECK(pList1 != NULL);

    pList2 = List_create();
    CHECK(pList2 != NULL);

    List* pList3 = List_create();
    CHECK(pList3 != NULL);//should equal NULL since there are no free lists

    List* pList4 = List_create();
    CHECK(pList4 == NULL);//should equal NULL since there are no free lists
    

    int five = 5;
    int six = 6;
    int seven = 7;
    int eight = 8;
    int dummy = 123921;
    //checking if add, insert, append , prepend works
    Node* temp;
    temp = pList1->header;//list is empty rn so current is the header
    CHECK(List_add(pList1,&one) == 0);
    CHECK(temp->next->item == List_curr(pList1));
    CHECK(List_count(pList1) == 1);
    CHECK(List_curr(pList1) == &one);

    //checking if insert works
    temp = pList->current;
    CHECK(List_insert(pList1, &two) == 0);
    CHECK(temp->prev->item == List_curr(pList1));
    CHECK(List_count(pList1) == 2);
    CHECK(List_curr(pList1) == &two);

    //checking if append works
    CHECK(List_append(pList1, &three) == 0);
    CHECK(List_count(pList1) == 3);
    CHECK(List_curr(pList1) == &three);
    CHECK(List_last(pList1) == &three);

    //checking if prepend works 
    CHECK(List_prepend(pList1, &four) == 0);
    CHECK(List_count(pList1) == 4);
    CHECK(List_curr(pList1) == &four);
    CHECK(List_first(pList1) == &four);

    //testing list behaviour when all nodes are used

    CHECK(List_prepend(pList1, &five)== 0);
    CHECK(List_prepend(pList1, &six) == 0);//list should be full now
    CHECK(List_count(pList1) == 6);

    CHECK(List_add(pList1,&dummy) == -1);//add,insert,append,prepend should all fail
    CHECK(List_insert(pList1,&dummy) == -1);
    CHECK(List_append(pList1,&dummy) == -1);
    CHECK(List_prepend(pList1,&dummy) == -1);

    CHECK(List_add(pList2,&dummy) == -1);//add,insert,append,prepend should all fail
    CHECK(List_insert(pList2,&dummy) == -1);
    CHECK(List_append(pList2,&dummy) == -1);
    CHECK(List_prepend(pList2,&dummy) == -1);

    CHECK(List_first(pList1) == &six);//current is now at front of the list
    CHECK(List_next(pList1) == &five);//iterateing through list
    CHECK(List_next(pList1) == &four);
    CHECK(List_next(pList1) == &two);
    CHECK(List_next(pList1) == &one);
    CHECK(List_next(pList1) == &three);//at end of list
    CHECK(List_next(pList1) == NULL);//at end of list, current should be trailer, and list_next should return NULL
    CHECK(List_curr(pList1) == pList1->trailer->item);

    ///time to go the other way now
    CHECK(List_prev(pList1) == &three);
    CHECK(List_prev(pList1) == &one);
    CHECK(List_prev(pList1) == &two);
    CHECK(List_prev(pList1) == &four);
    CHECK(List_prev(pList1) == &five);
    CHECK(List_prev(pList1) == &six);
    CHECK(List_prev(pList1) == NULL);//at beginning of list, current should be header, and list_prev should return NULL
    CHECK(List_curr(pList1) == pList1->header->item);

    //test concat next with empty list, plist1 should be unchanged.
    List_concat(pList1, pList2);
    CHECK(List_count(pList1) == 6);
    CHECK(List_first(pList1) == &six);
    CHECK(List_last(pList1) == &three);
    CHECK(pList2->header == NULL);//if the lists header = NULL this means that the list isnt in use

    //test concat with empty list and but the other way around now so plist2 will become a full list and pList1 is not in use
    pList2 = List_create();
    List_concat(pList2,pList1);
    CHECK(List_count(pList2) == 6);
    CHECK(List_first(pList2) == &six);
    CHECK(List_last(pList2) == &three);
    CHECK(pList1->header == NULL);//if the lists header = NULL this means that the list isnt in use
    
    //making the list full and using all possible nodes
    //checking if append works
    CHECK(List_append(pList2, &seven) == 0);
    CHECK(List_count(pList2) == 7);
    CHECK(List_curr(pList2) == &seven);
    CHECK(List_last(pList2) == &seven);

    CHECK(List_append(pList2, &eight) == 0);
    CHECK(List_count(pList2) == 8);
    CHECK(List_curr(pList2) == &eight);
    CHECK(List_last(pList2) == &eight);
    CHECK(List_create() == NULL);//should equal NULL now that there are no more nodes available even though a free list is available

    CHECK(List_first(pList2) == &six);//setting current to front of List
    
    //testing listremove
    CHECK(List_remove(pList2) == &six);
    CHECK(List_remove(pList2) == &five);//removing all elements
    CHECK(List_remove(pList2) == &four);
    CHECK(List_remove(pList2) == &two);
    CHECK(List_remove(pList2) == &one);
    CHECK(List_remove(pList2) == &three);
    CHECK(List_remove(pList2) == &seven);
    CHECK(List_remove(pList2) == &eight);//at end of list
    CHECK(pList2->trailer == pList2->current);//current should be at the trailer node
    CHECK(List_remove(pList2) == NULL);//we are beyond end of list so should return NULL
    CHECK(List_count(pList2) == 0);// list is empty and thus its size should be 0
    
    //freeing the list
    complexTestFreeCounter = 0;
    List_free(pList2, complexTestFreeFn);
    CHECK(complexTestFreeCounter == 0);
    CHECK(pList2->header == NULL);//list is not in use
    
//making 2 lists with 3 items each , where they share an item
    pList1 = List_create();
    pList2 = List_create();
    CHECK(pList1 != NULL);
    CHECK(pList2 != NULL);

    CHECK(List_append(pList2, &one) == 0);
    CHECK(List_count(pList2) == 1);
    CHECK(List_curr(pList2) == &one);
    CHECK(List_last(pList2) == &one);
    CHECK(List_append(pList2, &two) == 0);
    CHECK(List_count(pList2) == 2);
    CHECK(List_curr(pList2) == &two);
    CHECK(List_last(pList2) == &two);
    CHECK(List_append(pList2, &three) == 0);
    CHECK(List_count(pList2) == 3);
    CHECK(List_curr(pList2) == &three);
    CHECK(List_last(pList2) == &three);

    CHECK(List_append(pList1, &four) == 0);
    CHECK(List_count(pList1) == 1);
    CHECK(List_curr(pList1) == &four);
    CHECK(List_last(pList1) == &four);
    CHECK(List_append(pList1, &five) == 0);
    CHECK(List_count(pList1) == 2);
    CHECK(List_curr(pList1) == &five);
    CHECK(List_last(pList1) == &five);
    CHECK(List_append(pList1, &three) == 0);
    CHECK(List_count(pList1) == 3);
    CHECK(List_curr(pList1) == &three);
    CHECK(List_last(pList1) == &three);

    //test search comparison
    CHECK(List_first(pList1) == &four);
    CHECK(List_search(pList1, itemEquals, &four) == &four);
    CHECK(List_search(pList1, itemEquals, &five) == &five);
    CHECK(List_search(pList1, itemEquals, &one) == NULL);

    CHECK(List_first(pList1) == &four);
    CHECK(List_first(pList2) == &one);
    CHECK(List_search(pList1,itemEquals,&three) == List_search(pList2, itemEquals,&three));//shoulb be the same since they both contain the same item

    //test list trim
    CHECK(List_trim(pList2) == &three);
    CHECK(List_trim(pList2) == &two);
    CHECK(List_trim(pList2) == &one);
    CHECK(List_trim(pList2) == NULL);//should equal NULL no more items to trim
    CHECK(List_first(pList2) == NULL);//list is empty should return NULL
    CHECK(List_last(pList2) == NULL);//list is empty should return NULL
    CHECK(pList2->header->next == pList2->trailer && pList2->size == 0);//since list is empty from header next node should be trailer and previous node from the trailer should be the header

    //freeing up plist1
    complexTestFreeCounter = 0;
    List_free(pList1, complexTestFreeFn);
    CHECK(complexTestFreeCounter == 3);
    
    //freeing up pList2
    complexTestFreeCounter = 0;
    List_free(pList2, complexTestFreeFn);
    CHECK(complexTestFreeCounter == 0);

    //freeing up pList3
    complexTestFreeCounter = 0;
    List_free(pList3, complexTestFreeFn);
    CHECK(complexTestFreeCounter == 0);

    

    //list trim with the resutliting list of concat
    //add all nodes into one list then
    //test search comparison
    //test list free again

}

int main(int argCount, char *args[]) 
{
    testComplex();

    // We got here?!? PASSED!
    printf("********************************\n");
    printf("           PASSED\n");
    printf("********************************\n");
    return 0;
}