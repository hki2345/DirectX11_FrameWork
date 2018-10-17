#pragma once
#include "HLogic.h"
#include "IsoMapRender.h"
#include <HSoundPlayer.h>

class IsoEditor :
	public HLogic
{
public:
	HPTR<IsoMapRender> IsoRender;
	HPTR<HSoundPlayer> SoundPlayer;

public:
	bool Init() override;
	void Update() override;
	void DebugRender() override;

public:
	IsoEditor();
	~IsoEditor();
};

