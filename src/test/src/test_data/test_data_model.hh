#pragma once
#include <nael_utils/model/model_desc.hh>

//----------------------------------
// Small but quite exhaustive dto
//----------------------------------
namespace model_test
{

namespace dto
{
    //Enum
    MAKE_DTO_ENUM(
        //Name
        EnumDto,
        //Values
        PARAM_1,
        PARAM_2,
        PARAM_3
    )
    //EmptyStruct
    MAKE_DTO_STRUCT(
        IdDto,
        ((std::string)(id_att))
    )
    //Param
    MAKE_DTO_STRUCT(
        ParamDto,
        ((EnumDto)(param_att))
        ((std::string)(val_att))
    )
    //Struct with basic attributes
    MAKE_DTO_STRUCT(
        BasicAttDto,
        //Basic
        ((bool)(bool_att))
        ((unsigned)(unsigned_att))
        ((int)(int_att))
        ((int64_t)(long_att))
        ((double)(double_att))
        ((EnumDto)(enum_att))
        ((std::string)(string_att))
        ((bpt::time_duration)(time_duration_att))
        ((bg::date)(date_att))
        ((bpt::ptime)(ptime_att))
        ((std::vector<unsigned>)(vec_unsigned_att))
        ((std::vector<IdDto>)(vec_obj_att))
        ((std::vector<EnumDto>)(vec_enum_att))
    )
    //Small Context
    MAKE_DTO_STRUCT(
        BasicContextDto,
        ((bool)(bool_att))
        ((std::vector<IdDto>)(vec_id_obj))
        ((std::vector<BasicAttDto>)(vec_basic_obj))
        ((std::vector<ParamDto>)(vec_param_obj))
    )

} //namespace dto


//----------------------------------
// Small bo with corresponding dto
//----------------------------------
namespace bo
{

class FirstClass
{

public:
    FirstClass(std::string const &id, unsigned value);

//Basic attributes
    MAKE_CLASS_ATT(
        ( ( std::string)(id) ),
        ( ( unsigned)(value)(0) )
    )
};

class SecondClass
{
public:
    SecondClass(std::string const &id, double value, FirstClass &first);

private:
    SecondClass();
//Basic attributes
    MAKE_CLASS_ATT(
        ( (std::string)(id) ),
        ( (double)(value)(0.) )
    )
    MAKE_CLASS_REF_ATT(FirstClass, first)
    MAKE_CLASS_CONSTREF_ATT(FirstClass, const_first)
};

/**
 * @brief Dedicated exception for consistency
 */
MAKE_EXCEPTION(consistency)

class BoContext
{
public:
    virtual ~BoContext() = default;

    FirstClass &addFirstClass(std::string const &id, unsigned value);
    SecondClass &addSecondClass(std::string const &id, std::string const &first_id, double value);

//Attributes
    MAKE_STRHASH_MAP(FirstClass, firsts)
    MAKE_MAP(std::string, SecondClass, seconds)
};

} //namespace bo

namespace dto
{
    MAKE_DTO_STRUCT(
        FirstClassDto,
        ((std::string)(id)) // identifier
        ((unsigned)(value)) // identifier
    )
    MAKE_DTO_STRUCT(
        SecondClassDto,
        ((std::string)(id)) // identifier
        ((std::string)(first_id)) // identifier
        ((double)(value)) // identifier
    )
    //Full context
    MAKE_DTO_STRUCT(
        DtoContext,
        ((std::vector<FirstClassDto>)(first_collec))
        ((std::vector<SecondClassDto>)(second_collec))
    )
} //namespace dto

} //namespace model_test
