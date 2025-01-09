#pragma once
#include <nael_utils/model/model_desc.hh>

namespace bg = boost::gregorian;
namespace bpt = boost::posix_time;

namespace model_test
{
    //Enum
    MAKE_DTO_ENUM(
        //Name
        EnumDto,
        //Values
        VAL_1,
        VAL_2,
        VAL_3
    )
    //EmptyStruct
    MAKE_DTO_STRUCT(
        IdDto,
        ((std::string)(id_att))
    )
    //Struct with basic attributes
    MAKE_DTO_STRUCT(
        BasicAttDto,
        //Basic
        ((bool)(bool_att))
        ((unsigned)(unsigned_att))
        ((int)(int_att))
        ((long)(long_att))
        ((double)(double_att))
        ((EnumDto)(enum_att))
        ((std::string)(string_att))
        ((bpt::time_duration)(time_duration_att))
        ((bg::date)(date_att))
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
    )
} //namespace model_test
