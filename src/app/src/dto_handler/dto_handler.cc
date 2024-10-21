#include "dto_handler.hh"

namespace bpt = boost::posix_time;
namespace
{

    void parse_press(dto::DtoContext const& dto_context , bo::BoContext &bo_context)
    {
        //Create Press
        for(dto::PressDto const&dto_press : dto_context.presses)
        {
            auto it = bo_context.get_presses().insert( std::make_pair(dto_press.id, bo::Press()));
            if(!it.second)
            {
                std::cout << "warn duplicate press " << dto_press.id << std::endl;
            }
        }
    }

    void parse_mold(dto::DtoContext const& dto_context , bo::BoContext &bo_context)
    {
        //Create Mold
        for(dto::MoldDto const&dto_mold : dto_context.molds)
        {
            auto it = bo_context.get_molds().insert( std::make_pair(dto_mold.id, bo::Mold()));
            if(!it.second)
            {
                std::cout << "warn duplicate mold " << dto_mold.id << std::endl;
            }
            else
            {
                bo::Mold &mold(it.first->second);
                mold.set_setUp(bpt::seconds(dto_mold.setUpInSeconds));
                mold.set_setDown(bpt::seconds(dto_mold.setDownInSeconds));
                mold.set_allowOffloading(dto_mold.allowOffloading);
                mold.set_idleTimeBeforeChangeOver(bpt::seconds(dto_mold.idleTimeBeforeChangeOverInSeconds));
                mold.set_idleTimeAfterChangeOver(bpt::seconds(dto_mold.idleTimeAfterChangeOverInSeconds));
                //add link to press
                for(std::string const &idpress : dto_mold.idPresses)
                {
                    auto itp = bo_context.get_presses().find(idpress);
                    if(itp != bo_context.get_presses().end())
                    {
                        mold.get_presses().push_back(&(itp->second));
                    }
                    else
                    {
                        std::cout << "warn unknown press " << idpress << std::endl;
                    }
                }
            }
        }
    }
}

namespace dto_handler
{

    void dto_to_bo(dto::DtoContext const& dto_context , bo::BoContext &bo_context)
    {
        parse_press(dto_context, bo_context);
        parse_mold(dto_context, bo_context);
    }

}
