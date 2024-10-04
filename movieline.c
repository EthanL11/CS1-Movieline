#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//customer info
typedef struct customer{
	char name[51];
	int tickets;
	long long int time;
	int queue_pos;
}customer;

typedef struct node{
	customer* person;
	struct node* next;
}node;

//queue for the customer
typedef struct queue{
	int size;
	node* front;
	node* end;
}queue;

//creates a node for the linklist
node* create_node(customer* person){
	node* newnode=malloc(sizeof(node));
	newnode->person=person;
	newnode->next=NULL;
	return (newnode);
}

//adds info for each customer
customer* create_customer(char name[], int tickets, int time, int queue_pos) {
	customer* person=malloc(sizeof(customer));
	strcpy(person->name, name);
	person->tickets=tickets;
	person->time=time;
	person->queue_pos=queue_pos;
	return (person);
}

//creates queue
queue* create_queue(){
	queue* booth_queue=malloc(sizeof(queue));
	booth_queue->front=NULL;
	booth_queue->end=NULL;
	booth_queue->size=0;
	return (booth_queue);
}

//puts customer in queue
void enqueue(queue* booth_queue, customer* person){
	node* newnode=create_node(person);
	newnode->person=person;
	newnode->next=NULL;

	//checks end of node
	if(booth_queue->end!=NULL){
		booth_queue->end->next=newnode;
	}
	booth_queue->end=newnode;

	//adds node to front
	if(booth_queue->front==NULL){
		booth_queue->front=newnode;
	}
	booth_queue->size++;
}

//takes customer off of queue
customer* dequeue(queue* booth_queue){
	//checks if there if any in queue
	if(booth_queue->front==NULL){
		return (NULL);
	}
	node* temp=booth_queue->front;
	customer* result = temp->person;
	booth_queue->front=booth_queue->front->next;
	if(booth_queue->front==NULL){
		booth_queue->end=NULL;
	}
	free(temp);
	booth_queue->size--;
	return (result);
}

//returns front of queue
customer* peek(queue* booth_queue){
	if(booth_queue->front==NULL){
		return (NULL);
	}
	return (booth_queue->front->person);
}

//empties queue
int empty(queue* booth_queue){
	return (booth_queue->front==NULL);
}

//returns size of queue
int size(queue* booth_queue){
	if(booth_queue->front==NULL){
		return (0);
	}
	else{
		return (booth_queue->size);
	}
}

int main(void){
	int n, b;
	scanf("%d %d",&n,&b);

	//create booth queues
	queue* booths[b];
	for(int i=0;i<b;i++){
		booths[i]=create_queue();
	}

	//create queues
	queue* queues[12];
	for(int i=1;i<12;i++){
		queues[i]=create_queue();
	}

	//get customer info
	for(int i=0;i<n;i++){
		char name[51];
		int tickets;
		int time;
		scanf("%s %d %d",name,&tickets,&time);
		customer* person=create_customer(name,tickets,time,i+1);

		//takes first letter
		int first_letter=name[0]-'A';
		int queue_pos;
		if(first_letter%b!=0){
			queue_pos=first_letter%b;
		}
		else{
			queue_pos=1;
			int min=booths[0]->size;
			for(int j=0;j<b;j++){
				if(booths[j]->size<min){
					min=booths[j]->size;
					queue_pos=j+1;
				}
			}
		}
		enqueue(booths[queue_pos-1],person);
	}

	int booth_queues[b], k=0;
	for(int i=0;i<12&&k<b;i++){
		if(!empty(queues[i])){
			booth_queues[k++]=i;
		}
	}

	//print booth # and each customers queue and checkout time
	for(int i=0;i<b;i++){
		printf("Booth %d\n",i+1);
		int booth_queue=booth_queues[i];
		queue* booth=queues[booth_queue];
		while(!empty(booths[i])){
			customer* person=dequeue(booths[i]);
			int checkout=person->time+(person->queue_pos*person->tickets);
			printf("%s from line %d checks out at time %d.\n",person->name,person->queue_pos,checkout);
			free(person);
		}
		printf("\n");
	}

	//free memory
	for(int i=0;i<12;i++){
		while(!empty(queues[i])){
			free(dequeue(queues[i]));
		}
		free(queues[i]);
	}

	for(int i=0;i<b;i++){
		free(booths[i]);
	}

	return (0);
}