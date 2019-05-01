/**
 * @file ControllerEx2.cpp
 * @brief Source file for class ControllerEx2
 * @date 06/04/2018
 * @author Andre Neto
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
 * the class ControllerEx2 (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ControllerEx2.h"
#include "CLASSMETHODREGISTER.h"
#include "RegisteredMethodsMessageFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe2Tutorial {

ControllerEx2::ControllerEx2() : Object(), MessageI() {
    using namespace MARTe;
    tau = 0.f;
    ReferenceT<RegisteredMethodsMessageFilter> filter(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetDestination(this);
    MessageI::InstallMessageFilter(filter);
}

ControllerEx2::~ControllerEx2 () {
    if (GetName() != NULL) {
        REPORT_ERROR_STATIC(MARTe::ErrorManagement::Information, "No more references pointing at %s [%s]. "
        "The Object will be safely deleted.", GetName(), GetClassProperties()->GetName());
    }
}

bool ControllerEx2::Initialise(MARTe::StructuredDataI &data) {
    using namespace MARTe;
    bool ok = Object::Initialise(data);
    if (ok) {
        ok = data.Read("Tau", tau);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Tau shall be specified");
        }
    }
    if (ok) {
        ok = data.MoveRelative("Gains1.Low");
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Gains1.Low shall be specified");
        }
    }
    if (ok) {
        ok = LoadGains(gains1.lowGains, data);
        if (ok) {
            ok = data.MoveToAncestor(2u); 
        }
    }
    if (ok) {
        ok = data.MoveRelative("Gains1.High");
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Gains1.High shall be specified");
        }
    }
    if (ok) {
        ok = LoadGains(gains1.highGains, data);
        if (ok) {
            ok = data.MoveToAncestor(2u); 
        }
    }

    if (ok) {
        PrintGainsInfo();
    }
    return ok;
}

void ControllerEx2::PrintGainsInfo() {
    using namespace MARTe;
    REPORT_ERROR(ErrorManagement::Information, "Tau %f", tau);
    REPORT_ERROR(ErrorManagement::Information, "Gains 1 low gains");
    PrintGains(gains1.lowGains);
    REPORT_ERROR(ErrorManagement::Information, "Gains 1 high gains");
    PrintGains(gains1.highGains);
    REPORT_ERROR(ErrorManagement::Information, "Gains 2 low gains");
    PrintGains(gains2.lowGains);
    REPORT_ERROR(ErrorManagement::Information, "Gains 2 high gains");
    PrintGains(gains2.highGains);
}

bool ControllerEx2::LoadGains(Gain &gainToLoad, MARTe::StructuredDataI &data) {
    using namespace MARTe;
    bool ok = data.Read("Gain1", gainToLoad.gain1);
    if (!ok) {
        REPORT_ERROR(ErrorManagement::Information, "Gain1 shall be specified");
    }
    AnyType arrayDescription = data.GetType("Gain3");
    if (ok) {
        ok = (arrayDescription.GetTypeDescriptor() != voidAnyType.GetTypeDescriptor());
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Gain3 shall be specified");
        }
    }
    uint32 numberOfElements = 0u;
    if (ok) {
        numberOfElements = arrayDescription.GetNumberOfElements(0u);
        ok = (numberOfElements > 0u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "No elements defined in the Gain3");
        }
    }
    if (ok) {
        ok = (numberOfElements == 6u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Gain3 shall have exactly 6 elements and not %d", numberOfElements);
        }
    }
    if (ok) {
        Vector<float32> readVector(reinterpret_cast<float32 *>(&gainToLoad.gain3), numberOfElements);
        ok = data.Read("Gain3", readVector);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not read the Gain3 parameter");
        }
    }
    return ok; 
}

void ControllerEx2::PrintGains(Gain &gainToPrint) {
    using namespace MARTe;
    REPORT_ERROR(ErrorManagement::Information, "Gain1 %f", gainToPrint.gain1);
    REPORT_ERROR(ErrorManagement::Information, "Gain2 %d", gainToPrint.gain2);
    REPORT_ERROR(ErrorManagement::Information, "Gain3 %f", gainToPrint.gain3);
}

MARTe::ErrorManagement::ErrorType ControllerEx2::ResetGain1() {
    using namespace MARTe;
    REPORT_ERROR(ErrorManagement::Information, "Resetting gain1");
    gains1.lowGains.gain1 = 0;
    gains1.highGains.gain1 = 0;
    gains2.lowGains.gain1 = 0;
    gains2.highGains.gain1 = 0;
    PrintGainsInfo();
    return ErrorManagement::NoError;
}

CLASS_REGISTER(ControllerEx2, "")
CLASS_METHOD_REGISTER(ControllerEx2, ResetGain1)
}


