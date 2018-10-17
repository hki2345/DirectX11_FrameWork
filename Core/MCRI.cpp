#include "MCRI.h"

bool MCRI::bInit = false;
CRITICAL_SECTION MCRI::MapCB = CRITICAL_SECTION();
std::unordered_map<int, CRITICAL_SECTION*> MCRI::m_Map;

MCRI::Creator MCRI::CreatorInst = MCRI::Creator();
MCRI::Destroy MCRI::DestroyInst = MCRI::Destroy();
