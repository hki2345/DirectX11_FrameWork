#pragma once

template<typename RETURN, typename MAP, typename KEY>
RETURN MapFind(MAP& _Map, const KEY& _Key)
{
	// std::unordered_map<std::wstring, HPTR<HWindow>>::iterator FindIter = g_NWinMap.find(_Name);
	MAP::iterator FindIter = _Map.find(_Key);
	if (FindIter == _Map.end())
	{
		return nullptr;
	}
	return FindIter->second;
}

template<typename MAP, typename KEY>
bool IsMapFind(MAP& _Map, const KEY& _Key)
{
	// std::unordered_map<std::wstring, HPTR<HWindow>>::iterator FindIter = g_NWinMap.find(_Name);
	MAP::iterator FindIter = _Map.find(_Key);
	if (FindIter == _Map.end())
	{
		return false;
	}
	return true;
}

template<typename STL, typename KEY>
void StlErase(STL& _Map, const KEY& _Key)
{
	// std::unordered_map<std::wstring, HPTR<HWindow>>::iterator FindIter = g_NWinMap.find(_Name);
	STL::iterator FindIter = _Map.find(_Key);
	if (FindIter == _Map.end())
	{
		return;
	}
	_Map.erase(FindIter);
}