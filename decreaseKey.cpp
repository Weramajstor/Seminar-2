/*
Usporeddba binarne i Fibonaccijeve hrpe na slučaju 
kada je 1123123 decrease key operacija gdje između
svakih 100 bude jedna extractMin operacija
*/

#include <bits/stdc++.h>
#include <windows.h>
 #include<unistd.h>
using namespace std;


typedef long long ll;


template<typename T>
struct cvor{
	
	T val;//numericka vrijednost cvora, int i long long primarno, s floatovima bi mozda radilo li-la
	int info;//najcesce cemu pripada ovaj cvor(kojem cvoru u grafu u dijskstri npr)
	int deg;
	bool marked;
	
	cvor* parent;// left, right, child;
	cvor* left;
	cvor* right;
	cvor* child;
	
	cvor(){
		deg=0;
		marked=false;
		
		parent=left=right=child=nullptr;
	}//treba bit i defaultni konstruktor iz razloga(jer onda instanciranje cvor<T> * ne valja
	
	cvor( T _val , int _info){
		
		val=_val;
		info=_info;
		
		deg=0;
		marked=false;
		
		parent=left=right=child=nullptr;//ovo mozda treba mjenjat ovisno o
									 //mjestu inserta
		
	}
	
	
};


template<typename T>
struct FibonacciHeap{
	
	int n=0;
	cvor<T> * mini;
	vector< cvor<T> >cvorovi;
//	vector<cvor<T> > stek;
	T malo=numeric_limits<T>::min();
	
	FibonacciHeap(int n){
		cvorovi.resize(n+5);
//		stek.resize(n+5);
	}
	
	T minimum(){
		return mini->val;
	}
	
	void ispisi( cvor<T> * ptr ){
		cout<<ptr->val<<" "<<ptr->info<<endl;
	}
	
	int size(){
		return n;
	}
	
	inline void brisi( cvor<T> * ptr ){
//		cout<<"brise se "<<ptr->info<<endl;
//		cvorovi[ptr->info]=nullptr;
//		delete ptr;
//		ptr=nullptr;
	}
	
	inline void spoji( cvor<T>* stari , cvor<T>* novi ){//stavi jednog izmedu 2
	
		novi->left=stari;
		novi->right=stari->right;
		
		(stari->right)->left=novi;
		stari->right=novi;
		
	}
	
	inline void spoji2( cvor<T>* lev , cvor<T>* des ){//spoji nakon brisanja ovog izmedu njih
		lev->right=des;
		des->left=lev;
	}
	
	void struktura( cvor<T> * hm ){
	//	cout<<"struktur"<<endl;
		cvor<T>* iduci = hm->right;
		
		vector<cvor<T> * > vek;
		
		vek.push_back(hm);
		
		if( iduci!=hm ){
			int k=0;
			while(iduci!=hm){
				k++;
		//		if(k>12)exit(0);
				vek.push_back(iduci);
				iduci=iduci->right;
			}
		}
		
		for(int i=0;i<vek.size();i++){
			cvor<T> * ptr = vek[i];
			cout<<ptr->val<<" "<<ptr->info<<" ";
			if(ptr->parent==nullptr){
				cout<<-1<<endl;
			}
			else cout<<(ptr->parent)->info<<endl;
			
			cout<<vek.size()<<endl;
			
			if(vek[i]->child!=nullptr){
				struktura( vek[i]->child );
			}
		}
		
	}
	
	void insert( T val , int info){//tu treba dodat doavanje lijevo i desno
		
//		stek[info]=cvor<T>( val, info );
//		cvor<T>* novi = &stek[info];
		cvorovi[info]=cvor<T>( val, info );
		cvor<T>* novi = cvorovi.data()+info;
		
//		cout<<"dio1"<<endl;
		
		if(!n){
			mini=novi;
			mini->right=mini;
			mini->left=mini;
		}
		else{
//			cout<<"spoji"<<endl;
			spoji( mini , novi );
//			cout<<"vanspoji"<<endl;
			if( novi->val < mini->val ){
				mini=novi;
			}
		}
//		cout<<"dio2"<<endl;
		n++;
//		cout<<"dio3"<<endl;

/*		cout<<"IZLAZ Insert"<<n<<endl;
		struktura(mini);
		cout<<"krajstrukt"<<endl;
*/
	}
	
	
	pair<T,int> extractMin(){
		
	//	system("pause");
	/*
		cout<<"ULAZ ExtractMin "<<n<<endl;
		struktura(mini);
		cout<<"krajstrukt"<<endl;
		*/
		pair<T,int> ret=pair<T,int>(mini->val, mini->info);
		
		if(!n)return pair<T,int>(-1,-1);//nece valjda doc do ovog
		
		if(n==1){
			brisi(mini);
			n--;
			return ret;
		}
		
		n--;
		
		vector< cvor<T>* > vek;
		
		cvor<T> * sin = mini->child;
		
		if(sin!=nullptr){
			
//			skipaj sve
//			cout<<"JEL JE? "<<mini->val<<" "<<mini->info<<endl;
//			cout<<sin->val<<" "<<sin->info<<endl;
			
			cvor<T> * iduci=sin->right;
//			cout<<iduci->val<<" "<<iduci->info<<endl;
			
			sin->parent=nullptr;
			vek.push_back(sin);
			
			while(iduci!=sin){
//				cout<<"eh"<<endl;
				vek.push_back(iduci);
				iduci->parent=nullptr;
				iduci=iduci->right;
			}
			
		}
//		else cout<<"NIJEFKT"<<endl;
		
		if( mini->right==mini ){//znaci da ima djece i nema susjeda
//			cout<<"1"<<endl;
//			ispisi(mini);
			brisi(mini);
			
//			cout<<vek.size()<<endl;
			mini=vek[0];
			
			mini->right=mini->left=mini;
			
			for(int i=1;i<vek.size();i++){
				spoji(mini,vek[i]);
			}
		}
		else{
//			cout<<"2"<<endl;
			cvor<T>*ptrR=mini->right;
			cvor<T>*ptrL=mini->left;
			brisi(mini);
			mini = ptrR;
			
//			ispisi(ptrL); ispisi(ptrR);
			
			spoji2( ptrL , ptrR );
			
			
			for(int i=0;i<vek.size();i++){
				spoji(mini,vek[i]);
			}
				
		}
		
		if(mini->right==mini){//ak je na kraju tog samo jedan gore
			return ret;
		}
		
		//pospajaj stvari u logn
		int logn=1;
		
		for( ; (1<<logn)<n ; logn++ );
		
		logn=2*logn;//?
		cvor<T> * d[logn+1];
		
		for(int i=0;i<=logn;i++)d[i]=nullptr;
		
		
		//nalazenje novog minimuma
		cvor<T> * iduci=mini->right;
		cvor<T> * tag=mini;
		T val=mini->val;
		
		int param2=mini->deg;
		
		while(iduci!=mini){
			
//			cout<<"perfect girl"<<endl;
			
			if( iduci->val<val  ){
				val=iduci->val;
				tag=iduci;
				param2=iduci->deg;
			}
			
			if( iduci->val==val && iduci->deg>param2 ){
				val=iduci->val;
				tag=iduci;
				param2=iduci->deg;
			}
			
			iduci=iduci->right;
		}
		
		mini=tag;
		
		iduci=mini;
//		ispisi(mini);
//		ispisi(iduci);

		
		while(1){
			
//			cout<<"poslije perfect girl"<<endl;
//			ispisi(iduci);
			cvor<T>*ptr=iduci->right;
			cvor<T>*pom;
			
			if(d[ iduci->deg ]==nullptr){
				d[ iduci->deg ]=iduci;
//				cout<<"okej "<<iduci->deg<<endl;
			}
			else{
				
				while( d[iduci->deg]!=nullptr ){//bilo je i s ovim degovima problema
					
//					cout<<"degree\n";
					
	//				cout<<iduci->deg<<endl;
				
					if( iduci->val < d[iduci->deg]->val ){
						pom=d[iduci->deg];
					}
					else{
//						cout<<"ovdjemsm"<<endl;
						pom=iduci;
						iduci=d[iduci->deg];
					}
					
/*					
					cout<<"===\n";
					ispisi(iduci);ispisi(pom);
					cout<<"===\n";
*/

					if(iduci->deg==0){// OVDJE TREBA POPRAVIT OVO U SMISLU DA right i left ne valja vjv
						iduci->child=pom;
						pom->parent=iduci;
						spoji2( pom->left , pom->right );
						
						pom->left=pom->right=pom;
//						cout<<"ovd1"<<endl;
						d[iduci->deg]=nullptr;
						iduci->deg++;
				/*		cout<<"struktura\n";
						struktura( mini );
						cout<<"krajstrukture\n";*/
					}
					else{
						pom->parent=iduci;
						spoji2( pom->left , pom->right);
						
						spoji( iduci->child , pom );
//						cout<<"ovd2"<<endl;

						d[iduci->deg]=nullptr;
						iduci->deg++;
				/*		cout<<"MINI:\n";
						ispisi(mini);
						cout<<"degree: "<<iduci->deg<<endl;
						cout<<"struktura\n";
						struktura( mini );
						cout<<"krajstrukture\n";
						system("pause");*/
					}
					
				}
				
				d[iduci->deg]=iduci;
				
			}
			
//			system("pause");
			
			iduci=ptr;
			if(iduci==mini){
				break;
			}
		}
		
		/*
		cout<<"IZLAZ ExtractMin "<<n<<endl;
		struktura(mini);
		cout<<"krajstrukt"<<endl;
		ispisi(mini);
		system("pause");
		*/
		return ret;
	}
	
	inline void cut( cvor<T> * child , cvor<T> * parent ){
		
		child->parent=nullptr;
		child->marked=false;
		
		cvor<T> * susybaka = child->right;
		
		if(parent->deg==2){
			spoji2( susybaka, susybaka );
		}
		else if(parent->deg>2){
			spoji2( child->left , susybaka );
		}
		
		spoji( mini , child );
		
		if( parent->deg==1 ){
			parent->child=nullptr;
		}
		else{
			parent->child=susybaka;
		}
		
		parent->deg--;
		
	}
	
	void decreaseKey( int k , int val ){//koji, value
		/*
		cout<<"ULAZ DecreaseKey "<<n<<" meta k: "<<k<<endl;
		struktura(mini);
		cout<<"krajstrukt"<<endl;
		system("pause");
		*/
		cvor<T> * meta = cvorovi.data()+k;
		
		meta->val=val;//alternativno meta->val-=val
		
		if(meta->parent!=nullptr && meta->val<(meta->parent)->val){
			
	//		ispisi(meta);ispisi(meta->parent);
			
			cvor<T> * cur=meta->parent;
			
			cut(meta,meta->parent);
/*
			cout<<"===\n";
			struktura(mini);
			cout<<"===\n";
*/

			while(cur->parent!=nullptr){
//				cout<<"decreaskey"<<endl;
				if(cur->marked==false){
					cur->marked=true;
					break;
				}
				else{
					cvor<T> * ptr=cur->parent;
					cut( cur , cur->parent );
					cur=ptr;
				}
				
			}
			
		}
		
		if (meta->val<mini->val){
			mini=meta;
		}
		/*
		system("pause");
		cout<<"izlaz DecreaseKey "<<n<<" meta k: "<<k<<endl;
		struktura(mini);
		cout<<"krajstrukt"<<endl;
		system("pause");
		*/
	}
	
	
	void deletaj( int k ){//ovo mi u biti opce ne treba za dijkstru
		
		cvor<T> * meta = cvorovi.data()+k;
		
		meta->val=malo;
		
		decreaseKey( k , malo );
		
		extractMin();
		
	}
	
	bool ima( int k ){
		if(cvorovi.data()+k==nullptr)return false;
		else return true;
	}
	
	void ocisti(){
		//sam se vektor ocisti
	}
	
};









//GEEKS FOR GEEKS
// Prototype of a utility function to swap two integers
void swap(int *x, int *y);
 
// A class for Min Heap
class MinHeap//geeks4geeks kod
{
    int *harr; // pointer to array of elements in heap
    int capacity; // maximum possible size of min heap
    int heap_size; // Current number of elements in min heap
public:
    // Constructor
    MinHeap(int capacity);
 
    // to heapify a subtree with the root at given index
    void MinHeapify(int );
 
    int parent(int i) { return (i-1)/2; }
 
    // to get index of left child of node at index i
    int left(int i) { return (2*i + 1); }
 
    // to get index of right child of node at index i
    int right(int i) { return (2*i + 2); }
 
    // to extract the root which is the minimum element
    int extractMin();
 
    // Decreases key value of key at index i to new_val
    void decreaseKey(int i, int new_val);
 
    // Returns the minimum key (key at root) from min heap
    int getMin() { return harr[0]; }
 
    // Deletes a key stored at index i
    void deleteKey(int i);
 
    // Inserts a new key 'k'
    void insertKey(int k);
    
    //cisti memoriju
    void ocisti();
};
 
// Constructor: Builds a heap from a given array a[] of given size
MinHeap::MinHeap(int cap)
{
    heap_size = 0;
    capacity = cap;
    harr = new int[cap];
}
 
// Inserts a new key 'k'
void MinHeap::insertKey(int k)
{
    if (heap_size == capacity)
    {
        cout << "\nOverflow: Could not insertKey\n";
        return;
    }
 
    // First insert the new key at the end
    heap_size++;
    int i = heap_size - 1;
    harr[i] = k;
    
    // Fix the min heap property if it is violated
    while (i != 0 && harr[parent(i)] > harr[i])
    {
       swap(&harr[i], &harr[parent(i)]);
       i = parent(i);
    }
    
}
 
// Decreases value of key at index 'i' to new_val.  It is assumed that
// new_val is smaller than harr[i].
int ve=0;
void MinHeap::decreaseKey(int i, int new_val)
{
//	ve=0;
    harr[i] = new_val;
    while (i != 0 && harr[parent(i)] > harr[i])
    {
//    	ve++;
       swap(&harr[i], &harr[parent(i)]);
       i = parent(i);
    }
//    cout<<"ve "<<ve<<endl;
}
 
// Method to remove minimum element (or root) from min heap
int MinHeap::extractMin()
{
    if (heap_size <= 0)
        return INT_MAX;
    if (heap_size == 1)
    {
        heap_size--;
        return harr[0];
    }
 
    // Store the minimum value, and remove it from heap
    int root = harr[0];
    harr[0] = harr[heap_size-1];
    heap_size--;
    MinHeapify(0);
 
    return root;
}

void MinHeap::ocisti(){
	delete[] harr;
}
 
// This function deletes key at index i. It first reduced value to minus
// infinite, then calls extractMin()
void MinHeap::deleteKey(int i)//NULA INDEKSIRANO 000000000
{
    decreaseKey(i, INT_MIN);
    extractMin();
}
 
// A recursive method to heapify a subtree with the root at given index
// This method assumes that the subtrees are already heapified
void MinHeap::MinHeapify(int i)
{
    int l = left(i);
    int r = right(i);
    int smallest = i;
    if (l < heap_size && harr[l] < harr[i])
        smallest = l;
    if (r < heap_size && harr[r] < harr[smallest])
        smallest = r;
    if (smallest != i)
    {
        swap(&harr[i], &harr[smallest]);
        MinHeapify(smallest);
    }
}
 
// A utility function to swap two elements
void swap(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}
 
const int N=100123;
 
//bool bio[N];
 
// Driver program to test above functions
int main(){
	
	
//    MinHeap h(11);
//    h.insertKey(3);
//    h.insertKey(2);
//    cout << h.getMin() << " ";
//    h.deleteKey(0);
//    cout << h.getMin() << " ";
//    h.insertKey(15);
//    h.insertKey(5);
//    h.insertKey(4);
//    h.insertKey(45);
//    cout << h.extractMin() << " ";
//    cout << h.getMin() << " ";
//    h.decreaseKey(2, 1);
//    cout << h.getMin();
    int num_nodes=N, kol =1e3;
    int br_mjerenja=1;
    int t;
    cout<<"unesi 2 na koju i koliko decreaseKeyova da prode algoritam po velicini strukture"<<endl;
    cin>>t>>kol;
    
    srand(time(NULL));
    
    for(int g=1;g<=t;g++){//unos najvise moze bit 25
    	
    	num_nodes=(1<<(g))-1;
	    cout<<g<<" "<<num_nodes<<endl;
	    
	    int niz[kol+5];
	    
	    double mikrosek=0;
	    
		chrono::steady_clock::time_point begin;
		chrono::steady_clock::time_point end;
		
		int baza=( (1<<(g-1)) - 1 );
		int prostor=1<<(g-1);
		
		cout<<"binary_heap"<<endl;
		
		double smol=20*1123123, bigus=-5;
		
		
	    for(int korak=0;korak<br_mjerenja;korak++){
	    	
	    	for(int i=0;i<kol;i++){
//				niz[i]=(rand()*rand() + rand())%prostor+baza;
				niz[i]=(rand()*rand() + rand())%num_nodes;
			}
	    	
	    	MinHeap h(num_nodes);
	    	
		    for(int i=0;i<num_nodes;i++){
				h.insertKey( num_nodes-i );
		    }
		    
			begin = chrono::steady_clock::now();
			
//			int ka=0,br=0,to=0;
//			int z=num_nodes;
			int ka=0;
			for(int j=0;j<kol;j++){
//				h.decreaseKey( (ll)((ll)j*j*343+j)%(num_nodes/2)+num_nodes/2-1 , (int) h.getMin()-1 );
				h.decreaseKey( niz[j] , (int) h.getMin()-1 );
//				h.decreaseKey( (rand()*rand() + rand())%(num_nodes/2)+num_nodes/2-1 , (int) h.getMin()-1 );
				ka++;
				if(ka==100){
					int ze=h.extractMin();
					h.insertKey( ze+10 );
					ka=0;
				}
			}
	    	
	    	end = chrono::steady_clock::now();
	    	double lok=chrono::duration_cast<chrono::microseconds>(end - begin).count();
	    	mikrosek+=lok;
	    	
	    	smol=min(smol,lok);
	    	bigus=max(bigus,lok);
	    	h.ocisti();
	    }
	    
	    cout << mikrosek/1e6/br_mjerenja <<"  min i max "<< smol/1e6<< " "<< bigus/1e6 << endl;
	    
		mikrosek=0;
		smol=20*1123123, bigus=-5;
		
		cout<<"fibonacci"<<endl;
		
			
		for(int korak=0;korak<br_mjerenja;korak++){
			
			for(int i=0;i<kol;i++){
				niz[i]=(rand()*rand() + rand())%num_nodes+1;
			}
			
			FibonacciHeap<int> hrpa = FibonacciHeap<int>(num_nodes+3);
			
			for(int i=1;i<=num_nodes;i++){
		    	hrpa.insert( 89 , i );
		    }
		    
			
			pair<int,int> par2=hrpa.extractMin();
			hrpa.insert( par2.first , par2.second );//zapravo napravi vise operacija jer se ovdje jebe s vise elemenata
			
			begin = chrono::steady_clock::now();
			
			int to=1;
			int ka=0;
			for(int j=0;j<kol;j++){
		//		if(j%1000==0)cout<<j<<endl;
		//		if(bio[to])continue;
//				hrpa.decreaseKey( (ll)((ll)j*j*343+j)%num_nodes+1 , hrpa.minimum()-1 );//bre od rng
//				hrpa.decreaseKey( (rand()*rand() + rand())%num_nodes+1 , (int) hrpa.minimum()-1 );
				hrpa.decreaseKey( niz[j] , hrpa.minimum()-1 );
//				hrpa.decreaseKey( ((ll)j*j+j+13)%num_nodes+1 , hrpa.minimum()-1 );
				ka++;
				if(ka==100){
					pair<int,int> par=hrpa.extractMin();
					hrpa.insert( par.first+5 , par.second );
					ka=0;
				}
			}
				
			end = chrono::steady_clock::now();
	    	double lok=chrono::duration_cast<chrono::microseconds>(end - begin).count();
	    	mikrosek+=lok;
	    	
	    	smol=min(smol,lok);
	    	bigus=max(bigus,lok);
			hrpa.ocisti();
		}
		
		cout << mikrosek/1e6/br_mjerenja <<"  min i max "<< smol/1e6<< " "<< bigus/1e6 << endl;
		cout<<"========="<<endl;
		cout<<"========="<<endl;
	
	}
	
	system("pause");
	return 0;
}




