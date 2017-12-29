#include <cstdio>
#include <vector>
#include <algorithm>
#include <stdlib.h>
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
    double ave;//平均收益
    int cnt;//经过的次数
    int father;//父亲节点
    bool over=false;//是否是叶子节点
    node(int a,int b，int father,int dir):ave(a),cnt(b),father(father),dir(dir){}
    node(){}
};
//地图节点信息
struct mapp{
    int ani;//剩余动物个数
    std::vector<int> feet;//脚步
}m[7][7];

struct point{
    int x,y;
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
int deep;//当前树的深度
double score;//当前分数



int selection_attacker(int u){

    int len=a[u].size();
    if(len==0) return u;
    else{
        double minn=vnode_attacker[a[u][0]].ave;
        int id=-1;
        for(int i=1;i<len;i++){
            if(vnode_attacker[att[u][i]].ave<minn){
                minn=vnode_attacker[att[u][i]].ave;
                id=att[u][i];
            }
        }
        ranger.x+=dx[vnode_attacker[id].dir];
        ranger.y+=dy[vnode_attacker[id].dir];
        deep++;
        selection(id);
    }
}
int selection_ranger(int u){

    int len=a[u].size();
    if(len==0) return u;
    else{
        double minn=vnode[a[u][0]].ave;
        int id=-1;
        for(int i=1;i<len;i++){
            if(vnode[a[u][i]].ave>minn){
                minn=vnode[a[u][i]].ave;
                id=a[u][i];
            }
        }
        attacker.x+=dx[vnode[id].dir];
        attacker.y+=dy[vnode[id].dir];
        deep++;
        selection(id);
    }
    
}
//扩展节点
void expansion_ranger(int u){
    vector<int> tmp;
    for (int i = 0; i < 5; ++i)
    {
        int xx=ranger.x+dx[i];
        int yy=ranger.y+dy[i];
        if(xx<0||xx>6||yy<0||yy>6) continue;
        tmp.push_back(i);
    }
    int sel=rand()%tmp.size();
    ran[u].push_back(vnode_ranger.size());
    vnode_ranger.push_back(node(0,1,u,tmp[sel]));
    ranger.x+=dx[tmp[sel]];
    ranger.y+=dy[tmp[sel]];
}
void expansion_attacker(int u){
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
    att[u].push_back(vnode_attacker.size());
    vnode_attacker.push_back(node(0,1,u,tmp[sel]));
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
    if(f) return true;
    return false;
}

void Simulation_attacker(){
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

void Backpropagation(int u,int v){

}
void MCTS(){

    vnode_attacker.push_back(node(0,0,0,0));
    attacker[0].push_back(1);attacker[0].push_back(2);
    attacker[0].push_back(3);attacker[0].push_back(4);
    for (int i = 1; i <= 4; ++i)
    {
        vnode_attacker.push_back(node(i,0,0,0));
    }
    for(int it=1;it<=1;it++ ){
        ranger.x=3;
        ranger.y=3;
        deep=0;
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
        if(u==1) attacker.x=0;attacker.y=3;
        else if(u==2) attacker.x=3;attacker.y=6;
        else if(u==3) attacker.x=6;attacker.y=3;
        else if(u==4) attacker.x=3;attacker.y=0;

        for (int i = 0; i < 30; ++i)
        {
            if(state1==false&&state2==false){
                if (estimate()){
                	break;
                }
                if(att[u].size()==0){
                    state1=true;
                    expansion_attacker(u);
                    u=vnode_attacker.size();
                }
                else { 
                    u=selection_attacker(u);
                }
                if(ran[v].size()==0){
                    state2=true;
                    expansion_ranger(v);
                    v=vnode_ranger.size();
                }
                else { 
                    u=selection_attacker(u);
                }
            }
            else if(!state1&&state2){
                if (estimate()){
                	break;
                }
                if(att[u].size()==0){
                    state1=true;
                    expansion_attacker(u);
                    u=vnode_attacker.size();
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
                if(ran[u].size()==0){
                    state2=true;
                    expansion_ranger(v);
                    v=vnode_ranger.size();
                }
                else { 
                    v=selection_ranger(v);
                }
                Simulation_attacker();
            }
            else{
                if (estimate()){
                	break;
                }
                Simulation_attacker();
                Simulation_ranger();            	
            }
        }
        int curr=selection(0);
        if(!vnode[curr].over)
        {
            expansion(curr);
            double v=Simulation(vnode.size());
        }
        Backpropagation(exp);
    }

}
int main(){

    return 0;
} 
