#pragma once
// �̾ȿ� �ٸ� ����� ����.
// ������ �������ְ� �ϰ� ������.
// c#������ �̷��� �������̽���� �Ѵ�.
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

