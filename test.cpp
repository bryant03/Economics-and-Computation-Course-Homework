#include <iostream>
#include <vector>
#include <stdlib.h>
using namespace std;
int main(){
	for(int i=1;i<=10;i++){
		cout<<rand()%10<<endl;
	}
	for(int i=1;i<=10;i++){
		cout<<rand()%10<<endl;
	}
	vector<int> v;
	v.push_back(1);
	cout<<endl<<v.back()<<endl;
	v.back()=2;
	cout<<endl<<v.back()<<endl;
	return 0;
}
