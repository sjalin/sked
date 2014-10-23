#include "sched.h"
#include "stuff.h"

#define MAX_PROCESSES	32u

#define uint32max 0xFFFFFFFFu

#define NULL 0
#define NO_EVENT 0

typedef struct scheduling_element_t scheduling_element_t;

typedef struct scheduling_element_t{
	void (*ptrFunction) (uint16);
	uint32 nextTime;
	uint32 intervall;
	uint16 arg;
	scheduling_element_t* next;
	scheduling_element_t* prev;
	uint16 prio;
} sched_elem_t;

typedef struct{
	scheduling_element_t processes[MAX_PROCESSES];
	scheduling_element_t* first;
	scheduling_element_t* last;	

   uint32 timeOfNextEvent;
} scheduling_list_t;

void ClearData(scheduling_element_t* element);

scheduling_list_t scheduling_list;
static uint16 runScheduler = 1; 

//TODO: rearrange file so that public functions are first and private are last
//TODO: doxygen all

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

   scheduling_list.first = &scheduling_list.processes[0];
   scheduling_list.last = &scheduling_list.processes[0];
   scheduling_list.processes[0].next = &scheduling_list.processes[1];
   scheduling_list.processes[MAX_PROCESSES-1].prev = &scheduling_list.processes[MAX_PROCESSES-2];
   for (i = 1; i < MAX_PROCESSES-1; i++){
      scheduling_list.processes[i].next = &scheduling_list.processes[i+1];
      scheduling_list.processes[i].prev = &scheduling_list.processes[i-1];
      ClearData(&scheduling_list.processes[i]);
   }
   scheduling_list.first->nextTime = uint32max;
   scheduling_list.last->nextTime = uint32max;
}


/**
 * @name          AddToList
 * @brief         Add an element to a specific list defined by first and last element
 * @param [in]    first       first element in sorted list to put entry
 * @param [in]    last        last element in sorted list to put entry
 * @param [in]    arg         entry->arg to put into sorted list between fist and last
 * @param [out]   None.
 * @return                    Valid scheduling returns defined in Scheduling.h
 */
int16 AddToList(scheduling_element_t** first, scheduling_element_t** last, scheduling_element_t entry)
{
   int16 returnValue = LIST_FULL;
   
   #warning: TODO: add process before if prio is more urgent

   scheduling_element_t* element = (*last)->next;
   scheduling_element_t* before = *first;
   if (NULL != element){
      if ((*first)->ptrFunction == NULL){
         (*first)->ptrFunction = entry.ptrFunction;
         (*first)->arg = entry.arg;
         (*first)->intervall = entry.intervall;
         (*first)->nextTime = entry.nextTime;
      }else{
         element->ptrFunction = entry.ptrFunction;
         element->arg = entry.arg;
         element->intervall = entry.intervall;
         element->nextTime = entry.nextTime;

         if (entry.nextTime >= (*last)->nextTime){
            (*last) = element;
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
int16 AddEvent( void (*functionPtr) (uint16), prio_t prio, uint32 after, cyclic_control_t cyclic, uint16 arg)//, uint16 arg)
{
   int16 returnValue = -1;
   scheduling_element_t entry = {functionPtr, GetCurrentTime()+after, (cyclic)?after:0, arg, NULL, NULL, prio};

   if (entry.nextTime < scheduling_list.timeOfNextEvent){
      scheduling_list.timeOfNextEvent = entry.nextTime;
   }

   returnValue = AddToList(&scheduling_list.first, &scheduling_list.last, entry);
   
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
   element->ptr;
   element->intervall = 0;
   element->nextTime = uint32max;
   element->arg = NO_EVENT;
   element->prio = 0; 
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

	#warning: implement or remove

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

	#warning remove or implemet

}

/**
 * @name          CalculateTimeOfNextEvent
 * @brief         Calculates the next time an event is meant to be triggered.
 * @param [in]    None.
 * @param [out]   None.
 * @return                    Time when next event should be run.
 */
uint32 CalculateTimeOfNextEvent(){
   uint32 time = scheduling_list.first->nextTime;
   //TODO: maybe calculate the process after that has higher prio to so that one can interrupt the fist process if it takes to long

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
		(*scheduling_list.first->ptrFunction) (scheduling_list.first->arg);
		if (scheduling_list.first->intervall){
			scheduling_element_t element = (*scheduling_list.first);
			element.nextTime += element.intervall;
			ClearFirst(&scheduling_list.first,&scheduling_list.last);
			AddToList(&scheduling_list.first, &scheduling_list.last, element);
		}else{
			ClearFirst(&scheduling_list.first,&scheduling_list.last);
		}
	}
	else{
		//Should not come here, maybe some error handling in case of.
		//Might come here in current version on timer overflow, no handling for that I think
	}
}

uint32 GetTimeOfNextEvent()
{
	return scheduling_list.timeOfNextEvent;  
}

void StartScheduler()
{
	while (runScheduler){
		ExecuteSchedulerRun(); 
		//sleep_enable();
		//SetNextWake(GetTimeOfNextEvent());
		//sleep_cpu();
		//sleep_disable();
	}
}

void StopScheduler()
{
	runScheduler = 0; 
}
