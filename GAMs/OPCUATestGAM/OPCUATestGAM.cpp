/**
 * @file OPCUATestGAM.cpp
 * @brief Source file for class OPCUATestGAM
 * @date 30 Jan 2019 TODO Verify the value and format of the date
 * @author lporzio TODO Verify the name and format of the author
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
 * the class OPCUATestGAM (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "OPCUATestGAM.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

OPCUATestGAM::OPCUATestGAM() :
        GAM() {
    firstInputSignal = NULL_PTR(float64 *);
    firstOutputSignal = NULL_PTR(float64 *);
    time = NULL_PTR(float64 *);
    check = 0u;
    counter = 0.0;
    statCpuCount = 0u;
    firstCpuCount = 0u;
    lastCpuCount = 0u;
    mode = 0u;
    signalType = InvalidType;
    numberOfDimensions = 0u;
    numberOfElements = 0u;
    numCycles = 0u;
}

OPCUATestGAM::~OPCUATestGAM() {

}

bool OPCUATestGAM::Initialise(StructuredDataI &data) {
    bool ok = GAM::Initialise(data);
    if (ok) {
        data.Read("Mode", mode);
    }
    return ok;
}

bool OPCUATestGAM::Setup() {
    bool ok = true;


    firstInputSignal = reinterpret_cast<float64 *>(GetInputSignalMemory(0u));
    time = reinterpret_cast<float64*>(GetOutputSignalMemory(0u));
    firstOutputSignal = reinterpret_cast<float64*>(GetOutputSignalMemory(1u));

    return ok;
}

bool OPCUATestGAM::Execute() {
    bool ok = false;

    counter += 1.0;
    if (mode == 0) {
        if (check == 0u) {
	    statCpuCount = HighResolutionTimer::Counter();
            SetOutput();
            firstCpuCount = HighResolutionTimer::Counter();
            check = 1u;
        }
        else if (check == 1u) {
            ok = (firstInputSignal[0] == firstOutputSignal[0]);
            if (ok) {
                lastCpuCount = HighResolutionTimer::Counter();
                float64 timePassed = HighResolutionTimer::TicksToTime(lastCpuCount, firstCpuCount);
                *time = timePassed*1000;
		//REPORT_ERROR(ErrorManagement::Information, "Time: %f", timePassed*1000);
                numCycles++;
		if(numCycles == 500000){
		     REPORT_ERROR(ErrorManagement::Information, "\aTest completed! - NumOfCycles = %d - Time = %f", numCycles, HighResolutionTimer::TicksToTime(lastCpuCount, statCpuCount)/60);
		     exit(1);
		}
		counter+=1.0;
                SetOutput();
                firstCpuCount = HighResolutionTimer::Counter();
            }
        }
    }
    else if (mode == 1) {
        if (check == 0u) {
	    statCpuCount = HighResolutionTimer::Counter();
            check = 1u;
        }
        else if (check == 1u) {
                lastCpuCount = HighResolutionTimer::Counter();
                //float64 timePassed = HighResolutionTimer::TicksToTime(lastCpuCount, firstCpuCount);
                //*time = timePassed*1000;
		//numCycles++;
                if ((HighResolutionTimer::TicksToTime(lastCpuCount, statCpuCount)/60) > 2.00) {
                    REPORT_ERROR(ErrorManagement::Information, "\aTest completed! - Time = %f", HighResolutionTimer::TicksToTime(lastCpuCount, statCpuCount)/60);
		    exit(1);
                }
        }
    }
    else if (mode == 3) {
        //REPORT_ERROR_STATIC(ErrorManagement::Information, "Running...");
    }
    return true;
}

void OPCUATestGAM::SetOutput() {
    bool ok = false;
    bool isArray = false;
    for (uint32 signalIndex = 1u; (signalIndex < GetNumberOfOutputSignals()); signalIndex++) {
        signalType = GetSignalType(OutputSignals, signalIndex);
        ok = GetSignalNumberOfDimensions(OutputSignals, signalIndex, numberOfDimensions);

        if (ok) {
            isArray = (numberOfDimensions > 0);
        }
        if (ok && isArray) {
            ok = GetSignalNumberOfElements(OutputSignals, signalIndex, numberOfElements);
        }
        if (ok) {
            if (signalType == SignedInteger8Bit) {
                int8* value = reinterpret_cast<int8*>(GetOutputSignalMemory(signalIndex));
                if (isArray) {
                    for (uint32 i = 0u; i < numberOfElements; i++) {
                        if (counter == 1.0) {
                            value[i] = 1;
                        }
                        else {
                            value[i] += 1;
                        }
                    }
                }
                else {
                    if (counter == 1.0) {
                        *value = 1;
                    }
                    else {
                        *value += 1;
                    }
                }
                //REPORT_ERROR_STATIC(ErrorManagement::Information, "Value set to %d", *reinterpret_cast<int8*>(GetOutputSignalMemory(signalIndex)));
            }

            if (signalType == SignedInteger16Bit) {
                int16* value = reinterpret_cast<int16*>(GetOutputSignalMemory(signalIndex));
                if (isArray) {
                    for (uint32 i = 0u; i < numberOfElements; i++) {
                        if (counter == 1.0) {
                            value[i] = 1;
                        }
                        else {
                            value[i] += 1;
                        }
                    }
                }
                else {
                    if (counter == 1.0) {
                        *value = 1;
                    }
                    else {
                        *value += 1;
                    }
                }
                //REPORT_ERROR_STATIC(ErrorManagement::Information, "Value set to %d", *reinterpret_cast<int16*>(GetOutputSignalMemory(signalIndex)));
            }

            if (signalType == SignedInteger32Bit) {
                int32* value = reinterpret_cast<int32*>(GetOutputSignalMemory(signalIndex));
                if (isArray) {
                    for (uint32 i = 0u; i < numberOfElements; i++) {
                        if (counter == 1.0) {
                            value[i] = 1;
                        }
                        else {
                            value[i] += 1;
                        }
                    }
                }
                else {
                    if (counter == 1.0) {
                        *value = 1;
                    }
                    else {
                        *value += 1;
                    }
                }
                //REPORT_ERROR_STATIC(ErrorManagement::Information, "Value set to %d", *reinterpret_cast<int32*>(GetOutputSignalMemory(signalIndex)));
            }

            if (signalType == SignedInteger64Bit) {
                int64* value = reinterpret_cast<int64*>(GetOutputSignalMemory(signalIndex));
                if (isArray) {
                    for (uint32 i = 0u; i < numberOfElements; i++) {
                        if (counter == 1.0) {
                            value[i] = 1;
                        }
                        else {
                            value[i] += 1;
                        }
                    }
                }
                else {
                    if (counter == 1.0) {
                        *value = 1;
                    }
                    else {
                        *value += 1;
                    }
                }
                //REPORT_ERROR_STATIC(ErrorManagement::Information, "Value set to %d", *reinterpret_cast<int64*>(GetOutputSignalMemory(signalIndex)));
            }

            if (signalType == UnsignedInteger8Bit) {
                uint8* value = reinterpret_cast<uint8*>(GetOutputSignalMemory(signalIndex));
                if (isArray) {
                    for (uint32 i = 0u; i < numberOfElements; i++) {
                        if (counter == 1.0) {
                            value[i] = 1u;
                        }
                        else {
                            value[i] += 1u;
                        }
                    }
                }
                else {
                    if (counter == 1.0) {
                        *value = 1u;
                    }
                    else {
                        *value += 1u;
                    }
                }
                //REPORT_ERROR_STATIC(ErrorManagement::Information, "Value set to %d", *reinterpret_cast<uint8*>(GetOutputSignalMemory(signalIndex)));
            }

            if (signalType == UnsignedInteger16Bit) {
                uint16* value = reinterpret_cast<uint16*>(GetOutputSignalMemory(signalIndex));
                if (isArray) {
                    for (uint32 i = 0u; i < numberOfElements; i++) {
                        if (counter == 1.0) {
                            value[i] = 1u;
                        }
                        else {
                            value[i] += 1u;
                        }
                    }
                }
                else {
                    if (counter == 1.0) {
                        *value = 1u;
                    }
                    else {
                        *value += 1u;
                    }
                }
                //REPORT_ERROR_STATIC(ErrorManagement::Information, "Value set to %d", *reinterpret_cast<uint16*>(GetOutputSignalMemory(signalIndex)));
            }

            if (signalType == UnsignedInteger32Bit) {
                uint32* value = reinterpret_cast<uint32*>(GetOutputSignalMemory(signalIndex));
                if (isArray) {
                    for (uint32 i = 0u; i < numberOfElements; i++) {
                        if (counter == 1.0) {
                            value[i] = 1u;
                        }
                        else {
                            value[i] += 1u;
                        }
                    }
                }
                else {
                    if (counter == 1.0) {
                        *value = 1u;
                    }
                    else {
                        *value += 1u;
                    }
                }
                //REPORT_ERROR_STATIC(ErrorManagement::Information, "Value set to %d", *reinterpret_cast<uint32*>(GetOutputSignalMemory(signalIndex)));
            }

            if (signalType == UnsignedInteger64Bit) {
                uint64* value = reinterpret_cast<uint64*>(GetOutputSignalMemory(signalIndex));
                if (isArray) {
                    for (uint32 i = 0u; i < numberOfElements; i++) {
                        if (counter == 1.0) {
                            value[i] = 1u;
                        }
                        else {
                            value[i] += 1u;
                        }
                    }
                }
                else {
                    if (counter == 1.0) {
                        *value = 1u;
                    }
                    else {
                        *value += 1u;
                    }
                }
                //REPORT_ERROR_STATIC(ErrorManagement::Information, "Value set to %d", *reinterpret_cast<uint64*>(GetOutputSignalMemory(signalIndex)));
            }

            if (signalType == Float32Bit) {
                float32* value = reinterpret_cast<float32*>(GetOutputSignalMemory(signalIndex));
                if (isArray) {
                    for (uint32 i = 0u; i < numberOfElements; i++) {
                        if (counter == 1.0) {
                            value[i] = 1.0;
                        }
                        else {
                            value[i] += 1.0;
                        }
                    }
                }
                else {
                    if (counter == 1.0) {
                        *value = 1.0;
                    }
                    else {
                        *value += 1.0;
                    }
                }
                //REPORT_ERROR_STATIC(ErrorManagement::Information, "Value set to %f", *reinterpret_cast<float32*>(GetOutputSignalMemory(signalIndex)));
            }

            if (signalType == Float64Bit) {
                float64* value = reinterpret_cast<float64*>(GetOutputSignalMemory(signalIndex));
                if (isArray) {
                    for (uint32 i = 0u; i < numberOfElements; i++) {
                        if (counter == 1.0) {
                            value[i] = 1.0;
                        }
                        else {
                            value[i] += 1.0;
                        }
                    }
                }
                else {
                    if (counter == 1.0) {
                        *value = 1.0;
                    }
                    else {
                        *value += 1.0;
                    }
                }
                //REPORT_ERROR_STATIC(ErrorManagement::Information, "Value set to %f", *reinterpret_cast<float64*>(GetOutputSignalMemory(signalIndex)));
            }
        }
    }
}

CLASS_REGISTER(OPCUATestGAM, "")

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

