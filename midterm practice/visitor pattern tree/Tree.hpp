#ifndef BOTTCHER_VISTIOR_TREE_H
#define BOTTCHER_VISITOR_TREE_H
#include <iostream>

//https://arne-mertz.de/2016/04/visitor-pattern-oop/
//https://en.wikipedia.org/wiki/Visitor_pattern#C++_example
//https://cpppatterns.com/patterns/visitor.html - Ken's
class Node;
class Node_String;
class Node_Int;

class AbstractNodeVisitor {
public:
	virtual ~AbstractNodeVisitor() {}
	virtual void visit(Node_String&) = 0;
	virtual void visit(Node_Int&) = 0;
};

class NodeVisitor : public AbstractNodeVisitor {
public:
	~NodeVisitor() {}
	NodeVisitor() = default;
	virtual void visit(Node_String& stringElement) override 
	{
		if(&stringElement == nullptr) return stringElement;
		std::cout << "Node value: " << stringElement.value << std::endl;
		Node* left = stringElement.left();
		Node* right = stringElement.right();
		if(left != nullptr) left->accept(*this);
		if(right != nullptr) right->accept(*this);
	}
	virtual void visit(Node_Int& intElement) override 
	{
		if(&intElement == nullptr) return intElement;
		std::cout << "Node value: " << stringElement.value << std::endl;
		Node* left = intElement.left();
		Node* right = intElement.right();
		if(left != nullptr) left->accept(*this);
		if(right != nullptr) right->accept(*this);
	}
};

class Node {
public:
	virtual void accept(AbstractNodeVisitor&)=0;
	Node* left();
	Node* right();
};

class Node_String : public Node {
public:
	std::string value;
	virtual void accept(AbstractNodeVisitor& v) override
	{
		v.visit(*this);
	}
};

class Node_Int : public Node {
public:
	int value;
	virtual void accept(AbstractNodeVisitor& v) override
	{
		v.visit(*this);
	}
};

#endif
