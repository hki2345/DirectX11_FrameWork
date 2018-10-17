#pragma once
#include "HTrans.h"
#include "TimeMgr.h"

class HBaseCom : public HComponent
{
protected:
	HTrans* m_Trans;

public:
	HTrans* Trans() 
	{
		return m_Trans;
	}

public:
	virtual void ComInit() override;

public:
	HBaseCom();
	~HBaseCom();
};

