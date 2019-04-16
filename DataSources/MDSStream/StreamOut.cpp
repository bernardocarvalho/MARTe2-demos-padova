/**
 * @file ConsoleOut.cpp
 * @brief Source file for class ConsoleOut
 * @date 11/08/2017
 * @author Andre' Neto
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
 * the class ConsoleOut (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "CLASSMETHODREGISTER.h"
#include "StreamOut.h"
#include "MemoryMapSynchronisedOutputBroker.h"
#include <mdsobjects.h>

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
static const int32 FILE_FORMAT_BINARY = 1;
static const int32 FILE_FORMAT_CSV = 2;

StreamOut::StreamOut() :
        DataSourceI(),
        MessageI() {
	dataSourceMemory = NULL_PTR(char8 *);
	offsets = NULL_PTR(uint32 *);
	broker = NULL_PTR(MemoryMapSynchronisedOutputBroker *);
	streamBuffers = NULL_PTR(float32 **);
	counter = 0;
	shotNumber = 0;
	nOfSignals = 0;
}

/*lint -e{1551} -e{1579} the destructor must guarantee that the memory is freed and the file is flushed and closed.. The brokerAsyncTrigger is freed by the ReferenceT */
StreamOut::~StreamOut() {
    if (dataSourceMemory != NULL_PTR(char8 *)) {
        GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(reinterpret_cast<void *&>(dataSourceMemory));
    }
    if (offsets != NULL_PTR(uint32 *)) {
        GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(reinterpret_cast<void *& >(offsets));
    }
    if (streamBuffers != NULL_PTR(float32 **))
    {
      for(uint32 i = 0; i < nOfSignals; i++) {
        GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(reinterpret_cast<void *& > (streamBuffers[i]));
      }
      GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(reinterpret_cast<void *& >(streamBuffers));
    }
    
}

bool StreamOut::AllocateMemory() {
    return true;
}

uint32 StreamOut::GetNumberOfMemoryBuffers() {
    return 1u;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The signalAddress is independent of the bufferIdx.*/
bool StreamOut::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void*& signalAddress) {
    bool ok = (dataSourceMemory != NULL_PTR(char8 *));
    if (ok) {
        /*lint -e{613} dataSourceMemory cannot be NULL here*/
        char8 *memPtr = &dataSourceMemory[offsets[signalIdx]];
        signalAddress = reinterpret_cast<void *&>(memPtr);
    }
    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The brokerName only depends on the direction and on the storeOnTrigger property (which is load before).*/
const char8* StreamOut::GetBrokerName(StructuredDataI& data, const SignalDirection direction) {
    const char8* brokerName = "";
    if (direction == OutputSignals) {
            brokerName = "MemoryMapSynchronisedOutputBroker";
    }
    return brokerName;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: InputBrokers are not supported. Function returns false irrespectively of the parameters.*/
bool StreamOut::GetInputBrokers(ReferenceContainer& inputBrokers, const char8* const functionName, void* const gamMemPtr) {
    return false;
}

bool StreamOut::IsSupportedBroker(const SignalDirection direction, const uint32 functionIdx, const uint32 functionSignalIdx, const char8* const brokerClassName)
{
     return true;
}

bool StreamOut::GetOutputBrokers(ReferenceContainer& outputBrokers, const char8* const functionName, void* const gamMemPtr) {
  
    bool ok = true;
    ReferenceT<MemoryMapSynchronisedOutputBroker> brokerSynch("MemoryMapSynchronisedOutputBroker");
    ok = brokerSynch.IsValid();
    if (ok) {
	ok = brokerSynch->Init(OutputSignals, *this, functionName, gamMemPtr);
    }
    if (ok) {
	ok = outputBrokers.Insert(brokerSynch);
    }
   return ok;
}

bool StreamOut::Synchronise() {
    bool ok = true;
    uint32 n;
    uint32 nOfSignals = GetNumberOfSignals();
    for (n = 0u; (n < nOfSignals) && (ok); n++) {
	TypeDescriptor type = GetSignalType(n);
	if(type == Float32Bit)
	  streamBuffers[n][bufIdx] = *reinterpret_cast<float32 *>(&dataSourceMemory[offsets[n]]);
	else if (type == Float64Bit)
	{
	  streamBuffers[n][bufIdx] = *reinterpret_cast<float64 *>(&dataSourceMemory[offsets[n]]);
	}
	else //uint32
	{
	  streamBuffers[n][bufIdx] = *reinterpret_cast<int32 *>(&dataSourceMemory[offsets[n]]);
	}
    }
    bufIdx = (bufIdx + 1)%bufSamples;
    counter++;
    if(bufIdx == 0)
    {
      float32 times[bufSamples];
      for(uint i = 0; i < bufSamples; i++)
      times[i] = streamBuffers[timeIdx][i]/1E6;
       
       for (n = 0u; (n < nOfSignals) && (ok); n++) {
	  if(n != timeIdx)
	  {
            StreamString signalName;
            ok = GetSignalName(n, signalName);
	    if (ok) MDSplus::EventStream::send(shotNumber, signalName.Buffer(), bufSamples, times, streamBuffers[n]);
	  }
       }
    }
    return ok;
}
 
 
 
 
/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: NOOP at StateChange, independently of the function parameters.*/
bool StreamOut::PrepareNextState(const char8* const currentStateName, const char8* const nextStateName) {
    return true;
}

bool StreamOut::Initialise(StructuredDataI& data) {
    bool ok = DataSourceI::Initialise(data);
    if (ok) {
        ok = data.Read("ShotNumber", shotNumber);
	if(!ok)
	{
	  shotNumber = 0;
	  ok = true;
	}
    }
    if(ok) {
        ok = data.Read("TimeIdx", timeIdx);
       if (!ok) {
            REPORT_ERROR(ErrorManagement::Information, "TimeIdx (index of tiem input) shall be specified");
        }
    }
    if(ok)
    {
      ok = data.Read("EventDivision", eventDivision);
      if(ok)
      {
	bufSamples = eventDivision;
	if(bufSamples == 0)
	{
	  bufSamples = 1;
	}
      }
      else
      {
	bufSamples = 1;
	ok = true;
      }
    }
    return ok;
}

bool StreamOut::SetConfiguredDatabase(StructuredDataI& data) {
    bool ok = DataSourceI::SetConfiguredDatabase(data);
    //Check signal properties and compute memory
    uint32 nOfSignals = 0u;
    
    if (ok) { // Check that only one GAM is Connected to the MDSReaderNS
        uint32 auxNumberOfFunctions = GetNumberOfFunctions();
        ok = (auxNumberOfFunctions == 1u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Exactly one Function allowed to interact with this StreamOut DataSource. Number of Functions = %u",
                         auxNumberOfFunctions);
        }
    }
    if (ok) { //read number of nodes per function numberOfNodeNames
        //0u (second argument) because previously it is checked
        ok = GetFunctionNumberOfSignals(OutputSignals, 0u, nOfSignals);        //0u (second argument) because previously it is checked
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetFunctionNumberOfSignals() returned false");
        }
        if (ok) {
            ok = (nOfSignals > 1u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "The number of signals must be at least 2");
            }
        }
    }
    if (ok) {
        uint32 nOfInputSignals = GetNumberOfSignals();
	
	ok = (nOfInputSignals == nOfSignals); 
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError,
                         "nOfInputSignals %d must be equal than number %d of signals per function (since only one function can be connected to this data source)",
                         nOfInputSignals, nOfSignals);
        }
    }
    if(ok)
    {
      streamBuffers = reinterpret_cast<float32 **>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(nOfSignals * sizeof(float32 *)));
      for(uint32 i = 0; i < nOfSignals; i++)
	streamBuffers[i] = reinterpret_cast<float32 *>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(bufSamples * sizeof(float32)));
    }
    if (ok) {
        for (uint32 n = 0u; (n < nOfSignals) && ok; n++) {
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
    TypeDescriptor *type = NULL_PTR(TypeDescriptor *);
    if (ok) { //read the type specified in the configuration file 
        type = new TypeDescriptor[nOfSignals];
        //lint -e{613} Possible use of null pointer. type previously allocated (see previous line).
        for (uint32 i = 0u; (i < nOfSignals) && ok; i++) {
            type[i] = GetSignalType(i);
            ok = !(type[i] == InvalidType);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Invalid type");
            }
	    if (ok) { //read the type of the time. It should be uin64
	      bool cond1 = (type[i] == UnsignedInteger64Bit);
	      bool cond2 = (type[i] == UnsignedInteger32Bit);
	      bool cond3 = (type[i] == SignedInteger32Bit);
	      bool cond4 = (type[i] == SignedInteger64Bit);
	      bool cond5 = (type[i] == Float32Bit);
	      bool cond6 = (type[i] == Float64Bit);
	      ok = cond1 || cond2 || cond3 || cond4 || cond5 || cond6;
	      if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported time type. Possible time types are: uint64, int64, uin32 or int32\n");
	      }
	    }
	    else {
	      ok = false;
	    }
	}
    }
    if (ok) { //check number of elements
      for (uint32 i = 0u; (i < nOfSignals) && ok; i++) {
	uint32 numberOfElements;
	ok = GetSignalNumberOfElements(i, numberOfElements);
        if (!ok) {
          REPORT_ERROR(ErrorManagement::ParametersError, "Cannot read NumberOfElements");
        }
        if (ok) {
          ok = numberOfElements == 1u;
          if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "NumberOfElements for the time must be 1");
          }
	}
      }
    }
    //lint -e{661} [MISRA C++ 5-0-16] Possible access out-of-bounds. nOfSignals is always 1 unit larger than numberOfNodeNames.
    if (ok) { //Count and allocate memory for dataSourceMemory, lastValue and lastTime
        offsets = new uint32[nOfSignals];
	//Count the number of bytes
        uint32 totalSignalMemory = 0u;
        if (type != NULL_PTR(TypeDescriptor *)) {
            if ((offsets != NULL_PTR(uint32 *)) ) {
	      
                for (uint32 i = 0u; (i < nOfSignals) && ok; i++) {
                    if (ok) { // count the time as well
		      offsets[i] = totalSignalMemory;
		      uint32 nBytes = 0u;
		      ok = GetSignalByteSize(i, nBytes);
		      if (!ok) {
                        REPORT_ERROR(ErrorManagement::ParametersError, "Error while GetSignalByteSize() for signal %u", i);
		      }
		      totalSignalMemory += nBytes;
		    }
                }
            }
            else {
                ok = false;
	    }
	}
        else {
          ok = false;
	  }
	delete [] type;
        //Allocate memory
	if (ok) {
	   dataSourceMemory = reinterpret_cast<char8 *>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(totalSignalMemory));
	}
    }
    bufIdx = 0;
    return ok;
}




uint32 StreamOut::GetNumberOfBuffers() const {
    return 1;
}


CLASS_REGISTER(StreamOut, "1.0")
}

