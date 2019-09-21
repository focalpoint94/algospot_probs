#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <array>
#include <vector>
#include <tuple>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <list>
#include <queue>
#include <stack>
#include <sstream>
#include <string>
#include <algorithm>
#include <cmath>
#include <limits>

using namespace std;

typedef int KeyType;
struct Node {
	// Node�� ����
	KeyType key;
	// Node�� �켱����
	int priority;
	// ��ü ���� Ʈ���� ũ��
	int size;
	Node *left, *right;

	Node(const KeyType& _key) : key(_key), priority(rand()), size(1), left(NULL), right(NULL) {}
	void setLeft(Node* newLeft) { left = newLeft; calcSize(); }
	void setRight(Node * newRight) { right = newRight; calcSize(); }
	void calcSize() {
		size = 1;
		if (left) size += left->size;
		if (right) size += right->size;
	}
};
typedef pair<Node*, Node*> NodePair;
// root�� ��Ʈ�� �ϴ� Ʈ���� key �̸��� ���� �̻��� ���� ���� �� ���� Ʈ������ �и��Ͽ�, �� ���� �����͸� ��ȯ
NodePair split(Node* root, KeyType key) {
	if (root == NULL) return NodePair(NULL, NULL);
	if (key > root->key) {
		NodePair rs = split(root->right, key);
		root->setRight(rs.first);
		return NodePair(root, rs.second);
	}
	else {
		NodePair ls = split(root->left, key);
		root->setLeft(ls.second);
		return NodePair(ls.first, root);
	}
}
// root�� ��Ʈ�� �ϴ� Ʈ���� �� ��� node�� ������ �� ��� Ʈ���� ��Ʈ�� ��ȯ
Node* insert(Node* root, Node* node) {
	if (root == NULL) return node;
	if (root->priority < node->priority) {
		NodePair splitted = split(root, node->key);
		node->setLeft(splitted.first);
		node->setRight(splitted.second);
		return node;
	}
	if (node->key < root->key)
		root->setLeft(insert(root->left, node));
	else
		root->setRight(insert(root->right, node));
	return root;
}
void print(Node* root) {
	if (root == NULL) return;
	print(root->left);
	cout << root->key << " ";
	print(root->right);
}
void return_memory(Node* root) {
	if (root->left != NULL) return_memory(root->left);
	if (root->right != NULL) return_memory(root->right);
	delete root;
}
Node* merge(Node* a, Node* b) {
	if (a == NULL) return b;
	if (b == NULL) return a;
	if (a->priority < b->priority) {
		b->setLeft(merge(a, b->left));
		return b;
	}
	else {
		a->setRight(merge(a->right, b));
		return a;
	}
}
Node* erase(Node* root, KeyType key) {
	if (root == NULL) return root;
	if (root->key == key) {
		Node* ret = merge(root->left, root->right);
		delete root;
		return ret;
	}
	if (key < root->key)
		root->setLeft(erase(root->left, key));
	else
		root->setRight(erase(root->right, key));
	return root;
}
// k��° ���Ҹ� ��ȯ
Node* kth(Node* root, int k) {
	int leftSize = 0;
	if (root->left != NULL) leftSize = root->left->size;
	if (k <= leftSize) return kth(root->left, k);
	if (k == leftSize + 1) return root;
	return kth(root->right, k - leftSize - 1);
}
int countLessThan(Node* root, KeyType key) {
	if (root == NULL) return 0;
	if (root->key >= key)
		return countLessThan(root->left, key);
	int ls = (root->left != NULL ? root->left->size : 0);
	return ls + 1 + countLessThan(root->right, key);
}

int N, a, b;

struct RNG {
	unsigned seed;
	int a, b;
	RNG(int a_, int b_) : seed(1983), a(a_), b(b_) {}
	unsigned next() {
		unsigned ret = seed;
		seed = (seed * (long long)a + b) % 20090711;
		return ret;
	}
};

int calc(Node* root, int size) {
	if (size % 2 == 0)
		return kth(root, size / 2)->key;
	return kth(root, size / 2 + 1)->key;
}

int main()
{
	vector<int> ans;
	int cases;
	cin >> cases;
	int count = 0;
	while (count < cases) {
		cin >> N >> a >> b;
		Node* root = NULL;
		RNG rng(a, b);
		unsigned total = 0;
		for (int i = 0; i < N; ++i) {
			root = insert(root, new Node(rng.next()));
			total += calc(root, i + 1);
			total %= 20090711;
		}
		ans.push_back(total);
		count++;
	}
	for (auto i = ans.begin(); i != ans.end(); ++i)
		cout << *i << endl;
	return 0;
}