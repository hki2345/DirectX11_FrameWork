#pragma once


// ImageSelectButton

class ImageSelectButton : public CButton
{
	DECLARE_DYNAMIC(ImageSelectButton)

public:
	CString m_ForderPath;
	CString m_ImageKey;

public:
	ImageSelectButton();
	virtual ~ImageSelectButton();

protected:
	DECLARE_MESSAGE_MAP()
};


