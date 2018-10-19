#pragma once
#include <KCore.h>

class ToolCoreBuilder : public KCore::Launcher
{
public:
	void Build() override;

public:
	ToolCoreBuilder();
	~ToolCoreBuilder();
};

