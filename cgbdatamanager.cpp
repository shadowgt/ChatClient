#include "cgbdatamanager.h"

CGBDataManager::CGBDataManager()
{

}

CGBDataManager& CGBDataManager::Instance()
{
    static CGBDataManager GBDataManager;
    return GBDataManager;
}
