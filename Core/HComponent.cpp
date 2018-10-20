#include "HComponent.h"
#include "WriteStream.h"
#include "ReadStream.h"
#include <Windows.h>


bool HComponent::IsMulti(HActor* _pActor) 
{
	return true;
}

HComponent::HComponent()
{
}


HComponent::~HComponent()
{
}

void HComponent::ComInit() {

}

// 엔진에서 업데이트 하는 녀석들의 처리는 여기서 끝난다는 것을 기억하자.
// 
// TransForm. 
void HComponent::FinalUpdate() 
{

}
void HComponent::EndUpdate()
{

}

void HComponent::Save(WriteStream& _Stream)
{
	std::string Name = TypeInfo()->name();
	 
	unsigned int Size = (UINT)Name.size() + 1;
	_Stream.Write(&Size, sizeof(unsigned int));
	_Stream.WriteT(Name.c_str(), sizeof(char) * Size);
}

void HComponent::Load(ReadStream& _Stream)
{
	char Arr[256];
	unsigned int Size;
	_Stream.Read(&Size, sizeof(unsigned int));
	_Stream.Read(Arr, sizeof(char) * Size);
}