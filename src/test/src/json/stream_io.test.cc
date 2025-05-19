#include <gtest/gtest.h>

#include "test_data/test_data_helper.hh"
#include "test_data/test_data_model.hh"

#include <nael_utils/json/json_handler.hh>

#include <chrono>
#include <thread>

/**
 * Class test for filesystem helpers
 */
class stream_io: public ::testing::Test {
protected:
    //SetUp (fill a basic context)
	virtual void SetUp()
    {
        //Fill the context
        _context.bool_att=false;
        //Push some simple object
        _context.vec_id_obj.push_back( model_test::makeIdDto("idA") );
        _context.vec_id_obj.push_back( model_test::makeIdDto("idB") );
        //Push some complex objects
        _context.vec_basic_obj.push_back( model_test::makeBasicAttDto("obj1") );
        _context.vec_basic_obj.push_back( model_test::makeBasicAttDto("obj2") );
        _context.vec_basic_obj.push_back( model_test::makeBasicAttDto("obj3") );
        //Push some param value
        _context.vec_param_obj.push_back( model_test::makeParamDto( model_test::dto::PARAM_2, "val_PARAM_2") );
        _context.vec_param_obj.push_back( model_test::makeParamDto( model_test::dto::PARAM_1, "val_PARAM_1") );
        _context.vec_param_obj.push_back( model_test::makeParamDto( model_test::dto::PARAM_3, "val_PARAM_3") );
    }

    //TearDown (do nothing)
	virtual void TearDown()
    {}

    //Method of comparison
    void compare_contexts(model_test::dto::BasicContextDto &c1, model_test::dto::BasicContextDto &c2)
    {
        //They should be identical to the reference
        ASSERT_EQ(_context, c1);
        ASSERT_EQ(_context, c2);
        //Check vector elements
        std::vector<std::string> new_ids, ref_ids{"idA", "idB"};
        std::ranges::transform(c1.vec_id_obj,
            std::back_inserter(new_ids),
            [](auto&& obj) { return obj.id_att; });
        ASSERT_EQ(new_ids, ref_ids);

        //Modify the new contexts
        c1.vec_basic_obj.push_back( model_test::makeBasicAttDto("new") );
        c2.bool_att=true;
        //They should be different now
        ASSERT_NE(_context, c1);
        ASSERT_NE(_context, c2);
    }

    //Reference context
    model_test::dto::BasicContextDto _context;
};

TEST_F(stream_io, export_import)
{
    EXPECT_EQ(_context.vec_id_obj.size(), 2u);
    //Export
    std::ostringstream oss;
    json::export_to_stream(oss, _context);

    //Reimport in a new contexts
    model_test::dto::BasicContextDto new_context, new_context2;
    std::istringstream iss1(oss.str());
    json::import_from_stream(iss1, new_context);
    std::istringstream iss2(oss.str());
    json::import_from_stream(iss2, new_context2);

    //Test
    compare_contexts(new_context, new_context2);
}

TEST_F(stream_io, export_import_file)
{
    EXPECT_EQ(_context.vec_id_obj.size(), 2u);
    //Export
    json::export_to_file("export.json", _context);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    //Reimport in a new contexts
    model_test::dto::BasicContextDto new_context, new_context2;
    json::import_from_file("export.json", new_context);
    json::import_from_file("export.json", new_context2);

    //Test
    compare_contexts(new_context, new_context2);
}
