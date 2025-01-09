#pragma once

#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/date_time/gregorian/gregorian_types.hpp>

#include <nael_utils/model/model_desc.hh>
#include<vector>

namespace dto
{

//Press description
MAKE_DTO_STRUCT(
    PressDto,
    ((std::string)(id)) // identifier
    ((unsigned)(relativePosition)) // identifier
)


//Mold description
MAKE_DTO_STRUCT(
    MoldDto,
    ((std::string)(id)) // identifier
    ((std::vector<std::string>)(idPresses)) // list of eligible presses
    ((unsigned)(setUpInSeconds)) // set up time
    ((unsigned)(setDownInSeconds)) // set down time
    ((bool)(allowOffloading)) // is offloading allowed
    ((unsigned)(idleTimeBeforeChangeOverInSeconds)) // idle time before change over
    ((unsigned)(idleTimeAfterChangeOverInSeconds)) // idle time after change over
)

//Description of a Configuration
MAKE_DTO_STRUCT(
    ConfigurationDto,
    ((std::string)(name)) //name of the configuration
    ((std::string)(idMold)) //id of the mold
    ((std::string)(idGranulate)) //id of the Granulate
    ((std::string)(idColor)) //id of the Color
    ((std::string)(idShade)) //id of the Shade
    ((unsigned)(productionTimeSeconds)) //prod time in seconds
    ((std::vector<std::string>)(idProducts)) //list of references
)

//Description of a change of granulate
MAKE_DTO_STRUCT(
    ChangeOfGranulateDto,
    ((std::string)(from)) //old granulate identifier
    ((std::string)(to)) //new granulate identifier
    ((unsigned)(nbCycles)) //number of cyles required
)

//Description of a change of shade
MAKE_DTO_STRUCT(
    ChangeOfShadeDto,
    ((std::string)(from)) //old shage identifier
    ((std::string)(to)) //new shage identifier
    ((unsigned)(nbCycles)) //number of cyles required
)

//Demand description
MAKE_DTO_STRUCT(
    DemandDto,
    ((std::string)(product)) //product
    ((boost::gregorian::date)(date)) //date
    ((boost::posix_time::time_duration)(timeOfDay)) //time
    ((unsigned)(quantity)) //quantity taken
    ((unsigned)(safetyWindow)) // min stock level at the time
)

//Constraint on shared ressources
MAKE_DTO_STRUCT(
    SharedResConstraintDto,
    ((unsigned)(max)) //max number of simultaneous change over
    ((std::vector<std::string>)(idPresses)) //list of presses
)

//Description of a closing
//Can apply on a press, a mold, or a change over on given press
MAKE_DTO_STRUCT(
    ClosingDto,
    ((std::string)(id)) // identifier of the object
    ((boost::gregorian::date) (dateFrom)) //date time from
    ((boost::posix_time::time_duration) (timeOfDayFrom))
    ((boost::gregorian::date) (dateTo)) //date time to
    ((boost::posix_time::time_duration) (timeOfDayTo))
)

//List of known parameters
MAKE_DTO_ENUM( ParamaterNameDto,
    //Horizon definition
    SCHEDULE_DATE, SCHEDULE_STARTING_TIME, SCHEDULE_DURATION,
    //Pre process configuration
    UNDERLOAD_LIMIT, OVERLOAD_LIMIT,
    //Post process configuration
    POSTPROCESS, EARLIEST_SAFETYSTOCK_USAGE, PP_SAFETYWINDOW, PP_SHOPSTOCK,
    PP_SAFETY_STOCK, PP_RESTART_PROD, MIN_DURATION_ON_RESTART,
    //Weight of the objective function
    RUPTURE_WITHOUT_SAFETY_STOCK, RUPTURE_TIME, RUPTURE_WITH_SAFETY_STOCK,
    MAX_SAFETY_WINDOW_USAGE_IN_PERCENT, SAFETY_WINDOW_USAGE, AVERAGE_BATCH_DURATION,
    //Other
    NB_THREADS)

//Description of a parameter
MAKE_DTO_STRUCT(
    ParametersDto,
    ((ParamaterNameDto)(name)) //parameter name
    ((std::string)(value)) //parameter value
)

//Full context
MAKE_DTO_STRUCT(
    DtoContext,
    ((std::vector<PressDto>)(presses)) //list of presses
    ((std::vector<MoldDto>)(molds)) //list of molds
    ((std::vector<std::string>)(colors)) //list of colors
    ((std::vector<std::string>)(granulates)) //list of granulates
    ((std::vector<std::string>)(shades)) //list of shades
    ((std::vector<ConfigurationDto>)(configurations)) //list of shades
    ((std::vector<ChangeOfGranulateDto>)(changesOfGranulate)) //list of description of change of granulate
    ((std::vector<ChangeOfShadeDto>)(changesOfShade)) //list of description of change of granulate
    ((std::vector<ParametersDto>)(parameters)) //list of parameters with value
    ((std::vector<SharedResConstraintDto>)(sharedResources)) //
    ((std::vector<ClosingDto>)(pressClosing)) //period of closing by press
    ((std::vector<ClosingDto>)(moldClosing)) //period of closing by press
    ((std::vector<ClosingDto>)(changeOverClosing)) //period of closing by press
    ((std::vector<DemandDto>)(demands)) //period of closing by press
)

}
