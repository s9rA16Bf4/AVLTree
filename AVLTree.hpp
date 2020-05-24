#ifndef AVLTREE_H
#define AVLTREE_H

#include <memory>
#include <cstdlib>
#include <exception>
#include <stdexcept>
#include <utility>
#include <iterator>
#include <vector>

template <typename T>
struct Node{
	T key;
	Node<T>* leftChild;
	Node<T>* rightChild;
	int height;
};

template <typename T>
class AVLTree{
	private:
		Node<T>* root = nullptr;

		// Rotations
		void leftRotate(Node<T>* &node);
		void rightRotate(Node<T>* &node);
		void doubleLeftRotate(Node<T>* &node);
		void doubleRightRotate(Node<T>* &node);

		const T& max(const T& x, const T& y);
		void balance(Node<T>* &node);

		// Wrappers to make the insert, remove functions easier to handles
		void recursiveInsert(const T& element, Node<T>* &node);
		void recursiveRemove(const T& element, Node<T>* &node);

		T findMin(Node<T>* minNode);
		T findMax(Node<T>* maxNode);
		int nodeHeight(Node<T>* node); // Returns the height for a specific node

		std::vector<Node<T>*> gatherAllNodes(Node<T>* node); // Finds and returns all nodes in the tree

	public:
		AVLTree<T>() = default;
		~AVLTree<T>(); // Destructor

		void insert(const T& element);
		void remove(const T& element);
		bool find(const T& element);

		// Walkers
		std::vector<T> preOrderWalk();
		std::vector<T> inOrderWalk();
		std::vector<T> postOrderWalk();

		std::size_t getTreeHeight();

		T getMin();
		T getMax();

		/*Pre-given functions */
		std::string ToGraphviz();
		void ToGraphvizHelper(std::string& listOfNodes, std::string& listOfConnections, Node<T>* toWorkWith, std::size_t& uniqueID);
};

// Public functions
template <typename T>
AVLTree<T>::~AVLTree(){
	std::vector<Node<T>*> allNodes = this->gatherAllNodes(this->root);

	for (Node<T>* node:allNodes){
		if (node){ // Continue only if the node already is not a nullptr
			delete node;
			node = nullptr;
		}
	}
}

template <typename T>
void AVLTree<T>::insert(const T& element){
	recursiveInsert(element, this->root);
}

template <typename T>
void AVLTree<T>::remove(const T& element){
	recursiveRemove(element, this->root);
}

template <typename T>
bool AVLTree<T>::find(const T& element){
	std::vector<T> foundNodeKeys = this->postOrderWalk(); // Gather all node keys

	for (T entries:foundNodeKeys){
		if (entries == element){
			return true;
		}
	}
	return false;
}

template <typename T>
std::vector<T> AVLTree<T>::preOrderWalk(){
	std::vector<T> toReturn, foundValues;

	if(this->root){
		Node<T>* tempRoot = this->root;

		toReturn.push_back(this->root->key);

		this->root = tempRoot->leftChild;
		foundValues = preOrderWalk();
		for (T element:foundValues){ toReturn.push_back(element); }

		this->root = tempRoot->rightChild;
		foundValues = preOrderWalk();
		for (T element:foundValues){ toReturn.push_back(element); }

		this->root = tempRoot;
	}
	return toReturn;
}

template <typename T>
std::vector<T> AVLTree<T>::inOrderWalk(){
	std::vector<T> toReturn, foundValues;

	if(this->root){
		Node<T>* tempRoot = this->root;

		this->root = tempRoot->leftChild;
		foundValues = inOrderWalk();
		for (T element:foundValues){ toReturn.push_back(element); }

		this->root = tempRoot;
		toReturn.push_back(this->root->key);

		this->root = tempRoot->rightChild;
		foundValues = inOrderWalk();
		for (T element:foundValues){ toReturn.push_back(element); }

		this->root = tempRoot;
	}

	return toReturn;
}

template <typename T>
std::vector<T> AVLTree<T>::postOrderWalk(){
	std::vector<T> toReturn, foundValues;

	if(this->root){
		Node<T>* tempRoot = this->root;

		this->root = tempRoot->leftChild;
		foundValues = postOrderWalk();
		for (T element:foundValues){ toReturn.push_back(element); }

		this->root = tempRoot->rightChild;
		foundValues = postOrderWalk();
		for (T element:foundValues){ toReturn.push_back(element); }

		this->root = tempRoot;

		toReturn.push_back(this->root->key);
	}

	return toReturn;
}

template <typename T>
std::size_t AVLTree<T>::getTreeHeight(){
	if(this->root){
		return static_cast<std::size_t>(this->root->height);
	}else{
		return static_cast<std::size_t>(-1);
	}
}

template <typename T>
T AVLTree<T>::getMin(){
	std::vector<T> foundNodeKeys = this->postOrderWalk(); // Gather all node keys
	T lowestValue = T();

	if (this->root){
		for (T entries:foundNodeKeys){
			if (entries < lowestValue || lowestValue == T()){
				lowestValue = entries;
			}
		}
	}else{
		throw std::out_of_range("Error: Empty tree, terminating");
	}


	return lowestValue;
}

template <typename T>
T AVLTree<T>::getMax(){
	std::vector<T> foundNodeKeys = this->postOrderWalk(); // Gather all node keys
	T highestValue = T();

	if (this->root){
		for (T entries:foundNodeKeys){
			if (entries > highestValue || highestValue == T()){
				highestValue = entries;
			}
		}
	}else{
		throw std::out_of_range("Error: Empty tree, terminating");
	}

	return highestValue;
}

// Private functions
template <typename T>
void AVLTree<T>::leftRotate(Node<T>* &parentNode){
		Node<T>* newNode = parentNode->leftChild;

		// Do the rotations
		parentNode->leftChild = newNode->rightChild;
		newNode->rightChild = parentNode;

		// Update heights
		parentNode->height = this->max(nodeHeight(parentNode->leftChild), nodeHeight(parentNode->rightChild)) + 1;
		newNode->height = this->max(static_cast<std::size_t>(nodeHeight(newNode->leftChild)), parentNode->height) + 1;

		parentNode = newNode;
}

template <typename T>
void AVLTree<T>::rightRotate(Node<T>* &parentNode){
		Node<T>* newNode = parentNode->rightChild;

		// Do the rotations
		parentNode->rightChild = newNode->leftChild;
		newNode->leftChild = parentNode;

		// Update heights
		parentNode->height = this->max(nodeHeight(parentNode->rightChild), nodeHeight(parentNode->leftChild) + 1);
		newNode->height = this->max(static_cast<std::size_t>(nodeHeight(newNode->rightChild)), parentNode->height) + 1;

		parentNode = newNode;
}

template <typename T>
void AVLTree<T>::doubleLeftRotate(Node<T>* &Node){
		rightRotate(Node->leftChild);
		leftRotate(Node);
}

template <typename T>
void AVLTree<T>::doubleRightRotate(Node<T>* &Node){
	leftRotate(Node->rightChild);
	rightRotate(Node);
}

template <typename T>
const T& AVLTree<T>::max(const T& a, const T& b){
		return (a > b)? a:b;
}

template <typename T>
void AVLTree<T>::balance(Node<T>* &Node){
	if(Node){
		if(nodeHeight(Node->leftChild) - nodeHeight(Node->rightChild) > 1){
			if(nodeHeight(Node->leftChild->leftChild) >= nodeHeight(Node->leftChild->rightChild)){
				this->leftRotate(Node);
			}else{
				this->doubleLeftRotate(Node);
			}

		}else{
			if(nodeHeight(Node->rightChild) - nodeHeight(Node->leftChild) > 1){
				if(nodeHeight(Node->rightChild->rightChild) >= nodeHeight(Node->rightChild->leftChild)){
					this->rightRotate(Node);
				}else{
					this->doubleRightRotate(Node);
				}
			}
		}
		Node->height = this->max(nodeHeight(Node->leftChild), nodeHeight(Node->rightChild)) + 1;
	}
}

template <typename T>
void AVLTree<T>::recursiveInsert(const T& element, Node<T>* &ParentNode){
		if(!this->find(element)){ // Does a node with this element already exist=
			if(!ParentNode){ // New child
				Node<T>* newChildNode = new Node<T>;
				newChildNode->key = element;
				newChildNode->leftChild = nullptr;
				newChildNode->rightChild = nullptr;
				newChildNode->height = 0;

				ParentNode = newChildNode;

			}else if(element < ParentNode->key){ // go left
				recursiveInsert(element, ParentNode->leftChild);

			}else if(element > ParentNode->key){ // go right
				recursiveInsert(element, ParentNode->rightChild);
			}
			balance(ParentNode); // Balance
		}else{
			throw std::invalid_argument("Error, cannot insert same element twice");
		}
}

template <typename T> // FIX
void AVLTree<T>::recursiveRemove(const T& element, Node<T>* &ParentNode){
		if(ParentNode){
			if(this->find(element)){ // Those the element exist?
				if(element < ParentNode->key){ // go left
					recursiveRemove(element, ParentNode->leftChild);

				}else if(element > ParentNode->key){ // go right
					recursiveRemove(element, ParentNode->rightChild);

				}else if(ParentNode->leftChild && ParentNode->rightChild){  // There are two children two this node
					ParentNode->key = findMin(ParentNode->rightChild);
					recursiveRemove(ParentNode->key, ParentNode->rightChild);

				}else{ // We have entry to delete node
					Node<T>* oldNode = ParentNode;
					ParentNode = (ParentNode->leftChild) ? ParentNode->leftChild : ParentNode->rightChild;
					delete oldNode;
				}

				balance(ParentNode); // Balance

			}else{ throw std::invalid_argument("Error: The element did not exist in the treee"); }
		}else{ throw std::out_of_range("Error: root is nullptr, terminating"); }
}

template <typename T>
std::vector<Node<T>*> AVLTree<T>::gatherAllNodes(Node<T>* node){
	std::vector<Node<T>*> toReturn, foundNodes;
	if (node){ // Checking so that the root exists
		toReturn.push_back(node);
		foundNodes = this->gatherAllNodes(node->leftChild);

		for (Node<T>* entries:foundNodes){ toReturn.push_back(entries); }

		foundNodes = this->gatherAllNodes(node->rightChild);

		for (Node<T>* entries:foundNodes){ toReturn.push_back(entries); }

	}
		return toReturn;
}

template <typename T>
T AVLTree<T>::findMin(Node<T>* node){ // Wrapper
	Node<T>* tempRoot = this->root;
	this->root = node;

	T lowestValue = this->getMin();
	this->root = tempRoot;

	return lowestValue;
}

template <typename T> // Wrapper
T AVLTree<T>::findMax(Node<T>* node){
	Node<T>* tempRoot = this->root;
	this->root = node;

	T largestValue = this->getMax();
	this->root = tempRoot;

	return largestValue;
}

template <typename T>
int AVLTree<T>::nodeHeight(Node<T>* Node){
	if (Node){
			return Node->height;
	}else{
			return -1;
	}
} // Like "getTreeHeight" but for a specific node

/* Pre-given functions */
template <typename T>
std::string AVLTree<T>::ToGraphviz() // Member function of the AVLTree class
{
	std::string toReturn = "";
	if(root) // root is a pointer to the root node of the tree
	{
		std::string listOfNodes;
		std::string listOfConnections = std::string("\t\"Root\" -> ") + std::to_string(0) + std::string(";\n");
		toReturn += std::string("digraph {\n");
		std::size_t id = 0;
		ToGraphvizHelper(listOfNodes, listOfConnections, root, id);
		toReturn += listOfNodes;
		toReturn += listOfConnections;
		toReturn += std::string("}");
	}
	return std::move(toReturn);
}

template <typename T>
void AVLTree<T>::ToGraphvizHelper(std::string& listOfNodes, std::string& listOfConnections, Node<T>* toWorkWith, std::size_t& uniqueID) // Member function of the AVLTree class
{
	std::size_t myID = uniqueID;
	listOfNodes += std::string("\t") + std::to_string(myID) + std::string(" [label=\"") + std::to_string(toWorkWith->key) + std::string("\"];\n");
	if(toWorkWith->leftChild)
	{
		listOfConnections += std::string("\t") + std::to_string(myID) + std::string(" -> ") + std::to_string(uniqueID + 1) + std::string(" [color=blue];\n");
		ToGraphvizHelper(listOfNodes, listOfConnections, toWorkWith->leftChild, ++uniqueID);
	}
	else
	{
		listOfNodes += std::string("\t") + std::to_string(++uniqueID) + std::string(" [label=") + std::string("nill, style = invis];\n");
		listOfConnections += std::string("\t") + std::to_string(myID) + std::string(" -> ") + std::to_string(uniqueID) + std::string(" [ style = invis];\n");
	}

	if(toWorkWith->rightChild)
	{
		listOfConnections += std::string("\t") + std::to_string(myID) + std::string(" -> ") + std::to_string(uniqueID + 1) + std::string(" [color=red];\n");
		ToGraphvizHelper(listOfNodes, listOfConnections, toWorkWith->rightChild, ++uniqueID);
	}
	else
	{
		listOfNodes += std::string("\t") + std::to_string(++uniqueID) + std::string(" [label=") + std::string("nill, style = invis];\n");
		listOfConnections += std::string("\t") + std::to_string(myID) + std::string(" -> ") + std::to_string(uniqueID) + std::string(" [ style = invis];\n");
	}
}
#endif
