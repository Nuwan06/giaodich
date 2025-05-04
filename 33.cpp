#include<bits/stdc++.h>
using namespace std;
int main() {
    int N;
    cin >> N;
    for (int i = 0; i < N; i++) {
        int n,k;
        cin >> n;
        int arr[n];
        for (int i = 0; i < n; i++) {
            cin >> arr[i];        

        }
        sort(arr,arr+n);
        int u=0;
        if(n%2==0) {
        for (int i =n-1;i>=n/2;i--) {
            cout <<arr[i]<<" ";
            cout<<arr[u]<<" ";
            u+=1;
        }
        
        } 
        else{
            for ( k =n-1;k>n/2;k--) {
                cout <<arr[k]<<" ";
                cout<<arr[u]<<" ";
                u+=1;
            }
            cout << arr[k];

          } cout << endl;
        }     
    
    return 0;
}