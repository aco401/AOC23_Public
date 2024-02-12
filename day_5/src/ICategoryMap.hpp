#ifndef NS_DAY_5_ICATEGORY_MAP_H
#define NS_DAY_5_ICATEGORY_MAP_H

namespace ns_day_5 {

/**
 * @brief Interface to 
 * 
 */
class ICategoryMap {
    //  == INSTANTIATION ==
  protected:
    //  -- Constructors --
    ICategoryMap() = default;
    ICategoryMap(const ICategoryMap &) = default;
    ICategoryMap(ICategoryMap &&) = default;

  public:
    //  -- Destructors --
    virtual ~ICategoryMap(){};

    //  == OPERATORS ==
  protected:
    //  -- Assignment --
    ICategoryMap &operator=(const ICategoryMap &) = default;
    ICategoryMap &operator=(ICategoryMap &&) = default;

    //  == METHODS ==
  public:
    virtual int findDestination(int source) const = 0;
};

} // namespace ns_day_5

#endif // NS_DAY_5_ICATEGORY_MAP_H