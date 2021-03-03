#pragma once
#include <array>
#include <memory>
#include <utility>
#include <iostream>
template<typename T, size_t size>
class Allocator {
public:
	Allocator();

	void restart();

	std::pair<T*, size_t> getNextFree();

	template<typename ...U>
	void alloc(U ...args);
private:
	std::unique_ptr<std::array<std::unique_ptr<T>, size>> m_container;
	size_t m_lastIndex;
};

template<typename T, size_t size>
Allocator<T, size>::Allocator()
	: m_lastIndex(0)
{}

template<typename T, size_t size>
void Allocator<T, size>::restart()
{
	m_lastIndex = 0;
	for (auto& k : *m_container)
		k->restart();
}

template<typename T, size_t size>
std::pair<T*, size_t> Allocator<T, size>::getNextFree()
{
	if (m_lastIndex < size)
	{
		auto& k = m_container->operator[](m_lastIndex);
		return std::pair<T*, size_t>(k.get(), static_cast<size_t>(m_lastIndex++));
	}
	std::cout << "Overload\n";
	auto& k = m_container->operator[](0);
	return std::pair<T*,size_t>(k.get(), size_t(0));
}

template<typename T, size_t size>
template<typename ...U>
void Allocator<T, size>::alloc(U ...args)
{
	m_container = std::make_unique<std::array<std::unique_ptr<T>, size>>();
	for (auto& k : *m_container)
	{
		k = std::make_unique<T>(U...);
	}
}
