#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "common.h"
#include "MFD.h"
#include "Fileopera.h"

extern vector < vector<UFD> > FileInfo;//�û��ļ�Ŀ¼
extern UFD FileInput;
extern vector<Cluster> FileCluster;
extern vector<MFD> UsrInfo;//���ļ�Ŀ¼
extern vector< vector<UOF> > FileState;//�û����ļ���
extern UOF StateInput;
extern vector< Cluster> FileCluster;
extern Cluster ClusterInput;

void do_Chmod()
{
	int temp;
	//Chmod	filename mode  
	for (int i = 0; i < FileInfo[curID].size(); i++)
	{
		if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			stringstream ss;
			ss << cmd_in.cmd_num[2];
			ss >> temp;
			FileInfo[curID][i].mode = temp;
			break;
		}
	}
	for (int i = 0; i < FileState[curID].size(); i++)
	{
		if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			FileState[curID][i].mode = temp;
			break;
		}
	}
	cout << "�޸�Ȩ�޳ɹ�!"<<endl;
}
void do_Chown()
{

	//��������û����ļ�����״̬��
	int tempID;
	for (int i = 0; i < UsrInfo.size(); i++)
	{
		if (strcmp(UsrInfo[i].usrname, cmd_in.cmd_num[2].c_str()) == 0)
		{
			tempID = i;
			break;
		}
	}
	int flag = 0;
	//�Ƿ����ͬ���ļ����Ƿ��滻��
	for (int i = 0; i < FileInfo[tempID].size(); i++)
	{
		if (strcmp(FileInfo[tempID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			flag = 1;
			break;
		}
	}
	if (!flag)
	{
		FileInfo[tempID].push_back(FileInput);
		FileState[tempID].push_back(StateInput);
		cout << "�ı��ļ�ӵ���߳ɹ���" << endl;
	}
	else
	{
		cout << "���û����Ѿ�ӵ�б����Ƶ��ļ���" << endl;
	}

	// Chown filename new_owner 
     vector<UFD>::iterator it;
	 for (it = FileInfo[curID].begin(); it != FileInfo[curID].end()&& flag == 0; it++)
	{
		if (strcmp((*it).filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			strcpy(FileInput.filename,(*it).filename);
			FileInput.length = (*it).length;
			FileInput.mode = (*it).mode;
			FileInput.addr = (*it).addr;

			FileInfo[curID].erase(it);
			break;
		}
	}

	 //ɾ��״̬��
	 vector<UOF>::iterator it_num1;
	 for (it_num1 = FileState[curID].begin(); it_num1 != FileState[curID].end(); it_num1++)
	 {
		 if (strcmp((*it_num1).filename, cmd_in.cmd_num[1].c_str()) == 0)
		 {
			 strcpy(StateInput.filename, (*it_num1).filename);
			 StateInput.mode = (*it_num1).mode;
			 StateInput.state = (*it_num1).state;
			 StateInput.read_poit = (*it_num1).read_poit;
			 StateInput.write_poit = (*it_num1).write_poit;

			 FileState[curID].erase(it_num1);
			 break;
		 }
	 }

}
void do_Mv()
{
	//Mv srcFile desFile
	for (int i = 0; i < FileInfo[curID].size(); i++)
	{
		if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			strcpy(FileInfo[curID][i].filename, cmd_in.cmd_num[2].c_str());
			break;
		}
	}

	for (int i = 0; i < FileState[curID].size(); i++)
	{
		if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			strcpy(FileState[curID][i].filename, cmd_in.cmd_num[2].c_str());
			break;
		}
	}
	cout << "�������ļ��ɹ�" << endl;
}
//void do_Write();
void doTempWrite();
void make_new_copy()
{
	//Type filename
	string Tempbuff;
	int addre;
	for (int i = 0; i < FileInfo[curID].size(); i++)
	{
		if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			addre = FileInfo[curID][i].addr;
			break;
		}
	}
	int index;
	for (int i = 0; i < FileState[curID].size(); i++)
	{
		if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			index = i;
			break;
		}
	}

	while (1)
	{
		if (FileCluster[addre].next_num == addre)
		{
			for (int i = 0; i < FileState[curID][index].write_poit; i++)
			{
				//cout << FileCluster[addre].data[i];
				Tempbuff += FileCluster[addre].data[i];
			}
			break;
		}
		else
		{
			for (int i = 0; i < 256; i++)
			{
				//cout << FileCluster[addre].data[i];
				Tempbuff += FileCluster[addre].data[i];
			}
			if (FileCluster[addre].next_num != addre)
			{
				addre = FileCluster[addre].next_num;
			}
			else
				break;
		}
	}

	//Write	filename buffer nbytes д�ļ�   ����ռ�68
	cmd_in.cmd_num[0] = "Write";
	cmd_in.cmd_num[1] = cmd_in.cmd_num[2];
	cmd_in.cmd_num[2] = Tempbuff;
	stringstream ss;
	ss << Tempbuff.length();
	string curtp;
	ss >> curtp;
	cmd_in.cmd_num[3] = curtp;
	//cout << cmd_in.cmd_num[0] << " " << cmd_in.cmd_num[1] << "  " << cmd_in.cmd_num[2] << " " << cmd_in.cmd_num[3] << endl;
	//do_Write();
	doTempWrite();

}
void do_Create();
void do_Copy()
{
	//Copy  srcFile desFile 
	int address;
	for (int i = 0; i < FileInfo[curID].size(); i++)
	{
		if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			address = FileInfo[curID][i].addr;
			break;
		}
	}

	int flag = 0;
	for (int i = 0; i < FileInfo[curID].size(); i++)
	{
		if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[2].c_str()) == 0)
		{
			flag = 1;
			break;
		}
	}
	string s1, s2, s3;
	s1 = cmd_in.cmd_num[0];
	s2 = cmd_in.cmd_num[1];
	s3 = cmd_in.cmd_num[2];
	//�µ��ļ�������....
	if (flag)
	{
		make_new_copy();
	}
	else   //�µ��ļ��������ڵĻ�Ҫ�½�һ��
	{
		//Create name mode
		cmd_in.cmd_num[0]="Create";
		cmd_in.cmd_num[1]=cmd_in.cmd_num[2];
		cmd_in.cmd_num[2]="2";
		do_Create();
		cmd_in.cmd_num[0] = s1;
		cmd_in.cmd_num[1] = s2;
		cmd_in.cmd_num[2] = s3;
		make_new_copy();

	}

}
void do_dir()
{
	for (int i = 0; i < FileInfo[curID].size(); i++)
	{
		cout << FileInfo[curID][i].filename << "  ";
	}
	cout << endl;

}
void do_Type()
{
    //Type filename
	int address;
	for (int i = 0; i < FileInfo[curID].size(); i++)
	{
		if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			address = FileInfo[curID][i].addr;
			break;
		}
	}
	int index;
	for (int i = 0; i < FileState[curID].size(); i++)
	{
		if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			index = i;
			break;
		}
	}

	while (1)
	{
		if (FileCluster[address].next_num == address)
		{
			for (int i = 0; i < FileState[curID][index].write_poit; i++)
				cout << FileCluster[address].data[i];
			break;
		}
		else
		{
			for (int i = 0; i < 256; i++)
			{
				cout << FileCluster[address].data[i];
			}
			if (FileCluster[address].next_num != address)
			{
				address = FileCluster[address].next_num;
			}
			else
				break;
		}
	}
	cout << endl;
}
void do_Passwd()
{
	//Passwd oldPwd  newPwd 
	if (strcmp(UsrInfo[curID].usrpass, cmd_in.cmd_num[1].c_str()) == 0)
	{
		strcpy(UsrInfo[curID].usrpass, cmd_in.cmd_num[2].c_str());
		cout << "�����޸ĳɹ�" << endl;
	}
	else
		cout << "��������" << endl;
}
void do_sysc()
{
	out_to_file();
	cout << "д�뵽���̳ɹ�!" << endl;
}
void do_Login()
{
	//Login	userName pwd
	int flag = 0;
	for (int i = 0; i < UsrInfo.size(); i++)
	{
		if (strcmp(cmd_in.cmd_num[1].c_str(), UsrInfo[i].usrname) == 0 && strcmp(cmd_in.cmd_num[2].c_str(), UsrInfo[i].usrpass) == 0)
		{
			curID = i;   //ȫ�ּ�¼��½�ߵ�ID
			flag = 1;
			break;
		}
	}
	if (flag)
	{
		cout << "���Ѿ���½�ɹ���" << endl;
		return;
	}
	else
	{
		cout << "�����û���������,�����µ�½!" << endl;
	}
}
void do_exit()
{
	do_sysc();
	cout << "�ɹ��˳�!" << endl;
	exit(0);
}
void do_Logout()
{
	if (curID == 0)cout << "Ĭ�Ϲ���Ա�˺��޷�ע��! " << endl;
	else {
		curID = 0;
		cout << "ע���ɹ�����ѡ���û���½!" << endl; return;
	}}
void do_register()
{
	//Register usrName pwd
	if (UsrInfo.size() < 16)
	{
		int flag = 1;//�ж�����
		if (strlen(cmd_in.cmd_num[1].c_str()) > 14) { cout << "�û�������14���ַ���" << endl; return; }
		if (strlen(cmd_in.cmd_num[2].c_str()) > 14) { cout << "���볬��14���ַ���" << endl; return; }
		for (int i = 0; i < UsrInfo.size(); i++)
		{
			if (strcmp(cmd_in.cmd_num[1].c_str(), UsrInfo[i].usrname) == 0)
			{
				flag = 0;
				break;
			}
		}
		if (flag)
		{
			strcpy(UsrInput.usrname, cmd_in.cmd_num[1].c_str());
			strcpy(UsrInput.usrpass, cmd_in.cmd_num[2].c_str());
			UsrInput.link = UsrInfo.size();
			UsrInfo.push_back(UsrInput);

			//Ϊ�µ��û����ٿռ�
			vector<UFD> t;
			FileInfo.push_back(t);

			vector<UOF> temp;
		    FileState.push_back(temp);

			cout << "�����û��������ɹ�!" << endl;

		}
		else
		{
			cout << "�û����Ѿ����ڣ�������ע��" << endl;
		}
	}
	else
		cout << "�������������޷�ע��" << endl;
}
void do_Create()
{
	//create name mode
	if (FileInfo[curID].size() < 16)
	{
		int flag = 1;
		for (int i = 0; i < FileInfo[curID].size(); i++)
		{
			if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
			{
				flag = 0;
				break;
			}
		}
		if (!flag)
		{
			cout << "���ܽ���ͬ���ļ���" << endl;
			return;
		}

		strcpy(FileInput.filename, cmd_in.cmd_num[1].c_str());
		stringstream ss;
		ss << cmd_in.cmd_num[2];
		int temp;
		ss >> temp;
		FileInput.mode = temp;
		FileInput.length = 0;
		//Ҫ��������������ݴ洢...
		for (int i = 0; i < FileCluster.size(); i++)
		{
			if (FileCluster[i].is_data == 0)//����û�����ݣ�           
			{
				FileInput.addr = i;
				FileCluster[i].is_data = 1;
				break;
			}
		}
		FileInfo[curID].push_back(FileInput);
		cout << "�ļ������ɹ�" << endl;

		//״̬��
		strcpy(StateInput.filename, cmd_in.cmd_num[1].c_str());
		StateInput.state = 0;
		StateInput.mode = FileInput.mode;
		StateInput.read_poit = 0;
		StateInput.write_poit = 0;

		FileState[curID].push_back(StateInput);

	}
	else
		cout << "�����ļ����ޣ�" << endl;
	

}
void do_Delete()
{
	//delete filename
	int flag = 0;
	vector<UFD>::iterator it;
	for (it = FileInfo[curID].begin(); it != FileInfo[curID].end(); it++)
	{
		if (strcmp((*it).filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			//FileInfo[curID].erase(it);
			flag = 1;
			break;
		}
	}
	if (flag)
	{
		//ע��Ӧ�����ռ�õ��ڴ�
	    
		//Type filename
		int address;
		for (int i = 0; i < FileInfo[curID].size(); i++)
		{
			if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
			{
				address = FileInfo[curID][i].addr;
				break;
			}
		}
		int index;
		for (int i = 0; i < FileState[curID].size(); i++)
		{
			if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
			{
				index = i;
				break;
			}
		}

		vector<int> reset_addr;
		reset_addr.clear();

		while (1)
		{
			if (FileCluster[address].next_num == address)
			{
				for (int i = 0; i < FileState[curID][index].write_poit; i++)
					FileCluster[address].data[i]=0;
				reset_addr.push_back(address);
				break;
			}
			else
			{
				reset_addr.push_back(address);
				for (int i = 0; i < 256; i++)
				{
					 FileCluster[address].data[i]=0;
				}
				if (FileCluster[address].next_num != address)
				{
					address = FileCluster[address].next_num;
				}
				else
					break;
			}
		}

		for (int w = 0; w < reset_addr.size(); w++)
		{
			FileCluster[reset_addr[w]].is_data = 0;
			FileCluster[reset_addr[w]].next_num = reset_addr[w];
		}

		//����ļ���Ϣ
		vector<UFD>::iterator it;
		for (it = FileInfo[curID].begin(); it != FileInfo[curID].end(); it++)
		{
			if (strcmp((*it).filename, cmd_in.cmd_num[1].c_str()) == 0)
			{
				FileInfo[curID].erase(it);
				flag = 1;
				break;
			}
		}
		//���״̬��
		vector<UOF>::iterator it1;
		for (it1 = FileState[curID].begin(); it1 != FileState[curID].end(); it1++)
		{
			if (strcmp((*it1).filename, cmd_in.cmd_num[1].c_str()) == 0)
			{
				FileState[curID].erase(it1);
				break;
			}
		}

		cout << "ɾ���ɹ�!" << endl;
	}
	else
		cout << "�ļ��Ѿ�����!" << endl;

	
}

void do_Open()
{
	//Open  filename mode
	stringstream ss;
	ss << cmd_in.cmd_num[2];
	int temp;
	ss >> temp;
	//�ж��Ƿ���Ȩ��,��û����......
	int flag = 0;
	for (int i = 0; i < FileState[curID].size(); i++)
	{
		if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			FileState[curID][i].state = 1; 
			flag = 1;
			break;
		}
	}
	if (flag)
		cout << "���ļ��ɹ�!" << endl;
	else
	{
		strcpy(StateInput.filename, cmd_in.cmd_num[1].c_str());
		StateInput.mode = temp;
		StateInput.state = 1;
		StateInput.read_poit = StateInput.write_poit = 0;
		FileState[curID].push_back(StateInput);
		cout << "���ļ��ɹ���" << endl;
	}
	
}
void do_Close()
{
	//close filename
	vector<UOF>::iterator it;
	for (it = FileState[curID].begin(); it != FileState[curID].end(); it++)
	{
		if (strcmp((*it).filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			FileState[curID].erase(it);
			cout << "�ر��ļ��ɹ���" << endl;
			break;
		}
	}
}

/*int num;
int next_num;
int is_data;
char data[256];*/

void do_Write()
{
	//Write	filename buffer nbytes д�ļ�   ����ռ�68

	char buf[1024];
	stringstream ss;
	ss << cmd_in.cmd_num[3];
	int temp;
	ss >> temp;


	for (int i = 0; i < FileInfo[curID].size(); i++)
	{
		if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			if (FileInfo[curID][i].mode == 1 || FileInfo[curID][i].mode == 2)//�ж�Ȩ��
			{
				break;
			}
			else
			{
				cout << "û��д��Ȩ��!" << endl;
				return;
			}
		}
	}

	int index;
	for (int i = 0; i < FileState[curID].size(); i++)
	{
		if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			index = i;
			break;
		}
	}
	//��ʼ�����
	int address;
	for (int i = 0; i < FileInfo[curID].size(); i++)
	{
		if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			address = FileInfo[curID][i].addr;
			break;
		}
	}
            //ע�⣺�˴������˸��ģ�
			cout << "������buff�����ݣ�" << endl;
			gets_s(buf);
			fflush(stdin);
		
	        //strcpy(buf, cmd_in.cmd_num[2].c_str());

			int wbegin;
			wbegin = FileState[curID][index].write_poit;
			
			//�ҵ�дָ�����ڵ����һ������
			while (FileCluster[address].next_num != address)
				address = FileCluster[address].next_num;

			vector <int> newspace_num;//���㽫Ҫռ�õ�����������
			newspace_num.clear();

			//int num = (256-wbegin+temp) / 256-1;
			if (temp <= 256 - wbegin)
				num = 0;
			else
			{
				num = ceil((temp - (256 - wbegin))*1.0 / 256);
			}

			newspace_num.push_back(address);

			//cout << newspace_num.size() << endl;//

			for (int i = 0; i < FileCluster.size(); i++)
			{
				if (newspace_num.size() == num+1)
					break;
				if (FileCluster[i].is_data == 0)
				{
					newspace_num.push_back(i);
					FileCluster[i].is_data = 1;
				}
			}

			for (int k = 0; k < newspace_num.size() - 1; k++)
			{
				FileCluster[newspace_num[k]].next_num = newspace_num[k + 1];
			}
			for (int i = 0; i < temp; i++)
			{
				if (wbegin == 256)
				{
					wbegin = 0;
					address = FileCluster[address].next_num;
				}
				FileCluster[address].data[wbegin] = buf[i];
				wbegin++;
			}

			//����дָ��
			FileState[curID][index].write_poit = wbegin;
			cout << "����д��ɹ�!" << endl;
			return;

}

//write_poit��¼���մ��̵�дָ�룬read_poit��¼ȫ�������Ķ�ָ���λ�á�

void do_Read()
{
	//Read 	filename buffer nbytes 
	char buf[1024];
	stringstream ss;
	ss << cmd_in.cmd_num[3];
	int temp;
	ss >> temp;

	for (int i = 0; i < FileInfo[curID].size(); i++)
	{
		if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			if (FileInfo[curID][i].mode == 0 || FileInfo[curID][i].mode == 2)//�ж�Ȩ��
			{
				break;
			}
			else
			{
				cout << "û�ж���Ȩ��!" << endl;
				return;
			}
		}
	}

	int index;
	for (int i = 0; i < FileState[curID].size(); i++)
	{
		if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			index = i;
			break;
		}
	}

	int address;
	for (int i = 0; i < FileInfo[curID].size(); i++)
	{
		if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			address = FileInfo[curID][i].addr;
			break;
		}
	}

	int rbegin;
	rbegin = FileState[curID][index].read_poit;
	int addcur = rbegin / 256;

	for (int i = 0; i < addcur; i++)
	{
		address = FileCluster[address].next_num;
	}
    
	for (int i = 0; i < temp; i++)
	{
		if (rbegin % 256 == 255)
		{
			address = FileCluster[address].next_num;
		}
		buf[i] = FileCluster[address].data[rbegin % 256];
		rbegin++;
	}

	FileState[curID][index].read_poit = rbegin;

	cout << "�����������ǣ�" << endl;
	for (int i = 0; i < temp; i++)
		cout << buf[i];
	cout << endl;

}
void do_Help()
{
	cout << "Login	userName pwd	�û���½" << endl;
	cout << "Logout	�û��ǳ�" << endl;
	cout << "Register usrName pwd   �û�ע��" << endl;
	cout << "Passwd	oldPwd  newPwd    �޸��û�����" << endl;
	cout << "Open   filename mode   ���ļ�" << endl;
	cout << "Close  filename   �ر��ļ�" << endl;
	cout << "Create	 filename mode	  �����ļ�" << endl;
	cout << "Delete	 filename   ɾ���ļ�" << endl;
	cout << "Write	filename buffer nbytes   д�ļ�" << endl;
	cout << "Read 	filename buffer nbytes   ���ļ�" << endl;
	cout << "dir   �г����û��������ļ�" << endl;
	cout << "Chmod	filename mode  	 �ı��ļ�Ȩ��" << endl;
	cout << "Chown	filename new_owner    �ı��ļ�ӵ����" << endl;
	cout << "Mv	srcFile desFile	   �ı��ļ���" << endl;
	cout << "Copy   srcFile desFile   �ļ�����" << endl;
	cout << "Type	filename     ��ʾ�ļ�����" << endl;
	cout << "Exit   �˳�����" << endl;
	cout << "sysc   ͬ�������� " << endl;
}

void doTempWrite()
{
	//Write	filename buffer nbytes д�ļ�   ����ռ�68

	char buf[1024];
	stringstream ss;
	ss << cmd_in.cmd_num[3];
	int temp;
	ss >> temp;


	for (int i = 0; i < FileInfo[curID].size(); i++)
	{
		if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			if (FileInfo[curID][i].mode == 1 || FileInfo[curID][i].mode == 2)//�ж�Ȩ��
			{
				break;
			}
			else
			{
				cout << "û��д��Ȩ��!" << endl;
				return;
			}
		}
	}

	int index;
	for (int i = 0; i < FileState[curID].size(); i++)
	{
		if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			index = i;
			break;
		}
	}
	//��ʼ�����
	int address;
	for (int i = 0; i < FileInfo[curID].size(); i++)
	{
		if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
		{
			address = FileInfo[curID][i].addr;
			break;
		}
	}
	//ע�⣺�˴������˸��ģ�
	/*cout << "������buff�����ݣ�" << endl;
	gets(buf);
	fflush(stdin);*/


	strcpy(buf, cmd_in.cmd_num[2].c_str());

	int wbegin;
	wbegin = FileState[curID][index].write_poit;

	//�ҵ�дָ�����ڵ����һ������
	while (FileCluster[address].next_num != address)
		address = FileCluster[address].next_num;

	vector <int> newspace_num;//���㽫Ҫռ�õ�����������
	newspace_num.clear();

	//int num = (256-wbegin+temp) / 256-1;
	if (temp <= 256 - wbegin)
		num = 0;
	else
	{
		num = ceil((temp - (256 - wbegin))*1.0 / 256);
	}

	newspace_num.push_back(address);

	//cout << newspace_num.size() << endl;//

	for (int i = 0; i < FileCluster.size(); i++)
	{
		if (newspace_num.size() == num + 1)
			break;
		if (FileCluster[i].is_data == 0)
		{
			newspace_num.push_back(i);
			FileCluster[i].is_data = 1;
		}
	}

	for (int k = 0; k < newspace_num.size() - 1; k++)
	{
		FileCluster[newspace_num[k]].next_num = newspace_num[k + 1];
	}
	for (int i = 0; i < temp; i++)
	{
		if (wbegin == 256)
		{
			wbegin = 0;
			address = FileCluster[address].next_num;
		}
		FileCluster[address].data[wbegin] = buf[i];
		wbegin++;
	}

	//����дָ��
	FileState[curID][index].write_poit = wbegin;
	cout << "����д��ɹ�!" << endl;
	return;

}

void do_Clear()
{
	system("cls");
}

#endif
