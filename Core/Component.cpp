#include "Component.h"
#include "WriteStream.h"
#include "ReadStream.h"
#include "KMacro.h"


bool Component::IsMulti(TheOne* _pActor) 
{
	return true;
}

Component::Component()
{
}


Component::~Component()
{
}

void Component::ComInit() {

}

// �������� ������Ʈ �ϴ� �༮���� ó���� ���⼭ �����ٴ� ���� �������.
// ���� �����̶� �ڸ�
void Component::FinalUpdate() 
{

}
void Component::EndUpdate()
{

}

void Component::Save(WriteStream& _Stream)
{
	std::string TempName = TypeInfo()->name();
	 
	KUINT TempSize = (KUINT)TempName.size() + 1;
	_Stream.Write(&TempSize, sizeof(unsigned int));
	_Stream.WriteT(TempName.c_str(), sizeof(char) * TempSize);
}

void Component::Load(ReadStream& _Stream)
{
	char Arr[256];
	unsigned int Size;
	_Stream.Read(&Size, sizeof(unsigned int));
	_Stream.Read(Arr, sizeof(char) * Size);
}