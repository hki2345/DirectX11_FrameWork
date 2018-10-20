#pragma once
#include <HRenderer.h>
#include <HImage.h>
#include <unordered_map>
#include <map>

class IsoMapRender : public HRenderer
{
private:
	KPtr<HImage> m_Img;
	KColor	     m_Color;

	KMatrix TilePosMat;
	KMatrix TileSizeMat;

	KVector2 Index;
	KVector4 TilePos;
	KVector4         m_TileSIze;

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
	KPtr<HTile> FindTile(KVector2 _Tile);
	// ���콺 ������ �־������� �������� ��.
	void CreateTile(KVector2 _WorldPos, int _Index = 0);
	// �ε��� �־ �����.
	void CreateTile(int _x, int _y, int _Index = 0);
	KVector2 WorldToIndex(KVector2 _WorldPos);

public:
	IsoMapRender();
	~IsoMapRender();

public:
	void Image(const wchar_t* _ImageName);
	KPtr<HImage> Image() { return m_Img; }

public:
	bool Init(int _Order = 0) override;
	bool Init(const wchar_t* _ImgName, KVector2 _Size,int _Order = 0);
	void Render(KPtr<HCamera> _Camera) override;

private: ///////////////////////////////// AStar
	class PathNode : public SmartPtr
	{
	public:
		float F;
		float H;
		float G;
		KVector2 m_Index;
		KVector2 m_Pos;
		PathNode* m_Parent;

	public:
		PathNode();
		~PathNode();
	};

private: ///////////////////// AstartMember

	KVector2 StartIndex;
	KVector2 EndIndex;
	KPtr<PathNode> m_CurNode;

	KPtr<PathNode> m_EndNode;
	KVector2 CheckDir[4];

	std::multimap<float, KPtr<PathNode>> m_OpenList; // ã�ƾ��� ���ɼ��� �ִ� ���
	std::map<__int64, KPtr<PathNode>> m_OpenFindList; // ã�ƾ��� ���ɼ��� �ִ� ���
	std::map<__int64, KPtr<PathNode>> m_CloseList; // �̹� ã�Ƽ� �� ã�� �ʿ䰡 ���� ���.

private:
	void CreateNode(KVector2 _Index);
	void CalNode(KPtr<PathNode> _Node);
	void CheckNode();

	void PopFastNode();

public:
	// ���� ��ġ�� �ش�.
	std::list<KVector2> WorldPathFind(KVector2 _Start, KVector2 _End);
	std::list<KVector2> IndexPathFind(KVector2 _Start, KVector2 _End);

};

