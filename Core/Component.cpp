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

// 엔진에서 업데이트 하는 녀석들의 처리는 여기서 끝난다는 것을 기억하자.
// 최종 적용이란 솟리
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