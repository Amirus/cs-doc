/**
�ļ����� tiger.idc
��;  �� ͨ�������������ó�������ͼ�ҳ�Σ�յĵ��á�
˵��  �� ���HP-UX PAָ�
��д  �� watercloud
����  �� 2003-3-1
**/


/**
��������void func1arg(string,string,long,long,string)  
˵����  ��ͼ����ȫ����__funcName�ĵ���ʱʹ��_reg�Ĵ����ĵ����龰��
ǿ����  ����Ϊ����Ŀ�Ķ�__debugFlag>10ʱ�������⴦����ʱ__funcNameΪһ����ַ����ʾ�Ӹ�
        ��ַ���ݣ��ڻ���·���ϲ鿴_reg��ʹ�����,��ʱ_debugFlag=__debugFlag-10 ,researchFlag =1��
**/
static func1arg(__funcName,_reg,_deepLen,__debugFlag,_aligStr)
{

	auto type,addr,addrX,funcAddr,_funcName,funcName,_debugFlag;
	auto n,tmp,tmpStr,tmpStr1,tmpStr2,tmpAddr,tmpReg,value;
	auto researchFlag;

	if(_deepLen <=0)
		return;
	

	if(__debugFlag > 10) //����������,���ݷ���ĳ���Ĵ���ʹ�������
	{
		researchFlag = 1;
		_debugFlag = __debugFlag - 10;
		tmpStr=GetFunctionName(__funcName);
		if(tmpStr== "")
			return;
		funcAddr=__funcName;
		if(GetMnem(funcAddr) == "call")
			_funcName=GetOpnd(funcAddr,0);
		else
			_funcName="";
		funcAddr=funcAddr+4;
	}
	else //ͨ������ȫ�Ķ��ض������ض���������ʹ�����
	{
		researchFlag=0;
		_funcName=__funcName;
		_debugFlag = __debugFlag;
		funcAddr=LocByName(_funcName);
		if(funcAddr == BADADDR)
			return;
		if(_reg != "%r26" && _reg != "%r25" && _reg != "%r24" && _reg !="%r23")
			return;
	}

	n=0;
	addrX=RfirstB(funcAddr);
	while(addrX != BADADDR)
	{
		n ++;
		if(researchFlag == 0 ) //ͨ�����
		{
			type = XrefType(); 
			if(type != fl_CF && type !=fl_CN) //����������Ͳ��Ǻ������������
			{
				addrX = RnextB(funcAddr,addrX);
				n--;
				continue;
			}
		}
		else //�����������������������ѭ�����˳�
		{
			if(n == 2)
				break;
		}

		//׷��������Դ
		value = getDataResEx(addrX +4,_reg,_debugFlag);
		funcName = GetFunctionName(addrX);
		type = getDRType(value);

		tmpStr = getString(value);
		//������ǵ���ģʽ,�Ҵ�Ϊ��֪�����
		if( tmpStr !="" && _debugFlag <=1) 
		{
			addrX = RnextB(funcAddr,addrX);
			n--;
			continue;
		}

		//���������ʾ���Ժ��������ٴλ��ݵݹ�
		Message("%s%-2i %x\t %s:\t call %s() %s=",_aligStr,n,addrX,funcName,_funcName,_reg);

		if( isDRValue(value) )
		{
			Message("%x:\"%s\"\n",value,tmpStr);
		}
		else if(type == "ARG" )
		{
			Message("%s\n",getDRStr0_1(value));
			func1arg(funcName,getDRReg(value),_deepLen-1,_debugFlag,_aligStr+"  ");
		}
		else if(type == "RET" )
		{
			tmpStr = getDRFunName(value);
			if(tmpStr == "")
				Message("%s\n",getDRStr0_1(value));
			else
				Message("RET %s()\n",tmpStr);
		}
		else if(type == "STA" )
		{
			tmpStr = getDRStrFunName(value);
			if(tmpStr == "")
				Message("%s\n",getDRStr0_1(value));
			else
				Message("RET %s()\n",tmpStr);
		}
		else
		{
			if(_debugFlag >=1 )
				Message("%s\n",getDRStr0_1(value));
			else
				Message("û���ҵ�\n");
		}

		addrX = RnextB(funcAddr,addrX);
	}
} //end of func1arg

/**
��������void func2arg(string,long,long,string)  
˵����  ������������ĺ���������Ҫ��memcpy,stcat,strcpy . . .����ͼ����ȫ����_funcName�ĵ���ʱ���龰��
**/
static func2arg(_funcName,_deepLen,_debugFlag,_aligStr)
{
	auto n,addr,addrX,typeX,typeSrc,typeDst,nowFunc,valueSrc,valueDst,strSrc,strDst;
	auto desSrc,desDst;

	if(_deepLen <= 0 )
		return;

	addr=LocByName(_funcName);
	if(addr == BADADDR)
		return;

	n=0;
	addrX=RfirstB(addr);
	while(addrX != BADADDR )
	{
		n++;

		typeX = XrefType(); 
		if(typeX != fl_CF && typeX !=fl_CN) //����������Ͳ��Ǻ������������
		{
			addrX = RnextB(addr,addrX);
			n--;
			continue;
		}

		nowFunc = GetFunctionName(addrX);

		//��ȡ����Դ
		valueSrc = getDataResEx(addrX +4,"%r25",_debugFlag);
		typeSrc = getDRType(valueSrc);
		valueDst=getDataResEx(addrX+4,"%r26",_debugFlag);
		typeDst=getDRType(valueDst);

		//�������������Ϣ
		strSrc="";
		strDst="";

		if( isDRValue(valueSrc) )
			strSrc = getString(valueSrc);
		else if(typeSrc == "RET")
			desSrc= typeSrc +" " + getDRFunName(valueSrc) + "()";
		else if(typeSrc == "ARG")
			desSrc= getDRStr0_1(valueSrc);
		else
		{
			if(_debugFlag >=1)
				desSrc = getDRStr0_1(valueSrc);
			else
				desSrc=typeSrc;
		}

		if( isDRValue(valueDst) )
			strDst = getString(valueDst);
		else if(typeDst == "RET")
			desDst =typeDst + " " + getDRFunName(valueDst) + "()";
		else if(typeDst == "ARG")
			desDst= getDRStr0_1(valueDst);
		else
		{
			if(_debugFlag >=1)
				desDst = getDRStr0_1(valueDst);
			else
				desDst=typeDst;
		}

		//���Դ��֪���ҷǵ���ģʽ�����
		if( isDRValue(valueSrc) && strSrc !="" && _debugFlag <=1)
		{
			addrX = RnextB(addr,addrX);
			n--;
			continue;
		}


		//��ʼ��ӡ�����Ϣ

		Message("%s%-2i %x\t %s:\t call %s(",_aligStr,n,addrX,nowFunc,_funcName);

		if(isDRValue(valueDst) )
			Message("%x:\"%s\"," ,valueDst,strDst);
		else 
			Message("%s,",desDst);

		if(isDRValue(valueSrc) )
			Message("%x:\"%s\"" ,valueSrc,strSrc);
		else 
			Message("%s",desSrc);

		Message(")\n");

		if(typeDst== "ARG")
			func1arg(nowFunc,getDRReg(valueDst),_deepLen-1,_debugFlag,_aligStr+"  ");
		if(typeSrc== "ARG")
			func1arg(nowFunc,getDRReg(valueSrc),_deepLen-1,_debugFlag,_aligStr+"  ");
		 
		addrX = RnextB(addr,addrX);

	}
	Message("\n");
} //end of func2arg



/**
��������void funcNarg(string,long,long,string)  
˵����  ������������ĺ���������Ҫ��strncpy. . .����ͼ����ȫ����_funcName�ĵ���ʱ���龰��
**/
static funcNarg(_funcName,_deepLen,_debugFlag,_aligStr)
{
	auto n,addr,addrX,typeX,typeSrc,typeDst,typeN,typeR23,nowFunc,valueSrc;
	auto valueDst,argNumber,tmpStr,index,frameSize;
	auto valueN,valueR23,strSrc,strDst,strN,strR23,desSrc,desDst,desN,desR23;

	if(_deepLen <= 0 )
		return;

	addr=LocByName(_funcName);
	if(addr == BADADDR)
		return;

	n=0;
	addrX=RfirstB(addr);
	while(addrX != BADADDR )
	{
		n++;

		typeX = XrefType(); 
		if(typeX != fl_CF && typeX !=fl_CN) //����������Ͳ��Ǻ������������
		{
			addrX = RnextB(addr,addrX);
			n--;
			continue;
		}

		valueSrc =0;  valueDst=0;  valueN=0;
		argNumber=0;
		strSrc="";    strDst="";
		strN=""; 

		nowFunc = GetFunctionName(addrX);

		//׷��3��������������Դ
		valueSrc = getDataResEx(addrX +4,"%r25",_debugFlag);
		typeSrc = getDRType(valueSrc);
		valueDst=getDataResEx(addrX+4,"%r26",_debugFlag);
		typeDst=getDRType(valueDst);
		valueN = getDataResEx(addrX +4,"%r24",_debugFlag);
		typeN = getDRType(valueN);

		//���Ŀ��Ϊջ����֪������󳤶�N����N<FrameSize�ҷǵ���ģʽ�����
		frameSize = GetFrameSize(addrX);
		if( ( frameSize <=0 || valueN <=frameSize ) &&  typeDst == "STA" && isDRValue(valueN)  && _debugFlag <= 1 )
		{
			addrX = RnextB(addr,addrX);
			n--;
			continue;
		}

		//����Դ����Ϣ
		if( isDRValue(valueSrc) )
			strSrc = getString(valueSrc);
		else if(typeSrc == "RET")
			desSrc= typeSrc +" " + getDRFunName(valueSrc) + "()";
		else if(typeSrc == "ARG")
			desSrc= getDRStr0_1(valueSrc);
		else
		{
			if(_debugFlag >=1)
				desSrc = getDRStr0_1(valueSrc);
			else
				desSrc=typeSrc;
		}

		//���ԴΪ��֪���ҷǵ���ģʽ�����
		if( isDRValue(valueSrc) && strSrc !="" && _debugFlag <= 1 )
		{
			addrX = RnextB(addr,addrX);
			n--;
			continue;
		}

		//����Ŀ����Ϣ
		if( isDRValue(valueDst) )
			strDst = getString(valueDst);
		else if(typeDst == "RET")
			desDst =typeDst + " " + getDRFunName(valueDst) + "()";
		else if(typeDst == "ARG")
			desDst= getDRStr0_1(valueDst);
		else
		{
			if(_debugFlag >=1)
				desDst = getDRStr0_1(valueDst);
			else
				desDst=typeDst;
		}


		//����N��Ϣ
		if( isDRValue(valueN) )
			strN = getString(valueN);
		else if(typeN == "RET")
			desN =typeN + " " + getDRFunName(valueN) + "()";
		else if(typeN == "ARG")
			desN= getDRStr0_1(valueN);
		else
		{
			if(_debugFlag >=1)
				desN = getDRStr0_1(valueN);
			else
				desN=typeN;
		}


		//��ʾ�����Ϣ
		Message("%s%-2i %x\t %s:\t call %s(",_aligStr,n,addrX,nowFunc,_funcName);

		if(isDRValue(valueDst) )
			Message("%x:\"%s\"," ,valueDst,strDst);
		else 
			Message("%s,",desDst);

		if(isDRValue(valueSrc) )
			Message("%x:\"%s\"," ,valueSrc,strSrc);
		else 
			Message("%s,",desSrc);

		if(isDRValue(valueN) )
			Message("%x" ,valueN);
		else 
			Message("%s",desN);

		if( _debugFlag >=2 && typeDst=="STA")
			Message(") ջ�ռ��С=%x\n",frameSize);
		else
			Message(")\n");


		if(typeDst== "ARG")
			func1arg(nowFunc,getDRReg(valueDst),_deepLen-1,_debugFlag,_aligStr+"  ");
		if(typeSrc== "ARG")
			func1arg(nowFunc,getDRReg(valueSrc),_deepLen-1,_debugFlag,_aligStr+"  ");
		if(typeN== "ARG")
			func1arg(nowFunc,getDRReg(valueN),_deepLen-1,_debugFlag,_aligStr+"  ");

		 
		addrX = RnextB(addr,addrX);

	}
	Message("\n");
} //end of funcNarg


/**
��������void funcMarg(string,long,long,string)  
˵����  ������������ĺ���������Ҫ��sprintf . . .����ͼ����ȫ����_funcName�ĵ���ʱ���龰��
**/
static funcMarg(_funcName,_deepLen,_debugFlag,_aligStr)
{
	auto n,addr,addrX,typeX,typeFmt,typeDst,typeR24,typeR23,nowFunc,valueFmt;
	auto valueDst,argNumber,tmpStr,index;
	auto valueR24,valueR23,strFmt,strDst,strR24,strR23,desFmt,desDst,desR24,desR23;

	if(_deepLen <= 0 )
		return;

	addr=LocByName(_funcName);
	if(addr == BADADDR)
		return;

	n=0;
	addrX=RfirstB(addr);
	while(addrX != BADADDR ) //ѭ���������жԸú����ĵ���
	{
		n++;

		typeX = XrefType(); 
		if(typeX != fl_CF && typeX !=fl_CN) //����������Ͳ��Ǻ������������
		{
			addrX = RnextB(addr,addrX);
			n--;
			continue;
		}

		valueFmt =0;  valueDst=0;  valueR24=0;  valueR23=0;
		argNumber=0;
		strFmt="";    strDst="";
		strR24="";    strR23="";

		nowFunc = GetFunctionName(addrX);

		//׷��Ŀ���ַ����ʽ������ص�������Դ
		valueFmt = getDataResEx(addrX +4,"%r25",_debugFlag);
		typeFmt = getDRType(valueFmt);
		valueDst=getDataResEx(addrX+4,"%r26",_debugFlag);
		typeDst=getDRType(valueDst);


		//�����ʽ�������Ϣ
		if( isDRValue(valueFmt) )
			strFmt = getString(valueFmt);
		else if(typeFmt == "RET")
			desFmt= typeFmt +" " + getDRFunName(valueFmt) + "()";
		else if(typeFmt == "ARG")
			desFmt= getDRStr0_1(valueFmt);
		else
		{
			if(_debugFlag >=1)
				desFmt = getDRStr0_1(valueFmt);
			else
				desFmt=typeFmt;
		}

		//����Ŀ���ַ�����Ϣ
		if( isDRValue(valueDst) )
			strDst = getString(valueDst);
		else if(typeDst == "RET")
			desDst =typeDst + " " + getDRFunName(valueDst) + "()";
		else if(typeDst == "ARG")
			desDst= getDRStr0_1(valueDst);
		else
		{
			if(_debugFlag >=1)
				desDst = getDRStr0_1(valueDst);
			else
				desDst=typeDst;
		}


		//�鿴�ж��ٸ�%s���жϻ��ж��ٸ������账��
		tmpStr=strFmt;
		index=strstr(tmpStr,"%s");
		while(index >=0)
		{
			argNumber ++;
			tmpStr=substr(tmpStr,index+2,-1);
			index=strstr(tmpStr,"%s");
		}

		//�����1������%s����ڶ�������
		if(argNumber >=1)
		{
			valueR24 = getDataResEx(addrX +4,"%r24",_debugFlag);
			typeR24 = getDRType(valueR24);

			if( isDRValue(valueR24) )
				strR24 = getString(valueR24);
			else if(typeR24 == "RET")
				desR24 =typeR24 + " " + getDRFunName(valueR24) + "()";
			else if(typeR24 == "ARG")
				desR24= getDRStr0_1(valueR24);
			else
			{
				if(_debugFlag >=1)
					desR24 = getDRStr0_1(valueR24);
				else
					desR24=typeR24;
			}
		}

		//�������������%s����ڶ�������
		if(argNumber >=2)
		{
			valueR23 = getDataResEx(addrX +4,"%r23",_debugFlag);
			typeR23 = getDRType(valueR23);

			if( isDRValue(valueR23) )
				strR23 = getString(valueR23);
			else if(typeR23 == "RET")
				desR23 =typeR23 + " " + getDRFunName(valueR23) + "()";
			else if(typeR23 == "ARG")
				desR23= getDRStr0_1(valueR23);
			else
			{
				if(_debugFlag >=1)
					desR23 = getDRStr0_1(valueR23);
				else
					desR23=typeR23;
			}
		}

		
		//���û�в����Ҹ�ʽ����֪�����
		if( (argNumber == 0 && strFmt !="" && isDRValue(valueFmt) )  && _debugFlag <=1)
		{
			addrX = RnextB(addr,addrX);
			n--;
			continue;
		}
		//�����һ�����������ò�������֪�����Ҳ��ǵ���ģʽ�����
		if( (argNumber == 1 && strR24 !="" && isDRValue(valueR24) )  && _debugFlag <=1)
		{
			addrX = RnextB(addr,addrX);
			n--;
			continue;
		}
		//�����2���������Ҳ���������֪�����Ҳ��ǵ���ģʽ�����
		if((argNumber ==2 && strR24 != "" && strR23 != ""&&isDRValue(valueR24)&&isDRValue(valueR23)) && _debugFlag <=1)
		{
			addrX = RnextB(addr,addrX);
			n--;
			continue;
		}


		//��ʼ��ӡ�����Ϣ
		if(_debugFlag < 2)
		{
		}

		Message("%s%-2i %x\t %s:\t call %s(",_aligStr,n,addrX,nowFunc,_funcName);

		if(isDRValue(valueDst) )
			Message("%x:\"%s\"," ,valueDst,strDst);
		else 
			Message("%s,",desDst);

		if(isDRValue(valueFmt) )
			Message("%x:\"%s\"" ,valueFmt,strFmt);
		else 
			Message("%s",desFmt);

		if(argNumber >=1)
		{
			if(isDRValue(valueR24) )
				Message(",%x:\"%s\"," ,valueR24,strR24);
			else 
				Message(",%s,",desR24);
		}

		if(argNumber >=2)
		{
			if(isDRValue(valueR23) )
				Message("%x:\"%s\"," ,valueR23,strR23);
			else 
				Message("%s,",desR23);
		}

		if(argNumber >=3)
			Message(" . . . ");

		Message(")\n");


		if(typeDst== "ARG")
			func1arg(nowFunc,getDRReg(valueDst),_deepLen-1,_debugFlag,_aligStr+"  ");
		if(typeFmt== "ARG")
			func1arg(nowFunc,getDRReg(valueFmt),_deepLen-1,_debugFlag,_aligStr+"  ");
		if(typeR24== "ARG")
			func1arg(nowFunc,getDRReg(valueR24),_deepLen-1,_debugFlag,_aligStr+"  ");
		if(typeR23== "ARG")
			func1arg(nowFunc,getDRReg(valueR23),_deepLen-1,_debugFlag,_aligStr+"  ");

		 
		addrX = RnextB(addr,addrX);

	}
	Message("\n");
} //end of funcMarg

/**
��������void funcSnprintf(string,long,long,string)  
˵����  ����snprintf . . .����ͼ����ȫ����_funcName�ĵ���ʱ���龰��
**/
static funcSnprintf(_funcName,_deepLen,_debugFlag,_aligStr)
{
	auto n,addr,addrX,typeX,typeFmt,typeDst,typeN,typeR23,nowFunc,valueFmt;
	auto valueDst,argNumber,tmpStr,index,frameSize;
	auto valueN,valueR23,strFmt,strDst,strN,strR23,desFmt,desDst,desN,desR23;

	if(_deepLen <= 0 )
		return;

	addr=LocByName(_funcName);
	if(addr == BADADDR)
		return;

	n=0;
	addrX=RfirstB(addr);
	while(addrX != BADADDR )
	{
		n++;

		typeX = XrefType(); 
		if(typeX != fl_CF && typeX !=fl_CN) //����������Ͳ��Ǻ������������
		{
			addrX = RnextB(addr,addrX);
			n--;
			continue;
		}

		valueFmt =0;  valueDst=0;  valueN=0;  valueR23=0;
		argNumber=0;
		strFmt="";    strDst="";
		strN="";    strR23="";

		nowFunc = GetFunctionName(addrX);


		//׷��Ŀ���ַ�������������N����ʽ��������Դ
		valueFmt = getDataResEx(addrX +4,"%r24",_debugFlag);
		typeFmt = getDRType(valueFmt);
		valueDst=getDataResEx(addrX+4,"%r26",_debugFlag);
		typeDst=getDRType(valueDst);
		valueN = getDataResEx(addrX +4,"%r25",_debugFlag);
		typeN = getDRType(valueN);

		frameSize = GetFrameSize(addrX); //���ջ��С


		//�����ʽ�������Ϣ
		if( isDRValue(valueFmt) )
			strFmt = getString(valueFmt);
		else if(typeFmt == "RET")
			desFmt= typeFmt +" " + getDRFunName(valueFmt) + "()";
		else if(typeFmt == "ARG")
			desFmt= getDRStr0_1(valueFmt);
		else
		{
			if(_debugFlag >=1)
				desFmt = getDRStr0_1(valueFmt);
			else
				desFmt=typeFmt;
		}

		//����Ŀ���ַ�����Ϣ
		if( isDRValue(valueDst) )
			strDst = getString(valueDst);
		else if(typeDst == "RET")
			desDst =typeDst + " " + getDRFunName(valueDst) + "()";
		else if(typeDst == "ARG")
			desDst= getDRStr0_1(valueDst);
		else
		{
			if(_debugFlag >=1)
				desDst = getDRStr0_1(valueDst);
			else
				desDst=typeDst;
		}

		//���������
		tmpStr=strFmt;
		index=strstr(tmpStr,"%s");
		while(index >=0)
		{
			argNumber ++;
			tmpStr=substr(tmpStr,index+2,-1);
			index=strstr(tmpStr,"%s");
		}


		//���Ŀ����ջ����N<ջ֡��С,���ҷǵ���ģʽ,�Ҹ�ʽ��Ϊ��֪�����
		if((frameSize <=0 || valueN <=frameSize ) && isDRValue(valueN) && typeDst=="STA" && strFmt!="" && _debugFlag <=2 )
		{
			addrX = RnextB(addr,addrX);
			n--;
			continue;
		}

		//����N�����Ϣ
		if( isDRValue(valueN) )
			strN = getString(valueN);
		else if(typeN == "RET")
			desN =typeN + " " + getDRFunName(valueN) + "()";
		else if(typeN == "ARG")
			desN= getDRStr0_1(valueN);
		else
		{
			if(_debugFlag >=1)
				desN = getDRStr0_1(valueN);
			else
				desN=typeN;
		}

		//�����1������%s�����1������
		if(argNumber >=1)
		{
			valueR23 = getDataResEx(addrX +4,"%r23",_debugFlag);
			typeR23 = getDRType(valueR23);

			if( isDRValue(valueR23) )
				strR23 = getString(valueR23);
			else if(typeR23 == "RET")
				desR23 =typeR23 + " " + getDRFunName(valueR23) + "()";
			else if(typeR23 == "ARG")
				desR23= getDRStr0_1(valueR23);
			else
			{
				if(_debugFlag >=1)
					desR23 = getDRStr0_1(valueR23);

				else
					desR23=typeR23;
			}
		}


		//���û�в����Ҹ�ʽ����֪�����
		if( (argNumber == 0 && strFmt !="" && isDRValue(valueFmt) )  && _debugFlag <=1)
		{
			addrX = RnextB(addr,addrX);
			n--;
			continue;
		}
		//�����һ�����������ò�������֪�����Ҳ��ǵ���ģʽ�����
		if( (argNumber == 1 && strR23 !="" && isDRValue(valueR23) )  && _debugFlag <=1)
		{
			addrX = RnextB(addr,addrX);
			n--;
			continue;
		}

		//��ʼ��ʾ�����Ϣ
		Message("%s%-2i %x\t %s:\t call %s(",_aligStr,n,addrX,nowFunc,_funcName);

		if(isDRValue(valueDst) )
			Message("%x:\"%s\"," ,valueDst,strDst);
		else 
			Message("%s,",desDst);

		if(isDRValue(valueN) )
			Message("%x," ,valueN);
		else 
			Message("%s,",desN);

		if(isDRValue(valueFmt) )
			Message("%x:\"%s\"" ,valueFmt,strFmt);
		else 
			Message("%s",desFmt);

		if(argNumber >=1)
		{
			if(isDRValue(valueR23) )
				Message(",%x:\"%s\"" ,valueR23,strR23);
			else 
				Message(",%s",desR23);
		}

		if(argNumber >=2)
			Message(" . . . ");

		Message(")\n");


		//�����Ҫ����Ѱ����Ӧ���ò��������������Ӧ����
		if(typeDst== "ARG")
			func1arg(nowFunc,getDRReg(valueDst),_deepLen-1,_debugFlag,_aligStr+"  ");
		if(typeN== "ARG")
			func1arg(nowFunc,getDRReg(valueN),_deepLen-1,_debugFlag,_aligStr+"  ");
		if(typeFmt== "ARG")
			func1arg(nowFunc,getDRReg(valueFmt),_deepLen-1,_debugFlag,_aligStr+"  ");
		if(typeR23== "ARG")
			func1arg(nowFunc,getDRReg(valueR23),_deepLen-1,_debugFlag,_aligStr+"  ");

		 
		addrX = RnextB(addr,addrX);

	}
	Message("\n");
} //end of funcSnprintf


/**
��������void funcFmt(string,long,long,string)  
˵����  ����xxprintf��ʽ������ . . .����ͼ����ȫ����_funcName�ĵ���ʱ���龰��
**/
static funcFmt(_funcName,_deepLen,_debugFlag,_aligStr)
{
	auto n,addr,addrX,typeX,typeFmt,nowFunc,valueFmt;
	auto strFmt,desFmt,regFmt;

	if(_deepLen <= 0 )
		return;

	addr=LocByName(_funcName);
	if(addr == BADADDR)
		return;

	if( _funcName =="vsnprintf" || _funcName == "snprintf")
		regFmt="%r24";
	else if(_funcName =="fprintf" || _funcName=="vfprintf" ||_funcName=="sprintf")
		regFmt="%r25";
	else //printf  vprintf
		regFmt="%r26";

	n=0;
	addrX=RfirstB(addr);
	while(addrX != BADADDR )
	{
		n++;

		typeX = XrefType(); 
		if(typeX != fl_CF && typeX !=fl_CN) //����������Ͳ��Ǻ������������
		{
			addrX = RnextB(addr,addrX);
			n--;
			continue;
		}

		valueFmt =0;  
		strFmt="";
		desFmt="";

		nowFunc = GetFunctionName(addrX);

		//׷�ٸ�ʽ��������Դ
		valueFmt = getDataResEx(addrX +4,regFmt,_debugFlag);
		typeFmt = getDRType(valueFmt);


		//�����ʽ�������Ϣ
		if( isDRValue(valueFmt) )
			strFmt = getString(valueFmt);
		else if(typeFmt == "RET")
			desFmt= typeFmt +" " + getDRFunName(valueFmt) + "()";
		else if(typeFmt == "ARG")
			desFmt= getDRStr0_1(valueFmt);
		else
		{
			if(_debugFlag >=1)
				desFmt = getDRStr0_1(valueFmt);
			else
				desFmt=typeFmt;
		}

		//�����ʽ����֪���Ҳ��ǵ���ģʽ�����
		if( strFmt !="" && isDRValue(valueFmt)  && _debugFlag <=1)
		{
			addrX = RnextB(addr,addrX);
			n--;
			continue;
		}

		//��ʼ��ʾ�����Ϣ
		Message("%s%-2i %x\t %s:\t call %s( ��ʽ��Ϊ��",_aligStr,n,addrX,nowFunc,_funcName);

		if(isDRValue(valueFmt) )
			Message("%x:\"%s\"" ,valueFmt,strFmt);
		else 
			Message("%s",desFmt);

		Message(" )\n");


		//�����Ҫ����Ѱ����Ӧ���ò��������������Ӧ����
		if(typeFmt== "ARG")
			func1arg(nowFunc,getDRReg(valueFmt),_deepLen-1,_debugFlag,_aligStr+"  ");
		 
		addrX = RnextB(addr,addrX);

	}
	Message("\n");
} //end of funcFmt


