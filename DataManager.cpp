#include "pch.h"
#include "DataManager.h"

CDataManager CDataManager::m_instance;

CDataManager::CDataManager()
{
}


CDataManager& CDataManager::Instance()
{
    return m_instance;
}
