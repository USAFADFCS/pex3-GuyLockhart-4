/** pagequeue.c
 * ===========================================================
 * Name: Will Lockhart
 * Section: CS483 / M3
 * Project: PEX3 - Page Replacement Simulator
 * Purpose: Implementation of the PageQueue ADT — a doubly-linked
 *          list for LRU page replacement.
 *          Head = LRU (eviction end), Tail = MRU end.
 * =========================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pagequeue.h"

/**
 * @brief Create and initialize a page queue with a given capacity
 */
PageQueue *pqInit(unsigned int maxSize) {
    // TODO: malloc a PageQueue, set head and tail to NULL,
    //       size to 0, maxSize to maxSize, and return the pointer
    PageQueue* list = (PageQueue*)malloc(sizeof(PageQueue));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->maxSize = maxSize;

    return list;
}

/**
 * @brief Access a page in the queue (simulates a memory reference)
 */
long pqAccess(PageQueue *pq, unsigned long pageNum) {
    // TODO: Search the queue for pageNum (suggest searching tail->head
    //       so you naturally count depth from the MRU end).
    PqNode* currentNode = pq->tail;
    long depth = 0;
    while (currentNode != NULL){
        if (currentNode->pageNum == pageNum){
            break;
        }
        depth++;
        currentNode = currentNode->prev;
        
    }

    // HIT path (page found at depth d):
    //   - Remove the node from its current position and re-insert
    //     it at the tail (most recently used).
    //   - Return d.
    if (currentNode != NULL){
        if (depth != 0){
            // Removing from current place
            PqNode* prevNode = currentNode->prev;
            PqNode* nextNode = currentNode->next;
            if (currentNode == pq->head){
                pq->head = pq->head->next;
            }

            if (prevNode != NULL){
                prevNode->next = nextNode;
            }
            if (nextNode != NULL){
                nextNode->prev = prevNode;
            }
            currentNode->next = NULL;
            currentNode->prev = NULL;

            // Add to tail
            PqNode* oldTail = pq->tail;
            oldTail->next = currentNode;
            currentNode->prev = oldTail;
            pq->tail = currentNode;
        }
        
    }
    
    // MISS path (page not found):
    //   - Allocate a new node for pageNum and insert it at the tail.
    //   - If size now exceeds maxSize, evict the head node (free it).
    //   - Return -1.
    else{
        depth = -1;
        PqNode* newNode = (PqNode*)malloc(sizeof(PqNode));
        newNode->pageNum = pageNum;
        newNode->next = NULL;
        newNode->prev = NULL;

        if (pq->size == 0){
            pq->head = newNode;
            pq->tail = newNode;
        }
        else {
            PqNode* oldTail = pq->tail;
            oldTail->next = newNode;
            newNode->prev = oldTail;
            pq->tail = newNode;
        }


        if (pq->size < pq->maxSize){
            pq->size++;
        }
        else{
            PqNode* oldHead = pq->head;
            pq->head = oldHead->next;
            pq->head->prev = NULL;
            free(oldHead);
        }
   
    }
    return depth;
}

/**
 * @brief Free all nodes in the queue and reset it to empty
 */
void pqFree(PageQueue *pq) {
    // TODO: Walk from head to tail, free each node, then free
    //       the PageQueue struct itself.
    PqNode* currentNode = pq->head;

    while (currentNode != NULL){
        PqNode* nodeToDelete = currentNode;
        currentNode = currentNode->next;
        free(nodeToDelete);
    }

    free(pq);
}

/**
 * @brief Print queue contents to stderr for debugging
 */
void pqPrint(PageQueue *pq) {
    // TODO (optional): Print each page number from head to tail,
    //                  marking which is head and which is tail.
    //                  Useful for desk-checking small traces.

    fprintf(stderr, "Head ");
    PqNode* currentNode = pq->head;

    while (currentNode != NULL){
        fprintf(stderr, "%lu ", currentNode->pageNum);
        currentNode = currentNode->next;
    }
    fprintf(stderr, "Tail\n");
}
