#include <stddef.h>
#include "ll_cycle.h"

int ll_has_cycle(node *head) {
    /* your code here */
    node *fast = head;
    node *slow = head;

    while(fast != NULL)
    {
	slow = slow->next;
	fast = fast->next;
	if(fast == NULL)
	{
	    break;
	}
	fast  = fast->next;
	
	if(fast == slow)
	{
	    return 1;
	}
    }

    return 0;
}
