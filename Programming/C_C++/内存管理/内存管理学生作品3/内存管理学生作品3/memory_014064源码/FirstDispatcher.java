import java.util.*;

/*
 * Created on 2004-5-24
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */

/**
 * 
 * @author Liu Junhui
 *
 * �״���Ӧ�㷨�ķ����࣬�̳з������
 * �����㷨,�ҵ���һ�����ڻ���������С�ķ����ͷ���
 */
class FirstDispatcher extends MyDispatcher {
	public FirstDispatcher() {
		freeList = new ArrayList();
		SpaceNode initNode = new SpaceNode(0, Utility.initMemory,0);
		freeList.add(initNode);
		useList = new ArrayList();
	}
	
	//�ϲ������ĺ���
	public void addFreeNode(SpaceNode freeNode)
	{		
		SpaceNode preNode = null;
		SpaceNode nextNode = null;
		SpaceNode node = null;
		int preAddr = -1;
		int nextAddr = -1;
		
		for (int i=0;i<freeList.size();i++)
		{
			node = (SpaceNode)freeList.get(i);
			if (node.getAddrFrom() < freeNode.getAddrFrom())
			{
				preAddr = i;
			}
			else if (node.getAddrFrom() > freeNode.getAddrFrom())
			{
				nextAddr = i;
				break;
			}
		}
		
		//��������Ϊ��,ֱ�Ӳ��ȥ
		if (freeList.size() == 0)
		{
			freeNode.setDispatchID(0);
			freeList.add(freeNode);
			return ;
		}
		
		if (preAddr != -1)
		{
			preNode = (SpaceNode)freeList.get(preAddr);
			
			//��ǰ�����ڷ����ϲ�
			if (preNode.getAddrNext() == freeNode.getAddrFrom())
			{
				preNode.setLen(preNode.getLen() + freeNode.getLen());
				if (nextAddr != -1)
				{
					nextNode = (SpaceNode)freeList.get(nextAddr);
					//����������ڷ����ϲ�
					if (freeNode.getAddrNext() == nextNode.getAddrFrom())
					{
						preNode.setLen(preNode.getLen() + nextNode.getLen());
						freeList.remove(nextAddr);
					}
				}
				return;
			}
		}
		if (nextAddr != -1)
		{
			nextNode = (SpaceNode)freeList.get(nextAddr);
			//����������ڷ����ϲ�
			if (freeNode.getAddrNext() == nextNode.getAddrFrom())
			{
				nextNode.setAddrFrom(freeNode.getAddrFrom());
				nextNode.setLen(nextNode.getLen() + freeNode.getLen());
				return;
			}		
		}
		
		//�޷��ϲ�,�嵽�ʵ�λ��
		freeNode.setDispatchID(0);
		freeList.add(preAddr+1,freeNode);		
	}
}


