/* ex_gcc1.cpp
*
*  ��������͸��������ʾ����
*  ���ߣ�san, alert7, eyas, watercloud
*
*  GCC�ж���Ŀռ���֯���������ʾ������
*  ���Ի�����FreeBSD 4.4 + gcc 2.95.3
*/

#include<iostream.h>
class ClassTest
{
public:
  long buff[1];   //��СΪ1
  virtual void test(void)
  {
     cout << "ClassTest test()" << endl;
  }
};

void entry(void)
{
  cout << "Why are u here ?!" << endl;
}

int main(void)
{
  ClassTest a,*p =&a;
  long addr[] = {0,0,0,(long)entry}; //�������麯����
                                  //test() -> entry()

  a.buff[1] = ( long ) addr;// ������������麯���б�ָ��
  a.test();    //��̬����ģ���������
  p->test();   //��̬����ģ������ǵĺ�����ȥ�ҵ�ַ��
               // ����ͱ���˵��ú���  entry()

}
