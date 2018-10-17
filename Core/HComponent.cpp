#include "HComponent.h"
#include "BWStream.h"
#include "BRStream.h"
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

// �������� ������Ʈ �ϴ� �༮���� ó���� ���⼭ �����ٴ� ���� �������.
// 
// TransForm. 
void HComponent::FinalUpdate() 
{

}
void HComponent::EndUpdate()
{

}

void HComponent::Save(BWStream& _Stream)
{
	std::string Name = TypeInfo()->name();
	 
	unsigned int Size = (UINT)Name.size() + 1;
	_Stream.Write(&Size, sizeof(unsigned int));
	_Stream.WriteT(Name.c_str(), sizeof(char) * Size);
}

void HComponent::Load(BRStream& _Stream)
{
	char Arr[256];
	unsigned int Size;
	_Stream.Read(&Size, sizeof(unsigned int));
	_Stream.Read(Arr, sizeof(char) * Size);
}