#include "IsoMapRender.h"
#include <KMacro.h>

IsoMapRender::PathNode::PathNode() 
{

}

IsoMapRender::PathNode::~PathNode() 
{
}

void IsoMapRender::CreateNode(KVector2 _Index)
{
	// 안만들때 체크해줘야 한다.


	KPtr<PathNode> NewNode = new PathNode();
	NewNode->m_Index = _Index;
	NewNode->m_Pos = NewNode->m_Index.IntToFloat();
	NewNode->m_Parent = m_CurNode;

	if (_Index == EndIndex)
	{
		m_EndNode = NewNode;
		return;
	}

	CalNode(NewNode);

	m_OpenList.insert(std::multimap<float, KPtr<PathNode>>::value_type(NewNode->F, NewNode));
	m_OpenFindList.insert(std::map<__int64, KPtr<PathNode>>::value_type(_Index, NewNode));
}

void IsoMapRender::CalNode(KPtr<IsoMapRender::PathNode> _Node) 
{
	if (nullptr == _Node->m_Parent)
	{
		_Node->G = 0;
	}
	else {
		_Node->G = _Node->m_Parent->G;
		KVector2 CheckDir = _Node->m_Pos - _Node->m_Parent->m_Pos;
		_Node->G += CheckDir.length();
	}

	KVector2 EndDir = EndIndex.IntToFloat();
	_Node->H = (_Node->m_Pos - EndDir).length();
	_Node->F = _Node->G + _Node->H;
}

void IsoMapRender::PopFastNode() 
{
	if (0 == m_OpenList.size())
	{
		m_CurNode = nullptr;
		return;
	}

	m_CurNode = m_OpenList.begin()->second;

	m_OpenList.erase(m_OpenList.begin());
	m_OpenFindList.erase(m_CurNode->m_Index);

	m_CloseList.insert(std::map<__int64, KPtr<PathNode>>::value_type(m_CurNode->m_Index, m_CurNode));
	return;
}

void IsoMapRender::CheckNode() 
{
	if (nullptr == m_CurNode)
	{
		KASSERT(true);
		return;
	}

	KVector2 CheckIndex;

	for (size_t i = 0; i < 4; i++)
	{
		CheckIndex.ix = CheckDir[i].ix + m_CurNode->m_Index.ix;
		CheckIndex.iy = CheckDir[i].iy + m_CurNode->m_Index.iy;

		if (m_TileMap.end() == m_TileMap.find(CheckIndex))
		{
			continue;
		}

		if (m_CloseList.end() != m_CloseList.find(CheckIndex))
		{
			continue;
		}

		if (m_OpenFindList.end() != m_OpenFindList.find(CheckIndex))
		{
			continue;
		}


		CreateNode(CheckIndex);


		if (nullptr != m_EndNode)
		{
			return;
		}
	}

}

std::list<KVector2> IsoMapRender::WorldPathFind(KVector2 _Start, KVector2 _End) 
{
	return IndexPathFind(WorldToIndex(_Start), WorldToIndex(_End));
}

std::list<KVector2> IsoMapRender::IndexPathFind(KVector2 _Start, KVector2 _End) 
{
	if (_Start == _End)
	{
		std::list<KVector2> ReturnList = std::list<KVector2>();
		ReturnList.push_back(_End);
		return ReturnList;
	}

	CheckDir[0] = KVector2((int)1, (int)0);
	CheckDir[1] = KVector2((int)-1, (int)0);
	CheckDir[2] = KVector2((int)0, (int)1);
	CheckDir[3] = KVector2((int)0, (int)-1);

	m_CurNode = nullptr;
	m_EndNode = nullptr;


	m_OpenList.clear();
	m_OpenFindList.clear();
	m_CloseList.clear();


	StartIndex = _Start;
	EndIndex = _End;


	CreateNode(_Start);
	
	while (0 != m_OpenList.size())
	{
		PopFastNode();

		if (nullptr == m_CurNode)
		{
			return std::list<KVector2>();
		}

		CheckNode();

		if (nullptr != m_EndNode)
		{
			break;
		}
	}

	if (nullptr == m_EndNode)
	{
		return std::list<KVector2>();
	}

	std::list<KVector2> ReturnList = std::list<KVector2>();

	m_CurNode = m_EndNode;

	while (nullptr != m_CurNode)
	{
		ReturnList.push_front(m_CurNode->m_Index);
		m_CurNode = m_CurNode->m_Parent;
	}

	return ReturnList;

}