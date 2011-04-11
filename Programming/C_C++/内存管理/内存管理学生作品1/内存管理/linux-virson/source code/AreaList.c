#include "AreaList.h"
#include "AreaPointerList.h"


#define LEN  sizeof(struct area)


extern struct area * idle;						//ȫ�ֱ��������з�������ͷָ��
extern struct area * used;						//ȫ�ֱ�����ռ�÷�������ͷָ��
extern struct AreaPointer_list * whole;			//ȫ�ֱ���������ָ������ͷָ��


//p(previcious) n(next)ָ���������еĺδ����������ɵ�Ԫ��
//p==NULL  ������ͷ���룬����ͷָ��
//p!=NULL  �������л�����β���룬���ص�ǰ�����Ԫ�ص�ָ��
struct area * insert(int s,int l,int j,struct area * p,struct area * n)
{
	struct area * current = (struct area * )malloc(LEN);
	current->start = s;
	current->length = l;
	current->job = j;
	if(p == NULL){//������ͷ����
		current->next = n;
		return current;
	}
	else{
		if(p->next == NULL){//������β����
			current->next = NULL;
			p->next = current;
		}
		else{//�������в���
			current->next = p->next;
			p->next = current;
		}
		return current;
	}
}
//��ģ����ڴ�Ҫ��λ��ֻ��ʹ��һ��










//��ӡ��������
void print(struct area * head)
{
	if(head == NULL){
		printf("Area list is null...\n");
	}
	else{
		while(head != NULL)
		{
			if(head->job == 0)
				printf("begin:%dK\tlength:%dK\t����\t\t|\n",head->start,head->length);
			else
				printf("begin:%dK\tlength:%dK\tuse:Job%d\t|\n",head->start,head->length,head->job);
			head = head->next;
		}
	}
}
void file_print(struct area * head,FILE * file)
{
	if(head == NULL){
		fprintf(file,"Area list is null...\n");
	}
	else{
		while(head != NULL)
		{
			if(head->job == 0)
				fprintf(file,"begin:%dK\tlength:%dK\t����\t\t|\n",head->start,head->length);
			else
				fprintf(file,"begin:%dK\tlength:%dK\tuse:Job%d\t|\n",head->start,head->length,head->job);
			head = head->next;
		}
	}
}
//��ӡ��������


//�ͷŷ�������ռ�
void free_AreaList(struct area * head)
{
	struct area * temp;
	while(head != NULL){
		temp = head;
		head = head->next;
		free(temp);
	}
}
//�ͷŷ�������ռ�


//�ڷ�����������������λ��
//flag==0 ��������������ʼ��ַ��С��������
//flag==1 �������������������ȴ�С��������
//������� element ����ΪNULL
struct area * search_pos(struct area * element,struct area * head,int flag)
{
	struct area * p = NULL;
	while(head != NULL){
		if(flag == 0)
		{
			if(element->start < head->start)
				break;
		}
		else
		{
			if(element->length < head->length)
				break;
		}
		p = head;
		head = head->next;
	}
	return p;
}
//����ֵp==NULL��������λ��������ͷ
//����ֵp!=NULL��������λ����p ֮��


//���з��������ʵ�ʲ��빤��
//flag==0 ��������������ʼ��ַ��С��������
//flag==1 �������������������ȴ�С��������
//������� element->next ҪΪNULL
struct area * insert_list(struct area * element,struct area * list,int flag)
{
	if(list == NULL)
		list = element;
	else{
		struct area * pos = search_pos(element,list,flag);
		if(pos == NULL){
			element->next = list;
			list = element;
		}
		else{
			element->next = pos->next;
			pos->next = element;
		}
	}
	return list;
}//���ز���Ԫ��֮���������ͷָ��


//���в�ѯ���з�������̬���������ʵ�ʹ������㷨���裺
//1����ѯ���з����������Ƿ��г��ȴ��ڻ�������볤�ȵķ�������û�з���FALSE
//2�������ҵ����������ķ����������ӿ���������ȡ��
//3�����������ȡ���Ŀ��з����ֿ飬���µ�ռ�÷�����ʣ����з����ֱ��������
//ע�⣺����ռ�÷��������չ̶��ĵ�ַ�Ⱥ�˳�򣬲�����з�������ķ�ʽҪ����flag��ֵ
int memory_alloc(int length,int job,int flag)
{
	struct area * used_element;
	struct area * free_element;

	struct area * head = idle;
	struct area * head_temp = used;
	struct area * p = NULL;


	//����������ҵ���Ƿ����
	while(head_temp != NULL)
	{
		if(head_temp->job == job)
			return 2;
		head_temp = head_temp->next;
	}

	//�ڿ��з��������в���
	while(head != NULL)
	{
		if(head->length >= length)
			break;
		p = head;
		head = head->next;
	}

	if(head != NULL)
	{
		//�ӿ�����������ȡ��
		if(p == NULL)//�����еĵ�һ��������������
		{
			idle = idle->next;
		}
		else
		{
			p->next = head->next;
		}
		head->next = NULL;
	}
	else return 0;

	//�����µ�ռ��������Ԫ�ز�����ռ��������
	used_element = (struct area * )malloc(LEN);
	used_element->start = head->start;
	used_element->length = length;
	used_element->job = job;
	used_element->next = NULL;

	used = insert_list(used_element,used,0);


	//�����з����ֿ����ʣ�࣬�����µĿ���������Ԫ�ز��������������
	if(head->length > length){
		free_element = (struct area * )malloc(LEN);
		free_element->start = head->start + length;
		free_element->length = head->length - length;
		free_element->job = 0;
		free_element->next = NULL;

		idle = insert_list(free_element,idle,flag);
	}
	
	//�ͷſռ�
	free(head);

	return 1;
}


//���в�ѯռ�÷�������̬�ͷŷ�����ʵ�ʹ������㷨���裺
//1��������ҵ�Ų�ѯ��ռ�÷���������Ҫ�ͷŵķ�������û�з���FALSE
//2�������ҵ�Ҫ�ͷŵķ����������ӿ���������ȡ��
//3������ȡ���ķ��������ݽ����µĿ��з���
//4���ڿ��з��������в�ѯ�Ƿ��к��¿��з������ڵĿ��з���������ϲ�
//5������flag��ȡֵ�����ض���ʽ������з�������
int memory_free(int job,int flag)
{
	struct area * used_element;
	struct area * free_element;
	struct area * head = used;
	struct area * p = NULL;

	struct area * previcious1 = NULL;
	struct area * current1 = NULL;
	struct area * previcious2 = NULL;
	struct area * current2 = NULL;

	//������ҵ����ռ�÷��������в���
	while(head != NULL)
	{
		if(head->job == job)
			break;
		p = head;
		head = head->next;
	}

	if(head != NULL)
	{
		//��ռ����������ȡ��
		if(p == NULL)//�����еĵ�һ��������������
		{
			used = used->next;
		}
		else
		{
			p->next = head->next;
		}
		head->next = NULL;
	}
	else return 0;

	//�����µĿ��з���
	used_element = head;
	free_element = (struct area * )malloc(LEN);
	free_element->start = used_element->start;
	free_element->length = used_element->length;
	free_element->job = 0;
	free_element->next = NULL;

	
	//�ӿ�����������Һ��µĿ��з������ڷ���
	head = idle;
	p = NULL;

	while(head != NULL)
	{
		if(head->start + head->length == used_element->start)
		{
			previcious1 = p;
			current1 = head;
		}
		if(used_element->start + used_element->length == head->start)
		{
			previcious2 = p;
			current2 = head;
		}
		p = head;
		head = head->next;
	}
	

	//�ϲ����ڿ��з���
	if( current1 != NULL )
	{
		//�Ѻ��·������ڵķ����ӿ��з���������ȡ��
		if( previcious1 == NULL )
			idle = idle->next;
		else
			previcious1->next = current1->next;
		current1->next = NULL;

		//�޸��¿��з������������
		free_element->start = current1->start;
		free_element->length = free_element->length + current1->length;
	}
	if( current2 != NULL )
	{
		//�Ѻ��·������ڵķ����ӿ��з���������ȡ��
		if( previcious2 == NULL )
			idle = idle->next;
		else
			previcious2->next = current2->next;
		current2->next = NULL;

		//�޸��¿��з������������
		free_element->length = free_element->length + current2->length;
	}

	//����flag��ȡֵ�����ض���ʽ������з�������
	idle = insert_list(free_element,idle,flag);

	//�ͷſռ�
	free(used_element);

	return 1;
}
