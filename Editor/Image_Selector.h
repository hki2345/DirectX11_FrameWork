#pragma once


// Image_Selector

class Image_Selector : public CButton
{
	DECLARE_DYNAMIC(Image_Selector)

public:
	CString m_Path;
	CString m_ImageKey;

public:
	Image_Selector();
	virtual ~Image_Selector();

protected:
	DECLARE_MESSAGE_MAP()
};


