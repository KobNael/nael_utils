#include "test_data_helper.hh"

namespace model_test
{
    //helper
    BasicAttDto makeBasicAttDto(std::string const &id_p)
    {
        BasicAttDto basic_obj;
        //simple attributes
        basic_obj.string_att = id_p;
        basic_obj.bool_att = false;
        basic_obj.unsigned_att = 1u;
        basic_obj.int_att = 2;
        basic_obj.long_att = 3l;
        basic_obj.double_att = 4.02;
        basic_obj.enum_att = model_test::PARAM_2;
        basic_obj.time_duration_att = bpt::time_duration(1,2,3);
        basic_obj.date_att = bg::day_clock::local_day();
        basic_obj.date_att = bg::day_clock::local_day();
        //vector of simple values
        basic_obj.vec_unsigned_att.push_back(10u);
        //vector of objects
        basic_obj.vec_obj_att.push_back(IdDto());
        basic_obj.vec_obj_att.back().id_att="o1";
        basic_obj.vec_obj_att.push_back(IdDto());
        basic_obj.vec_obj_att.back().id_att="o2";
        //vector of enums
        basic_obj.vec_enum_att.push_back(PARAM_3);
        basic_obj.vec_enum_att.push_back(PARAM_2);
        basic_obj.vec_enum_att.push_back(PARAM_1);
        return basic_obj;
    }
    IdDto makeIdDto(std::string const &id_p)
    {
        IdDto obj;
        obj.id_att = id_p;
        return obj;
    }
    ParamDto makeParamDto(EnumDto param, std::string const &val)
    {
        ParamDto obj;
        obj.param_att = param;
        obj.val_att = val;
        return obj;
    }

} //namespace model_test
