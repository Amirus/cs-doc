/* bug.cpp
*
*  ��������͸��������ʾ����
*  ���ߣ�san, alert7, eyas, watercloud
*
*  ģ����ʵ������������
*/

#include<iostream.h> 
#include<fstream.h> 
#include<unistd.h> 

class ClassBase 
{ 
public: 
  char buff[128]; 

  void getBuff() 
  { 
     ifstream myin; 
     myin.open("bug.conf"); 
     cout << "Get buff from file : bug.conf" << endl; 
     myin >> buff;    // ���������÷����˲��������� ! 
  }; 
  virtual void printBuffer(void){}; 
}; 

class  ClassA :public ClassBase 
{ 
public: 
  void printBuffer(void) 
  { 
     cout << "Name :" << buff << endl; 
  }; 
}; 


int main(void) 
{ 
  ClassA a; 
  ClassBase * pa = &a; 


  cout << &a << endl; 

  a.getBuff();   // ----������û�б߽��� ! 
  pa->printBuffer(); 

  return 0; 
} 
