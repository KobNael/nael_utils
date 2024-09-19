#pragma once

#include "utils/model_desc.hh"

#include<vector>

namespace dto
{

//Press description
MAKE_DTO_STRUCT(
	PressDto,
	(std::string, id, identifier)
	(unsigned, pos, relative position)
)

//Mold description
MAKE_DTO_STRUCT(
	MoldDto,
	(std::string, id) // identifier
	(std::vector<std::string>, idPresses) // list of eligible presses
    (unsigned, setUpInSeconds) // set up time
    (unsigned, setDownInSeconds) // set down time
    (bool, allowOffloading) // is offloading allowed
)

//Description of a change of granulate
MAKE_DTO_STRUCT(
	ChangeOfGranulateDto,
    (std::string, from) //old granulate identifier
    (std::string, to) //new granulate identifier
    (unsigned, nbCycles) //number of cyles
)

//List of known parameters
BOOST_DEFINE_ENUM( ParamaterNameDto,
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
    (ParamaterNameDto, name) //parameter name
    (std::string, value) //parameter value
)

//Full instance
MAKE_DTO_STRUCT(
	ContextDto,
	(std::vector<PressDto>, presses) //list of presses
	(std::vector<MoldDto>, molds) //list of molds
    (std::vector<std::string>, colors) //list of colors
    (std::vector<std::string>, granulates) //list of granulates
    (std::vector<std::string>, shades) //list of shades
    (std::vector<ChangeOfGranulateDto>, changesOfGranulate) //list of description of change of granulate
    (std::vector<ParametersDto>, parameters) //list of parameters with value
)

}
