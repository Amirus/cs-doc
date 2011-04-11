import java.util.*;

/*
 * Created on 2004-5-25
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */

/**
 * @author Liu Junhui
 *
 * �����Ӧ�㷨�ķ����࣬�̳з������
 */
public class BestDispatcher extends MyDispatcher {

	public BestDispatcher() {
		freeList = new ArrayList();
		SpaceNode initNode = new SpaceNode(0, Utility.initMemory, 0);
		freeList.add(initNode);
		useList = new ArrayList();
	}

	//ע�⣺���㷨��freeList��һ�������з�����С�������ӵ���
	//      Ѱ�����ڵķ���Ҫ����freeList�������ϵ����ڲ�����freeList������
	public void addFreeNode(SpaceNode freeNode) {
		SpaceNode preNode = null;
		SpaceNode nextNode = null;
		SpaceNode node = null;
		SpaceNode insertNode = null; //�ϲ���Ҫ���뵽freeList�Ľڵ�
		int preAddr = -1;
		int nextAddr = -1;

		for (int i = 0; i < freeList.size(); i++) {
			node = (SpaceNode) freeList.get(i);
			//����Ѱ�����������ڵ�ǰ��
			if (node.getAddrNext() == freeNode.getAddrFrom()) {
				preAddr = i;
			}
			//����Ѱ�����������ڵĺ��
			if (freeNode.getAddrNext() == node.getAddrFrom()) {
				nextAddr = i;
			}
		}

		//�ϲ����ڷ���
		boolean hasRemove = false;
		if (preAddr != -1) {
			preNode = (SpaceNode) freeList.get(preAddr);
			preNode.setLen(preNode.getLen() + freeNode.getLen());
			insertNode = preNode;
			if (nextAddr != -1) {
				nextNode = (SpaceNode) freeList.get(nextAddr);
				insertNode.setLen(insertNode.getLen() + nextNode.getLen());
				if (nextAddr < preAddr)
				{
					freeList.remove(preAddr);
					hasRemove = true;
				}
				freeList.remove(nextAddr);
			}
			if (hasRemove == false) {
				freeList.remove(preAddr);
			}
		} else if (nextAddr != -1) {
			nextNode = (SpaceNode) freeList.get(nextAddr);
			nextNode.setAddrFrom(freeNode.getAddrFrom());
			nextNode.setLen(nextNode.getLen() + freeNode.getLen());
			insertNode = nextNode;
			freeList.remove(nextAddr);
		} else {
			insertNode = freeNode;
		}

		//���뵽��������
		int insertLen = insertNode.getLen();
		for (int i = 0; i < freeList.size(); i++) {
			node = (SpaceNode) freeList.get(i);
			if (insertNode.getLen() <= node.getLen()) {
				insertNode.setDispatchID(0);
				freeList.add(i, insertNode);
				return;
			}
		}
		insertNode.setDispatchID(0);
		freeList.add(insertNode);
	}
}
