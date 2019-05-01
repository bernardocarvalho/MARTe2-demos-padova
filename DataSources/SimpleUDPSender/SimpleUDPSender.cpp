/**
 * @file SimpleUDPSender.cpp
 * @brief Source file for class SimpleUDPSender
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
 * the class SimpleUDPSender (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "SimpleUDPSender.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe2Tutorial {
SimpleUDPSender::SimpleUDPSender() {
    using namespace MARTe;
    totalMemorySize = 0u;
    payload = NULL_PTR(char8 *);
    offsets = NULL_PTR(uint32 *); 
}

SimpleUDPSender::~SimpleUDPSender() {
    using namespace MARTe;
    if (payload != NULL_PTR(char8 *)) {
        GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(reinterpret_cast<void *&>(payload));
    }
    if (offsets != NULL_PTR(uint32 *)) {
         delete[] offsets;
    }
    (void) udpSocket.Close();
}

bool SimpleUDPSender::Initialise(MARTe::StructuredDataI & data) {
    using namespace MARTe;
    bool ok = DataSourceI::Initialise(data);
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "Could not Initialise the DataSourceI");
    }
    uint16 port;
    if (ok) {
        ok = data.Read("Port", port);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The parameter Port shall be set");
        }
    }
    StreamString address;
    if (ok) {
        ok = data.Read("Address", address);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The parameter Address shall be set");
        }
    }
    if (ok) {
        REPORT_ERROR(ErrorManagement::Information, "Parameter Port set to %d", port);
        REPORT_ERROR(ErrorManagement::Information, "Parameter Address set to %s", address.Buffer());
    }
    if (ok) {
        ok = udpSocket.Open();
        if (!ok) {
            REPORT_ERROR(ErrorManagement::FatalError, "Failed to Open socket");
        }
    }
    if (ok) {
        ok = udpSocket.Connect(address.Buffer(), port);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::FatalError, "Failed to Connect socket");
        }
    }
    if (ok) {
        REPORT_ERROR(ErrorManagement::Information, "Connected socket to %s:%d", address.Buffer(), port);
    }

    return ok;
}

bool SimpleUDPSender::SetConfiguredDatabase(MARTe::StructuredDataI& data) {
    using namespace MARTe;
    bool ok = DataSourceI::SetConfiguredDatabase(data);
    if (ok) {
        ok = data.MoveRelative("Signals");
    }

    //Check signal properties and compute memory
    totalMemorySize = 0u;
    if (ok) {
        //Do not allow samples
        uint32 functionNumberOfSignals = 0u;
        uint32 n;
        if (GetFunctionNumberOfSignals(OutputSignals, 0u, functionNumberOfSignals)) {
            for (n = 0u; (n < functionNumberOfSignals) && (ok); n++) {
                uint32 nSamples;
                ok = GetFunctionSignalSamples(OutputSignals, 0u, n, nSamples);
                if (ok) {
                    ok = (nSamples == 1u);
                }
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "The number of samples shall be exactly 1");
                }
            }
        }

        uint32 nOfSignals = GetNumberOfSignals();
        if (ok) {
            ok = (nOfSignals > 0u);
            offsets = new uint32[nOfSignals];
        }

        //Count the number of bytes
        for (n = 0u; (n < nOfSignals) && (ok); n++) {
            offsets[n] = totalMemorySize;
            uint32 nBytes = 0u;
            ok = GetSignalByteSize(n, nBytes);
            totalMemorySize += nBytes;
        }
    }
    //Only one and one GAM allowed to interact with this DataSourceI
    if (ok) {
        ok = (GetNumberOfFunctions() == 1u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Exactly one Function allowed to interact with this DataSourceI");
        }
    }

    return ok;
}

bool SimpleUDPSender::AllocateMemory() {
    using namespace MARTe;
    bool ok = (totalMemorySize > 0);
    if (ok) {
        payload = reinterpret_cast<char8 *>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(totalMemorySize));
    }
    return ok;
}

bool SimpleUDPSender::GetSignalMemoryBuffer(const MARTe::uint32 signalIdx, const MARTe::uint32 bufferIdx, void*& signalAddress) {
    using namespace MARTe;
    bool ok = (payload != NULL_PTR(char8 *));
    if (ok) {
        char8 *memPtr = &payload[offsets[signalIdx]];
        signalAddress = reinterpret_cast<void *&>(memPtr);
    }
    return ok;
}

const MARTe::char8* SimpleUDPSender::GetBrokerName(MARTe::StructuredDataI& data, const MARTe::SignalDirection direction) {
    return "";
}

bool SimpleUDPSender::PrepareNextState(const MARTe::char8* const currentStateName, const MARTe::char8* const nextStateName) {
    return true;
}
    
bool SimpleUDPSender::Synchronise() {
    bool ok = true;
    return ok;
}

CLASS_REGISTER(SimpleUDPSender, "")
}
