//Name: LIOR SAPIR
//I.D. 304916562
//Login: liorsp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slist.h"


/* Initialize a single linked list
param list - the list to initialize */
void slist_init(slist_t * list)
{
	if(list == NULL)
		return;
	slist_head(list) = NULL;
	slist_tail(list) = NULL;
	slist_size(list) = 0;//set to 0
}
/** Destroy and de-allocate the memory hold by a list
param list - a pointer to an existing list param dealloc
flag that indicates whether stored data should also be de-allocated */
void slist_destroy(slist_t * list, slist_destroy_t flag){
	if(list == NULL)
		return;//input validaition
	if(flag != SLIST_FREE_DATA && flag != SLIST_LEAVE_DATA)
		return;//input validaition
	if(slist_size(list) == 0)
		return;//input validaition
	if(slist_head(list) == NULL)
		return;//input validaition
	if(slist_size(list) == 1)
	{
		if(flag == SLIST_FREE_DATA)//in case of char or struct
			free(slist_data(slist_head(list)));
		free(slist_head(list));
		slist_head(list) = NULL;
		slist_tail(list) = NULL;
		slist_size(list) = 0;
		return;
	}
	else
	{	//list is more then 1
		slist_node_t *temp = slist_head(list);
		while(slist_size(list) > 0)
		{
			if(flag == SLIST_FREE_DATA)//in case pf not int
				free(slist_data(temp));
			temp = slist_next(slist_head(list));
			free(slist_head(list));
			slist_head(list) = temp;
			slist_size(list)--;
		}
		slist_head(list) = NULL;
		slist_tail(list) = NULL;
		slist_size(list) = 0;
	}
}
/* Pop the first element in the list
param list - a pointer to a list return a pointer to the data
of the element, or NULL if the list is empty */
void *slist_pop_first(slist_t *list, int *port)
{
	if(list == NULL)//input validaition
	{
		fprintf(stderr, "List is null\n");
		return NULL;
	}
	if(slist_size(list) == 1)//in case of one node
	{
		void *temp = slist_data(slist_head(list));//the void
		*port = slist_port(slist_head(list));
		free(slist_head(list));
		slist_head(list) = NULL;
		slist_tail(list) = NULL;
		slist_size(list) = 0;
		return temp;
	}
	if(slist_size(list) > 1)
	{
		void *temp = slist_data(slist_head(list));//the void
		*port = slist_port(slist_head(list));
		slist_node_t *temp2 = slist_head(list);
		slist_head(list) = slist_next(slist_head(list));
		free(temp2);//free the pop one
		slist_size(list)--;
		return temp;
	}
	fprintf(stderr, "Eror: Empty list");
	return NULL;
}

int slist_append(slist_t *list,void *obj, int port)
{
	if(obj == NULL)//input validaition
	{
		fprintf(stderr, "Append: Ilegal operation on list");
		return -1;
	}
	if(list == NULL)//input validaition
	{
		fprintf(stderr, "Append: Ilegal operation on list");
		return -1;
	}

	if(slist_size(list) == 0)
	{
		// create new node to list
		slist_node_t *temp = (slist_node_t*)calloc(1,sizeof(slist_node_t));
		if(temp == NULL)//input validaition
		{
			free(temp);
			slist_destroy(list,SLIST_LEAVE_DATA);
			exit(-1);
		}
		slist_next(temp) = NULL;
		slist_data(temp) = obj;
		slist_port(temp) = port;
		slist_head(list) = temp;
		slist_tail(list) = temp;
		slist_size(list)++;
		return 0;
	}
	else
	{

		slist_node_t *temp = (slist_node_t*)calloc(1,sizeof(slist_node_t));
		if(temp == NULL)//input validaition
		{
			free(temp);
			slist_destroy(list,SLIST_LEAVE_DATA);
			exit(-1);
		}
		slist_data(temp) = obj;
		slist_port(temp) = port;
		slist_next(temp) = NULL;//to null the tail next
		slist_next(slist_tail(list)) = temp; // to push to last plasce
		slist_tail(list) = temp;
		slist_size(list)++;
		return 0;
	}
}

int slist_prepend(slist_t *list,void *obj)
{
	if(obj == NULL)//input validaition
	{
		fprintf(stderr, "slist_prepend: Ilegal operation on list ");
		return -1;
	}
	if(list == NULL)//input validaition
	{
		fprintf(stderr, "Append: Ilegal operation on list");
		return -1;
	}
	if(slist_size(list) == 0)
	{
		slist_node_t *temp = (slist_node_t*)calloc(1,sizeof(slist_node_t));
		if(!temp)//input validaition
		{
			fprintf(stderr, "Ilegal malloc opreation");
			free(temp);
			slist_destroy(list,SLIST_LEAVE_DATA);
			exit(-1);
		}
		slist_data(temp) = obj;
		//slist_next(temp) = slist_next(slist_tail(list));
		slist_head(list) = temp;
		slist_tail(list) = temp;
		slist_size(list)++;
		return 0;
	}
	else
	{
		slist_node_t *temp = (slist_node_t*)calloc(1,sizeof(slist_node_t));
		if(!temp)//input validaition
		{
			fprintf(stderr, "Ilegal malloc opreation");
			exit(-1);
		}
		slist_data(temp) = obj;//enter input to node
		slist_next(temp) = slist_head(list);
		slist_head(list) = temp;
		slist_size(list)++;
		return 0;
	}
}

int slist_append_list(slist_t* dst_list, slist_t* src_list)
{
	if(src_list == NULL)//input validaition
		return -1;
	if(dst_list == NULL)//input validaition
		return -1;
	if(slist_size(src_list) <= 0)//input validaition
		return -1;
	if(slist_tail(src_list) == NULL)//input validaition
		return -1;
	if(slist_tail(dst_list) == NULL)//input validaition
		return -1;
	slist_node_t *temp = slist_head(src_list);
	int count = slist_size(src_list);
	while(count > 0)//insert list src to dst
	{
		slist_append(dst_list,slist_data(temp),slist_port(temp));
		temp = slist_next(temp);
		count--;
	}
	return 1;
}
