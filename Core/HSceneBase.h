#pragma once
// 이안에 다른 헤더가 없다.
// 무조건 구현해주게 하고 싶을때.
// c#에서는 이런걸 인터페이스라고 한다.
class HScene;
class HSceneBase
{
public:
	friend class HActor;
	friend class HComponent;
	friend class HScene;

private:
	HScene* m_pScene;

public:
	HScene* Scene()
	{
		return m_pScene;
	}

private:
	void Scene(HScene* _pScene)
	{
		m_pScene = _pScene;
	}

protected:
	HSceneBase() : m_pScene(nullptr) {}
	~HSceneBase() {}
};

