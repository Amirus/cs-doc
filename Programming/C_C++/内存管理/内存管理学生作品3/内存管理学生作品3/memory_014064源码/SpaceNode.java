/*
 * Created on 2004-5-24
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
 
/**
 * @author Liu Junhui
 *
 *������л�ʹ������ķ����ڵ�
 */
public class SpaceNode {
	private int addrFrom; //�����ĵ�ַ���
	private int len; //�÷����ĳ���
	private int dispatchID; //��ʾ�ѷ���ķ���ID��0��ʾΪ���з���

	public SpaceNode(int addrFrom, int len, int dispatchID) {
		this.addrFrom = addrFrom;
		this.len = len;
		this.dispatchID = dispatchID;
	}
/*
	public void reset(int addrFrom, int len) {
		this.addrFrom = addrFrom;
		this.len = len;
		this.dispatchID = dispatchID;
	}*/
	
	public int getLen()
	{
		return len;
	}
	
	public void setLen(int len)
	{
		this.len = len;
	}
	
	public int getAddrFrom()
	{
		return addrFrom;
	}
	
	public int getAddrNext()
	{
		return addrFrom + len;
	}
	
	public void setAddrFrom(int addrFrom)
	{
		this.addrFrom = addrFrom;
	}
	
	public String toString()
	{
		return "[DispatchID " + dispatchID + "] Address : " + addrFrom + "   Len : " + len;
	}
	
	
	public int getDispatchID()
	{
		return dispatchID;
	}
	
	public void setDispatchID(int dispatchID)
	{
		this.dispatchID = dispatchID;
	}
}

