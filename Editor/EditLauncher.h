#pragma once
#include <KCore.h>

class EditLauncher : public KCore::CoreLauncher
{
public:
	void Build() override;

public:
	EditLauncher();
	~EditLauncher();
};

