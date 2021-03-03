#include <utility>
#include <vector>
#include <cmath>
#include <functional>

template<typename T>
auto CalculateCohesionBehavior(const T& agent, std::vector<T*>& list,const std::function<bool(const T* ref)>& isFriendly) -> decltype(agent.position)
{
	if (list.size() == 0)
		return decltype(agent.position){ 0, 0 };

	decltype(agent.position) alignmentMove = { 0,0 };
	int counter = 0;
	for (auto& k : list)
	{
		if (isFriendly(k))
		{
			++counter;
			alignmentMove += k->position;
		}
	}
	if(counter == 0)
		return decltype(agent.position){ 0, 0 };

	alignmentMove /= static_cast<decltype(alignmentMove.x)>(counter);

	alignmentMove -= agent.position;

	return alignmentMove;
}



template<typename T>
T CalculateAlignmentBehavior(std::vector<T>& list)
{
	if (list.size() == 0)
		return T{ 0, 0 };


	T alignmentMove = { 0,0 };
	for (auto& k : list)
	{
		alignmentMove += k;
	}

	alignmentMove /= static_cast<decltype(alignmentMove.x)>(list.size());
	return alignmentMove;
}


template<typename T>
auto CalculateSeparationBehavior(const T& agent, std::vector<T*>& list, const std::function<bool(const T* ref)>& isFriendly) ->decltype(agent.position)
{
	if (list.size() == 0)
		return decltype(agent.position){ 0, 0 };

	int counter = 0;
	decltype(agent.position) temp;
	decltype(agent.position) separationMove = { 0,0 };
	for (auto& k : list)
	{
		if (isFriendly(k))
		{
			++counter;
			temp = agent.position - k->position;
			temp /= sqrt(temp.x * temp.x + temp.y * temp.y);
			separationMove += temp;
		}
	}
	if(counter == 0)
		return decltype(agent.position){ 0, 0 };

	separationMove /= static_cast<decltype(separationMove.x)>(counter);

	return separationMove;

}

template<typename T>
auto CalculateRunAwayBehavior(const T& agent, std::vector<T*>& list, const std::function<bool(const T* ref)>& isFriendly) ->decltype(agent.position)
{
	if (list.size() == 0)
		return decltype(agent.position){ 0, 0 };

	int counter = 0;
	decltype(agent.position) temp;
	decltype(agent.position) separationMove = { 0,0 };
	for (auto& k : list)
	{
		if (!isFriendly(k))
		{
			++counter;
			temp = agent.position - k->position;
			temp /= sqrt(temp.x * temp.x + temp.y * temp.y);
			separationMove += temp;
		}
	}
	if (counter == 0)
		return decltype(agent.position){ 0, 0 };

	separationMove /= static_cast<decltype(separationMove.x)>(counter);

	return separationMove;

}