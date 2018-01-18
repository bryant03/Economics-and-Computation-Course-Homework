#include <cstdio>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <ctime>
#include <string>
using namespace std;
int animal [7][7]={
0,0,0,1,1,2,2,
1,1,0,1,2,3,1,
1,0,2,2,5,5,1,
1,1,0,1,1,0,2,
2,3,1,0,0,1,1,
1,2,3,3,1,1,1,
2,0,1,1,0,1,1,
};
int total;
//树的节点信息
struct node{
    int dir;//方向
    double val;//UCT
    double ave;//平均收益
    int cnt;//经过的次数
    int father;//父亲节点
    bool over=false;//是否是叶子节点
    node(double val,double ave,int cnt,int father,int dir):val(val),ave(ave),cnt(cnt),father(father),dir(dir){}
    node(){}
};
//地图节点信息
struct mapp{
    int ani;//剩余动物个数
    std::vector<int> feet;//脚步
}m[7][7];

struct point{
    int x,y;
    point(){}
    point(int x,int y):x(x),y(y){}
}ranger,attacker;
std::vector<node> vnode_ranger;//存放树的所有节点
std::vector<node> vnode_attacker;//存放树的所有节点
std::vector<int> ran[10100];//存放树之间边的关系
std::vector<int> att[10100];//存放树之间边的关系
const int dx[]={0,-1,0,1,0};
const int dy[]={0,0,1,0,-1};
const int ddx[]={-1,3,7,3};
const int ddy[]={3,7,3,-1};
const double C=1.96;
#define failure -30;
#define hunt 8;
#define maxhunt 5
int deep;//当前树的深度
double score;//当前分数


bool legal(int x,int y){
    if(x<0||x>6||y<0||y>6) return false;
    return true;
}
int selection_attacker(int u){
	int len=att[u].size();
	int sel=rand()%att[u].size();
    double minn=vnode_attacker[att[u][sel]].val;
    int id=att[u][sel];
    for(int i=0;i<len;i++){
        if(vnode_attacker[att[u][i]].val>minn){
            minn=vnode_attacker[att[u][i]].val;
            id=att[u][i];
        }
    }
    attacker.x+=dx[vnode_attacker[id].dir];
    attacker.y+=dy[vnode_attacker[id].dir];
    return id;
}
int selection_ranger(int u){
    int len=ran[u].size();
    int sel=rand()%ran[u].size();
    double minn=vnode_ranger[ran[u][sel]].val;
    int id=ran[u][sel];
    for(int i=1;i<len;i++){
        if(vnode_ranger[ran[u][i]].val>minn){
            minn=vnode_ranger[ran[u][i]].val;
            id=ran[u][i];
        }
    }
    ranger.x+=dx[vnode_ranger[id].dir];
    ranger.y+=dy[vnode_ranger[id].dir];
    return id;    
}
//扩展节点
void expansion_ranger(int u){
    vector<int> tmp;
    for (int i = 0; i < 5; ++i)
    {
        int xx=ranger.x+dx[i];
        int yy=ranger.y+dy[i];
        if(!legal(xx,yy)) continue;
        tmp.push_back(i);
    }
    int sel=rand()%tmp.size();
    ran[u].push_back(vnode_ranger.size());
    vnode_ranger.push_back(node(0,0,0,u,tmp[sel]));
    ranger.x+=dx[tmp[sel]];
    ranger.y+=dy[tmp[sel]];
}
void expansion_attacker(int u){
    std::vector<int> tmp;
    for (int i = 0; i < 5; ++i)
    {
        int xx=attacker.x+dx[i];
        int yy=attacker.y+dy[i];
        if(!legal(xx,yy)){
            bool f=false;
            for (int j = 0; j < 4; ++j)
            {
                if(xx==ddx[j]&&yy==ddy[j]) f=true;
            }
            if(!f) continue;
        }
        tmp.push_back(i);
    }
    int sel=rand()%tmp.size();
    att[u].push_back(vnode_attacker.size());
//    printf(" attacker size %d\n",vnode_attacker.size());
    vnode_attacker.push_back(node(0,0,0,u,tmp[sel]));
//    printf("u= %d  attacker size %d father %d\n",u,vnode_attacker.size(),vnode_attacker[6].father);
    attacker.x+=dx[tmp[sel]];
    attacker.y+=dy[tmp[sel]];
}

bool estimate(){
    if(ranger.x==attacker.x&&ranger.y==attacker.y){
    	score=-30;
        return true;
    }
    bool f=false;
    for (int j = 0; j < 4; ++j)
    {
        if(attacker.x==ddx[j]&&attacker.y==ddy[j]) f=true;
    }
    if(f) {
        if(score==0) score-=5;
        return true;
    }
    return false;
}

int Simulation_attacker(){
	std::vector<int> tmp;
    for (int i = 0; i < 5; ++i)
    {
        int xx=attacker.x+dx[i];
        int yy=attacker.y+dy[i];
        if(xx<0||xx>6||yy<0||yy>6){
            bool f=false;
            for (int j = 0; j < 4; ++j)
            {
                if(xx==ddx[j]&&yy==ddy[j]) f=true;
            }
            if(!f) continue;
        }
        tmp.push_back(i);
    }
    int sel=rand()%tmp.size();
    attacker.x+=dx[tmp[sel]];
    attacker.y+=dy[tmp[sel]]; 
    return tmp[sel];   
}

void Simulation_ranger(){
	std::vector<int> tmp;
    for (int i = 0; i < 5; ++i)
    {
        int xx=ranger.x+dx[i];
        int yy=ranger.y+dy[i];
        if(xx<0||xx>6||yy<0||yy>6) continue;
        tmp.push_back(i);
    }
    int sel=rand()%tmp.size();
    ranger.x+=dx[tmp[sel]];
    ranger.y+=dy[tmp[sel]];    
}

void Backpropagation_attacker(int u){
    bool f=false;
    while(!f){
        if(!u) f=true;
        //cout<<u<<endl;
        double tmp=vnode_attacker[u].ave;
        int cnt=vnode_attacker[u].cnt;
        vnode_attacker[u].ave=(tmp*cnt+score)/(cnt+1);
        vnode_attacker[u].cnt++;
        vnode_attacker[u].val=vnode_attacker[u].ave+C*sqrt(log(1.0*total)/vnode_attacker[u].cnt);
        u=vnode_attacker[u].father;

    }
}

void Backpropagation_ranger(int v){
    bool f=false;
    while(!f){
        if(!v) f=true;
        //cout<<v<<endl;
        double tmp=vnode_ranger[v].ave;
        int cnt=vnode_ranger[v].cnt;
        vnode_ranger[v].ave=(tmp*cnt-score)/(cnt+1);
        vnode_ranger[v].cnt++;
        vnode_ranger[v].val=vnode_ranger[v].ave+C*sqrt(log(1.0*total)/vnode_ranger[v].cnt);
        v=vnode_ranger[v].father;
    }
}

void MCTS(){

    vnode_attacker.push_back(node(0,0,0,0,0));
    att[0].push_back(1);att[0].push_back(2);
    att[0].push_back(3);att[0].push_back(4);
    vnode_ranger.push_back(node(0,0,0,0,0));
//    ran[0].push_back(1);
//    vnode_ranger.push_back(node(0,0,0,0,0));
    for (int i = 1; i <= 4; ++i)
    {
        vnode_attacker.push_back(node(0,0,0,0,0));
    }
    for(int it=1;it<=100000;it++ ){
    	total = it;
    	int maxHunt=maxhunt;
//    	printf("round = %d \n",it);
		bool show =false;
		if(it%100000==0){
			show= true;
		}
        if(show) printf("round = %d \n",it);
        ranger.x=3;
        ranger.y=3;
        score=0;
        bool state1=false;
        bool state2=false;
        for (int i = 0; i < 7; ++i)
        { 
            for (int j = 0; j < 7; ++j)
            {
                m[i][j].ani=animal[i][j];
            }
        }
        int u,v=0;
        u=selection_attacker(0);
        if(u==1){
			attacker.x=0;attacker.y=3;
		}
        else if(u==2){
			attacker.x=3;attacker.y=6;
		}
        else if(u==3){
        	
			attacker.x=6;attacker.y=3;
		}
        else if(u==4){
			attacker.x=3;attacker.y=0;
		}
        int lastdir=vnode_attacker[u].dir;
        for (int i = 0; i < 30; ++i)
        {	
        	if(show){
        	printf("i= %d u= %d v= %d  state1= %d state2= %d attacker.x= %d attacker.y= %d ranger.x= %d  ranger.y= %d\n",i,u,v,state1,state2,attacker.x,attacker.y,ranger.x,ranger.y);
            }
            if(state1==false&&state2==false){
                if (estimate()){
                	break;
                }
                if(att[u].size()==0){
                    state1=true;
                    expansion_attacker(u);
                    u=vnode_attacker.size()-1;
                    lastdir=vnode_attacker[u].dir;
//                    printf("father %d\n",vnode_attacker[u].father);
                }
                else { 
                	// printf("select");
                    u=selection_attacker(u);
                }
                if(ran[v].size()==0){
                    state2=true;
                    expansion_ranger(v);
                    v=vnode_ranger.size()-1;
                }
                else { 
//                	printf("select");
                    v=selection_ranger(v);
                }
            }
            else if(!state1&&state2){
                if (estimate()){
                	break;
                }
                if(att[u].size()==0){
                    state1=true;
                    expansion_attacker(u);
                    u=vnode_attacker.size()-1;
                    lastdir=vnode_attacker[u].dir;
                }
                else { 
                    u=selection_attacker(u);
                }
                Simulation_ranger();
            }
            else if (state1&&!state2)
            {
                if (estimate()){
                	break;
                }
                if(ran[v].size()==0){
                    state2=true;
                    expansion_ranger(v);
                    v=vnode_ranger.size()-1;
                }
                else { 
                    v=selection_ranger(v);
                }
                lastdir=Simulation_attacker();
            }
            else{
                if (estimate()){
                	break;
                }
//                printf("attacker.x= %d attacker.y= %d ranger.x= %d  ranger.y= %d \n",attacker.x,attacker.y,ranger.x,ranger.y);
                lastdir=Simulation_attacker();
                Simulation_ranger();            	
            }
            if(legal(attacker.x,attacker.y)&&m[attacker.x][attacker.y].ani&&lastdir==0&&maxHunt){
            	//printf("asdadsa  %d %d\n",attacker.x,attacker.y);
                score+=hunt;
                maxHunt--;
                m[attacker.x][attacker.y].ani--;
            }
        }
        if(show) printf("finally\n");
        if(!estimate()) score=failure;
        Backpropagation_attacker(u);
        Backpropagation_ranger(v);
        printf("%lf %lf\n",vnode_attacker[0].ave, vnode_ranger[0].ave);
    }
    printf("id =%d cnt = %d val=%lf ave=%lf total=%d dir=%d \n",0,vnode_attacker[0].cnt,vnode_attacker[0].val,vnode_attacker[0].ave,total ,vnode_attacker[0].dir);
    for (int i = 0; i < att[0].size(); ++i)
    {	

        node tmp=vnode_attacker[att[0][i]];
        printf("id =%d cnt = %d val=%lf ave=%lf total=%d dir=%d \n",att[0][i],tmp.cnt,tmp.val,tmp.ave,total ,tmp.dir);
    }
    printf("id =%d cnt = %d val=%lf ave=%lf total=%d dir=%d \n",0,vnode_ranger[0].cnt,vnode_ranger[0].val,vnode_ranger[0].ave,total ,vnode_ranger[0].dir);
    for (int i = 0; i < ran[0].size(); ++i)
    {
        node tmp=vnode_ranger[ran[0][i]];
        printf("id= %d cnt = %d val=%lf ave=%lf total=%d dir=%d \n",ran[0][i],tmp.cnt,tmp.val,tmp.ave,total ,tmp.dir);
    }
}
int main(){

	freopen("out1.txt","w",stdout); 
	srand((unsigned)time(0));
    MCTS();
    return 0;
} 
