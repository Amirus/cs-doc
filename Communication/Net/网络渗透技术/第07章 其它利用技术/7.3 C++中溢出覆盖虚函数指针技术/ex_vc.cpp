/* ex_vc.cpp
*
*  ��������͸��������ʾ����
*  ���ߣ�san, alert7, eyas, watercloud
*
*  VC�ж���Ŀռ���֯���������ʾ������
*/

#include<iostream.h>
class ClassEx
{
public:
int buff[1];
virtual void test(void){ cout << "ClassEx::test()" << endl;};
};
void entry(void)
{
  cout << "Why a u here ?!" << endl;
};

ClassEx obj1,obj2,* pobj;

int main(void)
{

  pobj=&obj2;
  obj2.test();
  
  int vtab[1] = { (int) entry };//����vtab��
                                //entry����ڵ�ַ
  obj1.buff[1] = (int)vtab;     //obj1.buff[1]���� obj2��pvftable��
                                //�����޸��˺���ָ���б�ĵ�ַ��vtab
  pobj->test();
  return 0;
}
