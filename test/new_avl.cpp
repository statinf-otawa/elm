/*
 *	$Id$
 *	Copyright (c) 2003-07, IRIT UPS.
 *
 *	New AVL implementation.
 */

#include <elm/assert.h>
#include <elm/utility.h>
#include <elm/util/Option.h>
#include <stdlib.h>
 
using namespace elm;

inline int max(int x, int y) { return x > y ? x : y; }
inline int min(int x, int y) { return x > y ? y : x; }
inline int abs(int x) { if(x > 0) return x; else return -x; }

class AbstractAVLTree {
	static const int MAX_HEIGHT = 32;	
protected:
	
	class Node {
	public:
		Node(void): height(1) { link[0] = 0; link[1] = 0; }
		int height;
		Node *link[2];
		void computeHeight(void) {
			int lh = link[0] ? link[0]->height : 0;
			int rh = link[1] ? link[1]->height : 0;
			height = max(lh, rh) + 1;
		}
	};
	
	class Stack {
	public:
		inline Stack(AbstractAVLTree& _tree): tree(_tree), top(0)
			{ nodes[0] = tree.root; }
		inline void push(int dir) {
			dirs[top] = dir;
			top++;
			nodes[top] = nodes[top - 1]->link[dir];
		}
		inline void pop(void) { top--; }
		inline void set(Node *node) {
			nodes[top] = node;
			if(!top) tree.root = node;
			else nodes[top - 1]->link[dirs[top - 1]] = node;
		}
		inline bool atLeaf(void) const { return !nodes[top]; }
		inline Node *node(void) { return nodes[top]; }
		inline bool atRoot(void) const { return !top; }
		inline int height(void) const { return nodes[top]->height; }
		inline int diff(int dir = 0) {
			int h0 = nodes[top]->link[dir] ? nodes[top]->link[dir]->height : 0;
			int h1 = nodes[top]->link[!dir] ? nodes[top]->link[!dir]->height : 0;
			return h0 - h1;
		}
		inline void computeHeight(void) {
			int h0 = nodes[top]->link[0] ? nodes[top]->link[0]->height : 0;
			int h1 = nodes[top]->link[1] ? nodes[top]->link[1]->height : 0;
			nodes[top]->height = max(h0, h1) + 1;			
		}
	private:
		int top;
		AbstractAVLTree& tree;
		Node *nodes[MAX_HEIGHT];
		char dirs[MAX_HEIGHT];
	};

	inline AbstractAVLTree(void): root(0) { }
	
	inline int height(Node *node) {
		if(!node)
			return 0;
		else
			return node->height;
	}
	
	Node *root;

	void balance(Stack& stack) {
		while(true) {
			int old_height = stack.height();
			switch(stack.diff()) {
			case -2:
				balance(stack, 1);
				break;
			case -1: case 0: case 1:
				break;
			case 2:
				balance(stack, 0);
				break;
			}
			stack.computeHeight();
			if(stack.atRoot())
				return;
			if(old_height == stack.height())
				return;
			stack.pop();
		}
	}

	/*void removeNode(Step *path, Node *node) {
		join(path, node->link[0], node->link[1]);
		balance(path);
	}*/
	
private:

	void rotate(Stack& stack, int dir) {
		Node *node = stack.node();
		Node *subl = node->link[dir], *subr = node->link[!dir];
		Node *subll = subl->link[dir], *sublr = subl->link[!dir];
		subl->link[dir] = subll;
		subl->link[!dir] = node;
		node->link[dir] = sublr;
		stack.set(subl);
		node->computeHeight();
		subl->computeHeight();
	}

	void balance(Stack& stack, int dir) {
		stack.push(dir);
		int diff = stack.diff(dir);
		if(diff >= 0) {
			cerr << "SINGLE ROTATE " << dir << ", " << (void *)stack.node() << "\n";
			stack.pop();
			rotate(stack, dir);			
		}
		else {
			cerr << "DOUBLE ROTATE " << dir << ", " << (void *)stack.node() << "\n";
			rotate(stack, !dir);
			stack.pop();
			rotate(stack, dir);
		}
	}	

	/*void join(Step *path0, Node *node1, Node *node2) {
		Step *path = path0;
		Node *nodes[2] = { node1, node2 };
		
		// Perform the join
		while(true) {
			if(!nodes[0]) {
				path->set(nodes[1]);
				break;
			}
			else if(!nodes[1]) {
				path->set(nodes[0]);
				break;
			}
			else {
				int dir = nodes[0]->height > nodes[1]->height; 
				path->set(nodes[!dir]);
				path++;
				*path = Step(nodes[!dir], dir);
				nodes[!dir] = nodes[!dir]->link[dir];
			}
		}
		
		// Update the links and height
		while(path != path0) {
			path->computeHeight();
			path--;
		}
	}*/
};




template <class T, class C = elm::Comparator<T> >
class AVLTree: public AbstractAVLTree {

	class Node: public AbstractAVLTree::Node {
	public:
		inline Node(const T& value): val(value) { }
		inline Node *_link(int dir) {
			ASSERT(dir == 0 || dir == 1);
			return static_cast<Node *>(link[dir]);
		}
		T val;
	};
	
public:
	inline AVLTree(void) { }

	Option<T> find(const T& item) {
		Node *node = look(item);
		if(!node)
			return none;
		else
			return some(node->val);
	}
	
	inline bool contains(const T& item) { return look(item); }
	
	T& find(const T& item, const T& def) {
		Node *node = look(item);
		if(!node)
			return def;
		else
			return node->val;
	}
	
	void insert(const T& item) {
		Stack stack(*this);
		
		// Find the position and build the stack
		while(!stack.atLeaf()) {
			Node *node = (Node *)stack.node();
			int cmp = C::compare(node->val, item);
			if(cmp)
				stack.push(_dir(cmp));
			else {
				node->val = item;
				return;
			}
		}
		stack.set(new Node(item));
		if(!stack.atRoot()) {
			stack.pop();
			balance(stack);
		}
	}
	
	/*void remove(const T& item) {
		Step stack[MAX_HEIGHT];
		int top = 1;
		stack[0] = Step();
		for(Node *cur = _root(); cur;) {
			// stack[top++] = cur;
			int cmp = C::compare(cur->val, item);
			if(!cmp) {
				removeNode(&stack[top - 1], cur);
				delete cur;
				return;
			}
			else {
				stack[top++] = Step(cur, _dir(cmp));
				cur = stack[top - 1].get();
			}
		}
		ASSERTP(false, "item not in the graph");
	}*/
	
#	ifndef NDEBUG
		void check(Node *node) {
			if(!node)
				return;
			ASSERT(abs(height(node->link[0]) - height(node->link[1])) <= 1);
			check(node->_link(0));
			check(node->_link(1));
		}
		
		void check(void) {
			check(_root());
		}
		
		void dump(Node *node, int tab = 0) {
			for(int i = 0; i < tab; i++)
				cout << '\t';
			if(!node) {
				cout << "NULL\n";
				return;
			}
			cout << node->val << " (" << node->height << ", " << (void *)node << ")" << io::endl;
			if(!node->link[0] && !node->link[1])
				return;
			dump(node->_link(0), tab + 1);
			dump(node->_link(1), tab + 1);
		}
		void dump(void) {
			dump(_root());
		}
#	endif
	
private:

	static inline int _dir(int cmp) { return cmp <= 0; }
	inline Node *_root(void) const { return static_cast<Node *>(root); }
	
	Node *look(const T&item) {
		for(Node *p = _root(); p;) {
			int cmp = C::compare(item, p->val);
			if(!cmp)
				return p;
			else
				 p = (Node *)p->_link(cmp > 0);
		}
		return 0;
	}
};


#define COUNT	1000
int main(void) {
	AVLTree<int> tree;
	int list[COUNT];
	int top = 0;
	
	// Test loop
	for(int i = 0; i < COUNT; i++) {
		
		int v = rand();
		
		// Add-on
		//if(!top || v & 0x1) {
			int newi = (v >> 10) * COUNT / (RAND_MAX >> 10);
			cout << "INSERTING " << newi << io::endl;
			tree.insert(newi);
			list[top++] = newi;
		//}
		
		// Removal
		/*else {
			int remi = (v >> 10) * top / ((RAND_MAX >> 10) + 1);
			cout << "REMOVING " << list[remi] << io::endl; 
			tree.remove(list[remi]);
			for(int i = remi + 1; i < top; i++)
				list[i - 1] = list[i];
			top--;
		}*/
		
		// Check all
		tree.dump();
		tree.check();
		for(int i = 0; i < top; i++)
			ASSERT(tree.contains(list[i]));
	}
}