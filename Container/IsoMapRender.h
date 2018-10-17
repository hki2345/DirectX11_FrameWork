#pragma once
#include <HRenderer.h>
#include <HImage.h>
#include <unordered_map>
#include <map>

class IsoMapRender : public HRenderer
{
private:
	HPTR<HImage> m_Img;
	HCOLOR	     m_Color;

	HMAT TilePosMat;
	HMAT TileSizeMat;

	HVEC2 Index;
	HVEC TilePos;
	HVEC         m_TileSIze;

public:
	class HTile : public HRefBase
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
	std::unordered_map<__int64, HPTR<HTile>>::iterator StartIter;
	std::unordered_map<__int64, HPTR<HTile>>::iterator EndIter;
	std::unordered_map<__int64, HPTR<HTile>> m_TileMap;


public:
	HPTR<HTile> FindTile(HVEC2 _Tile);
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
	HPTR<HImage> Image() { return m_Img; }

public:
	bool Init(int _Order = 0) override;
	bool Init(const wchar_t* _ImgName, HVEC2 _Size,int _Order = 0);
	void Render(HPTR<HCamera> _Camera) override;

private: ///////////////////////////////// AStar
	class PathNode : public HRefBase
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
	HPTR<PathNode> m_CurNode;

	HPTR<PathNode> m_EndNode;
	HVEC2 CheckDir[4];

	std::multimap<float, HPTR<PathNode>> m_OpenList; // ã�ƾ��� ���ɼ��� �ִ� ���
	std::map<__int64, HPTR<PathNode>> m_OpenFindList; // ã�ƾ��� ���ɼ��� �ִ� ���
	std::map<__int64, HPTR<PathNode>> m_CloseList; // �̹� ã�Ƽ� �� ã�� �ʿ䰡 ���� ���.

private:
	void CreateNode(HVEC2 _Index);
	void CalNode(HPTR<PathNode> _Node);
	void CheckNode();

	void PopFastNode();

public:
	// ���� ��ġ�� �ش�.
	std::list<HVEC2> WorldPathFind(HVEC2 _Start, HVEC2 _End);
	std::list<HVEC2> IndexPathFind(HVEC2 _Start, HVEC2 _End);

};

