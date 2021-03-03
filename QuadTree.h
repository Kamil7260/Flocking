#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <math.h>

#include "Functions.h"
#include "Allocator.h"

#include "Utility.h"

template<typename T,size_t objectCount, size_t depth = 4u>
class QuadTree {
public:
	QuadTree();
	QuadTree(const sf::Vector2u& pos, const sf::Vector2u& s);


	void set(const sf::Vector2u& pos, const sf::Vector2u& s);

	void insertPoints(std::array<T, objectCount>* ptr);

	void addSingleIndex(const short int index);
	void addVectorIndex(std::vector<short int>&& vec);

	void start();

	void restart();

	void alloc();

	const sf::Vector2u& getPosition() const;
	const sf::Vector2u& getSize() const;
	const sf::Vector2u& getOrigin() const;

	void setPosition(const sf::Vector2u& ref);
	void setSize(const sf::Vector2u& ref);

	static short unsigned int showGeneration();

	void find(std::vector<short>& indexContainer, const sf::Vector2f& target, const float radius);

#ifdef debug
	void draw(sf::RenderWindow* window) {
		window->draw(lines);
		if (initt && m_age < depth) {
			for (int k = 0; k < 4; ++k)
			{
				m_children[k]->draw(window);
			}
		}
	}
	bool initt;
	sf::VertexArray lines;
#endif

	enum class Direction {
		up_right = 0,
		up_left = 1,
		bottom_left = 2,
		bottom_right = 3
	};

private:
	void setAge(const int& val);

	void init();

	template<typename U>
	void overlaps(const U& target,const U& origin, Direction& direction);
	void split();
	
	int m_age;

	sf::Vector2u m_position;
	sf::Vector2u m_size;
	sf::Vector2u m_origin;

	static short unsigned int s_generation;
	template<size_t arg>
	static size_t constexpr calculateMemory();

	template<>
	static size_t constexpr calculateMemory<0>() { return size_t(0); };

	std::array<QuadTree*,4> m_children;

	std::array<T, objectCount>* m_listT;

	std::vector<short int> m_idList;
	
	static Allocator<QuadTree<T, objectCount, depth>, calculateMemory<depth>()> s_container;
};

template<typename T, size_t objectCount, size_t depth>
QuadTree<T, objectCount, depth>::QuadTree()
	:m_position({ 0u,0u }), m_size({ 1200u,650u }), m_origin({ m_position.x + m_size.x / 2, m_position.y + m_size.y / 2 })
{}

template<typename T,size_t objectCount, size_t depth>
QuadTree<T, objectCount, depth>::QuadTree(const sf::Vector2u& pos, const sf::Vector2u& s)
	: m_age(0), m_position(pos),m_size(s), m_origin({ m_position.x + m_size.x / 2, m_position.y + m_size.y / 2 }), m_listT(nullptr) {
#ifdef debug
	initt = false;
	lines = sf::VertexArray(sf::LinesStrip, 5);
	lines[0].position = sf::Vector2f(pos.x, pos.y);
	lines[0].color = sf::Color::Green;
	lines[1].position = sf::Vector2f(pos.x+s.x, pos.y);
	lines[1].color = sf::Color::Green;
	lines[2].position = sf::Vector2f(pos.x+s.x, pos.y+s.y);
	lines[2].color = sf::Color::Green;
	lines[3].position = sf::Vector2f(pos.x, pos.y+s.y);
	lines[3].color = sf::Color::Green;
	lines[4].position = sf::Vector2f(pos.x, pos.y);
	lines[4].color = sf::Color::Green;
#endif
}

template<typename T, size_t objectCount, size_t depth>
void QuadTree<T, objectCount, depth>::set(const sf::Vector2u& pos, const sf::Vector2u& s) {
	m_position = pos;
	m_size = s;
#ifdef debug
	initt = false;
	lines = sf::VertexArray(sf::LinesStrip, 5);
	lines[0].position = sf::Vector2f(pos.x, pos.y);
	lines[0].color = sf::Color::Green;
	lines[1].position = sf::Vector2f(pos.x + s.x, pos.y);
	lines[1].color = sf::Color::Green;
	lines[2].position = sf::Vector2f(pos.x + s.x, pos.y + s.y);
	lines[2].color = sf::Color::Green;
	lines[3].position = sf::Vector2f(pos.x, pos.y + s.y);
	lines[3].color = sf::Color::Green;
	lines[4].position = sf::Vector2f(pos.x, pos.y);
	lines[4].color = sf::Color::Green;
#endif
}

template<typename T, size_t objectCount, size_t depth>
void QuadTree<T, objectCount,depth>::insertPoints(std::array<T, objectCount>* ptr) {
	m_listT = ptr;
}


template<typename T, size_t objectCount, size_t depth>
template<typename U>
void QuadTree<T, objectCount, depth>::overlaps(const U& target, const U& origin, Direction& direction) {
	if (origin.x < target.x)
	{
		if (origin.y > target.y)
			direction = QuadTree::Direction::up_right;
		else
			direction = QuadTree::Direction::bottom_right;
	}
	else {
		if (origin.y > target.y)
			direction = QuadTree::Direction::up_left;
		else
			direction = QuadTree::Direction::bottom_left;
	}
	return;
}

template<typename T, size_t objectCount, size_t depth>
template<size_t arg>
constexpr size_t QuadTree<T, objectCount, depth>::calculateMemory() {
	constexpr size_t N = calculateMemory<arg-1>() + QuadTreeUtility::pow(4,arg);
	return N * 4;
}


template<typename T, size_t objectCount, size_t depth>
void QuadTree<T, objectCount, depth>::split() {
	if (m_idList.size() <= 4u)
		return;

	if (m_age > depth)
		return;

	++s_generation;
	init();

	std::vector<short int> points[4];
	for (int k = 0; k < 4; ++k)
		points[k].reserve(100);

	sf::Vector2f origin = { m_position.x + (float)m_size.x / 2, m_position.y + (float)m_size.y / 2 };
	Direction dir;
	for (auto& index : m_idList)
	{
		overlaps(m_listT->operator[](index).getPosition(), origin, dir);
		points[static_cast<int>(dir)].push_back(index);
	}
	
	for (short int k = 0; k < 4; ++k)
	{
		m_children[k]->addVectorIndex(std::move(points[k]));
		m_children[k]->split();
	}
	return;
}

template<typename T, size_t objectCount, size_t depth>
void QuadTree<T, objectCount, depth>::addSingleIndex(const short int index) {
	m_idList.push_back(index);
}

template<typename T, size_t objectCount, size_t depth>
void QuadTree<T, objectCount, depth>::addVectorIndex(std::vector<short int>&& vec) {
	m_idList = std::move(vec);
}

template<typename T, size_t objectCount, size_t depth>
void QuadTree<T, objectCount, depth>::setAge(const int& val) {
	m_age = val;
}

template<typename T, size_t objectCount, size_t depth>
void QuadTree<T, objectCount, depth>::init()  {
	m_origin = { m_position.x + m_size.x / 2, m_position.y + m_size.y / 2 };

	// up right
	m_children[0] = s_container.getNextFree().first;
	m_children[0]->set(sf::Vector2u({ m_position.x + m_size.x / 2u, m_position.y }), m_size / 2u);
	m_children[0]->setAge(m_age + 1);

	// up left
	m_children[1] = s_container.getNextFree().first;
	m_children[1]->set(sf::Vector2u({ m_position.x, m_position.y }), m_size / 2u);
	m_children[1]->setAge(m_age + 1);

	// bottom left
	m_children[2] = s_container.getNextFree().first;
	m_children[2]->set(sf::Vector2u({ m_position.x, m_position.y + m_size.y / 2u }), m_size / 2u);
	m_children[2]->setAge(m_age + 1);

	// bottom right
	m_children[3] = s_container.getNextFree().first;
	m_children[3]->set(sf::Vector2u({ m_position.x + m_size.x / 2u, m_position.y + m_size.y / 2u }), m_size / 2u);
	m_children[3]->setAge(m_age + 1);

	for (int k = 0; k < 4; ++k)
		m_children[k]->insertPoints(m_listT);
#ifdef debug
	initt = true;
#endif
}

template<typename T, size_t objectCount, size_t depth>
void QuadTree<T, objectCount, depth>::start() {
	m_age = 0;

	s_container.restart();
	s_generation = 0;
	if (m_listT->size() <= 4u)
		return;
	++s_generation;
	init();

	sf::Vector2f origin = { m_position.x + (float)m_size.x / 2, m_position.y + (float)m_size.y / 2 };
	Direction dir;
	for (short int k = 0; k < objectCount; ++k) {
		overlaps(m_listT->operator[](k).getPosition(), origin, dir);
		m_children[static_cast<int>(dir)]->addSingleIndex(k);
	}

	for (short int k = 0; k < 4; ++k)
		m_children[k]->split();

	return;
}

template<typename T, size_t objectCount, size_t depth>
void QuadTree<T, objectCount, depth>::restart() {
	m_idList.clear();
	for (short int k = 0; k < 4; ++k)
		m_children[k] = nullptr;
#ifdef debug
	initt = false;
#endif
}

template<typename T, size_t objectCount, size_t depth>
void QuadTree<T, objectCount, depth>::alloc() {
	s_container.alloc();
}

template<typename T, size_t objectCount, size_t depth>
const sf::Vector2u& QuadTree<T, objectCount, depth>::getPosition() const {
	return m_position;
}

template<typename T, size_t objectCount, size_t depth>
const sf::Vector2u& QuadTree<T, objectCount, depth>::getSize() const {
	return m_size;
}

template<typename T, size_t objectCount, size_t depth>
const sf::Vector2u& QuadTree<T, objectCount, depth>::getOrigin() const {
	return m_origin;
}

template<typename T, size_t objectCount, size_t depth>
void QuadTree<T, objectCount, depth>::setPosition(const sf::Vector2u& ref) {
	m_position = ref;
}

template<typename T, size_t objectCount, size_t depth>
void QuadTree<T, objectCount, depth>::setSize(const sf::Vector2u& ref) {
	m_size = ref;
}

template<typename T, size_t objectCount, size_t depth>
short unsigned int QuadTree<T, objectCount, depth>::showGeneration() {
	return s_generation;
}

template<typename T, size_t objectCount, size_t depth>
void QuadTree<T, objectCount, depth>::find(std::vector<short>& indexContainer, const sf::Vector2f& target, const float radius) {
	if (m_children[0] == nullptr)
	{
		std::vector<short> idlist = m_idList;
		if (!idlist.empty())
		{
			for (auto k = idlist.begin(); k!= idlist.end() ;)
			{
				if ((target.x - m_listT->operator[](*k).getPosition().x) * (target.x - m_listT->operator[](*k).getPosition().x) +
					(target.y - m_listT->operator[](*k).getPosition().y) * (target.y - m_listT->operator[](*k).getPosition().y) > radius * radius)
				{
					k = idlist.erase(k);
				}
				else ++k;
			}

			indexContainer.insert(indexContainer.end(), idlist.begin(), idlist.end());
		}
			
		return;
	}
	float distanceX, distanceY;
	float closestX, closestY;
	for (short int k = 0; k < 4; ++k)
	{
		closestX = clamp<float>(target.x, m_children[k]->m_position.x, m_children[k]->m_position.x + m_children[k]->m_size.x);
		closestY = clamp<float>(target.y, m_children[k]->m_position.y, m_children[k]->m_position.y + m_children[k]->m_size.y);

		distanceX = target.x - closestX;
		distanceY = target.y - closestY;

		if (((distanceX * distanceX) + (distanceY * distanceY)) < radius * radius)
		{
			m_children[k]->find(indexContainer, target, radius);
		}
	}
	return;
}

template<typename T, size_t objectCount, size_t depth>
short unsigned int QuadTree<T, objectCount, depth>::s_generation = 0u;

template<typename T, size_t objectCount, size_t depth>
Allocator<QuadTree<T, objectCount, depth> , QuadTree<T, objectCount, depth>::calculateMemory<depth>()> QuadTree<T, objectCount, depth>::s_container;
