#include <iostream>
#include <vector>
#include <stdio.h>
#include <ctime>
#include <cmath>
#include <string>
#include <sstream>
#include <map>

#define lu(x) printf("%lu\n",x);
#define d(x) printf("%d\n",x);
#define loop0(i,n) for(int i=0;i<n;i++)
#define loop(i,m,n) for(int i=m;i<n;i++)

using namespace std;

class Node{
  public:
    Node* parent;
    Node* child;
    Node* right;
    Node* left;
    int data;
    int degree;
    bool mark;
    Node(){
        parent = NULL;
        child = NULL;
        right = this;
        left = this;
        data = 0;
        degree = 0;
        mark = false;
    }
};

class Heap{
  private:
    void consolidate(){
        Node* A[n];
        for(int i =0;i<n;i++)
            A[i]=NULL;
        Node *i = min;
        Node* root[n];
        int ctr=0;
        do{
            root[ctr]=i;
            i=i->right;
            ctr++;
        }while(i!=min);
        loop0(k,ctr){
            Node* x = root[k];
            int d = x->degree;
            while(A[d]!=NULL){
                Node* y = A[d];
                if(x->data>y->data){
                    Node *temp = x;
                    x=y;
                    y=temp;
                }
                heapLink(y,x);
                A[d]=NULL;
                d=d+1;
            }
            A[d]=x;
        }
        int loop =0;
        min=NULL;
        for(int j = 0;j<n;j++){
            if(A[j]!=NULL){
                if(min==NULL){
                    min=A[j];
                    min->right=min;
                    min->left=min;
                }
                else{
                    Node* temp=min->right;
                    min->right=A[j];
                    A[j]->left=min;
                    A[j]->right=temp;
                    temp->left=A[j];
                    if(A[j]->data<min->data)
                        min=A[j];
                }
            }
        }
    }
    void heapLink(Node* y,Node* x){
        y->left->right=y->right;
        y->right->left=y->left;
        y->parent = x;
        if(x->child!=NULL){
            Node* temp = x->child->right;
            x->child->right = y;
            y->left = x->child;
            y->right = temp;
            temp->left = y;
        }
        else{
            x->child = y;
            y->right=y;
            y->left=y;
        }
        x->degree = x->degree + 1;
        y->mark = false;
        
    }
    //not needed constructor does the work
    void createHeap(){
        min=NULL;
        n=0;
    }

  public:
    Node* min;
    int n;
    bool traceVal;
    Heap(){
        min=NULL;
        n=0;
        traceVal=false;
    }
    void insert(int key){
        Node* temp = new Node();
        temp->data = key;
        n = n+1;
        if(min == NULL){
            min = temp;
        }
        else{
            Node* t = min->right;
            min->right = temp;
            temp->left = min;
            temp->right = t;
            t->left = temp;
            if(key<min->data)
                min=temp;
        }
        if(traceVal) display();
    }
    void mergeHeap(Heap *h1){
        Node* start = h1->min;
        Node* end = h1->min->right;
        Node* temp = this->min->right;
        this->min->right = end;
        end->left = this->min;
        start->right = temp;
        temp->left=start;
        if(this->min==NULL || (h1->min!=NULL && h1->min->data<this->min->data)){
            this->min = h1->min;
        }
        this->n=this->n+h1->n;
        if(traceVal) display();
    }
    int findMin(){
        return min->data;
    }
    Node* extractMin(){
        Node* z = min;
        if(z != NULL){
            Node* child = z->child;
            Node* i = child;
            //for each child of z
           if(i!=NULL){
               for(int j=0;j<z->degree;j++){
                    //move node to root list
                    Node* x=i->right;
                    Node* t = min->right;
                    min->right = i;
                    i->left = min;
                    i->right = t;
                    t->left=i;
                    i->parent = NULL;
                    i = x;
                }
            }
            //remove z from root list
            z->left->right=z->right;
            z->right->left=z->left;
            if(z==z->right)
                min=NULL;
            else{
                min=z->right;
                consolidate();
            }
            n=n-1;
        }
        if(traceVal) display();
        return z;
    }
    //dfs
    void display(){/*
        Node* temp = min->right;
        while(temp!=min){
            cout<<temp->data<<"-";
            temp=temp->right;
        }
        cout<<temp->data<<endl;*/
        if(min==NULL){
            cout<<"heap empty"<<endl;
        }
        else{
            dispR(min);
            cout<<endl;
        }
    }
    void dispRt(Node* r,Node* min){
        cout<<r->data<<":"<<min->data<<"x";
        if(r!=min->left){
            if(r->child!=NULL){
                cout<<"->(";
                dispRt(r->child,r->child);
                cout<<")";
           }
            else{
                cout<<",";
                dispRt(r->right,min);
            }
            Node* temp=r->right;
        }
    }
    void dispR(Node* r){
        cout<<r->data;
        if(r->child!=NULL){
            cout<<"->(";
            dispR(r->child);
            cout<<")";
        }
        cout<<",";
        Node* temp=r->right;
        do{
            if(temp!=r){
                cout<<temp->data;
                if(temp->child!=NULL){
                    cout<<"->(";
                    dispR(temp->child);
                    cout<<")";
                }
                cout<<",";
            }
                temp=temp->right;
            }while(temp!=r->right);
    }
    void decreaseKey(Node* x,int k){
        if(x==NULL||k>x->data){cout<<"error value greater than current."<<endl;}
        else{
            x->data=k;
            Node* y = x->parent;

            if(y!=NULL && y->data>x->data){
                cut(x,y);
                cascadingCut(y);
            }
            if(x->data<min->data){
                min=x;
            }
        }
        if(traceVal) display();
    }
    void cut(Node* x,Node* y){
        if(x->right==x){
            y->child=NULL;
        } 
        else{
            if(y->child==x){
                y->child=x->right;
            }
            x->right->left=x->left;
            x->left->right=x->right;
        }
        y->degree--;
        Node* temp=min->right;
        min->right=x;
        x->left=min;
        x->right=temp;
        temp->left=x;
        x->parent=NULL;
        x->mark=false;
    }
    void cascadingCut(Node* y){
        Node* z = y->parent;
        if(z!=NULL){
            if(y->mark==false){
                y->mark=true;
            }
            else{
                cut(y,z);
                cascadingCut(z);
            }
        }
    }
    void freeHeap(){
        Node* temp;
        int k =0;
        int x = n;
        while(k<x){
            k++;
            temp = extractMin();
            delete(temp);
        }
        min=NULL;
        n=0;
    }
    void trace(bool val){
        traceVal=val;
    }
};


map<string,Heap*> Hvar;
map<string,Node*> Nvar;
int execute(vector<string> command){
    if(command[0]=="create"){
        Heap *H1 =new Heap();
        Hvar.insert(pair<string,Heap*>(command[1],H1));
    }
    
    else if(command[0]=="insert"){
        Heap *h = Hvar[command[1]];
        h->insert(stoi(command[2],NULL));
    }
    
    else if(command[0]=="merge"){
        Heap *h1 = Hvar[command[1]];
        Heap *h2 = Hvar[command[2]];
        h1->mergeHeap(h2);
    }
    else if(command[0]=="findMin"){
        Heap *h = Hvar[command[1]];
        int min = h->findMin();
        cout<<"Min: "<<min<<endl;
    }
    else if(command[0]=="extractMin"){
        Heap *h = Hvar[command[1]];
        Node *z=h->extractMin();
        cout<<"Min: "<<z->data<<endl;
    }
    else if(command[0]=="decreaseKey"){
        Heap *h = Hvar[command[1]];
        Node *z = Nvar[command[2]];
        int k = stoi(command[3],NULL);
        h->decreaseKey(z,k);
    }
    else if(command[0]=="free"){
        Heap *h = Hvar[command[1]];
        h->freeHeap();
        Hvar[command[1]]=NULL;
    }
    else if(command[0]=="trace"){
        Heap *h1 = Hvar[command[1]];
        bool res = (command[2]=="1")?true:false;
        h1->trace(res);
    }
    else if(command[0]=="display"){
        Heap *h = Hvar[command[1]];
        h->display();
    }
    else if(command[0]=="exit"){
        return 1;
    }
    else{
        if(Hvar.find(command[0])!=Hvar.end()){
            Heap *h = Hvar[command[0]];
            Node *z=h->min;
            loop(i,1,command.size()-1){
                if(command[i]=="parent"){
                    z=z->parent;
                }
                if(command[i]=="child"){
                    z=z->child;
                }
                if(command[i]=="right"){
                    z=z->right;
                }
                if(command[i]=="left"){
                    z=z->left;
                }
            }
            Nvar[command[command.size()-1]]=z;
        }
        else{
            cout<<"variable does not exists"<<endl;
        }
    }
    return 0;
}

int main(){
    //create <variable>
    //display <variable>
    //insert <variable> value
    //merge <var1> <var2>
    //findMin <var>
    //extractMin <var>
    //<heap variable> <right/left/parent/child> <node variable>
    //decreaseKey <heap variable> <node variable> <value>
    int exit=0;
    string command;
/*    map<string,Heap*> mp;
    Heap *h1 = new Heap();
    mp.insert(pair<string,Heap*>("h1",h1));
    Heap *h = mp["h1"];
    h->insert(5);
    h=mp["h1"];
    h->display();
*/
    while(!exit){
        getline(cin,command);
        vector<string> comm;
        stringstream ss(command);
        string item;
        while(getline(ss,item,' ')){
            comm.push_back(item);
        }
        /*
        loop0(i,comm.size()){
            cout<<i<<comm[i]<<endl;
        }
        */
        exit=execute(comm);
    }
    return 0;
}
