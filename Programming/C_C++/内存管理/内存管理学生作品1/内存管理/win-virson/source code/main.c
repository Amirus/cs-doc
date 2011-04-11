#include<stdio.h>
#include<malloc.h>

#define LEN  sizeof(struct area)
#define LEN_POINTER_LIST sizeof(struct AreaPointer_list)

struct area{
	int start;	//��������ʼ��ַ
	int length;	//�����ĳ���
	int job;	//������ҵռ��ֵΪ��ҵ�ţ�������ֵΪ0
	struct area * next;
};

//����ָ�������
//���ѿ��з��������ռ�÷��������յ�ַ�Ⱥ�˳��ϲ�
//����ʾ�����ڴ������ʱ��ʹ��
struct AreaPointer_list{
	struct area * data;
	struct AreaPointer_list * next;
};

struct area * idle;						//ȫ�ֱ��������з�������ͷָ��
struct area * used;						//ȫ�ֱ�����ռ�÷�������ͷָ��
struct AreaPointer_list * whole = NULL;	//ȫ�ֱ���������ָ������ͷָ��










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










//�ͷ���ָ��������صĲ����������ϲ����з��������ռ�÷���������������Ԫ�ص�ָ��
struct AreaPointer_list * search_position(int s,struct AreaPointer_list * head)
{
	struct AreaPointer_list * p = NULL;
	while(head != NULL){
		if(s < (head->data)->start)
			break;
		p = head;
		head = head->next;
	}
	return p;
}
struct AreaPointer_list * emerge(struct area * idle_temp,struct area * used_temp)
{
	struct AreaPointer_list * previcious;
	struct AreaPointer_list * temp;
	
	if(used_temp != NULL)
	{
		whole = (struct AreaPointer_list *)malloc(LEN_POINTER_LIST);
		whole->data = used_temp;
		whole->next = NULL;
		previcious = whole;
		used_temp = used_temp->next;
		while(used_temp != NULL){
			temp = (struct AreaPointer_list *)malloc(LEN_POINTER_LIST);
			temp->data = used_temp;
			temp->next = NULL;
			previcious->next = temp;
			previcious = temp;

			used_temp = used_temp->next;
		}
	}

	while(idle_temp != NULL){
		struct area * idle_next = idle_temp->next;
		struct AreaPointer_list * pos = search_position(idle_temp->start,whole);
		
		if(pos == NULL)
		{
			temp = (struct AreaPointer_list *)malloc(LEN_POINTER_LIST);
			temp->data = idle_temp;
			temp->next = whole;

			whole = temp;
		}
		else
		{
			temp = (struct AreaPointer_list *)malloc(LEN_POINTER_LIST);
			temp->data = idle_temp;
			temp->next = pos->next;

			pos->next = temp;
		}
		
		idle_temp = idle_next;
	}

	return whole;
}
void printall(struct AreaPointer_list * head)
{
	struct area * data_temp;
	if(head == NULL)
		printf("Area pointer list is null...\n");
	else{
		while(head != NULL)
		{
			data_temp = head->data;
			if(data_temp->job == 0)
				printf("begin:%dK\tlength:%dK\t����\t\t|\n",data_temp->start,data_temp->length);
			else
				printf("begin:%dK\tlength:%dK\tuse:Job%d\t|\n",data_temp->start,data_temp->length,data_temp->job);
			
			head = head->next;
		}
	}
}
void file_printall(struct AreaPointer_list * head,FILE * file)
{
	struct area * data_temp;
	if(head == NULL)
		fprintf(file,"Area pointer list is null...\n");
	else{
		while(head != NULL)
		{
			data_temp = head->data;
			if(data_temp->job == 0)
				fprintf(file,"begin:%dK\tlength:%dK\t����\t\t|\n",data_temp->start,data_temp->length);
			else
				fprintf(file,"begin:%dK\tlength:%dK\tuse:Job%d\t|\n",data_temp->start,data_temp->length,data_temp->job);
			
			head = head->next;
		}
	}
}
void free_PointerList(struct AreaPointer_list * head)
{
	struct AreaPointer_list * temp;
	while(head != NULL){
		temp = head;
		head = head->next;
		free(temp);
	}
}
//�ͷ���ָ��������صĲ����������ϲ����з��������ռ�÷���������������Ԫ�ص�ָ��










void input_by_hand()
{
	int job;
	int is_alloc;//1 ������� 0 �ͷŷ���
	int length;
	int flag;

	printf("��ѡ����������㷨������0---�������� ����1---��������\n");
	scanf("%d",&flag);
	while(flag != 0 && flag != 1)
	{
		printf("������������������ʾ��������\n");
		scanf("%d",&flag);
	}
	if(flag == 0)
		printf("ѡ�����������㷨--->����������������ݣ�(���� 0 0 0 ����)\n");
	if(flag == 1)
		printf("ѡ�����������㷨--->����������������ݣ�(���� 0 0 0 ����)\n");
	
	printf("�������ݸ�ʽ����ҵ��(int>0) [����1--����|����0--�ͷ�] ��������(int>0)\n");
	printf("�������� 5 1 130 ��ʾ ��ҵ5����130K\n");
	printf("�������� 3 0 200 ��ʾ ��ҵ3�ͷ�200K\n");

	while(1)//���� 0 0 0 ����
	{
		scanf("%d%d%d",&job,&is_alloc,&length);
		if(job == 0 && is_alloc == 0 && length == 0)
			break;
		while(job<=0 || (is_alloc != 0 && is_alloc != 1) || length<=0)
		{
			printf("������������������ʾ��������\n");
			scanf("%d%d%d",&job,&is_alloc,&length);
			if(job == 0 && is_alloc == 0 && length == 0)
				return;
		}
		

		if(is_alloc == 1)
		{
			int r = memory_alloc(length,job,flag);
			if(!r)
			{
				printf("\n");
				printf("û�з��������Ŀ��з����ɹ����䣬��ȴ��ͷ�...\n");
				printf("\n");
				continue;
			}
			if(r == 2)
			{
				printf("\n");
				printf("������ҵ���Ѵ�����ռ�÷�����������������...\n");
				printf("\n");
				continue;
			}
		}
		if(is_alloc == 0)
		{
			int r = memory_free(job,flag);
			if(!r)
			{
				printf("\n");
				printf("û����ָ����ҵ�ŷ��ϵ�ռ�÷���������������...\n");
				printf("\n");
				continue;
			}
		}

		emerge(idle,used);
		
		printf("\n");
		printf("-------------------------------------------------\n");
		printf("���з�������\t\t\t\t\t|\n");
		print(idle);
		printf("\t\t\t\t\t\t|\n");

		printf("ռ�÷�������\t\t\t\t\t|\n");
		print(used);
		printf("\t\t\t\t\t\t|\n");

		printf("�����ڴ������\t\t\t\t\t|\n");
		printf("�͵�ַ\t\t\t\t\t\t|\n");
		printall(whole);
		printf("�ߵ�ַ\t\t\t\t\t\t|\n");
		printf("-------------------------------------------------\n");
		printf("\n");
		free_PointerList(whole);
		whole = NULL;
	}

	//�ͷſռ�
	free_AreaList(idle);
	free_AreaList(used);
	idle = NULL;
	used = NULL;
}

void input_by_file(int flag)
{
	int job;
	int is_alloc;//1 ������� 0 �ͷŷ���
	int length;
	char* result;
	int r;

	FILE * file1;
	FILE * file2;

	if(flag == 0)
		result = "result_data_1.txt";
	else
		result = "result_data_2.txt";

	if((file1 = fopen("source_data.txt","r")) == NULL)
	{
		printf("���ܴ�source_data.txt�ļ�...\n");
		exit(0);
	}
	if((file2 = fopen(result,"w")) == NULL)
	{
		printf("���ܴ�source_data.txt�ļ�...\n");
		exit(0);
	}

	if(flag == 0)
	{
		printf("�������ȷ����㷨�ó��Ľ����\n\n");
		fprintf(file2,"�������ȷ����㷨�ó��Ľ����\n\n");
	}
	else
	{
		printf("�������ŷ����㷨�ó��Ľ����\n\n");
		fprintf(file2,"�������ŷ����㷨�ó��Ľ����\n\n");
	}


	while(!feof(file1)){
		fscanf(file1,"%d%d%d",&job,&is_alloc,&length);

		if(job<=0 || (is_alloc != 0 && is_alloc != 1) || length<=0)
		{
			printf("�ļ�������%d %d %d����ĸ�ʽ���󣬲��ڴ���\n\n",job,is_alloc,length);
			fprintf(file2,"�ļ�������%d %d %d����ĸ�ʽ���󣬲��ڴ���\n\n",job,is_alloc,length);
			continue;
		}

		if(is_alloc == 1)
		{
			printf("JOB %d����%dK\n\n",job,length);
			fprintf(file2,"JOB %d����%dK\n\n",job,length);

			r = memory_alloc(length,job,flag);
			if(!r)
			{
				printf("û�з��������Ŀ��з����ɹ����䣬���ڴ���\n\n");
				fprintf(file2,"û�з��������Ŀ��з����ɹ����䣬���ڴ���\n\n");
				continue;
			}
			if(r == 2)
			{
				printf("������ҵ���Ѵ�����ռ�÷����������ڴ���\n\n");
				fprintf(file2,"������ҵ���Ѵ�����ռ�÷����������ڴ���\n\n");
				continue;
			}
		}
		else
		{
			printf("JOB %d�ͷ�%dK\n\n",job,length);
			fprintf(file2,"JOB %d�ͷ�%dK\n\n",job,length);

			r = memory_free(job,flag);
			if(!r)
			{
				printf("û����ָ����ҵ�ŷ��ϵ�ռ�÷��������ڴ���\n\n");
				fprintf(file2,"û����ָ����ҵ�ŷ��ϵ�ռ�÷��������ڴ���\n\n");
				continue;
			}
		}

		emerge(idle,used);
		
		printf("-------------------------------------------------\n");
		fprintf(file2,"-------------------------------------------------\n");
		printf("���з�������\t\t\t\t\t|\n");
		fprintf(file2,"���з�������\t\t\t\t\t|\n");
		print(idle);
		file_print(idle,file2);
		printf("\t\t\t\t\t\t|\n");
		fprintf(file2,"\t\t\t\t\t\t|\n");

		printf("ռ�÷�������\t\t\t\t\t|\n");
		fprintf(file2,"ռ�÷�������\t\t\t\t\t|\n");
		print(used);
		file_print(used,file2);
		printf("\t\t\t\t\t\t|\n");
		fprintf(file2,"\t\t\t\t\t\t|\n");

		printf("�����ڴ������\t\t\t\t\t|\n");
		fprintf(file2,"�����ڴ������\t\t\t\t\t|\n");
		printf("�͵�ַ\t\t\t\t\t\t|\n");
		fprintf(file2,"�͵�ַ\t\t\t\t\t\t|\n");
		printall(whole);
		file_printall(whole,file2);
		printf("�ߵ�ַ\t\t\t\t\t\t|\n");
		fprintf(file2,"�ߵ�ַ\t\t\t\t\t\t|\n");
		printf("-------------------------------------------------\n");
		fprintf(file2,"-------------------------------------------------\n");
		printf("\n");
		fprintf(file2,"\n");
		free_PointerList(whole);
		whole = NULL;
	}

	printf("========================================\n\n");
	fprintf(file2,"========================================\n\n");

	//�ͷſռ�
	free_AreaList(idle);
	free_AreaList(used);
	idle = NULL;
	used = NULL;

	fclose(file1);
	fclose(file2);
}

int main(){
	
	int method;

	idle = insert(0,640,0,NULL,NULL);
	used = NULL;

	printf("��ѡ����Է�ʽ������0---�ֹ�����Դ���� ����1---�ļ�����Դ����\n");
	printf("�ֹ����룺�ֹ��ڿ���̨console���������������ݣ������ʾ�ڿ���̨console\n");
	printf("�ļ����룺���ļ�source_data.txt��ȡ���������ݣ��������result_data.txt\n");
	scanf("%d",&method);
	while(method != 0 && method != 1)
	{
		printf("������������������ʾ��������\n");
		scanf("%d",&method);
	}

	if( method == 0)
		input_by_hand();
	else
	{
		input_by_file(0);

		idle = insert(0,640,0,NULL,NULL);
		used = NULL;

		input_by_file(1);
	}
}
