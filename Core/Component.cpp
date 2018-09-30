#include "Component.h"
#include "WriteStream.h"
#include "ReadStream.h"


bool Component::Is_Multi(TheOne* _pTheOne)
{
	return	true;
}

Component::Component()
{
}


Component::~Component()
{
}

void Component::Init_Component()
{

}

void Component::FinalUpdate()
{

}

void Component::End_Update()
{

}


void Component::save(WriteStream& _Stream)
{
	std::string Name = typeinfo()->name();

	UINT Size = (UINT)Name.size() + 1;
	_Stream.write(&Size, sizeof(UINT));
	_Stream.write_template(Name.c_str(), sizeof(char) * Size);
}

void Component::load(ReadStream& _Stream)
{
	char Arr[256];
	unsigned int Size;
	_Stream.read(&Size, sizeof(UINT));
	_Stream.read(Arr, sizeof(char) * Size);
}