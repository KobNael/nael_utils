#include "test_data_model.hh"

namespace model_test
{

namespace bo
{

FirstClass::FirstClass(std::string const &id, unsigned value)
    : _id(id)
    , _value(value)
{}

SecondClass::SecondClass(std::string const &id, double value, FirstClass &first)
    : _id(id)
    , _value(value)
    , _first(first)
    , _const_first(first)
{}

FirstClass &BoContext::addFirstClass(std::string const &id, unsigned value)
{
    auto [iter, inserted] = _firsts.try_emplace(id, id, value);
    if(!inserted)
    {
        throw opt::consistency("An item with the same id already exists");
    }
    return iter->second;
}
SecondClass &BoContext::addSecondClass(std::string const &id, std::string const &first_id, double value)
{
    auto first = _firsts.find(first_id);
    if(first == _firsts.end())
    {
        throw consistency("Unknown first item " + first_id);
    }
    auto [iter, inserted] = _seconds.try_emplace(id, id, value, first->second);
    if(!inserted)
    {
        throw consistency("An item with the same id already exists");
    }
    return iter->second;
}

} //namespace bo

} //namespace model_test
