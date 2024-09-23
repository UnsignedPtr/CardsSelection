#include<windows.h>
#include<stdio.h>
#include<stack>
#include<string>
#include<vector>
#include<iostream>

 
std::string s[]={
	"豌豆射手","向日葵","樱桃炸弹","坚果","土豆地雷","寒冰射手","大嘴花","双发射手", 
	"小喷菇","阳光菇","大喷菇","墓碑吞噬者","迷幻菇","胆小菇","寒冰菇","毁灭菇",
	"荷叶","窝瓜","三线射手","缠绕水草","火爆辣椒","地刺","火炬树桩","高坚果",
	"水蘑菇","路灯花","仙人掌","三叶草","裂荚射手","杨桃","南瓜头","磁力菇",
	"卷心菜投手","花盆","玉米投手","咖啡豆","大蒜","保护伞","金盏花","西瓜投手",
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
	//运算符栈、操作数栈
	optr.push('#');//检验运算符栈非空的哨兵 
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
	printf("[HINT]植物编号对应表:\n");
	for(int i=0;i<40;i++){
		printf("[%d]%s\t",i,s[i].c_str());
		if(i%4==3)printf("\n");
	}
	printf("\n输入需要的植物编号表达式.\n");
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
	//修改帧间隔
	//相当于开个倍速，让动画播放快一点
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
	
	//printf("进入选卡界面\n");
	
	Sleep(5000);
	
	//读取选卡界面

	
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
		//printf("已选%d\n",p);
		chosen_plants.pop();
		for(int i=0;i<limits;i++){
			if(needed_plants[i]==p)goto L95;
		}
	}*/
	if(calc()==1)goto L95;
	TerminateProcess(handle,0);goto L38;
L95:
	//关闭倍速
	x=10;
	WriteProcessMemory(handle,LPCVOID(pvz_base+0x454),&x,4,NULL); 
	
	system("pause");
	return 0;
}
