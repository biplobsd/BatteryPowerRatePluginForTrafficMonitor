#pragma once
#include <string>

class CDataManager
{
private:
    CDataManager();

public:
    static CDataManager& Instance();

public:
    std::wstring m_cur_b_rate;

private:
    static CDataManager m_instance;
};
