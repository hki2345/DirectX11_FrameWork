#pragma once
#include <HRenderer.h>
#include <HImage.h>
#include <unordered_map>
#include <map>

class IsoMapRender : public HRenderer
{
private:
	KPtr<HImage> m_Img;
	HCOLOR	     m_Color;

	HMAT TilePosMat;
	HMAT TileSizeMat;

	HVEC2 Index;
	HVEC TilePos;
	HVEC         m_TileSIze;

public:
	class HTile : public SmartPtr
	{
	public:
		int Index;

	public:
		bool operator == (const int _Index) 
		{
			return Index == _Index;
		}

	public:
		HTile(int _Index) : Index(_Index)
		{

		}
	};

private:
	std::unordered_map<__int64, KPtr<HTile>>::iterator StartIter;
	std::unordered_map<__int64, KPtr<HTile>>::iterator EndIter;
	std::unordered_map<__int64, KPtr<HTile>> m_TileMap;


public:
	KPtr<HTile> FindTile(HVEC2 _Tile);
	// ���콺 ������ �־������� �������� ��.
	void CreateTile(HVEC2 _WorldPos, int _Index = 0);
	// �ε��� �־ �����.
	void CreateTile(int _x, int _y, int _Index = 0);
	HVEC2 WorldToIndex(HVEC2 _WorldPos);

public:
	IsoMapRender();
	~IsoMapRender();

public:
	void Image(const wchar_t* _ImageName);
	KPtr<HImage> Image() { return m_Img; }

public:
	bool Init(int _Order = 0) override;
	bool Init(const wchar_t* _ImgName, HVEC2 _Size,int _Order = 0);
	void Render(KPtr<HCamera> _Camera) override;

private: ///////////////////////////////// AStar
	class PathNode : public SmartPtr
	{
	public:
		float F;
		float H;
		float G;
		HVEC2 m_Index;
		HVEC2 m_Pos;
		PathNode* m_Parent;

	public:
		PathNode();
		~PathNode();
	};

private: ///////////////////// AstartMember

	HVEC2 StartIndex;
	HVEC2 EndIndex;
	KPtr<PathNode> m_CurNode;

	KPtr<PathNode> m_EndNode;
	HVEC2 CheckDir[4];

	std::multimap<float, KPtr<PathNode>> m_OpenList; // ã�ƾ��� ���ɼ��� �ִ� ���
	std::map<__int64, KPtr<PathNode>> m_OpenFindList; // ã�ƾ��� ���ɼ��� �ִ� ���
	std::map<__int64, KPtr<PathNode>> m_CloseList; // �̹� ã�Ƽ� �� ã�� �ʿ䰡 ���� ���.

private:
	void CreateNode(HVEC2 _Index);
	void CalNode(KPtr<PathNode> _Node);
	void CheckNode();

	void PopFastNode();

public:
	// ���� ��ġ�� �ش�.
	std::list<HVEC2> WorldPathFind(HVEC2 _Start, HVEC2 _End);
	std::list<HVEC2> IndexPathFind(HVEC2 _Start, HVEC2 _End);

};

