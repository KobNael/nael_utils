#include <gtest/gtest.h>
#include <nael_utils/json/json_handler.hh>
#include "test_data/test_data_model.hh"

namespace
{
    void parse_first(model_test::dto::DtoContext const& dto_context, model_test::bo::BoContext &bo_context)
    {
        //Create Press
        for(model_test::dto::FirstClassDto const &first : dto_context.first_collec)
        {
            bo_context.addFirstClass(first.id, first.value);
        }
    }

    void parse_second(model_test::dto::DtoContext const& dto_context , model_test::bo::BoContext &bo_context)
    {
        //Create Second Class elements
        for(model_test::dto::SecondClassDto const &second : dto_context.second_collec)
        {
            bo_context.addSecondClass(second.id, second.first_id, second.value);
        }
    }

    void dto_to_bo(model_test::dto::DtoContext const& dto_context , model_test::bo::BoContext &bo_context)
    {
        parse_first(dto_context, bo_context);
        parse_second(dto_context, bo_context);
    }
}

TEST(dto_handler, dto_to_bo)
{
	//Import dto
	model_test::dto::DtoContext dto_context;
    const char* content =
        "{"
            "\"first_collec\": ["
                "{\"id\":\"first_val_2\", \"value\": 2},"
                "{\"id\":\"first_val_10\", \"value\": 10},"
                "{\"id\":\"first_val_20\", \"value\": 20}"
            "],"
            "\"second_collec\": ["
                "{\"id\":\"second_val_5.5_20\", \"value\": 5.5, \"first_id\":\"first_val_20\"},"
                "{\"id\":\"second_val_2.3_2\", \"value\": 2.3, \"first_id\":\"first_val_2\"},"
                "{\"id\":\"second_val_-1.0_10\", \"value\": -1, \"first_id\":\"first_val_10\"},"
                "{\"id\":\"second_val_2.3_2bis\", \"value\": 2.3, \"first_id\":\"first_val_2\"}"
            "]"
        "}";
    std::istringstream iss(content);
	json::import_from_stream(iss, dto_context);

    model_test::bo::BoContext bo_context;
    ::dto_to_bo(dto_context, bo_context);

    //===============
    // Check content
    //===============
    //--------
    // Firsts
    //--------
    auto && firsts = bo_context.get_firsts();
    ASSERT_NO_THROW (firsts.at("first_val_2"));
    ASSERT_NO_THROW (firsts.at("first_val_10"));
    ASSERT_NO_THROW (firsts.at("first_val_20"));
    //---------
    // Seconds
    //---------
    auto && seconds = bo_context.get_seconds();
    ASSERT_NO_THROW( seconds.at("second_val_5.5_20") );
    ASSERT_NO_THROW( seconds.at("second_val_2.3_2") );
    ASSERT_NO_THROW( seconds.at("second_val_-1.0_10") );
    ASSERT_NO_THROW( seconds.at("second_val_2.3_2bis") );

    //==============
    // Check values
    //==============
    //--------
    // Firsts
    //--------
    auto && first_val_2 = firsts.at("first_val_2");
    EXPECT_EQ(first_val_2.get_id(), "first_val_2");
    EXPECT_EQ(first_val_2.get_value(), 2);
    auto && first_val_10 = firsts.at("first_val_10");
    EXPECT_EQ(first_val_10.get_id(), "first_val_10");
    EXPECT_EQ(first_val_10.get_value(), 10);
    auto && first_val_20 = firsts.at("first_val_20");
    EXPECT_EQ(first_val_20.get_id(), "first_val_20");
    EXPECT_EQ(first_val_20.get_value(), 20);
    //---------
    // Seconds
    //---------
    auto && second_val_5p5_20 = seconds.at("second_val_5.5_20");
    EXPECT_EQ(second_val_5p5_20.get_id(), "second_val_5.5_20");
    EXPECT_EQ(second_val_5p5_20.get_value(), 5.5);
    EXPECT_EQ(second_val_5p5_20.get_first().get_id(), "first_val_20");
    EXPECT_EQ(second_val_5p5_20.get_first().get_value(), 20);
    auto && second_val_2p3_2 = seconds.at("second_val_2.3_2");
    EXPECT_EQ(second_val_2p3_2.get_id(), "second_val_2.3_2");
    EXPECT_EQ(second_val_2p3_2.get_value(), 2.3);
    EXPECT_EQ(second_val_2p3_2.get_first().get_id(), "first_val_2");
    EXPECT_EQ(second_val_2p3_2.get_first().get_value(), 2);
    auto && second_val_minus1_10 = seconds.at("second_val_-1.0_10");
    EXPECT_EQ(second_val_minus1_10.get_id(), "second_val_-1.0_10");
    EXPECT_EQ(second_val_minus1_10.get_value(), -1.);
    EXPECT_EQ(second_val_minus1_10.get_first().get_id(), "first_val_10");
    EXPECT_EQ(second_val_minus1_10.get_first().get_value(), 10);
    auto && second_val_2p3_2bis = seconds.at("second_val_2.3_2bis");
    EXPECT_EQ(second_val_2p3_2bis.get_id(), "second_val_2.3_2bis");
    EXPECT_EQ(second_val_2p3_2bis.get_value(), 2.3);
    EXPECT_EQ(second_val_2p3_2bis.get_first().get_id(), "first_val_2");
    EXPECT_EQ(second_val_2p3_2bis.get_first().get_value(), 2);
    //==============
    // Check update
    //==============
    second_val_2p3_2bis.get_first().set_value(8);
    //invalid
    //second_val_2p3_2bis.get_const_first().set_value(16);
    EXPECT_EQ(second_val_2p3_2bis.get_first().get_value(), 8);
    EXPECT_EQ(second_val_2p3_2.get_first().get_value(), 8);
    EXPECT_EQ(second_val_2p3_2bis.get_const_first().get_value(), 8);
    EXPECT_EQ(second_val_2p3_2.get_const_first().get_value(), 8);
}

TEST(dto_handler, consistency)
{
	//Import inconsistent dto (duplicate key)
	model_test::dto::DtoContext dto_context;
    const char* content =
        "{"
            "\"first_collec\": ["
                "{\"id\":\"first_val_2\", \"value\": 2},"
                "{\"id\":\"first_val_2\", \"value\": 10}"
            "],"
            "\"second_collec\": []"
        "}";
    std::istringstream iss(content);
	json::import_from_stream(iss, dto_context);

    model_test::bo::BoContext bo_context;
    ASSERT_THROW( ::dto_to_bo(dto_context, bo_context), model_test::bo::consistency );

}