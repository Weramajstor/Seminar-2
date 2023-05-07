#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*
piso bum sa new i delete da dobim osjecaj za obiène c++ pointere
*/


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
	
	
	cvor( T _val , int _info){
		val=_val;
		info=_info;
		deg=0;
		marked=false;
		
		parent=left=right=child=nullptr;
	}
	
	cvor(){
		deg=0;
		marked=false;
		
		parent=left=right=child=nullptr;
	}//treba bit i defaultni konstruktor iz razloga(jer onda instanciranje cvor<T> * ne valja
	
	
	
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

typedef pair<int,int> pii;

const int velicina=10123123;

vector<pii> sus[velicina];

ll dist[velicina];
int bio[velicina];
const int puno=1123123123;


void solve(){
	
	int n,m;
	
	scanf("%d%d",&n,&m);
	
	memset(bio,0,sizeof(bio));
	for(int i=0;i<velicina;i++){
		dist[i]=puno;
		sus[i].clear();
	}
	
	while(m){
		int a,b,c;
		scanf("%d%d%d",&a,&b,&c);
		sus[a].push_back( pii(b,c) );
		m--;
	}
	
	
	int A,B;
	scanf("%d%d",&A,&B);
	
	FibonacciHeap<int> hrpa = FibonacciHeap<int>(velicina);
	
	hrpa.insert( 0 , A );
	
	chrono::steady_clock::time_point begin = chrono::steady_clock::now();
	double mikrosek=0;
	
	while(hrpa.size()){
		
//		cout<<"hm? "<<hrpa.size()<<endl;
		
		pair<int,int> par=hrpa.extractMin();
		
		int cur=par.second;
		int val=par.first;
		
//		cout<<"cur "<<cur<<" "<<val<<endl;
		
		bio[cur]=1;
		dist[cur]=val;
		
		for(int i=0;i<sus[cur].size();i++){
			int z = sus[cur][i].first;
			int c = dist[cur] + sus[cur][i].second;
			
//			cout<<"z "<<z<<" "<<c<<endl;
			
			if(bio[z])continue;
//			cout<<"prv"<<endl;
			if( hrpa.ima(z)==false ){
//				cout<<"nema"<<endl;
				hrpa.insert( c , z );
				dist[z]=c;
			}
			else if( c < dist[z] ){
//				cout<<"ima"<<endl;
				dist[z]=c;
				hrpa.decreaseKey( z , dist[z] );
			}
			
		}
		
//		cout<<"izlaz"<<endl;
	}
	
	if(dist[B]==puno){
		printf("NO\n");
	}
	else{
		printf( "%d\n" ,dist[B]);
	}
	
	chrono::steady_clock::time_point end = chrono::steady_clock::now();
	mikrosek=chrono::duration_cast<chrono::microseconds>(end - begin).count();
	cout<<mikrosek/1e6<<endl;
	
}


int main(){
	
	
//	int t;
//	scanf("%d",&t);
//	while(t){
//		solve();
//		t--;
//	}


	solve();
	
	return 0;
}




