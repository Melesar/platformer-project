#include "Application.h"

#undef main

struct Node
{
	float f;
	Node* parent;

	Node(float f) : f(f), parent(nullptr)
	{
		std::cout << "Ctor " << f << std::endl;
	}

	Node(Node&& other) noexcept : f(other.f), parent(other.parent)
	{
		std::cout << "Move" << std::endl;
	}

	Node(const Node& other) : f(other.f), parent(other.parent)
	{
		std::cout << "Copy " << f << std::endl;
	}

	~Node()
	{
		std::cout << "Dtor " << f << std::endl;
	}
};

int main (int argc, char** argv)
{
	std::set<std::unique_ptr<Node>> set;
	std::unique_ptr<Node> ptr = std::make_unique<Node>(10.f);
	std::cout << "Emplacing ptr" << std::endl;
	set.emplace(std::move(ptr));

	std::cout << "Getting iterator" << std::endl;
	Node* iter = (*set.begin()).get();
	std::cout << iter->f << std::endl;
	std::cout << "Program ends" << std::endl;
	
	
	Platformer::Application app;
	app.run(argc, argv);
	
	return 0;
}