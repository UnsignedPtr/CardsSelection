#include<windows.h>
#include<stdio.h>
#include<stack>
#include<string>
#include<vector>
#include<iostream>

 
std::string s[]={
	"�㶹����","���տ�","ӣ��ը��","���","��������","��������","���컨","˫������", 
	"С�繽","���⹽","���繽","Ĺ��������","�Իù�","��С��","������","����",
	"��Ҷ","�ѹ�","��������","����ˮ��","������","�ش�","�����׮","�߼��",
	"ˮĢ��","·�ƻ�","������","��Ҷ��","�Ѽ�����","����","�Ϲ�ͷ","������",
	"���Ĳ�Ͷ��","����","����Ͷ��","���ȶ�","����","����ɡ","��յ��","����Ͷ��",
};
int chosen[50];
std::stack<int> chosen_plants;
std::vector<int> needed_plants;
int limits=0;

std::string tok[1000];
int tok_max=0;

int calc_sub(char op,int num1,int num2){
	if(op=='a')return num1&num2;
	if(op=='o')return num1|num2;
}

int calc(){
	std::stack<int>optr,opnd;
	//�����ջ��������ջ
	optr.push('#');//���������ջ�ǿյ��ڱ� 
	for(int i=1;i<=tok_max;i++){
		if(isdigit(tok[i].at(0))){
			int num1=chosen[atoi(tok[i].c_str())];
			opnd.push(num1);
		}
		else if(tok[i].at(0)=='('){
			optr.push('(');
		}
		else if(tok[i].at(0)==')'){
			for(;optr.top()!='(';){
				int num2=opnd.top();opnd.pop();
				int num3=opnd.top();opnd.pop();
				char op=optr.top();optr.pop();
				opnd.push(calc_sub(op,num3,num2));
			}
			optr.pop();
		}
		else{
			for(;optr.top()!='#' && optr.top()!='(';){
				int num2=opnd.top();opnd.pop();
				int num3=opnd.top();opnd.pop();
				char op=optr.top();optr.pop();
				opnd.push(calc_sub(op,num3,num2));
			}
			optr.push(tok[i].at(0));
		}
	}
	//printf("%d\n",opnd.top());
	
	for(;optr.top()!='#';){
		int num2=opnd.top();opnd.pop();
		int num3=opnd.top();opnd.pop();
		char op=optr.top();optr.pop();
		opnd.push(calc_sub(op,num3,num2));
	}
	return opnd.top();
}


int main(){
	printf("[HINT]ֲ���Ŷ�Ӧ��:\n");
	for(int i=0;i<40;i++){
		printf("[%d]%s\t",i,s[i].c_str());
		if(i%4==3)printf("\n");
	}
	printf("\n������Ҫ��ֲ���ű��ʽ.\n");
	printf(">");
	for(;;){
		std::cin>>tok[++tok_max];
		if(getchar()=='\n')break;
	}
	/*for(int i=1;i<=tok_max-1;i++){
		printf("%d ",tok[i].size());
	}for(;;);*/
	//tok_max--;
	
	HWND hwnd;
	HANDLE handle;
	DWORD pid=1437;
	int pvz_base, x;
	int main_obj, cards;

L38:
	ShellExecute(0,"open","pvz.exe","","",SW_SHOW);
	
	Sleep(2000);
	
	
	hwnd=FindWindowA(NULL,"Plants vs. Zombies");
	GetWindowThreadProcessId(hwnd,&pid);
	handle=OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);
	//printf("PID..%d\n",pid);
	

	ReadProcessMemory(handle,LPCVOID(0x6A9EC0),&pvz_base,4,NULL);
	ReadProcessMemory(handle,LPCVOID(pvz_base+0x454),&x,4,NULL);
	//�޸�֡���
	//�൱�ڿ������٣��ö������ſ�һ��
	x=3;
	WriteProcessMemory(handle,LPCVOID(pvz_base+0x454),&x,4,NULL);
	 
	Sleep(3000);
	for(int i=0;i<10;i++){
		SendMessage(hwnd,WM_LBUTTONDOWN,0,MAKELONG(412,553));
		SendMessage(hwnd,WM_LBUTTONUP,0,MAKELONG(412,553));
	}

	
	Sleep(2000);
	for(int i=0;i<10;i++){
		SendMessage(hwnd,WM_LBUTTONDOWN,0,MAKELONG(675,145));
		SendMessage(hwnd,WM_LBUTTONUP,0,MAKELONG(675,145));
	}
	
	//printf("����ѡ������\n");
	
	Sleep(5000);
	
	//��ȡѡ������

	
	chosen_plants.push(-1);
	ReadProcessMemory(handle,LPCVOID(pvz_base+0x768),&main_obj,4,NULL);
	ReadProcessMemory(handle,LPCVOID(pvz_base+0x774),&cards,4,NULL);
	for(int i=0;i<48;i++){
		ReadProcessMemory(handle,LPCVOID(cards+0xDC+0x3C*i),&x,4,NULL);
		if(x==1)chosen[i]=1;
		else chosen[i]=0;
	}/*
	while(chosen_plants.top()!=-1){
		int p=chosen_plants.top();
		//printf("��ѡ%d\n",p);
		chosen_plants.pop();
		for(int i=0;i<limits;i++){
			if(needed_plants[i]==p)goto L95;
		}
	}*/
	if(calc()==1)goto L95;
	TerminateProcess(handle,0);goto L38;
L95:
	//�رձ���
	x=10;
	WriteProcessMemory(handle,LPCVOID(pvz_base+0x454),&x,4,NULL); 
	
	system("pause");
	return 0;
}
