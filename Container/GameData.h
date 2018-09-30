#pragma once
class Split_Data
{
public:
	wchar_t path[256];
	wchar_t name[256];

	size_t X;
	size_t Y;

public:
	Split_Data() : X(1), Y(1)
	{

	}
};