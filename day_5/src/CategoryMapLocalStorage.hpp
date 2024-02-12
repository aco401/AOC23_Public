#ifndef NS_DAY_5_CATEGORY_MAP_LOCAL_STORAGE_H
#define NS_DAY_5_CATEGORY_MAP_LOCAL_STORAGE_H

#include <ICategoryMap.hpp>
#include <ICategoryMapInputHandler.hpp>
#include <ILog.hpp>
#include <map>
#include <memory>

namespace ns_day_5 {

/**
 * @brief Store Category information with cols Destination Start, Source Start and Range length 
 * LocalStorage means it holds local data like a map, instead of only relying on the input handler
 */
class CategoryMapLocalStorage : public ns_day_5::ICategoryMap {
  public:
    CategoryMapLocalStorage(std::shared_ptr<ILog> logger,
                            std::unique_ptr<ICategoryMapInputHandler> handler);
    int findDestination(int source) const override;
    ~CategoryMapLocalStorage() override; // not necessary

  private:
    std::shared_ptr<ILog> logger;
    std::unique_ptr<ICategoryMapInputHandler> handler;

    std::map<int, std::pair<int, int>> data;
};

} // namespace ns_day_5

#endif // NS_DAY_5_CATEGORY_MAP_LOCAL_STORAGE_H