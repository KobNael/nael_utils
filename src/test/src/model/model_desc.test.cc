#include <gtest/gtest.h>

#include "test_data/test_data_helper.hh"
#include "test_data/test_data_model.hh"

TEST(model_desc, basics)
{
    //access for simple attributes
    model_test::dto::BasicAttDto basic_obj = model_test::makeBasicAttDto();
    ASSERT_EQ( basic_obj.bool_att, false );
    ASSERT_EQ( basic_obj.unsigned_att, 1u );
    ASSERT_EQ( basic_obj.int_att, 2 );
    ASSERT_EQ( basic_obj.long_att, 3l );
    ASSERT_EQ( basic_obj.double_att, 4.02 );
    ASSERT_EQ( basic_obj.enum_att, model_test::dto::PARAM_2 );
    ASSERT_EQ( basic_obj.string_att, "id" );
    ASSERT_EQ( basic_obj.time_duration_att, bpt::time_duration(1,2,3) );
    ASSERT_EQ( basic_obj.date_att, bg::day_clock::local_day() );

    //access for vector of simple values
    ASSERT_EQ( basic_obj.vec_unsigned_att.size(), 1u );
    ASSERT_EQ( basic_obj.vec_unsigned_att.at(0), 10u );
    //access for vector of obj values
    ASSERT_EQ( basic_obj.vec_obj_att.size(), 2u );
    ASSERT_EQ( basic_obj.vec_obj_att.at(0).id_att, "o1" );
    ASSERT_EQ( basic_obj.vec_obj_att.at(1).id_att, "o2" );
    //access for vector of simple values
    ASSERT_EQ( basic_obj.vec_enum_att.size(), 3u );
    ASSERT_EQ( basic_obj.vec_enum_att.at(0), model_test::dto::PARAM_3 );
    ASSERT_EQ( basic_obj.vec_enum_att.at(1), model_test::dto::PARAM_2 );
    ASSERT_EQ( basic_obj.vec_enum_att.at(2), model_test::dto::PARAM_1 );
}

TEST(model_desc, print)
{
    model_test::dto::BasicAttDto basic_obj = model_test::makeBasicAttDto();
    std::ostringstream oss, oss_ref;
    oss << basic_obj;
    oss_ref << "BasicAttDto{"
        << "bool_att=0, unsigned_att=1, int_att=2, long_att=3, double_att=4.02"
        << ", enum_att=1, string_att=id, time_duration_att=01:02:03"
        << ", date_att=" << bg::day_clock::local_day()
        << ", vec_unsigned_att=[10]"
        << ", vec_obj_att=[IdDto{id_att=o1},IdDto{id_att=o2}]"
        << ", vec_enum_att=[2,1,0]"
        << "}";
    ASSERT_EQ( oss.str(), oss_ref.str() );
}
