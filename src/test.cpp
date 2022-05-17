#include <iostream>
#include <cstdio>
using namespace std;
void test(int* a)
{
    a[0]=a[1];
}
int main()
{
    int a[2]={0,1};
    cout<<a[0]<<a[1]<<endl;
    test(a);
    cout<<a[0]<<a[1]<<endl;
}