#pragma once
#include <HCore.h>

class ToolCoreBuilder : public HCore::HCoreBuilder
{
public:
	void Build() override;

public:
	ToolCoreBuilder();
	~ToolCoreBuilder();
};

