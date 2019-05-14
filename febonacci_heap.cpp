#include <cmath> 
#include <cstdlib> 
#include <iostream> 
#include <malloc.h> 
#include <vector>
#include<limits>
#include<map>
using namespace std;



template <class V> class FibonacciHeap;
 
template <class V> struct node {
private:
	node<V>* prev;
	node<V>* next;
	node<V>* child;
	node<V>* parent;
	V value;
	int degree;
	bool marked;
public:
	friend class FibonacciHeap<V>;
	node<V>* getPrev() {return prev;}
	node<V>* getNext() {return next;}
	node<V>* getChild() {return child;}
	node<V>* getParent() {return parent;}
	V getValue() {return value;}
	bool isMarked() {return marked;}
 
	bool hasChildren() {return child;}
	bool hasParent() {return parent;}
};
 
template <class V> class FibonacciHeap {
protected:
	node<V>* heap;
public:
 
	FibonacciHeap() {
		heap=_empty();
	}
	virtual ~FibonacciHeap() {
		if(heap) {
			_deleteAll(heap);
		}
	}
	node<V>* insert(V value) {
		node<V>* ret=_singleton(value);
		heap=_merge(heap,ret);
		return ret;
	}
	void merge(FibonacciHeap& other) {
		heap=_merge(heap,other.heap);
		other.heap=_empty();
	}
 
	bool isEmpty() {
		return heap==NULL;
	}
 
	V getMinimum() {
		return heap->value;
	}
 
	V removeMinimum() {
		node<V>* old=heap;
		heap=_removeMinimum(heap);
		V ret=old->value;
		delete old;
		return ret;
	}
 
	void decreaseKey(node<V>* n,V value) {
		heap=_decreaseKey(heap,n,value);
	}
 
	node<V>* find(V value) {
		return _find(heap,value);
	}
private:
	node<V>* _empty() {
		return NULL;
	}
 
	node<V>* _singleton(V value) {
		node<V>* n=new node<V>;
		n->value=value;
		n->prev=n->next=n;
		n->degree=0;
		n->marked=false;
		n->child=NULL;
		n->parent=NULL;
		return n;
	}
 
	node<V>* _merge(node<V>* a,node<V>* b) {
		if(a==NULL)return b;
		if(b==NULL)return a;
		if(a->value>b->value) {
			node<V>* temp=a;
			a=b;
			b=temp;
		}
		node<V>* an=a->next;
		node<V>* bp=b->prev;
		a->next=b;
		b->prev=a;
		an->prev=bp;
		bp->next=an;
		return a;
	}
 
	void _deleteAll(node<V>* n) {
		if(n!=NULL) {
			node<V>* c=n;
			do {
				node<V>* d=c;
				c=c->next;
				_deleteAll(d->child);
				delete d;
			} while(c!=n);
		}
	}
 
	void _addChild(node<V>* parent,node<V>* child) {
		child->prev=child->next = child;
		child->parent = parent;
		parent->degree++;
		parent->child=_merge(parent->child,child);
	}
 
	void _unMarkAndUnParentAll(node<V>* n) {
		if(n==NULL)return;
		node<V>* c=n;
		do {
			c->marked=false;
			c->parent=NULL;
			c=c->next;
		}while(c!=n);
	}
 
	node<V>* _removeMinimum(node<V>* n) {
		_unMarkAndUnParentAll(n->child);
		if(n->next==n) {
			n=n->child;
		} else {
			n->next->prev=n->prev;
			n->prev->next=n->next;
			n=_merge(n->next,n->child);
		}
		if(n==NULL)return n;
		node<V>* trees[64]={NULL};
 
		while(true) {
			if(trees[n->degree]!=NULL) {
				node<V>* t=trees[n->degree];
				if(t==n)break;
				trees[n->degree]=NULL;
				if(n->value<t->value) {
					t->prev->next=t->next;
					t->next->prev=t->prev;
					_addChild(n,t);
				} else {
					t->prev->next=t->next;
					t->next->prev=t->prev;
					if(n->next==n) {
						t->next=t->prev=t;
						_addChild(t,n);
						n=t;
					} else {
						n->prev->next=t;
						n->next->prev=t;
						t->next=n->next;
						t->prev=n->prev;
						_addChild(t,n);
						n=t;
					}
				}
				continue;
			} else {
				trees[n->degree]=n;
			}
			n=n->next;
		}
		node<V>* min=n;
		do {
			if(n->value<min->value)min=n;
			n=n->next;
		} while(n!=n);
		return min;
	}
 
	node<V>* _cut(node<V>* heap,node<V>* n) {
		if(n->next==n) {
			n->parent->child=NULL;
		} else {
			n->next->prev=n->prev;
			n->prev->next=n->next;
			n->parent->child=n->next;
		}
		n->next=n->prev=n;
		n->marked=false;
		return _merge(heap,n);
	}
 
	node<V>* _decreaseKey(node<V>* heap,node<V>* n,V value) {
		if(n->value<value)return heap;
		n->value=value;
		if(n->value<n->parent->value) {
			heap=_cut(heap,n);
			node<V>* parent=n->parent;
			n->parent=NULL;
			while(parent!=NULL && parent->marked) {
				heap=_cut(heap,parent);
				n=parent;
				parent=n->parent;
				n->parent=NULL;
			}
			if(parent!=NULL && parent->parent!=NULL)parent->marked=true;
		}
		return heap;
	}
 
	node<V>* _find(node<V>* heap,V value) {
		node<V>* n=heap;
		if(n==NULL)return NULL;
		do {
			if(n->value==value)return n;
			node<V>* ret=_find(n->child,value);
			if(ret)return ret;
			n=n->next;
		}while(n!=heap);
		return NULL;
	}
};





struct nodeValue{
int priority;
int row;
int col;
nodeValue(int p,int r,int c): priority(p) ,row(r), col(c){}
};










int shortestDistance(vector<vector<int>> graph, vector<int> start, vector<int> end)
{
	int m = graph.size();
	int n = graph[0].size();
	int row = start[0];
	int col = start[1];
	map<int, map<int, int>> distance;

	int inf = numeric_limits<int>::max();
	for (int i = 0; i < m; i++)
	{
		distance[i] = map<int, int>();
		for (int j = 0; j < n; j++)
		{
			distance[i][j] = inf;
		}
	};

	int rowSize = graph.size();
	int colSize = graph[0].size();

	vector<vector<int>> dir = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}};


nodeValue val (0,row,col);
FibonacciHeap<nodeValue> heap;
heap.insert(val);  

	//insertion(0, row, col);
// no_of_nodes
	while (!heap.isEmpty())
	{
		//keyValues *queue = mini->values;
		cout << queue->row << "----queue before--->" << queue->col << "\n";
		//Extract_min();
		//  if( distance.count(queue->row) && distance[queue->row].count(queue->col) &&  distance[queue->row][queue->col] <= queue->priority)continue;
		distance[queue->row][queue->col] = queue->priority;
		for (int i = 0; i < 4; i++)
		{
			int row = queue->row;
			int col = queue->col;
			int distancePriority = queue->priority;
			while (row >= 0 && row < rowSize && col >= 0 && col < colSize && graph[row][col] == 0)
			{
				row += dir[i][0];
				col += dir[i][1];
				distancePriority++;
			};
			row -= dir[i][0];
			col -= dir[i][1];
			distancePriority--;
			if (queue->priority + distancePriority < distance[row][col])
			{
		//		insertion(distancePriority, row, col);
				distance[row][col] = queue->priority + distancePriority;
			}
		};
	};

	return distance[end[0]][end[1]] == inf? -1 : distance[end[0]][end[1]];

}





int main(){

FibonacciHeap<int> heap; 



heap.insert(9);
heap.insert(11);
heap.insert(20);
heap.removeMinimum(); 
cout << heap.getMinimum();


    return 0; 
}