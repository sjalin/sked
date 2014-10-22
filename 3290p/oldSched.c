#include "sched.h"

#define BACKGROUND_QUEUE_LENGHT  16u
#define LOW_QUEUE_LENGHT         16u
#define MEDIUM_QUEUE_LENGHT      16u
#define HIGH_QUEUE_LENGHT        5u
#define URGENT_QUEUE_LENGHT      5u

#define uint32max 0xFFFFFFFFu

#define NULL 0
#define NO_EVENT 0

typedef struct scheduling_element_t scheduling_element_t;

typedef struct scheduling_element_t{
   void (*ptrFunction) (int);
	uint32 nextTime;
	uint32 intervall;
	int event;
	scheduling_element_t* next;
	scheduling_element_t* prev;
} sched_elem_t;

typedef struct{
   scheduling_element_t background[BACKGROUND_QUEUE_LENGHT];
   scheduling_element_t low[LOW_QUEUE_LENGHT];
   scheduling_element_t medium[MEDIUM_QUEUE_LENGHT];
   scheduling_element_t high[HIGH_QUEUE_LENGHT];
   scheduling_element_t urgent[URGENT_QUEUE_LENGHT];
   scheduling_element_t* bFirst;
   scheduling_element_t* bLast;
   scheduling_element_t* lFirst;
   scheduling_element_t* lLast;
   scheduling_element_t* mFirst;
   scheduling_element_t* mLast;
   scheduling_element_t* hFirst;
   scheduling_element_t* hLast;
   scheduling_element_t* uFirst;
   scheduling_element_t* uLast;
   uint32 timeOfNextEvent;
} scheduling_list_t;

void ClearData(scheduling_element_t* element);

scheduling_list_t scheduling_list;


/**
 * @name          InitScheduler
 * @brief         Initiation of scheduler, must be run before using the scheduler
 * @param [in]    None.
 * @param [out]   None.
 * @retval        None.
 */
void InitScheduler()
{
   uint16 i;

   scheduling_list.bFirst = &scheduling_list.background[0];
   scheduling_list.bLast = &scheduling_list.background[0];
   scheduling_list.background[0].next = &scheduling_list.background[1];
   scheduling_list.background[BACKGROUND_QUEUE_LENGHT-1].prev = &scheduling_list.background[BACKGROUND_QUEUE_LENGHT-2];
   for (i = 1; i < BACKGROUND_QUEUE_LENGHT-1; i++){
      scheduling_list.background[i].next = &scheduling_list.background[i+1];
      scheduling_list.background[i].prev = &scheduling_list.background[i-1];
      ClearData(&scheduling_list.background[i]);
   }
   scheduling_list.bFirst->nextTime = uint32max;
   scheduling_list.bLast->nextTime = uint32max;

   scheduling_list.lFirst = &scheduling_list.low[0];
   scheduling_list.lLast = &scheduling_list.low[0];
   scheduling_list.low[0].next = &scheduling_list.low[1];
   scheduling_list.low[LOW_QUEUE_LENGHT-1].prev = &scheduling_list.low[LOW_QUEUE_LENGHT-2];
   for (i = 1; i < LOW_QUEUE_LENGHT-1; i++){
      scheduling_list.low[i].next = &scheduling_list.low[i+1];
      scheduling_list.low[i].prev = &scheduling_list.low[i-1];
      ClearData(&scheduling_list.low[i]);
   }
   scheduling_list.lFirst->nextTime = uint32max;
   scheduling_list.lLast->nextTime = uint32max;

   scheduling_list.mFirst = &scheduling_list.medium[0];
   scheduling_list.mLast = &scheduling_list.medium[0];
   scheduling_list.medium[0].next = &scheduling_list.medium[1];
   scheduling_list.medium[MEDIUM_QUEUE_LENGHT-1].prev = &scheduling_list.medium[MEDIUM_QUEUE_LENGHT-2];
   for (i = 1; i < MEDIUM_QUEUE_LENGHT-1; i++){
      scheduling_list.medium[i].next = &scheduling_list.medium[i+1];
      scheduling_list.medium[i].prev = &scheduling_list.medium[i-1];
      ClearData(&scheduling_list.medium[i]);
   }
   scheduling_list.mFirst->nextTime = uint32max;
   scheduling_list.mLast->nextTime = uint32max;

   scheduling_list.hFirst = &scheduling_list.high[0];
   scheduling_list.hLast = &scheduling_list.high[0];
   scheduling_list.high[0].next = &scheduling_list.high[1];
   scheduling_list.high[HIGH_QUEUE_LENGHT-1].prev = &scheduling_list.high[HIGH_QUEUE_LENGHT-2];
   for (i = 1; i < HIGH_QUEUE_LENGHT-1; i++){
      scheduling_list.high[i].next = &scheduling_list.high[i+1];
      scheduling_list.high[i].prev = &scheduling_list.high[i-1];
      ClearData(&scheduling_list.high[i]);
   }
   scheduling_list.hFirst->nextTime = uint32max;
   scheduling_list.hLast->nextTime = uint32max;

   scheduling_list.uFirst = &scheduling_list.urgent[0];
   scheduling_list.uLast = &scheduling_list.urgent[0];
   scheduling_list.urgent[0].next = &scheduling_list.urgent[1];
   scheduling_list.urgent[URGENT_QUEUE_LENGHT-1].prev = &scheduling_list.urgent[URGENT_QUEUE_LENGHT-2];
   for (i = 1; i < URGENT_QUEUE_LENGHT-1; i++){
      scheduling_list.urgent[i].next = &scheduling_list.urgent[i+1];
      scheduling_list.urgent[i].prev = &scheduling_list.urgent[i-1];
      ClearData(&scheduling_list.urgent[i]);
   }
   scheduling_list.uFirst->nextTime = uint32max;
   scheduling_list.uLast->nextTime = uint32max;
}


/**
 * @name          AddToList
 * @brief         Add an element to a specific list defined by first and last element
 * @param [in]    first       first element in sorted list to put entry
 * @param [in]    last        last element in sorted list to put entry
 * @param [in]    entry       entry-event to put into sorted list between fist and last
 * @param [out]   None.
 * @return                    Valid scheduling returns defined in Scheduling.h
 */
int16 AddToList(scheduling_element_t** first, scheduling_element_t** last, scheduling_element_t entry)
{
   int16 returnValue = LIST_FULL;

   scheduling_element_t* element = (*last)->next;
   scheduling_element_t* before = *first;
   if (NULL != element){
      if ((*first)->ptrFunction == NULL){
         (*first)->ptrFunction = entry.ptrFunction;
         (*first)->event = entry.event;
         (*first)->intervall = entry.intervall;
         (*first)->nextTime = entry.nextTime;
         //return;
      }else{
         element->ptrFunction = entry.ptrFunction;
         element->event = entry.event;
         element->intervall = entry.intervall;
         element->nextTime = entry.nextTime;

         if (entry.nextTime >= (*last)->nextTime){
            (*last) = element;
            //return;
         }else{
            (*last)->next = element->next;
            if (NULL != element->next){
               element->next->prev = (*last);
            }

            while(before->nextTime <= entry.nextTime){
              before = before->next;
            }
            element->next = before;
            element->prev = before->prev;
            if (NULL != element->prev){
               element->prev->next = element;
            }
            element->next->prev = element;
            if (before == (*first)){
               (*first) = element;
            }
         }
      }
      returnValue = OK;
   }
   return returnValue;
}

/**
 * @name          AddEvent
 * @brief         Add an event to a list of a specific priority
 * @param [in]    functionPtr Pointer to function that will handle the event.
 * @param [in]    prio        Priority of the list to add the event to.
 * @param [in]    after       The time from now to send the event.
 * @param [in]    cyclic      If the event should be cyclic or not.
 * @param [in]    event       The actual event.
 * @param [out]   None.
 * @return                    Valid scheduling returns defined in Scheduling.h
 */
int16 AddEvent( void (*functionPtr) (int), prio_t prio, uint32 after, int cyclic, int event)//, uint16 arg)
{
   int16 returnValue = -1;
   scheduling_element_t entry = {functionPtr, GetCurrentTime()+after, (cyclic)?after:0, event, /*arg,*/ NULL, NULL};

   if (entry.nextTime < scheduling_list.timeOfNextEvent){
      scheduling_list.timeOfNextEvent = entry.nextTime;
   }

   switch (prio){
      case BACKGROUND:
         returnValue = AddToList(&scheduling_list.bFirst, &scheduling_list.bLast, entry);
         break;
      case LOW:
         returnValue = AddToList(&scheduling_list.lFirst, &scheduling_list.lLast, entry);
         break;
      case MEDIUM:
         returnValue = AddToList(&scheduling_list.mFirst, &scheduling_list.mLast, entry);
         break;
      case HIGH:
         returnValue = AddToList(&scheduling_list.hFirst, &scheduling_list.hLast, entry);
         break;
      case URGENT:
         returnValue = AddToList(&scheduling_list.uFirst, &scheduling_list.uLast, entry);
         break;
      default:
         returnValue = INVALID_PRIO;
         break;
   }
   return returnValue;
}

/**
 * @name          ClearData
 * @brief         Clear all the data from a scheduling element, not pointers.
 * @param [in]    element     Scheduling element to clear the data from.
 * @param [out]   None.
 * @return        None.
 */
void ClearData(scheduling_element_t* element)
{
   element->ptrFunction = NULL;
   element->intervall = 0;
   element->nextTime = uint32max;
   element->event = NO_EVENT;
}


/**
 * @name          ClearFirst
 * @brief         Clear the fist element of list defined by fist and last and move it after last in the list.
 * @param [in]    first       Fist element in list.
 * @param [in]    last        Last occupied element in list.
 * @param [out]   None.
 * @return        None.
 */
void ClearFirst(scheduling_element_t** first, scheduling_element_t** last)
{
   ClearData(*first);

   if ((*first) != (*last)){
      (*first)->prev = (*last);
      (*first)->next->prev = NULL;

      if (NULL != (*last)->next){
         (*last)->next->prev = (*first);
         (*first) = (*first)->next;
         (*last)->next->prev->next = (*last)->next;//->prev;
         (*last)->next = (*last)->next->prev;
      }else{
         (*last)->next = (*first);
         (*first) = (*first)->next;
         (*last)->next->next = NULL;
      }

   }
}

/**
 * @name          ClearElement
 * @brief         Clear the selected element in the selected list (selected by prio) and move it after the last element in the list.
 * @param [in]    element     Element to clear.
 * @param [in]    prio        Prio of the list to clear the element from
 * @param [out]   None.
 * @return        None.
 */
void ClearElement(scheduling_element_t* element, prio_t prio){
   if (URGENT == prio){
      if (scheduling_list.uFirst == element){
         ClearFirst(&scheduling_list.uFirst, &scheduling_list.uLast);
      }else{
         ClearData(element);

         if (element == scheduling_list.uLast){
            scheduling_list.uLast = element->prev;
         }else{
            element->prev->next = element->next;
            element->next->prev = element->prev;


            element->next = scheduling_list.uLast->next;
            element->prev = scheduling_list.uLast;

            element->prev->next = element;
            element->next->prev = element;
         }
      }
   }else if (HIGH == prio){
      if (scheduling_list.hFirst == element){
         ClearFirst(&scheduling_list.hFirst, &scheduling_list.hLast);
      }else{
         ClearData(element);

         if (element == scheduling_list.hLast){
            scheduling_list.hLast = element->prev;
         }else{
            element->prev->next = element->next;
            element->next->prev = element->prev;

            element->next = scheduling_list.hLast->next;
            element->prev = scheduling_list.hLast;

            element->prev->next = element;
            element->next->prev = element;
         }
      }
   }else if (MEDIUM == prio){
      if (scheduling_list.mFirst == element){
         ClearFirst(&scheduling_list.mFirst, &scheduling_list.mLast);
      }else{
         ClearData(element);

         if (element == scheduling_list.mLast){
            scheduling_list.mLast = element->prev;
         }else{
            element->prev->next = element->next;
            element->next->prev = element->prev;

            element->next = scheduling_list.mLast->next;
            element->prev = scheduling_list.mLast;

            element->prev->next = element;
            element->next->prev = element;
         }
      }
   }else if (LOW == prio){
      if (scheduling_list.lFirst == element){
         ClearFirst(&scheduling_list.lFirst, &scheduling_list.lLast);
      }else{
         ClearData(element);

         if (element == scheduling_list.lLast){
            scheduling_list.lLast = element->prev;
         }else{
            element->prev->next = element->next;
            element->next->prev = element->prev;

            element->next = scheduling_list.lLast->next;
            element->prev = scheduling_list.lLast;

            element->prev->next = element;
            element->next->prev = element;
         }
      }
   }else if (BACKGROUND == prio){
      if (scheduling_list.bFirst == element){
         ClearFirst(&scheduling_list.bFirst, &scheduling_list.bLast);
      }else{
         ClearData(element);

         if (element == scheduling_list.bLast){
            scheduling_list.bLast = element->prev;
         }else{
            element->prev->next = element->next;
            element->next->prev = element->prev;

            element->next = scheduling_list.bLast->next;
            element->prev = scheduling_list.bLast;

            element->prev->next = element;
            element->next->prev = element;
         }
      }
   }
}

/**
 * @name          RemoveEvent
 * @brief         Remove all occurrences  of function and event from all lists.
 * @param [in]    functionPtr Pointer to the function corresponding  tho event to clear.
 * @param [in]    event       Event to clear when combined with function that functionPtr points to.
 * @param [out]   None.
 * @return        None.
 */
void RemoveEvent( void (*functionPtr) (int), int event){
   scheduling_element_t* element = scheduling_list.uFirst;

   while (element != NULL){
      if (element->ptrFunction == functionPtr && element->event == event){
         element = element->next;
         ClearElement(element->prev, URGENT);
      }
      else{
         element = element->next;
      }
   }
   element = scheduling_list.hFirst;
   while (element != NULL){
      if (element->ptrFunction == functionPtr && element->event == event){
         ClearElement(element, HIGH);
      }
      element = element->next;
   }
   element = scheduling_list.mFirst;
   while (element != NULL){
      if (element->ptrFunction == functionPtr && element->event == event){
         ClearElement(element, MEDIUM);
      }
      element = element->next;
   }
   element = scheduling_list.lFirst;
   while (element != NULL){
      if (element->ptrFunction == functionPtr && element->event == event){
         ClearElement(element, LOW);
      }
      element = element->next;
   }
   element = scheduling_list.bFirst;
   while (element != NULL){
      if (element->ptrFunction == functionPtr && element->event == event){
         ClearElement(element, BACKGROUND);
      }
      element = element->next;
   }

}

/**
 * @name          CalculateTimeOfNextEvent
 * @brief         Calculates the next time an event is meant to be triggered.
 * @param [in]    None.
 * @param [out]   None.
 * @return                    Time when next event should be run.
 */
uint32 CalculateTimeOfNextEvent(){
   uint32 time = scheduling_list.uFirst->nextTime;

   if (time > scheduling_list.hFirst->nextTime){
      time = scheduling_list.hFirst->nextTime;
   }
   if (time > scheduling_list.mFirst->nextTime){
      time = scheduling_list.mFirst->nextTime;
   }
   if (time > scheduling_list.lFirst->nextTime){
      time = scheduling_list.lFirst->nextTime;
   }
   if (time > scheduling_list.bFirst->nextTime){
      time = scheduling_list.bFirst->nextTime;
   }
   return time;
}

/**
 * @name          ExecuteSchedulerRun
 * @brief         Run one task from the scheduler, the task is selected first by prio then by time it should be run.
 * @param [in]    None.
 * @param [out]   None.
 * @return        None.
 */
void ExecuteSchedulerRun()
{
   if (scheduling_list.timeOfNextEvent <= GetCurrentTime()){
      if (scheduling_list.uFirst->nextTime <= GetCurrentTime() && scheduling_list.uFirst->ptrFunction != NULL){
         (*scheduling_list.uFirst->ptrFunction) (scheduling_list.uFirst->event);
         if (scheduling_list.uFirst->intervall){
            scheduling_element_t element = (*scheduling_list.uFirst);
            element.nextTime += element.intervall;
            //scheduling_list.uFirst->nextTime += scheduling_list.uFirst->intervall;
            ClearFirst(&scheduling_list.uFirst,&scheduling_list.uLast);
            AddToList(&scheduling_list.uFirst, &scheduling_list.uLast, element);
         }else{
            ClearFirst(&scheduling_list.uFirst,&scheduling_list.uLast);
         }
      }else if (scheduling_list.hFirst->nextTime <= GetCurrentTime() && scheduling_list.hFirst->ptrFunction != NULL){
         (*scheduling_list.hFirst->ptrFunction) (scheduling_list.hFirst->event);
         if (scheduling_list.hFirst->intervall){
            scheduling_element_t element = (*scheduling_list.hFirst);
            element.nextTime += element.intervall;
            //scheduling_list.uFirst->nextTime += scheduling_list.uFirst->intervall;
            ClearFirst(&scheduling_list.hFirst,&scheduling_list.hLast);
            AddToList(&scheduling_list.hFirst, &scheduling_list.hLast, element);
         }else{
            ClearFirst(&scheduling_list.hFirst,&scheduling_list.hLast);
         }
      }else if (scheduling_list.mFirst->nextTime <= GetCurrentTime() && scheduling_list.mFirst->ptrFunction != NULL){
         (*scheduling_list.mFirst->ptrFunction) (scheduling_list.mFirst->event);
         if (scheduling_list.mFirst->intervall){
            scheduling_element_t element = (*scheduling_list.mFirst);
            element.nextTime += element.intervall;
            //scheduling_list.uFirst->nextTime += scheduling_list.uFirst->intervall;
            ClearFirst(&scheduling_list.mFirst,&scheduling_list.mLast);
            AddToList(&scheduling_list.mFirst, &scheduling_list.mLast, element);
         }else{
            ClearFirst(&scheduling_list.mFirst,&scheduling_list.mLast);
         }
      }else if (scheduling_list.lFirst->nextTime <= GetCurrentTime() && scheduling_list.lFirst->ptrFunction != NULL){
         (*scheduling_list.lFirst->ptrFunction) (scheduling_list.lFirst->event);
         if (scheduling_list.lFirst->intervall){
            scheduling_element_t element = (*scheduling_list.lFirst);
            element.nextTime += element.intervall;
            //scheduling_list.uFirst->nextTime += scheduling_list.uFirst->intervall;
            ClearFirst(&scheduling_list.lFirst,&scheduling_list.lLast);
            AddToList(&scheduling_list.lFirst, &scheduling_list.lLast, element);
         }else{
            ClearFirst(&scheduling_list.lFirst,&scheduling_list.lLast);
         }
      }else if (scheduling_list.bFirst->nextTime <= GetCurrentTime() && scheduling_list.bFirst->ptrFunction != NULL){
         (*scheduling_list.bFirst->ptrFunction) (scheduling_list.bFirst->event);
         if (scheduling_list.bFirst->intervall){
            scheduling_element_t element = (*scheduling_list.bFirst);
            element.nextTime += element.intervall;
            //scheduling_list.uFirst->nextTime += scheduling_list.uFirst->intervall;
            ClearFirst(&scheduling_list.bFirst,&scheduling_list.bLast);
            AddToList(&scheduling_list.bFirst, &scheduling_list.bLast, element);
         }else{
            ClearFirst(&scheduling_list.bFirst,&scheduling_list.bLast);
         }
      }else{
         scheduling_list.timeOfNextEvent = CalculateTimeOfNextEvent();
      }
   }
   else{
   }
}
