#include <gtest/gtest.h>

#include "test_data/test_data_helper.hh"
#include "test_data/test_data_model.hh"

#include <nael_utils/json/json_handler.hh>

TEST(stream_io, export_import)
{
    model_test::BasicContextDto context;
    context.bool_att=false;
    //Push some simple object
    context.vec_id_obj.push_back( model_test::makeIdDto("idA") );
    context.vec_id_obj.push_back( model_test::makeIdDto("idB") );
    //Push some complex objects
    context.vec_basic_obj.push_back( model_test::makeBasicAttDto("obj1") );
    context.vec_basic_obj.push_back( model_test::makeBasicAttDto("obj2") );
    context.vec_basic_obj.push_back( model_test::makeBasicAttDto("obj3") );

    //Export
    std::ostringstream oss;
    json::export_to_stream(oss, context);

    //Reimport in a new contexts
    model_test::BasicContextDto new_context, new_context2;
    std::istringstream iss1(oss.str());
    json::import_from_stream(iss1, new_context);
    std::istringstream iss2(oss.str());
    json::import_from_stream(iss2, new_context2);

    //They should be identical
    ASSERT_EQ(context, new_context);
    ASSERT_EQ(context, new_context2);
    //Modify the new contexts
    new_context.vec_basic_obj.push_back( model_test::makeBasicAttDto("new") );
    new_context2.bool_att=true;
    //They should be different now
    ASSERT_NE(context, new_context);
    ASSERT_NE(context, new_context2);
}

TEST(stream_io, export_import_file)
{
    model_test::BasicContextDto context;
    context.bool_att=false;
    //Push some simple object
    context.vec_id_obj.push_back( model_test::makeIdDto("idA") );
    context.vec_id_obj.push_back( model_test::makeIdDto("idB") );
    //Push some complex objects
    context.vec_basic_obj.push_back( model_test::makeBasicAttDto("obj1") );
    context.vec_basic_obj.push_back( model_test::makeBasicAttDto("obj2") );
    context.vec_basic_obj.push_back( model_test::makeBasicAttDto("obj3") );

    //Export
    json::export_to_file("export.json", context);
    //Reimport in a new contexts
    model_test::BasicContextDto new_context, new_context2;
    json::import_from_file("export.json", new_context);
    json::import_from_file("export.json", new_context2);
    //They should be identical
    ASSERT_EQ(context, new_context);
    ASSERT_EQ(context, new_context2);
    //Modify the new contexts
    new_context.vec_basic_obj.push_back( model_test::makeBasicAttDto("new") );
    new_context2.bool_att=true;
    //They should be different now
    ASSERT_NE(context, new_context);
    ASSERT_NE(context, new_context2);
}
