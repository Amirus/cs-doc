import java.awt.Color;
import java.awt.Container;
import java.awt.FlowLayout;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.geom.Rectangle2D;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Map;
import java.util.SortedMap;
import java.util.TreeMap;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;


//	 ����ģ���ڴ�������
public class MemoryAllocTest {

	public MemoryAllocTest() {
		super();

	}
	
	//	������
	public static void main(String[] args) {
		//	����ܵ�ʵ����
		MemoryAllocFrame frame = new MemoryAllocFrame("MemoryAlloc");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		frame.setLocation(300, 10);
		frame.show();

	}
}

//	����ܵ�ʵ����
class MemoryAllocFrame extends JFrame {
	//	���õĴ��ڳ��Ϳ�
	public static final int DEFAULT_WIDTH = 500;
	public static final int DEFAULT_HEIGHT = 730;
	// 	�������Ĺ��캯��
	public MemoryAllocFrame(String str) {
		super(str);
		setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
		Container contentPane = getContentPane();
		//	������ڵ�Panel���ʵ����
		MemoryAllocPanel panel = new MemoryAllocPanel();
		contentPane.add(panel);
	}
}

//	������ʾ��ģ����ڴ�������״ͼ����
class GraphicPanel extends JPanel {
	//	��״ͼ��λ�ú� ����
	public static final int RECT_WIDTH = 100;
	public static final int RECT_HEIGHT = 640;
	public static final int INIT_LOCA_X = 50;
	public static final int INIT_LOCA_Y = 30;

	public SortedMap allocatedMap = new TreeMap();
	public SortedMap allocatedStartMap = new TreeMap();

	public GraphicPanel() {

	}
	
	//	���ڽ��շ��������ڴ������Ϣ�����ܺ�����ʾ
	public void setAllocatedMap(SortedMap map, SortedMap startMap) {
		allocatedMap = map;
		allocatedStartMap = startMap;

		Iterator it = allocatedStartMap.keySet().iterator();
		//		while(it.hasNext())
		{
			//			System.out.println(allocatedStartMap.get(it.next()));
		}
	}
	//	������״ͼ������ģ���ڴ�ķ���
	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		Graphics2D g2 = (Graphics2D) g;
		
		//	�����
		Rectangle2D rect =
			new Rectangle2D.Double(
				INIT_LOCA_X,
				INIT_LOCA_Y,
				RECT_WIDTH,
				RECT_HEIGHT);
		g2.draw(rect);
		//	���������ֵı��
		for (int i = 0; i < 10; ++i) {
			String str = "" + i * 80 + "K";
			g2.drawLine(
				INIT_LOCA_X + RECT_WIDTH,
				INIT_LOCA_Y + i * 80,
				INIT_LOCA_X + RECT_WIDTH + 10,
				INIT_LOCA_Y + i * 80);
			g2.drawString(str, 170, i * 80 + INIT_LOCA_Y);
		}

		Iterator itAllocatedMap = allocatedMap.keySet().iterator();
		while (itAllocatedMap.hasNext()) {
			Double StartRect = (Double) itAllocatedMap.next();
			Double LengthOfRect = (Double) (allocatedMap.get(StartRect));

			double startRect = StartRect.doubleValue();
			double length = LengthOfRect.doubleValue();

			double d = INIT_LOCA_Y + startRect;
			
			//	��ģ����ڴ����
			Rectangle2D allocRect =
				new Rectangle2D.Double(
					INIT_LOCA_X,
					INIT_LOCA_Y + startRect,
					RECT_WIDTH,
					length);

			g2.setPaint(Color.red);
			g2.fill(allocRect);		//	���
			g2.setPaint(Color.black);
			g2.draw(allocRect);

			Iterator it = allocatedStartMap.keySet().iterator();
			//				while(it.hasNext())
			{
				//					System.out.println(allocatedStartMap.get(it.next()));
			}
			
			//	����ʶ����ID��ProcessID
			if (length >= 10) {
				String str =
					""
						+ ((Integer) allocatedStartMap.get(StartRect)).intValue();
				g2.setPaint(Color.black);
				g2.drawString(
					str,
					INIT_LOCA_X + 45,
					(int) (INIT_LOCA_Y + startRect + length / 2 + 5));
			}
		}

	}
}

//	��ʾ������Ϣ���࣬������ʾ��ӳÿһ���Ĳ������
class DispPanel extends JPanel {
	private JTextArea textArea;
	private JScrollPane scrollPane;

	//	����Ĺ��캯��
	public DispPanel() {

		setLayout(new GridLayout(2, 1));

		add(new JLabel(""));
		
		textArea = new JTextArea(3, 40); //3��40��
		textArea.setEditable(false);
		scrollPane = new JScrollPane(textArea);
		add(scrollPane);

	}

	//	��һЩ��ʾ��Ϣ
	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		Graphics2D g2 = (Graphics2D) g;

		String str1 = "�ڴ����֮����ҵ";
		String str2 = "��̬�������䷽ʽ��ģ��";
		String str3 = "1.  ���������㷨";
		String str4 = "2.  ��������㷨";
		g2.drawString(str1, 50, 50);
		g2.drawString(str2, 35, 75);
		g2.drawString(str3, 50, 100);
		g2.drawString(str4, 50, 125);
	}
	
	//	�����ı������ݣ���ʾÿ���Ĳ������
	public void updateTextArea(String msg) {
		textArea.append(msg);
	}
	
	public void upDateDisp(
		SortedMap allocatedMap,
		SortedMap allocatedStartMap) {
		Iterator it = allocatedMap.keySet().iterator();

		while (it.hasNext()) {
			Double d = (Double) it.next();
			//			System.out.println(""+ d + "     "+allocatedStartMap.get(d));
		}
	}
}

//	�����ڴ������ͷŵ���
class OperatorPanel extends JPanel {
	private JTextField allocField = new JTextField(10);
	private JTextField freeField = new JTextField(10);
	private ControlPanel controlPanel;
	private DispPanel dispPanel;
	private JComboBox methodSelectionCombo;
	
	//	����Ĺ��캯���������������Ĳ���
	public OperatorPanel(ControlPanel panel, DispPanel disp) {
		controlPanel = panel;
		dispPanel = disp;

		setLayout(new GridLayout(3, 1));
		
		//	ѡ������㷨�������б��
		methodSelectionCombo = new JComboBox();
		methodSelectionCombo.setEditable(false);
		methodSelectionCombo.addItem("���������㷨"); //������Ͽ�����
		methodSelectionCombo.addItem("��������㷨");

		JPanel selectPanel = new JPanel();
		selectPanel.setLayout(new GridLayout(3, 1));
		selectPanel.add(new JLabel(""));
		selectPanel.add(new JLabel("���������㷨��ѡ��"));
		selectPanel.add(methodSelectionCombo);

		ActionListener comboListener = new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				int selectIndex = methodSelectionCombo.getSelectedIndex();
				System.out.println(selectIndex);

				controlPanel.setAllocMethod(selectIndex);
			}
		};
		methodSelectionCombo.addActionListener(comboListener);
		
		//	������ͷ��ڴ� �İ�ť
		JButton allocButton = new JButton("Alloc");
		JButton freeButton = new JButton("Free");

		JPanel buttonPanel = new JPanel();
		buttonPanel.setLayout(new GridLayout(3, 1));

		JPanel p1 = new JPanel();
		p1.setLayout(new FlowLayout());
		p1.add(new JLabel("�ڴ��С"));
		p1.add(allocField);
		p1.add(allocButton);

		JPanel p2 = new JPanel();
		p2.setLayout(new FlowLayout());
		p2.add(new JLabel("���̱��"));
		p2.add(freeField);
		p2.add(freeButton);

		//	�����ڴ水ť�ļ�����
		ActionListener allocButtonL = new ActionListener() {
			public void actionPerformed(ActionEvent event) {

				String strInput = allocField.getText();

				allocNewMemory(strInput);

			}
		};
		allocButton.addActionListener(allocButtonL);

		//	�ͷ��ڴ水ť�ļ�����
		ActionListener freeButtonL = new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				String strInput = freeField.getText();
				freeMemory(strInput);
			}
		};
		freeButton.addActionListener(freeButtonL);

		buttonPanel.add(new JLabel(""));
		buttonPanel.add(p1);
		buttonPanel.add(p2);
		
		//	�˳���ť
		JButton exitButton = new JButton("Exit");

		ActionListener ExitL = new ActionListener() {
			public void actionPerformed(ActionEvent event) {
				System.exit(0);
			}
		};
		exitButton.addActionListener(ExitL);

		JPanel exitPanel = new JPanel();
		exitPanel.setLayout(new FlowLayout());
		exitPanel.add(exitButton);

		JPanel bottomPanel = new JPanel();
		bottomPanel.setLayout(new GridLayout(3, 1));
		bottomPanel.add(new JLabel(""));
		bottomPanel.add(exitPanel);
		bottomPanel.add(new JLabel("                 012574     �����"));

		add(selectPanel);
		add(buttonPanel);
		add(bottomPanel);

	}
	
	//	�ͷ��ڴ�Ĵ�����
	private void freeMemory(String strInput) {
		try {
			int ProcessID = Integer.parseInt(strInput.trim());
			//	System.out.println(ProcessID);
			//	��controlPanel��freeMemory(ProcessID)�����������ͷ��ڴ�
			controlPanel.freeMemory(ProcessID);

		} catch (Exception e) {
			freeField.setText("");

			dispPanel.updateTextArea(
				"Error          input wrong!\n        Please input the ProcessID\n");

			System.out.println("fail");
			//	System.out.println("input wrong");
		}
	}
	//	�����ڴ�Ĵ�����
	private void allocNewMemory(String strInput) {
		try {

			double size = Double.parseDouble(strInput.trim());
			//			System.out.println("intput right");
			if (size < 0.0000001 || size > 640) {
				dispPanel.updateTextArea("Error          input wrong!\n");
				return;
			}
			//	��controlPanel��allocNewMemory(size)��������������ڴ�
			controlPanel.allocNewMemory(size);

		} catch (Exception e) {
			allocField.setText("");
			dispPanel.updateTextArea("Error          input wrong!\n");
			//			System.out.println("input wrong");
		}

	}
}

//	�����ڴ������Ϣ�Ϳ����ڴ���Ϣ�Ŀ�����
class ControlPanel extends JPanel {
	private GraphicPanel graphicPanel;
	private DispPanel dispPanel;
	//	allocatedMap������ڴ�ķ�����Ϣ��key�Ƿ����˵��ڴ����ʼ��ַ��value��key��ʼ���ǿ��ڴ�ĳ���
	private SortedMap allocatedMap = new TreeMap(); 
	
	private SortedMap allocatedStartMap = new TreeMap();
	//	freeMap����ſ����ڴ����Ϣ��key��һ������ڴ����ʼ��ַ��value ��key��ʼ�Ŀ����ڴ�ĳ���
	private SortedMap freeMap = new TreeMap();
	private ArrayList allocatedMarkArray = new ArrayList();
	private int MemoryID = 0;
	private boolean isBestAdapted = false;

	//	����Ĺ��캯��
	public ControlPanel(GraphicPanel panel) {
		graphicPanel = panel;
		setLayout(new GridLayout(2, 1));
		dispPanel = new DispPanel();
		OperatorPanel operatorPanel = new OperatorPanel(this, dispPanel);

		add(dispPanel);
		add(operatorPanel);

		Double start = new Double(0);
		Double length = new Double(640);
		freeMap.put(start, length);

	}
	
	public ArrayList getAllocatedMarkArray() {
		return allocatedMarkArray;
	}
	
	//	���������ͷ��ڴ�ĺ���
	public void freeMemory(int ProcessID) {

		// �ҵ�Ҫ�ͷŵ��ǿ��ڴ�
		Iterator it = allocatedStartMap.keySet().iterator();
		while (it.hasNext()) {
			//			System.out.println(ProcessID);
			Double startLocation = (Double) it.next();
			if (ProcessID
				== ((Integer) allocatedStartMap.get(startLocation)).intValue()) {
				//	System.out.println("aa");
				//	�ͷ��ڴ棬Ҳ���޸�ά���ڴ������Ϣ��Map
				updateMaps(startLocation, ProcessID);

				//	dispMap(allocatedMap);
				
				// 	�����˵��ڴ���������֪ͨģ������ڴ������࣬Ҫ����»���
				graphicPanel.setAllocatedMap(allocatedMap, allocatedStartMap);
				dispPanel.upDateDisp(allocatedMap, allocatedStartMap);
				graphicPanel.repaint();

				dispPanel.updateTextArea(
					"free  Successful\n        The space of Process "
						+ ProcessID
						+ " has been freed! \n");

				return;
			}

		}
		dispPanel.updateTextArea(
			"Error          input wrong!\n        Please input the ProcessID that is existed\n");

	}
	

	//	���ͷ�һ���ڴ�ʱ�������Ƿ�Ҫ�����ڵĿ����ڴ�ϲ�
	private void modifyFreeMap() {
		Iterator it = freeMap.keySet().iterator();

		SortedMap temp = new TreeMap();
		double preStartFreeLocation = -1;
		double preFreeLength = -1;
		if (it.hasNext()) {
			Double freeStart = (Double) it.next();
			Double freeLength = (Double) freeMap.get(freeStart);
			preStartFreeLocation = freeStart.doubleValue();
			preFreeLength = freeLength.doubleValue();
		}
		while (it.hasNext()) {
			Double freeStart = (Double) it.next();
			Double freeLength = (Double) freeMap.get(freeStart);

			double temp1 = preStartFreeLocation + preFreeLength;
			double temp2 = freeStart.doubleValue();
			if (Math.abs(temp1 - temp2) < 0.00001) {

				double newLength = preFreeLength + freeLength.doubleValue();

				preFreeLength = newLength;

				temp.put(
					new Double(preStartFreeLocation),
					new Double(preFreeLength));

			} else {
				temp.put(
					new Double(preStartFreeLocation),
					new Double(preFreeLength));
				//update preStartFreeLocation and preFreeLength
				preStartFreeLocation = freeStart.doubleValue();
				preFreeLength = freeLength.doubleValue();

			}

		}
		temp.put(new Double(preStartFreeLocation), new Double(preFreeLength));
		//dispMap(temp);
		freeMap = temp;
		System.out.println("After modify freeMap");
		dispMap(freeMap);

	}
	
	//	�����ڴ�ά����Ϣ��Map
	private void updateMaps(Double startLocation, int ProcessID) {
		
		Double length = (Double) allocatedMap.get(startLocation);
		//	�����ڴ������Ϣ��Map
		allocatedMap.remove(startLocation);
		allocatedStartMap.remove(startLocation);
		//		dispMap(freeMap);
		//	���¿����ڴ�ķֲ���Ϣ
		freeMap.put(startLocation, length);
		//	���ͷ�һ���ڴ�ʱ�������Ƿ�Ҫ�����ڵĿ����ڴ�ϲ�
		modifyFreeMap();

	}

	//	������������ڴ�ĺ���
	public void allocNewMemory(double size) {
		//	System.out.println("Receive command");
		//	dispMap(freeMap);
		//	���Ƿ��ܹ�ֱ�ӷ���������ڴ�
		if (setAllocatedMap(size)) {
			//	System.out.println("true");
			graphicPanel.setAllocatedMap(allocatedMap, allocatedStartMap);
			

			
			dispPanel.upDateDisp(allocatedMap, allocatedStartMap); 

			graphicPanel.repaint();
			return;
		}
		//		System.out.println("aaaaaaa");
		dispPanel.updateTextArea("No enough space to alloc\n");
		// ��û��ֱ�ӷ���Ŀ����ڴ�ʱ���Ե�ǰ���ڴ������н���
		adjustMemory();
		dispPanel.updateTextArea("After contraction\n");
		//	�������ٴγ��Է���
		if (setAllocatedMap(size)) {
			//	System.out.println("true");
			// �ٴη��䣬�ɹ��������ʾ
			graphicPanel.setAllocatedMap(allocatedMap, allocatedStartMap);

			for (int i = 0; i < allocatedMarkArray.size(); ++i) {
				//	System.out.println(allocatedMarkArray.get(i));
			}

			dispPanel.upDateDisp(allocatedMap, allocatedStartMap);  

			graphicPanel.repaint();
			return;
		}
		//	�ٴγ��Է��䣬ʧ�ܺ��ṩ��ʾ��Ϣ
		dispPanel.updateTextArea("Alloc failed   The Process is too large\n");

	}
	// �Կ����ڴ���н����ĺ���
	private void adjustMemory() {
		//System.out.println("aaaaaaa");
		//		dispMap(allocatedMap);

		SortedMap tempMap = new TreeMap();

		Iterator it = allocatedMap.keySet().iterator();
		double currentLocation = 0;
		//		dispMap(allocatedMap);
		while (it.hasNext()) {
			//System.out.println("aaaaaaa");
			Double d = (Double) it.next();
			Double length = (Double) allocatedMap.get(d);
			Double currentDouble = new Double(currentLocation);
			//	allocatedMap.remove(d);
			tempMap.put(currentDouble, length);
			currentLocation = currentLocation + length.doubleValue();

			Integer ProcessID = (Integer) allocatedStartMap.get(d);
			allocatedStartMap.remove(d);
			allocatedStartMap.put(currentDouble, ProcessID);

		}
		//		dispMap(tempMap);
		allocatedMap = tempMap;
		//		dispMap(allocatedMap);
		//		System.out.println("aaaaa");
		freeMap.clear();
		Double freeStart = new Double(currentLocation);
		freeMap.put(freeStart, new Double(640 - freeStart.doubleValue()));

		//		dispMap(freeMap);

		graphicPanel.setAllocatedMap(allocatedMap, allocatedStartMap);

		dispPanel.upDateDisp(allocatedMap, allocatedStartMap); //yao 
		//		dispMap(allocatedMap);
		graphicPanel.repaint();

		return;

	}
	
	//	�����ڴ���䷽ʽ���л�
	public void setAllocMethod(int selectIndex) {
		if (selectIndex == 0) {
			isBestAdapted = false;
		} else {
			isBestAdapted = true;
		}

	}
	
	//	ʹ�������Ӧ�㷨���ڴ���亯��
	private boolean canBeBestAdapted(double size) {
		Iterator itFreeMap = freeMap.keySet().iterator();
		double min = -1;
		Double minStart = null;
		Double StartRect = null;
		Double LengthOfRect = null;
		double freeLength = 0;
		//	���Ƿ��ܹ�ֱ�ӷ���������ڴ�
		while (itFreeMap.hasNext()) {
			StartRect = (Double) itFreeMap.next();
			LengthOfRect = (Double) (freeMap.get(StartRect));

			freeLength = LengthOfRect.doubleValue();
			if (freeLength >= size) {
				//	System.out.println(freeLength);
				if (min == -1) {
					minStart = StartRect;
					min = freeLength;
					//	System.out.println(StartRect+ "   "+freeLength);
				} else {
					if (min > freeLength) {
						minStart = StartRect;
						min = freeLength;
						//	System.out.println(StartRect+ "   "+freeLength);
					}
				}
			}
		}
		if (min != -1) {

			Double Length = new Double(size);
			allocatedMap.put(minStart, Length);

			Integer numID = new Integer(++MemoryID);
			allocatedMarkArray.add(numID);
			allocatedStartMap.put(minStart, numID);
			//	dispMap(allocatedStartMap);

			Double newStart = new Double(minStart.doubleValue() + size);
			Double newLength = new Double(min - size);
			freeMap.remove(minStart);
			freeMap.put(newStart, newLength);
			//		dispMap(freeMap);
			dispPanel.updateTextArea(
				"Alloc Successful\n        Process "
					+ MemoryID
					+ " Start from "
					+ minStart
					+ "K  Length "
					+ Length
					+ "K !\n");

			return true;
		} else {
			return false;
		}
	}
	
	//	�����ڴ����ĵ���
	private boolean setAllocatedMap(double size) {
		
		if (isBestAdapted) {
			//	���������Ӧ�㷨
			boolean result = canBeBestAdapted(size);
			return result;
		}
		
		//	����������Ӧ�㷨
		Iterator itFreeMap = freeMap.keySet().iterator();
		while (itFreeMap.hasNext()) {
			Double StartRect = (Double) itFreeMap.next();
			Double LengthOfRect = (Double) (freeMap.get(StartRect));

			double freeLength = LengthOfRect.doubleValue();
			//	System.out.println(freeLength);
			if (freeLength >= size) {
				Double Length = new Double(size);
				allocatedMap.put(StartRect, Length);

				Integer numID = new Integer(++MemoryID);
				allocatedMarkArray.add(numID);
				allocatedStartMap.put(StartRect, numID);
				//	dispMap(allocatedStartMap);

				Double newStart = new Double(StartRect.doubleValue() + size);
				Double newLength = new Double(freeLength - size);
				freeMap.remove(StartRect);
				freeMap.put(newStart, newLength);

				dispPanel.updateTextArea(
					"Alloc Successful\n        Process "
						+ MemoryID
						+ " Start from "
						+ StartRect
						+ "K  Length "
						+ Length
						+ "K !\n");
				//dispMap(allocatedMap);
				//				System.out.println("aaaaaaaaaaaaaa");
				//dispMap(freeMap);
				//				System.out.println("bbbbbbbbbbbbbb");

				return true;
			}

		}
		return false;

	}
	
	//	��ʾMap�ĸ�����
	public void dispMap(Map map) {
		Iterator it = map.keySet().iterator();
		while (it.hasNext()) {
			Double d = (Double) it.next();

			System.out.println(d + "   " + map.get(d));
		}
		System.out.println("-----------------");
	}

}

//	����ܵ���Panel��
class MemoryAllocPanel extends JPanel {
	public MemoryAllocPanel() {

		setLayout(new GridLayout(1, 2));
		GraphicPanel graphicPanel = new GraphicPanel();
		ControlPanel controlPanel = new ControlPanel(graphicPanel);

		add(graphicPanel);
		add(controlPanel);
	}

}
