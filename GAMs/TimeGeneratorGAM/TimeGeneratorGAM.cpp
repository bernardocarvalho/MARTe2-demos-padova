/**
 * @file TimeGeneratorGAM.cpp
 * @brief Source file for class TimeGeneratorGAM
 * @date 06 mag 2019
 * @author pc
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This source file contains the definition of all the methods for
 * the class TimeGeneratorGAM (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "TimeGeneratorGAM.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

TimeGeneratorGAM::TimeGeneratorGAM() :
        GAM() {
    timeIncrement = 0u;
    time = NULL;
}

TimeGeneratorGAM::~TimeGeneratorGAM() {
    time = NULL;
}

bool TimeGeneratorGAM::Initialise(MARTe::StructuredDataI & data) {
    bool ok = GAM::Initialise(data);
    if (ok) {
        ok = data.Read("TimeIncrement", timeIncrement);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Please define TimeIncrement");
        }
    }
    return ok;
}

bool TimeGeneratorGAM::Setup() {

    time = reinterpret_cast<uint32 *>(GetOutputSignalMemory(0));
    *time = 0u;
    return true;
}

bool TimeGeneratorGAM::Execute() {
    *time += timeIncrement;
    return true;
}
CLASS_REGISTER(TimeGeneratorGAM, "")

}
