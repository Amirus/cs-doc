import java.util.*;

/*
 * Created on 2004-5-24
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */

/**
 * @author Liu Junhui
 *
 * ��̬�����㷨�ķ������
 */
public class MyDispatcher {
	protected ArrayList freeList; //������з���������
	protected ArrayList useList; //����ʹ�÷���������
	private boolean hasTight = false; //��־�Ѿ�����

	/* ���亯����
	 * ������Ҫ���ֽ��������ش�����з������ڴ��ַ,�������-1��ʾ�ڴ�ռ䲻��*/
	public int alloc(int nbyte) {
		System.out.println("### alloc memory " + nbyte + " ###");
		SpaceNode node;
		int len;

		for (int i = 0; i < freeList.size(); i++) {
			node = (SpaceNode) freeList.get(i);
			len = node.getLen();

			if (len >= nbyte) {
				if (len == nbyte) {
					//�Ѹ÷���������������

					int dispatchID = Utility.getDispatchID();
					node.setDispatchID(dispatchID);
					addUseNode(node);
					//�Ѹ÷����ӿ���������ɾȥ
					freeList.remove(i);
					System.out.println(toString());
					return dispatchID;
				}

				//��Ҫ�ָ����
				int dispatchID = Utility.getDispatchID();
				SpaceNode useNode =
					new SpaceNode(node.getAddrFrom(), nbyte, dispatchID);
				addUseNode(useNode);
				node.setAddrFrom(node.getAddrFrom() + nbyte);
				node.setLen(len - nbyte);
				System.out.println(toString());
				return dispatchID;
			}
		}

		//��û��ɽ�����ʱ��������������
		if (hasTight == false) {
			tightNode();
			hasTight = true;
			return alloc(nbyte);
		}
		System.out.println("not enough memory!");
		return -1;
	}

	//ʹ�������ǰ���ַ�������ӵ�
	public void addUseNode(SpaceNode useNode) {
		SpaceNode node;
		for (int i = 0; i < useList.size(); i++) {
			node = (SpaceNode) useList.get(i);
			if (useNode.getAddrFrom() < node.getAddrFrom()) {
				useList.add(i, useNode);
				return;
			}
		}
		useList.add(useNode);
	}

	//�ͷ��㷨,���Ҫ�ͷź���������ڷ����ϲ��ͺϲ�
	public boolean free(int dispatchID) {
		System.out.println("### free dispatchID " + dispatchID + " ###");
		for (int i = 0; i < useList.size(); i++) {
			SpaceNode node = (SpaceNode) useList.get(i);
			if (dispatchID == node.getDispatchID()) {
				addFreeNode(node); //�ϲ���������
				useList.remove(i);
				System.out.println(toString());
				hasTight = false;
				return true;
			}
		}
		return false;
	}

	//�ϲ����ڷ����ĺ���
	//ע�⣺������Ӧ�㷨�Ĳ��֮����������̳�
	public void addFreeNode(SpaceNode freeNode) {
	}

	public void tightNode() {
		System.out.println("tight!");
		if (useList.size() == 0) {
			return;
		}

		SpaceNode node;
		SpaceNode lastNode = (SpaceNode) useList.get(0);
		lastNode.setAddrFrom(0);
		int useSpace = lastNode.getLen();
		for (int i = 1; i < useList.size(); i++) {
			node = (SpaceNode) useList.get(i);
			useSpace += node.getLen();
			node.setAddrFrom(lastNode.getAddrNext());
			lastNode = node;
		}

		SpaceNode freeNode =
			new SpaceNode(
				lastNode.getAddrNext(),
				Utility.initMemory - useSpace,
				0);

		int useLen = freeList.size();
		for (int i = 0; i < useLen; i++) {
			freeList.remove(0);
		}
		freeList.add(freeNode);
		System.out.println(toString());
	}

	public ArrayList getFreeList()
	{
		return freeList;
	}
	
	public ArrayList getUseList()
	{
		return useList;
	}

	public String toString() {
		String str;
		str = "------------------Free List-----------------\n";
		for (int i = 0; i < freeList.size(); i++) {
			SpaceNode node = (SpaceNode) freeList.get(i);
			str = str + node.toString() + "\n";
		}
		str += "-----------------Use List-----------------\n";
		for (int i = 0; i < useList.size(); i++) {
			SpaceNode node = (SpaceNode) useList.get(i);
			str = str + node.toString() + "\n";
		}
		return str;
	}
}
