#include "Container.h"


Force_Container::Force_Container()
{
}
Force_Container::Force_Container(const Force_Container& _Other) : Color(_Other.Color)
{
	memcpy_s(Name, NAMENUM, _Other.Name, NAMENUM);
}
