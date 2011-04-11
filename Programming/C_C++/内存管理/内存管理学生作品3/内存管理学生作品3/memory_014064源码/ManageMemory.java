import java.awt.*;
import java.awt.event.*;
import java.awt.geom.*;
import java.util.*;
import javax.swing.*;

/*
 * Created on 2004-5-24
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */

/**
 * @author Liu Junhui
 *
 * ģ�⶯̬���������㷨
 */
public class ManageMemory {
	//for test
	static public MyDispatcher testDispatcher = Utility.initDispatcher;

	public static void main(String[] args) {
		//test2();

		MemoryFrame frame = new MemoryFrame();
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.show();
	}
	static void test2() {
		int addr1 = testDispatcher.alloc(100);
		int addr2 = testDispatcher.alloc(100);
		int addr3 = testDispatcher.alloc(100);
		int addr4 = testDispatcher.alloc(100);
		int addr5 = testDispatcher.alloc(100);
		int addr6 = testDispatcher.alloc(100);

		testDispatcher.free(addr1);
		testDispatcher.free(addr2);
		testDispatcher.free(addr3);
		testDispatcher.free(addr4);
	}

	/*
	static void test1() {
		System.out.println(dispatcher);
		//��ҵ1����130K
		int addr1 = dispatcher.alloc(130);
		//��ҵ2����60K
		int addr2 = dispatcher.alloc(60);
		//��ҵ3����100k
		int addr3 = dispatcher.alloc(100);
		//��ҵ2�ͷ�60K
		dispatcher.free(addr2);
		//��ҵ4����200K
		int addr4 = dispatcher.alloc(200);
		//��ҵ3�ͷ�100K
		dispatcher.free(addr3);
		//��ҵ1�ͷ�130K
		dispatcher.free(addr1);
		//��ҵ5����140K
		int addr5 = dispatcher.alloc(140);
		//��ҵ6����60K
		int addr6 = dispatcher.alloc(60);
		//��ҵ7����50K
		int addr7 = dispatcher.alloc(50);
		//��ҵ6�ͷ�60K
		dispatcher.free(addr6);
	
		int addr8 = dispatcher.alloc(260);
	
		dispatcher.free(addr7);
	
		addr8 = dispatcher.alloc(260);
	}*/
}

class MemoryFrame extends JFrame {
	public MemoryFrame() {
		setSize(Utility.WIDTH, Utility.HEIGHT);
		setTitle("Memory Manage(014064 ������)");

		MemoryPanel memoryPanel = new MemoryPanel();
		Container contentPane = getContentPane();
		contentPane.add(memoryPanel);
	}
}

class MemoryPanel extends JPanel {
	private int allocLen = -1;
	private int dispatchID = -1;
	private JTextField textAlloc;
	private JTextField textFree;
	private JLabel labelResult;
	private JButton firstButton;
	private JButton bestButton;
	private DisplayMemoryPanel displayPanel;

	//��ѡ����ڴ������
	private MyDispatcher dispatcher = Utility.initDispatcher;
	private ArrayList useList = dispatcher.getUseList();
	private ArrayList freeList = dispatcher.getFreeList();

	public MemoryPanel() {
		setLayout(new BorderLayout());

		add(getSelectPanel(), BorderLayout.NORTH);
		add(getInputPanel(), BorderLayout.SOUTH);

		displayPanel = new DisplayMemoryPanel();
		displayPanel.setBackground(Color.GRAY);
		add(displayPanel);
	}

	//�õ�ѡ����壬ѡ��ʵ�ֵ��㷨
	private JPanel getSelectPanel() {
		JPanel panel = new JPanel();

		firstButton = addButton(panel, "First", new FirstAction());
		bestButton = addButton(panel, "Best", new BestAction());

		if (dispatcher.getClass() == FirstDispatcher.class) {
			firstButton.setBackground(Color.BLUE);
		} else {
			bestButton.setBackground(Color.BLUE);
		}

		return panel;
	}

	//�õ�������壬����Ҫ������ڴ��С���ͷŵĽ���ID
	private JPanel getInputPanel() {
		JPanel panel = new JPanel();
		JLabel label1 = new JLabel("Alloc Size (k)");
		JLabel label2 = new JLabel("Free ID");
		JLabel labelNull = new JLabel("      ");
		labelResult = new JLabel("Idle");
		
		textAlloc = new JTextField("100");
		textFree = new JTextField("1");
		textAlloc.setColumns(5);
		textFree.setColumns(5);

		JPanel resultPanel = new JPanel();
		resultPanel.add(labelResult);
		resultPanel.setBackground(Color.GREEN);

		JPanel inputPanel = new JPanel();
		inputPanel.add(label1);
		inputPanel.add(textAlloc);
		addButton(inputPanel, "OK", new AllocAction());
		inputPanel.add(labelNull);
		inputPanel.add(label2);
		inputPanel.add(textFree);
		addButton(inputPanel, "OK", new FreeAction());

		panel.setLayout(new BorderLayout());
		panel.add(resultPanel, BorderLayout.NORTH);
		panel.add(inputPanel, BorderLayout.SOUTH);
		return panel;
	}

	//ѡ�����������㷨
	private class FirstAction implements ActionListener {
		public void actionPerformed(ActionEvent event) {
			System.out.println("First");
			//��ť�仯
			firstButton.setBackground(Color.BLUE);
			bestButton.setBackground(null);
			//����������
			dispatcher = new FirstDispatcher();
			useList = dispatcher.getUseList();
			freeList = dispatcher.getFreeList();
			//ID����������
			Utility.resetDispatchID();
			//��ʾ����
			displayPanel.repaint();
			textAlloc.setText("100");
			textFree.setText("1");
			labelResult.setText("Idle");
		}
	}

	//ѡ�����������㷨
	private class BestAction implements ActionListener {
		public void actionPerformed(ActionEvent event) {
			System.out.println("Best");
			//��ť�仯
			bestButton.setBackground(Color.BLUE);
			firstButton.setBackground(null);
			//����������
			dispatcher = new BestDispatcher();
			useList = dispatcher.getUseList();
			freeList = dispatcher.getFreeList();
			//ID����������
			Utility.resetDispatchID();
			//��ʾ����
			displayPanel.repaint();
			textAlloc.setText("100");
			textFree.setText("1");
			labelResult.setText("Idle");
		}
	}

	//ȷ������
	private class AllocAction implements ActionListener {
		public void actionPerformed(ActionEvent event) {
			String strLen = textAlloc.getText();
			if (strLen.length() == 0)
				return;

			int len;
			textAlloc.setText("");

			try {
				if ((len = Integer.parseInt(strLen)) == 0) {
					return;
				}
				int dispatchID;
				if ((dispatchID = dispatcher.alloc(len)) != -1)
				{
				    labelResult.setText("Alloc " + len + "k for process " + dispatchID);
				}
				else 
				{
					labelResult.setText("Not enough memory for allocing " + len + "k");
				}
				displayPanel.repaint();
			} catch (NumberFormatException e) {
				labelResult.setText("Please input integer!");
			}
		}
	}

	//ȷ���ͷ�
	private class FreeAction implements ActionListener {
		public void actionPerformed(ActionEvent event) {
			String strID = textFree.getText();
			if (strID.length() == 0)
				return;
			int dispatchID;
			textFree.setText("");

			try {
				dispatchID = Integer.parseInt(strID);
				if ((dispatcher.free(dispatchID)) == true)
				{
					labelResult.setText("Free process " + dispatchID);
				}
				else 
				{
					labelResult.setText("process " + dispatchID + " doesn't exist!");
				}
				displayPanel.repaint();
			} catch (NumberFormatException e) {
				labelResult.setText("input integer!");
			}
		}
	}

	private JButton addButton(
		JPanel panel,
		String label,
		ActionListener listener) {
		JButton button = new JButton(label);
		button.addActionListener(listener);
		panel.add(button);
		return button;
	}

	/*
	 * ��ʾ�ڴ�״���Ϳ��з�����������
	 */
	class DisplayMemoryPanel extends JPanel {
		public DisplayMemoryPanel() {
		}

		public void paintComponent(Graphics g) {
			super.paintComponent(g);

			Graphics2D g2 = (Graphics2D) g;
			g2.setColor(Color.BLACK);
			Rectangle2D rect1 =
				new Rectangle2D.Double(
					Utility.SPACE,
					Utility.SPACE,
					Utility.MEMORYWIDTH,
					Utility.initMemory * Utility.KILO);
			g2.draw(rect1);
			Rectangle2D rect2 =
				new Rectangle2D.Double(
					Utility.MEMORYWIDTH + Utility.SPACE * 2,
					Utility.SPACE,
					Utility.MEMORYWIDTH,
					Utility.initMemory * Utility.KILO);
			g2.draw(rect2);

			paintMemory(g);
			paintFreeList(g);
		}

		public void paintMemory(Graphics g) {
			SpaceNode node;
			for (int i = 0; i < useList.size(); i++) {
				node = (SpaceNode) useList.get(i);
				paintNode(
					true,
					g,
					node.getAddrFrom(),
					node.getAddrFrom(),
					node.getLen(),
					node.getDispatchID(),
					Color.YELLOW);
			}
			for (int i = 0; i < freeList.size(); i++) {
				node = (SpaceNode) freeList.get(i);
				paintNode(
					true,
					g,
					node.getAddrFrom(),
					node.getAddrFrom(),
					node.getLen(),
					node.getDispatchID(),
					Color.WHITE);
			}
		}

		public void paintFreeList(Graphics g) {
			SpaceNode node;
			int addrFrom = 0;
			for (int i = 0; i < freeList.size(); i++) {
				node = (SpaceNode) freeList.get(i);
				paintNode(
					false,
					g,
					addrFrom,
					node.getAddrFrom(),
					node.getLen(),
					0,
					Color.WHITE);
				addrFrom += node.getLen();
			}
		}

		public void paintNode(boolean isLeft, //�Ƿ��ڻ��ܵ��ڴ�״��
		Graphics g,
			int drawAddrFrom,
			int actualAddrFrom,
			int len,
			int dispatchID,
			Color color) {
			double x, y, width, length;
			if (isLeft == true) {
				x = Utility.SPACE;
			} else {
				x = Utility.MEMORYWIDTH + Utility.SPACE * 2;
			}

			y = Utility.SPACE + drawAddrFrom * Utility.KILO;
			width = Utility.MEMORYWIDTH;
			length = len * Utility.KILO;

			Graphics2D g2 = (Graphics2D) g;
			Rectangle2D rect1 = new Rectangle2D.Double(x, y, width, length);
			g2.setColor(color);
			g2.fill(rect1);
			g2.setColor(Color.BLACK);
			g2.draw(rect1);

			String strDraw;
			if (dispatchID == 0) {
				strDraw = "[Free Node] ";
			} else {
				strDraw = "[Process " + dispatchID + "] ";
			}
			g2.drawString(
				strDraw + "Address: " + actualAddrFrom + "  Length: " + len,
				(int) x,
				(int) y + len / 3);
		}
	}
}
