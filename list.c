// List data type
// You may modify this file as needed; however,
// you may *NOT* modify the function prototypes or constant names.
//
// UPDATED MAY 19; search for "UPDATED"
#include "list.h"
#include <stdio.h>
#include <assert.h>

static bool firstCall= true;//keeps track if it's listCreate first time being called
static List Lists[LIST_MAX_NUM_HEADS];//pool of lists
static Node Nodes[LIST_MAX_NUM_NODES];//pool of Nodes
static Node* tracker = &Nodes[0];//the head of the singly linked list that tracks the available nodes


// General Error Handling:
// Client code is assumed never to call these functions with a NULL List pointer, or 
// bad List pointer. If it does, any behaviour is permitted (such as crashing).
// HINT: Use assert(pList != NULL); just to add a nice check, but not required.

// Makes a new, empty list, and returns its reference on success. 
// Returns a NULL pointer on failure.
List* List_create(){
    if(LIST_MAX_NUM_NODES==1){//if only 1 node fail, need at least 2 free nodes for a list
        return NULL;
    }

    else if(firstCall || Lists[0].header == NULL){ //if Lists[0] is available prioritize it above all other available lists
        if(firstCall){// if first time List_create is being called do
            for(int i = 0; i < LIST_MAX_NUM_NODES;i++){//setting up all nodes and lists
                if(i==LIST_MAX_NUM_NODES-1){
                    Nodes[i].next = NULL;//last element in the linked list should point to NULL
                    Lists[LIST_MAX_NUM_HEADS-1].freeList = NULL;//last element in the linked list should point to NULL
                }

                else if(i < LIST_MAX_NUM_HEADS){
                    Nodes[i].next = &Nodes[i+1];//making a singly linked list of nodes, 
                    Lists[i].freeList = &Lists[i+1];//making a singly linked list of Lists, 
                    Lists[i].size = 0;//Lists are all empty
                    Lists[i].header = NULL;// if List.header = NULL means list isnt being used
                }

                else{
                    Nodes[i].next = &Nodes[i+1];//making a single linked list, 
                }
            }
        }

        if(tracker == NULL || tracker->next == NULL){ // ensuring there are 2 Nodes available for a new List
            return NULL;
        }

        firstCall = false;//setting firstCall to false as List_create has been called before 
        Lists[0].header = tracker;//assigning sentinel node
        tracker = tracker->next;//updating the next free node
        Lists[0].trailer = tracker;//assigning sentinel node
        tracker = tracker->next;//updating the next free node 
        Lists[0].header->next = Lists[0].trailer;//having header point to trailer
        Lists[0].trailer->prev = Lists[0].header;//having trailer point to header
        Lists[0].trailer->next = NULL;//disconnecting the node from the free node list.
        Lists[0].current = Lists[0].header;//assigning current pointer to header of the list
        return &Lists[0];//return address of the newly created List
    }
    else{
        if(tracker == NULL || (Lists[0].freeList == NULL && Lists[0].header != NULL) || tracker->next == NULL ){//checking if there is enough free lists/nodes 
            return NULL; 
        }

        else{
            Lists[0].freeList->header = tracker;//assigning sentinel node
            tracker = tracker->next;//updating the next free node
            Lists[0].freeList->trailer = tracker;//assigning sentinel node
            tracker = tracker->next;//updating the next free node 
            Lists[0].freeList->header->next = Lists[0].freeList->trailer;//having header point to trailer
            Lists[0].freeList->trailer->prev = Lists[0].freeList->header;//having trailer point to header
            Lists[0].freeList->trailer->next = NULL;//disconnecting the node from the free node list.
            Lists[0].freeList->current = Lists[0].freeList->header;//assigning current pointer to header of the list
            List* temp = Lists[0].freeList;// creating the pointer to return.
            Lists[0].freeList = Lists[0].freeList->freeList;//updating the next freeList
            return temp;//returning address/pointer of the newly created List
        }
    }
}

// Returns the number of items in pList.
int List_count(List* pList){
    assert(pList != NULL && pList->header != NULL);//crashes program if a null pointer or if a list which isn't being used is passed into function
    return pList->size;//return size of List
}

// Returns a pointer to the first item in pList and makes the first item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_first(List* pList){
    assert(pList != NULL && pList->header != NULL); //crashes program if a null pointer or if a list which isn't being used is passed into function
    if(pList->size == 0 && pList->header->next == pList->trailer){//checking if list is empty
        pList->current->item = NULL;//set current item to NULL
        return NULL;//return null pointer
    }

    else{
        pList->current = pList->header->next;//setting current node to first node
        return pList->current->item;//returning current item
    }
}

// Returns a pointer to the last item in pList and makes the last item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_last(List* pList){
    assert(pList != NULL && pList->header != NULL); //crashes program if a null pointer or if a list which isn't being used is passed into function
    if(pList->size == 0 && pList->header->next == pList->trailer){//checking if list is empty
        pList->current->item = NULL;//set current item to NULL
        return NULL;//return null pointer
    }

    else{
        pList->current = pList->trailer->prev;//setting current node to last node
        return pList->current->item;//returning current item
    }
}

// Advances pList's current item by one, and returns a pointer to the new current item.
// If this operation advances the current item beyond the end of the pList, a NULL pointer 
// is returned and the current item is set to be beyond end of pList.
void* List_next(List* pList){
    assert(pList != NULL && pList->header != NULL); //crashes program if a null pointer or if a list which isn't being used is passed into function
    
    if(pList->current == pList->trailer->prev || pList->current == pList->trailer){//checking if current is at the end/beyond of list
        pList->current = pList->trailer;//advance/stay current beyond the end of pList.
        return NULL;//return NULL pointer
    }

    else{
        pList->current = pList->current->next;//advance current pointer by one.
        return pList->current->item;//return item in current node.
    }
}

// Backs up pList's current item by one, and returns a pointer to the new current item. 
// If this operation backs up the current item beyond the start of the pList, a NULL pointer 
// is returned and the current item is set to be before the start of pList.
void* List_prev(List* pList){
    assert(pList != NULL && pList->header != NULL); //crashes program if a null pointer or if a list which isn't being used is passed into function

    if(pList->current == pList->header->next || pList->current == pList->header){//checking if current is at front/before of list
        pList->current = pList->header;//advance current beyond the beginning of pList.
        return NULL;//return NULL pointer
    }

    else{
        pList->current = pList->current->prev;//advance  in backwards direction current pointer by one.
        return pList->current->item;//return item in current node.
    }
}

// Returns a pointer to the current item in pList.
void* List_curr(List* pList){
    assert(pList != NULL && pList->header != NULL); //crashes program if a null pointer or if a list which isn't being used is passed into function
    return pList->current->item;//return current item
}

// Adds the new item to pList directly after the current item, and makes item the current item. 
// If the current pointer is before the start of the pList, the item is added at the start. If 
// the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_add(List* pList, void* pItem){
    assert(pList != NULL && pList->header != NULL); //crashes program if a null pointer or if a list which isn't being used is passed into function

    if(tracker == NULL ){//checking if nodes are available
        return -1;
    }

    else if(pList->current == pList->header){
        Node* temp = tracker;//creating pointer to next free node
        temp->item = pItem;//assigning item to the free node
        tracker= tracker->next;//updating the next free node

        temp->next = pList->header->next;//linking in the new node with the node that comes after it
        pList->header->next->prev = temp;//linking in the new node with the node that comes after it

        pList->header->next = temp;//linking in the new node with the node that comes before it
        temp->prev = pList->header;//linking in the new node with the node that comes before it
        pList->size++;//updating list size
        pList->current = temp;//updating current
        return 0;
    }

    else if(pList->current == pList->trailer){
        Node* temp = tracker;//creating pointer to next free node
        temp->item = pItem;//assigning item to the free node
        tracker = tracker->next;//updating the next free node

        temp->prev = pList->trailer->prev;//linking in the new node with the node that comes before it
        pList->trailer->prev->next = temp;//linking in the new node with the node that comes before it

        pList->trailer->prev = temp;//linking in the new node with the node that comes after it
        temp->next = pList->trailer;//linking in the new node with the node that comes after it
        pList->size++;//updating list size
        pList->current = temp;//updating current
        return 0;
        
    }

    else{
        Node* temp = tracker;//creating pointer to next free node
        temp->item = pItem;//assigning item to the free node
        tracker = tracker->next;//updating the next free node

        temp->next = pList->current->next;//linking in the new node with the node that comes after it
        pList->current->next->prev = temp;//linking in the new node with the node that comes after it

        pList->current->next = temp;//linking in the new node with the node that comes before it
        temp->prev = pList->current;//linking in the new node with the node that comes before it
        pList->size++;//updating list size
        pList->current = temp;//updating current
        return 0;
    }
}

// Adds item to pList directly before the current item, and makes the new item the current one. 
// If the current pointer is before the start of the pList, the item is added at the start. 
// If the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_insert(List* pList, void* pItem){
    assert(pList != NULL && pList->header != NULL);//crashes program if a null pointer or if a list which isn't being used is passed into function

    if(tracker == NULL){//checking if nodes are available
        return -1;
    }

    else if(pList->current == pList->header){
        Node* temp = tracker;//creating pointer to next free node
        temp->item = pItem;//assigning item to the free node
        tracker = tracker->next;//updating the next free node

        temp->next = pList->header->next;//linking in the new node with the node that comes after it
        pList->header->next->prev = temp;//linking in the new node with the node that comes after it

        pList->header->next = temp;//linking in the new node with the node that comes before it
        temp->prev = pList->header;//linking in the new node with the node that comes before it
        pList->size++;//updating list size
        pList->current = temp;//updating current
        return 0;
    }

    else if(pList->current == pList->trailer){
        Node* temp = tracker;//creating pointer to next free node
        temp->item = pItem;//assigning item to the free node
        tracker = tracker->next;//updating the next free node

        temp->prev = pList->trailer->prev;//linking in the new node with the node that comes before it
        pList->trailer->prev->next = temp;//linking in the new node with the node that comes before it

        pList->trailer->prev = temp;//linking in the new node with the node that comes after it
        temp->next = pList->trailer;//linking in the new node with the node that comes after it
        pList->size++;//updating list size
        pList->current = temp;//updating current
        return 0;
    }

    else{
        Node* temp = tracker;//creating pointer to next free node
        temp->item = pItem;//assigning item to the free node
        tracker = tracker->next;//updating the next free node

        temp->prev = pList->current->prev;//linking in the new node with the node that comes before it
        pList->current->prev->next = temp;//linking in the new node with the node that comes before it

        pList->current->prev = temp;//linking in the new node with the node that comes after it
        temp->next = pList->current;//linking in the new node with the node that comes after it
        pList->size++;//updating list size
        pList->current = temp;//updating current
        return 0;
    }
}
// Adds item to the end of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_append(List* pList, void* pItem){
    assert(pList != NULL && pList->header != NULL);//crashes program if a null pointer or if a list which isn't being used is passed into function 

    if(tracker == NULL){//checking if nodes are available
            return -1;
        }

    else{
        Node* temp = tracker;//creating pointer to next free node
        temp->item = pItem;//assigning item to the free node
        tracker = tracker->next;//updating the next free node

        temp->prev = pList->trailer->prev;//linking in the new node with the node that comes before it
        pList->trailer->prev->next = temp;//linking in the new node with the node that comes before it

        pList->trailer->prev = temp;//linking in the new node with the node that comes after it
        temp->next = pList->trailer;//linking in the new node with the node that comes after it
        pList->size++;//updating list size
        pList->current = temp;//updating current
        return 0;
    }
}
// Adds item to the front of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_prepend(List* pList, void* pItem){
    assert(pList != NULL && pList->header != NULL);//crashes program if a null pointer or if a list which isn't being used is passed into function

    if(tracker == NULL){//checking if nodes are available
            return -1;
        }

    else{
        Node* temp = tracker;//creating pointer to next free node
        temp->item = pItem;//assigning item to the free noden
        tracker = tracker->next;//updating the next free node

        temp->next = pList->header->next;//linking in the new node with the node that comes after it
        pList->header->next->prev = temp;//linking in the new node with the node that comes after it

        pList->header->next = temp;//linking in the new node with the node that comes before it
        temp->prev = pList->header;//linking in the new node with the node that comes before it
        pList->size++;//updating list size
        pList->current = temp;//updating current
        return 0;
    }
}

// Return current item and take it out of pList. Make the next item the current one.
// If the current pointer is before the start of the pList, or beyond the end of the pList,
// then do not change the pList and return NULL.
void* List_remove(List* pList){
    assert(pList != NULL && pList->header != NULL);//crashes program if a null pointer or if a list which isn't being used is passed into function 

    if((pList->size == 0 && pList->header->next == pList->trailer) || pList->current == pList->header || pList->current == pList->trailer){//checking if list/current is empty or beyond the list
        return NULL;
    }

    else{
        Node* temp = pList->current->next;//saving the location of the node after current node
        pList->current->prev->next = pList->current->next;//linking out current node
        pList->current->next->prev = pList->current->prev;//linking out current node
        
        pList->current->next = tracker;//adding the node back into the single linked list of free nodes 
        tracker = pList->current;//adding the node back into the single linked list of free nodes 

        pList->size--;//updating list size
        pList->current = temp;//updating current
        return tracker->item;//returning the item after it was taken out of list
    }
}

// Adds pList2 to the end of pList1. The current pointer is set to the current pointer of pList1. 
// pList2 no longer exists after the operation; its head is available
// for future operations.
void List_concat(List* pList1, List* pList2){
    assert(pList1 != NULL || pList1->header == NULL || pList2 != NULL || pList2->header == NULL );//crashes program if a null pointer or if a list which isn't being used is passed into function 
    
    Node* temp = pList1->trailer;//saving the trailer node of list1
    

    pList2->header->next->prev = pList1->trailer->prev;//connecting the beginning of list2 to the end of list1
    pList1->trailer->prev->next = pList2->header->next;//connecting the end of list one to the beginning of the list2
    pList1->trailer = pList2->trailer;//making trailer of list2 also now be trailer of list one

    temp->next = tracker;//adding list1s old trailer node into the list of free nodes
    pList2->header->next = temp;//adding list2s header node into the list of free nodes
    tracker = pList2->header;//updating the beginning of the free node list



    pList1->size = pList1->size + pList2->size;//updating list1s new size
    pList2->size = 0;//updating size

    if(pList2 != &Lists[0]){//special condition if pList2 is &List[0]
        pList2->freeList = Lists[0].freeList;//making pList2 available and adding it into the list of free List structs
        Lists[0].freeList = pList2;//making beginning of available list structs be pList2
        pList2->header = NULL;//condition that tells us list is not in use 
    }
    else{
        pList2->header = NULL;//condition that tells us list is not in use 
    }
}

// Delete pList. pItemFreeFn is a pointer to a routine that frees an item. 
// It should be invoked (within List_free) as: (*pItemFreeFn)(itemToBeFreedFromNode);
// pList and all its nodes no longer exists after the operation; its head and nodes are 
// available for future operations.
// UPDATED: Changed function pointer type, May 19
typedef void (*FREE_FN)(void* pItem);
void List_free(List* pList, FREE_FN pItemFreeFn){
    assert(pList != NULL && pList->header != NULL); //crashes program if a null pointer or if a list which isn't being used is passed into function

    pList->current = pList->header->next;//making sure current is at front of list

    while(pList->size > 0){//runs till list is empty
       (*pItemFreeFn)(List_remove(pList));//List_remove removes node from list. Returned value gets passed to the routine
    }

    pList->trailer->next = tracker;//adding plist trailer into list of free nodes
    pList->header->next = pList->trailer;//adding pList header into list of free nodes
    tracker = pList->header;//updating tracker making sure it points at front of free node list

    if(pList != &Lists[0]){//checking if special condition pList = &List[0]
        if(Lists[0].freeList == NULL ){//if there are no other free lists in the list special condition
            pList->freeList = NULL;//have to set to null because it might be pointing to another list which is in use
            Lists[0].freeList = pList;//add list into list of free lists.
            pList->header = NULL;
        }
        
        else{
            pList->freeList = Lists[0].freeList;//making pList available and adding it into the list of free List structs
            Lists[0].freeList = pList;//making pList available and adding it into the list of free List structs
            pList->header = NULL;//setting header to null to indicate list not in use
        }
    }
    else{
        pList->header = NULL;//seting list[0].header to null so next time we create a list it will use list[0]
    }
}

// Return last item and take it out of pList. Make the new last item the current one.
// Return NULL if pList is initially empty.
void* List_trim(List* pList){
    assert(pList != NULL && pList->header != NULL); //crashes program if a null pointer or if a list which isn't being used is passed into function
    if(pList->size == 0 && pList->header->next == pList->trailer){
        return NULL;
    }

    else{
        pList->current = pList->trailer->prev;
        Node* temp = pList->current->prev;//saving the location of the node after current node

        pList->current->prev->next = pList->trailer;//linking out current node
        pList->trailer->prev = pList->current->prev;//linking out current node
        
        pList->current->next = tracker;//adding the node back into the single linked list of free nodes 
        tracker = pList->current;//adding the node back into the single linked list of free nodes 

        pList->size--;//updating list size
        pList->current = temp;//updating current
        return tracker->item;//return item
    }
}

// Search pList, starting at the current item, until the end is reached or a match is found. 
// In this context, a match is determined by the comparator parameter. This parameter is a
// pointer to a routine that takes as its first argument an item pointer, and as its second 
// argument pComparisonArg. Comparator returns 0 if the item and comparisonArg don't match, 
// or 1 if they do. Exactly what constitutes a match is up to the implementor of comparator. 
// 
// If a match is found, the current pointer is left at the matched item and the pointer to 
// that item is returned. If no match is found, the current pointer is left beyond the end of 
// the list and a NULL pointer is returned.
// 
// UPDATED: Added clarification of behaviour May 19
// If the current pointer is before the start of the pList, then start searching from
// the first node in the list (if any).
typedef bool (*COMPARATOR_FN)(void* pItem, void* pComparisonArg);
void* List_search(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg){
    assert(pList != NULL && pList->header != NULL); //crashes program if a null pointer or if a list which isn't being used is passed into function
    if((pList->size == 0 && pList->header->next == pList->trailer)|| pList->current == pList->trailer){//checking if list is empty or current is beyond the end of the list
        return NULL;//returns null
    }

    else{
        if(pList->current != pList->header){//checking if current node does not equal header node
            if((*pComparator)(pList->current->item,pComparisonArg) == 1){// checking if it is a match
                return pList->current->item;//if it is a match return the item
            }      
        }

        while(pList->current->next != pList->trailer){//iterates through the list, stops at the last node if no matches are found
            if((*pComparator)(List_next(pList),pComparisonArg) == 1){//checking if its a match
                return pList->current->item;//if match return the item
            }
        }

        pList->current = pList->trailer;// if no matches were found, update current to beyond the end of the list
        return NULL;//return null since no matches were found
    
    }

}

