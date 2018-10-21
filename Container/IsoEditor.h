#pragma once
#include "SomeOne.h"
#include "IsoMapRender.h"
#include <SoundPlayer.h>

class IsoEditor :
	public SomeOne
{
public:
	KPtr<IsoMapRender> IsoRender;
	KPtr<SoundPlayer> m_SndPlayer;

public:
	bool Init() override;
	void Update() override;
	void DebugRender() override;

public:
	IsoEditor();
	~IsoEditor();
};

