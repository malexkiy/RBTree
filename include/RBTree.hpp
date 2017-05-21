#ifndef RB_TREE_HPP
#define RB_TREE_HPP

#include <iostream>
#include <sstream>
#include <initializer_list>

const char RED = 'r';	//0head01;
const char BLACK = 'b';	// 0head00;

template <typename T>
class RBTree;

template <typename T>
struct Node;

template <typename T>
std::ifstream& operator>>(std::ifstream&, RBTree<T>&);

template <typename T>
std::ofstream& operator<<(std::ofstream&, const RBTree<T>&);

template <typename T>
std::ostream& operator<<(std::ostream&, const RBTree<T>&);


template <typename T>
struct instance_counter
{
private:
	static size_t _count;
public:
	instance_counter<T>() { ++_count; }
	~instance_counter<T>() { --_count; }
	size_t getCount() const { return _count; }
};


template <typename T>
size_t instance_counter<T>::_count = 0;


template <typename T>
struct Node : instance_counter<Node<T>> {
	Node<T>* parent;
	Node<T>* left;
	Node<T>* right;
	long value;
	char color;

	Node()
		: value(value), color(RED), parent(nullptr), left(nullptr), right(nullptr) {}

	Node(long value, char color = RED, Node<T>* parent = nullptr, Node<T>* left = nullptr, Node<T>* right = nullptr)
		: value(value), color(color), parent(parent), left(left), right(right) {}
};


template <typename T>
class RBTree {
private:
	Node<T>* _root;

	void rotateLeft(Node<T>*);
	void rotateRight(Node<T>*);
	Node<T>* grandParent(Node<T>*) const;
	Node<T>* uncle(Node<T>*) const;
	Node<T>* sibling(Node<T>*) const;
	void insertBalance(Node<T>*);
	void deleteBalance(Node<T>*);

	Node<T>* _deleteRoot(Node<T>*);
	void _deleteElements(Node<T>*);

	void swap(RBTree<T>&);

public:
	RBTree();
	RBTree(const std::initializer_list<T>&);
	~RBTree();
	void insertElement(long value);
	void deleteElement(long value);
	bool findElement(long value) const;
	size_t count() const;

	bool operator==(const RBTree<T>&) const;

	std::ostream& inorder(std::ostream&) const;
	std::ostream& RBTree<T>::preorder(std::ostream&) const;

	//friend std::ostream& operator<< (std::ostream&, const RBTree&);

	std::ifstream& inFile(std::ifstream&);
	std::ofstream& outFile(std::ofstream&) const;

	friend std::ifstream& operator>> <> (std::ifstream&, RBTree<T>&);

	friend std::ofstream& operator<< <> (std::ofstream&, const RBTree<T>&);

	friend std::ostream& operator<< <> (std::ostream&, const RBTree<T>&);
};


template <typename T>
RBTree<T>::RBTree()
{
	_root = nullptr;
}


template <typename T>
void RBTree<T>::swap(RBTree<T>& other)
{
	if (_root != other._root)
	{
		Node<T>* tmp = _root;
		_root = other._root;
		other._root = tmp;
	}
}


template <typename T>
RBTree<T>::RBTree(const std::initializer_list<T>& list)
{
	_root = nullptr;
	for (auto it = list.begin(); it != list.end(); ++it)
	{
		insertElement(*it);
	}
}


template <typename T>
RBTree<T>::~RBTree()
{
	_deleteElements(_root);
}


template <typename T>
bool RBTree<T>::operator==(const RBTree<T>& other) const
{
	std::stringstream ss1, ss2;
	std::string s1, s2;

	preorder(ss1);
	other.preorder(ss2);
	std::getline(ss1, s1);
	std::getline(ss2, s2);
	return s1 == s2;
}


template <typename T>
void RBTree<T>::rotateLeft(Node<T>* head)
{
	/*if (head->right)
	{
		Node<T>* pivot = head->right;
		pivot->parent = head->parent;

		if (head->parent)
		{
			if (head->parent->left == head)
				head->parent->left = pivot;
			else
				head->parent->right = pivot;
		}

		head->right = pivot->left;
		if (pivot->left)
			pivot->left->parent = head;

		head->parent = pivot;
		pivot->left = head;
	}*/

	Node<T>* pivot = head->right;

	//connect head->right to b
	head->right = pivot->left;
	if (head->right)
	{
		head->right->parent = head;
	}

	//connect pivot to parent's head
	pivot->parent = head->parent;
	if (!head->parent)
	{
		_root = pivot;
	}
	else if (head->parent->left == head)
	{
		head->parent->left = pivot;
	}
	else
	{
		head->parent->right = pivot;
	}

	//connect pivot to head
	pivot->left = head;
	head->parent = pivot;
}


template <typename T>
void RBTree<T>::rotateRight(Node<T>* head)
{
	/*if (head->left)
	{
		Node<T>* pivot = head->left;
		pivot->parent = head->parent;

		if (head->parent)
		{
			if (head->parent->left == head)
				head->parent->left = pivot;
			else
				head->parent->right = pivot;
		}

		head->left = pivot->right;
		if (pivot->right)
			pivot->right->parent = head;

		head->parent = pivot;
		pivot->right = head;
	}*/
	Node<T>* pivot = head->left;
	pivot = head->left;

	head->left = pivot->right;
	if (head->left)
	{
		head->left->parent = head;
	}

	pivot->parent = head->parent;
	if (!head->parent)
	{
		_root = pivot;
	}
	else if (head->parent->left == head)
	{
		head->parent->left = pivot;
	}
	else
	{
		head->parent->right = pivot;
	}

	pivot->right = head;
	head->parent = pivot;
}


template <typename T>
Node<T>* RBTree<T>::grandParent(Node<T>* node) const
{
	if (node && node->parent)
	{
		return node->parent->parent;
	}

	return nullptr;
}


template <typename T>
Node<T>* RBTree<T>::uncle(Node<T>* node) const
{
	Node<T>* gparent = grandParent(node);

	if (gparent)
	{
		if (node->parent == gparent->left)
			return gparent->right;
		else
			return gparent->left;
	}

	return nullptr;
}


template <typename T>
Node<T>* RBTree<T>::sibling(Node<T>* node) const
{
	if (node && node->parent)
	{
		if (node == node->parent->left)
			return node->parent->right;
		else
			return node->parent->left;
	}

	return nullptr;
}


template <typename T>
void RBTree<T>::insertBalance(Node<T>* node)
{
	if (node)
	{
		Node<T> *g, *u;
		while (node->parent && (node->parent->color == RED))
		{
			g = grandParent(node);
			u = uncle(node);
			if (node->parent == g->left)
			{
				if (u && (u->color == RED))
				{
					//case 1: P - R, U - R
					node->parent->color = BLACK;
					u->color = BLACK;
					g->color = RED;
					node = g;
				}
				else
				{
					if (node == node->parent->right)
					{
						//case 2 -> case 3: P - R, U - B
						node = node->parent;
						rotateLeft(node);
					}
					//case 3: P - R, n - R
					node->parent->color = BLACK;
					grandParent(node)->color = RED;
					rotateRight(grandParent(node));
				}
			}
			//symmetric
			else
			{
				if (u && (u->color == RED))
				{
					node->parent->color = BLACK;
					u->color = BLACK;
					g->color = RED;
					node = g;
				}
				else
				{
					if (node == node->parent->left)
					{
						node = node->parent;
						rotateRight(node);
					}
					node->parent->color = BLACK;
					grandParent(node)->color = RED;
					rotateLeft(grandParent(node));
				}
			}
		}
		_root->color = BLACK;
	}
}


template <typename T>
void RBTree<T>::insertElement(long value)
{
	if (_root)
	{
		Node<T>* cur = _root;

		while (cur)
		{
			if (value < cur->value)
			{
				if (!cur->left)
				{
					cur->left = new Node<T>(value, RED, cur);
					insertBalance(cur->left);
					break;
				}
				else
				{
					cur = cur->left;
				}
			}
			else if (value > cur->value)
			{
				if (!cur->right)
				{
					cur->right = new Node<T>(value, RED, cur);
					insertBalance(cur->right);
					break;
				}
				else
				{
					cur = cur->right;
				}
			}
		}
	}
	else
	{
		_root = new Node<T>(value, BLACK);
	}
}


template <typename T>
void RBTree<T>::deleteBalance(Node<T>* node)
{
	Node<T>* sib;
	while (node && (node != _root) && (node->color == BLACK))
	{
		sib = sibling(node);
		if (sib && (sib == sib->parent->right))
		{
			if (sib->color == RED)
			{
				sib->color = BLACK;
				node->parent->color = RED;
				rotateLeft(node->parent);
				sib = sibling(node);
				//sib = node->parent->right;
			}
			if ((!sib->left || sib->left->color == BLACK) && (!sib->right || sib->right->color == BLACK))
			{
				sib->color = RED;
				node = node->parent;
			}
			else
			{
				if (!sib->right || sib->right->color == BLACK)
				{
					sib->left->color = BLACK;
					sib->color = RED;
					rotateRight(sib);
					sib = sibling(node);
				}
				sib->color = node->parent->color;
				node->parent->color = BLACK;
				sib->right->color = BLACK;
				rotateLeft(node->parent);
				node = _root;
			}
		}
		else
		{
			if (sib->color == RED)
			{
				sib->color = BLACK;
				node->parent->color = RED;
				rotateRight(node->parent);
				sib = sibling(node);
				//sib = node->parent->left;
			}
			if ((!sib->right || sib->right->color == BLACK) && (!sib->left || sib->left->color == BLACK))
			{
				sib->color = RED;
				node = node->parent;
			}
			else
			{
				if (!sib || sib->left->color == BLACK)
				{
					sib->right->color = BLACK;
					sib->color = RED;
					rotateLeft(sib);
					sib = sibling(node);
				}
				sib->color = node->parent->color;
				node->parent->color = BLACK;
				sib->left->color = BLACK;
				rotateRight(node->parent);
				node = _root;
			}
		}
	}
	node->color = BLACK;
}


template <typename T>
bool RBTree<T>::findElement(long value) const
{
	if (!_root)
		throw "RB Tree is empty or not initialized\n";

	Node<T>* cur = _root;

	while (cur)
	{
		if (cur->value == value)
			return true;
		if (value < cur->value)
			cur = cur->left;
		else
			cur = cur->right;
	}

	return false;
}


template <typename T>
Node<T>* RBTree<T>::_deleteRoot(Node<T>* head)
{
	Node<T>* cur, *parent;
	if (head)
	{
		cur = head->right;
		if (!cur)
		{
			cur = head->left;
		}
		else
		{
			if (cur->left)
			{
				parent = head;
				while (cur->left)
				{
					parent = cur;
					cur = cur->left;
				}
				parent->left = cur->right;
				cur->right = head->right;
			}
			cur->left = head->left;
		}

		deleteBalance(head);
		delete head;
		return cur;
	}
	return nullptr;
}


template <typename T>
void RBTree<T>::deleteElement(long value)
{
	Node<T>* cur = _root, *parent;
	if (_root)
	{
		if (_root->value == value)
		{
			_root = _deleteRoot(_root);
		}
		else
		{
			parent = _root;
			if (value < parent->value) cur = parent->left;
			else cur = parent->right;
			while (cur)
			{
				if (cur->value == value)
				{
					if (value < parent->value) parent->left = _deleteRoot(parent->left);
					else parent->right = _deleteRoot(parent->right);
					return;
				}
				parent = cur;
				if (value < parent->value) cur = parent->left;
				else cur = parent->right;
			}
		}
	}
}


template <typename T>
void RBTree<T>::_deleteElements(Node<T>* node)
{
	if (!node)
		return;

	_deleteElements(node->left);
	_deleteElements(node->right);

	delete node;
}


template <typename T>
size_t RBTree<T>::count() const
{
	return _root->getCount();
}


template <typename T>
std::ostream& RBTree<T>::inorder(std::ostream& os) const
{
	if (!_root)
		throw "Red-Black Tree is empty or not initialized\n";

	Node<T>* cur = _root, *pre;

	while (cur)
	{
		if (!cur->left)
		{
			os << cur->value << " ";
			cur = cur->right;
		}
		else
		{
			pre = cur->left;
			while (pre->right && pre->right != cur)
				pre = pre->right;

			if (!pre->right)
			{
				pre->right = cur;
				cur = cur->left;
			}
			else
			{
				pre->right = nullptr;
				os << cur->value << " ";
				cur = cur->right;
			}
		}
	}

	return os;
}


template <typename T>
std::ostream& RBTree<T>::preorder(std::ostream& os) const
{
	if (!_root)
		throw "Red-Black Tree is empty or not initialized\n";

	Node<T>* root = _root;

	while (root)
	{
		if (!root->left)
		{
			os << root->value << " ";
			root = root->right;
		}
		else
		{
			Node<T>* cur = root->left;
			while (cur->right && cur->right != root)
				cur = cur->right;

			if (cur->right == root)
			{
				cur->right = nullptr;
				root = root->right;
			}
			else
			{
				os << root->value << " ";
				cur->right = root;
				root = root->left;
			}
		}
	}

	return os;
}


template <typename T>
std::ifstream& RBTree<T>::inFile(std::ifstream& is)
{
	RBTree<T> other;
	unsigned int count;
	T el;

	if (!is.is_open())
		throw "File not opened!";

	is >> count;

	while (count--)
	{
		is >> el;
		other.insertElement(el);
	}

	swap(other);
	return is;
}


template <typename T>
std::ofstream& RBTree<T>::outFile(std::ofstream& os) const
{
	if (!_root)
		throw "Binary Search Tree is empty or not initialized\n";

	if (!os.is_open())
		throw "File not opened!";

	os << count() << " ";
	preorder(os);

	return os;
}


template <typename T>
std::ifstream& operator>>(std::ifstream& is, RBTree<T>& rbt)
{
	return rbt.inFile(is);
}


template <typename T>
std::ofstream& operator<<(std::ofstream& os, const RBTree<T>& rbt)
{
	return rbt.outFile(os);
}


template <typename T>
std::ostream& printV(std::ostream& os, const Node<T>* node, unsigned int level)
{
	if (!node)
		return os;

	printV(os, node->right, level + 1);
	for (unsigned int i = 0; i < level; i++)
		os << "\t";
	os << node->value << node->color << std::endl;
	printV(os, node->left, level + 1);

	return os;
}


template <typename T>
std::ostream& operator<<(std::ostream& os, const RBTree<T>& rbt)
{
	if (!rbt._root)
		throw "Red-Black Tree is empty or not initialized\n";

	printV(os, rbt._root, 0);

	return os;
}

#endif