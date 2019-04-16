#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "MDSReaderNS.h"
#include "MemoryMapSynchronisedInputBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*lint -estring(1960, "*MDSplus::*") -estring(1960, "*std::*") Ignore errors that do not belong to this DataSource namespace*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

MDSReaderNS::MDSReaderNS() :
        DataSourceI() {
    tree = NULL_PTR(MDSplus::Tree *);
    dataExpr = NULL_PTR(StreamString *);
    timebaseExpr = NULL_PTR(StreamString *);
    numberOfNodeNames = 0u;
    nOfInputSignals = 0u;
    nOfInputSignalsPerFunction = 0u;
    byteSizeSignals = NULL_PTR(uint32 *);
    shotNumber = 0;
    type = NULL_PTR(TypeDescriptor *);
    bytesType = NULL_PTR(uint32 *);
    numberOfElements = NULL_PTR(uint32 *);
    dataSourceMemory = NULL_PTR(char8 *);
    offsets = NULL_PTR(uint32 *);

    currentTime = 0;
    frequency = 0.0;
    period = 0.0;
    endNode = NULL_PTR(bool *);
    nodeSamplingTime = NULL_PTR(float64 *);
    
    timebaseMode = 0;
    startTime = 0.;
    signalData = NULL_PTR(float64 **);
    signalTimebase = NULL_PTR(float64 **);
    numSignalSamples = NULL_PTR(uint32 *);
    lastSignalSample = NULL_PTR(uint32 *);
    nElements = NULL_PTR(uint32 *);
    useColumnOrder = NULL_PTR(bool *);
}

/*lint -e{1551} the destructor must guarantee that the MDSplus are deleted and the shared memory freed*/
MDSReaderNS::~MDSReaderNS() {

    if (tree != NULL_PTR(MDSplus::Tree *)) {
        delete tree;
        tree = NULL_PTR(MDSplus::Tree *);
    }

    if (dataExpr != NULL_PTR(StreamString *)) {
        delete[] dataExpr;
        dataExpr = NULL_PTR(StreamString *);
    }

    if (timebaseExpr != NULL_PTR(StreamString *)) {
        delete[] timebaseExpr;
        timebaseExpr = NULL_PTR(StreamString *);
    }

    if (byteSizeSignals != NULL_PTR(uint32 *)) {
        delete[] byteSizeSignals;

    }
    if (type != NULL_PTR(TypeDescriptor *)) {
        delete[] type;

    }
    if (offsets != NULL_PTR(uint32 *)) {
        delete[] offsets;
        offsets = NULL_PTR(uint32 *);
    }
    if (dataSourceMemory != NULL_PTR(char8 *)) {
        GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(reinterpret_cast<void *&>(dataSourceMemory));
        dataSourceMemory = NULL_PTR(char8 *);
    }
    if (dataManagement != NULL_PTR(uint8 *)) {
        delete[] dataManagement;
        dataManagement = NULL_PTR(uint8 *);
    }
    if (samplingTime != NULL_PTR(float64 *)) {
        delete[] samplingTime;
        samplingTime = NULL_PTR(float64 *);
    }
    if (endNode != NULL_PTR(bool *)) {
        delete[] endNode;
        endNode = NULL_PTR(bool *);
    }
    if (numberOfElements != NULL_PTR(uint32 *)) {
        delete[] numberOfElements;
        numberOfElements = NULL_PTR(uint32 *);
    }
    if (bytesType != NULL_PTR(uint32 *)) {
        delete[] bytesType;
        bytesType = NULL_PTR(uint32 *);
    }
    if (nodeSamplingTime != NULL_PTR(float64 *)) {
        delete[] nodeSamplingTime;
        nodeSamplingTime = NULL_PTR(float64 *);
    }
    if (signalData != NULL_PTR(float64 **)) {
        delete[] signalData;
        signalData = NULL_PTR(float64 **);
    }
    if (signalTimebase != NULL_PTR(float64 **)) {
        delete[] signalTimebase;
        signalTimebase = NULL_PTR(float64 **);
    }
    if (numSignalSamples != NULL_PTR(uint32 *)) {
        delete[] numSignalSamples;
        numSignalSamples = NULL_PTR(uint32 *);
    }
    if (lastSignalSample != NULL_PTR(uint32 *)) {
        delete[] lastSignalSample;
        lastSignalSample = NULL_PTR(uint32 *);
    }
    if (useColumnOrder != NULL_PTR(bool *)) {
        delete[] useColumnOrder;
        useColumnOrder = NULL_PTR(bool *);
    }
    if (nElements != NULL_PTR(uint32 *)) {
        delete[] nElements;
        nElements = NULL_PTR(uint32 *);
    }
    
}

bool MDSReaderNS::Initialise(StructuredDataI& data) {
    bool ok = DataSourceI::Initialise(data);
    if (ok) {
        ok = data.Read("TreeName", treeName);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "TreeName shall be specified");
        }
    }
    if (ok) {
        ok = data.Read("ShotNumber", shotNumber);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::Information, "ShotNumber shall be specified");
        }
        else {
            ok = (shotNumber >= 0);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "ShotNumber shall be 0 (last available shot) or positive");
            }
        }
    }
    if (ok) {
        ok = OpenTree();
    }
    if (ok) { //read Frequency
        ok = data.Read("Frequency", frequency);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Cannot read frequency");
        }
        else {
            period = 1.0 / frequency;
        }
    }
    if (ok) { //read StartTime
        ok = data.Read("StartTime", startTime);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Cannot read StartTime");
        }
    }
    if (ok) {
        ok = data.MoveRelative("Signals");
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not move to the Signals section");
        }
        if (ok) {
            ok = data.Copy(originalSignalInformation);
        }
        if (ok) {
            ok = originalSignalInformation.MoveToRoot();
        }
        //Do not allow to add signals in run-time
        if (ok) {
            ok = signalsDatabase.MoveRelative("Signals");
        }
        if (ok) {
            ok = signalsDatabase.Write("Locked", 1u);
        }
        if (ok) {
            ok = signalsDatabase.MoveToAncestor(1u);
        }
    }

    if (ok) {
        ok = data.MoveToAncestor(1u);
    }
    
    if (ok) {
        //Check if there are any Message elements set
        if (Size() > 0u) {
            ReferenceT<ReferenceContainer> msgContainer = Get(0u);
            if (msgContainer.IsValid()) {
                uint32 j;
                nOfMessages = msgContainer->Size();
		signalsEndedMsg = new MARTe::ReferenceT<MARTe::Message>[nOfMessages];
                for (j = 0u; (j < nOfMessages) && (ok); j++) {
                    ReferenceT<Message> msg = msgContainer->Get(j);
                    ok = msg.IsValid();
                    if (ok) {
                        StreamString msgName = msg->GetName();
                        //if (msgName == "SignalsEnded") {
                            signalsEndedMsg[j] = msg;
                        //}
                        //else {
                       //     REPORT_ERROR(ErrorManagement::ParametersError, "Message %s is not supported.", msgName.Buffer());
                      //      ok = false;
                     //   }
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::ParametersError, "Found an invalid Message in container %s", msgContainer->GetName());
                        ok = false;
                    }

                }
            }
        }
    }
    return ok;
}

bool MDSReaderNS::SetConfiguredDatabase(StructuredDataI & data) {
    bool ok = DataSourceI::SetConfiguredDatabase(data);
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "DataSourceI::SetConfiguredDatabase(data) returned false");
    }
    if (ok) { // Check that only one GAM is Connected to the MDSReaderNS
        uint32 auxNumberOfFunctions = GetNumberOfFunctions();
        ok = (auxNumberOfFunctions == 1u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Exactly one Function allowed to interact with this DataSourceI. number of Functions = %u",
                         auxNumberOfFunctions);
        }
    }
    if (ok) { //read number of nodes per function numberOfNodeNames
        //0u (second argument) because previously it is checked
        ok = GetFunctionNumberOfSignals(InputSignals, 0u, nOfInputSignalsPerFunction);        //0u (second argument) because previously it is checked
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetFunctionNumberOfSignals() returned false");
        }
        if (ok) {
            ok = (nOfInputSignalsPerFunction > 1u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "The number of signals must be at least 2 (1 node and one time)");
            }
        }
    }
    if (ok) {
        nOfInputSignals = GetNumberOfSignals();
//	ok = (nOfInputSignals == nOfInputSignalsPerFunction);
	ok = (nOfInputSignals == nOfInputSignalsPerFunction || nOfInputSignals == nOfInputSignalsPerFunction+1); //Time field may be excluded
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError,
                         "nOfInputSignals %d must be equal than number %d of signals per function (since only one function can be connected to this data source)",
                         nOfInputSignals, nOfInputSignalsPerFunction);
        }
    }
    if (ok) {
        numberOfNodeNames = nOfInputSignals - 1u;
    }
    if (ok) {
        for (uint32 n = 0u; (n < nOfInputSignals) && ok; n++) {
            uint32 nSamples;
            ok = GetFunctionSignalSamples(InputSignals, 0u, n, nSamples);
            if (ok) {
                ok = (nSamples == 1u);
            }
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "The number of samples shall be exactly 1");
            }
        }
    }

    if (ok) { //read dataExpr and timebaseExpr from originalSignalInformation
        dataExpr = new StreamString[numberOfNodeNames];
        timebaseExpr = new StreamString[numberOfNodeNames];
	useColumnOrder = new bool[numberOfNodeNames];
        for (uint32 i = 0u; (i < numberOfNodeNames) && ok; i++) {
            ok = originalSignalInformation.MoveRelative(originalSignalInformation.GetChildName(i));
            if (!ok) {
                uint32 auxIdx = i;
                REPORT_ERROR(ErrorManagement::ParametersError, "Cannot move to the children %u", auxIdx);
            }
            if (ok) {
                ok = originalSignalInformation.Read("DataExpr",dataExpr[i]);
                if (!ok) {
                    uint32 auxIdx = i;
                    REPORT_ERROR(ErrorManagement::ParametersError, "Parameter DataExpr for signal %u not found", auxIdx);
                }
            }
            if (ok) {
                ok = originalSignalInformation.Read("TimebaseExpr",timebaseExpr[i]);
                if (!ok) {
                    uint32 auxIdx = i;
                    REPORT_ERROR(ErrorManagement::ParametersError, "Parameter TimebaseExpr for signal %u not found", auxIdx);
                }
            }
	    if (ok) {
		int useColumnOrderId = 0;
		ok = originalSignalInformation.Read("UseColumnOrder", useColumnOrderId);
		if (ok)
		{
		    ok = (useColumnOrderId == 0 || useColumnOrderId == 1);
		    if (!ok) {
			REPORT_ERROR(ErrorManagement::ParametersError, "UseColumnOrder shall be 0 or 1");
		    }
		}
		else
		  ok = true; //Parameter is optional
		useColumnOrder[i] = (useColumnOrderId == 1);
	    }
            if (ok) {
                ok = originalSignalInformation.MoveToAncestor(1u);
                if (!ok) { //Should never happen
                    REPORT_ERROR(ErrorManagement::ParametersError, "Cannot move to the the immediate ancestor");
                }
            }
	}
    }
    if (ok) { //read the type specified in the configuration file 
        type = new TypeDescriptor[nOfInputSignals];
        //lint -e{613} Possible use of null pointer. type previously allocated (see previous line).
        for (uint32 i = 0u; (i < numberOfNodeNames) && ok; i++) {
            type[i] = GetSignalType(i);
            ok = !(type[i] == InvalidType);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Invalid type");
            }
        }
    }
    if (ok) { //read the type of the time. It should be uin64
        //lint -e{662} [MISRA C++ 5-0-16] Possible access out-of-bounds pointer. nOfInputSignals is always 1 unit larger than numberOfNodeNames.
        //lint -e{661} [MISRA C++ 5-0-16] Possible access out-of-bounds pointer. nOfInputSignals is always 1 unit larger than numberOfNodeNames.
        if (type != NULL_PTR(TypeDescriptor *)) {
            type[numberOfNodeNames] = GetSignalType(numberOfNodeNames);
            bool cond1 = (type[numberOfNodeNames] == UnsignedInteger64Bit);
            bool cond2 = (type[numberOfNodeNames] == UnsignedInteger32Bit);
            bool cond3 = (type[numberOfNodeNames] == SignedInteger32Bit);
            bool cond4 = (type[numberOfNodeNames] == SignedInteger64Bit);
            ok = cond1 || cond2 || cond3 || cond4;
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported time type. Possible time types are: uint64, int64, uin32 or int32\n");
            }
        }
        else {
            ok = false;
        }
    }

    if (ok) { //Compute the type byte size
        bytesType = new uint32[numberOfNodeNames];
        if ((bytesType != NULL_PTR(uint32 *)) && (type != NULL_PTR(TypeDescriptor *))) {
            for (uint32 i = 0u; i < numberOfNodeNames; i++) {
                bytesType[i] = static_cast<uint32>(type[i].numberOfBits) / 8u;
            }
        }
        else {
            ok = false;
        }
    }

    if (ok) { //read number of elements
        numberOfElements = new uint32[nOfInputSignals];
        for (uint32 i = 0u; (i < numberOfNodeNames) && ok; i++) {
            ok = GetSignalNumberOfElements(i, numberOfElements[i]);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Cannot read NumberOfElements");
            }
            if (ok) {
                ok = numberOfElements[i] != 0u;
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "NumberOfElements must be positive");
                }
            }
        }
        if (ok) {
            //lint -e{661} [MISRA C++ 5-0-16] Possible access out-of-bounds. nOfInputSignals is always 1 unit larger than numberOfNodeNames.
            ok = GetSignalNumberOfElements(numberOfNodeNames, numberOfElements[numberOfNodeNames]);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Cannot read NumberOfElements");
            }
        }
        if (ok) {
            //lint -e{661} [MISRA C++ 5-0-16] Possible access out-of-bounds. nOfInputSignals is always 1 unit larger than numberOfNodeNames.
            ok = numberOfElements[numberOfNodeNames] == 1u;
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "NumberOfElements for the time must be 1");
            }
        }
    }
    //lint -e{661} [MISRA C++ 5-0-16] Possible access out-of-bounds. nOfInputSignals is always 1 unit larger than numberOfNodeNames.
    if (ok) { //Count and allocate memory for dataSourceMemory, lastValue and lastTime
        offsets = new uint32[nOfInputSignals];
        byteSizeSignals = new uint32[nOfInputSignals];

	//Count the number of bytes
        uint32 totalSignalMemory = 0u;
        uint32 sumLastValueMemory = 0u;
        if (type != NULL_PTR(TypeDescriptor *)) {
            if ((offsets != NULL_PTR(uint32 *)) && (byteSizeSignals != NULL_PTR(uint32 *))) {
                for (uint32 i = 0u; (i < numberOfNodeNames) && ok; i++) {
                    offsets[i] = totalSignalMemory;
                    uint32 nBytes = 0u;
                    ok = GetSignalByteSize(i, nBytes);

                    byteSizeSignals[i] = nBytes;
                    if (!ok) {
                        uint32 auxIdx = i;
                        REPORT_ERROR(ErrorManagement::ParametersError, "Error while GetSignalByteSize() for signal %u", auxIdx);
                    }
                    totalSignalMemory += nBytes;
                    sumLastValueMemory += static_cast<uint32>(type[i].numberOfBits) / 8u;
                }
                if (ok) { // count the time as well
                    offsets[numberOfNodeNames] = totalSignalMemory;
                    uint32 nBytes = 0u;
                    ok = GetSignalByteSize(numberOfNodeNames, nBytes);
                    byteSizeSignals[numberOfNodeNames] = nBytes;
                    if (!ok) {
                        REPORT_ERROR(ErrorManagement::ParametersError, "Error while GetSignalByteSize() for signal %u", numberOfNodeNames);
                    }
                    totalSignalMemory += nBytes;

                }
            }
            else {
                ok = false;
            }
        }
        else {
            ok = false;
        }

        //Allocate memory
        if (ok) {
            dataSourceMemory = reinterpret_cast<char8 *>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(totalSignalMemory));
        }
    }
    if (ok) { //read DataManagement from originalSignalInformation
        dataManagement = new uint8[numberOfNodeNames];
        nodeSamplingTime = new float64[numberOfNodeNames];
	signalData = new float64 *[numberOfNodeNames];
	signalTimebase = new float64 *[numberOfNodeNames];
	numSignalSamples = new uint32[numberOfNodeNames];
	lastSignalSample = new uint32[numberOfNodeNames];
	nElements = new uint32[numberOfNodeNames];
        //lint -e{613} Possible use of null pointer. The pointer usage is protected by the ok variable.
        for (uint32 i = 0u; (i < numberOfNodeNames) && ok; i++) {
            ok = originalSignalInformation.MoveRelative(originalSignalInformation.GetChildName(i));
            if (!ok) {
                uint32 auxIdx = i;
                REPORT_ERROR(ErrorManagement::ParametersError, "Cannot move to the children %u", auxIdx);
            }
            if (ok) {
                ok = originalSignalInformation.Read("DataManagement", dataManagement[i]);
                if (!ok) {
                    uint32 auxIdx = i;
                    REPORT_ERROR(ErrorManagement::ParametersError, "Parameter DataManagement for signal %u not found", auxIdx);
                }
            }
            if (ok) {
                ok = (dataManagement[i] < 3u);
                if (!ok) {
                    uint32 auxIdx = i;
                    REPORT_ERROR(
                            ErrorManagement::ParametersError,
                            "Invalid DataManagement value. It could be 0 (nothing), 1 (linear interpolation), 2 (hold last value). dataManagement[%d] = %d",
                            auxIdx, dataManagement[auxIdx]);
                }
            }
            if (ok) {
	        ok = GetNodeDataAndSamplingTime(i, signalData[i], nElements[i], signalTimebase[i],  numSignalSamples[i], nodeSamplingTime[i]);
	    }

	    std::cout << "Number of elements: " << nElements[i] << "   Number of samples: " << numSignalSamples[i] << std::endl;
	    for(int j  =0; j < 20; j++) std::cout << signalData[i][j] << "  "; std::cout<< std::endl;	    
	    
	    
	    if(ok)  {
		lastSignalSample[i] = 0;
		if(nElements[i] != numberOfElements[i])
		{
                    REPORT_ERROR(ErrorManagement::ParametersError, "Declared number of elements %d is different from actual number of elements %d  for dataExpr = %s", 
				 numberOfElements[i], nElements[i], dataExpr[i].Buffer());
		    ok = false;
		}
            }
            if (ok) { //check time of doing nothing option
                if (dataManagement[i] == 0u) {
//                    float64 nodeSamplingTime = 0.0;
                    ok = IsEqual(samplingTime[i], nodeSamplingTime[i]);
                    if (!ok) {
                        REPORT_ERROR(
                                ErrorManagement::ParametersError,
                                "the sampling time of the node %s = %.9f is different than the sampling time calculated from the parameters = %.9f and the dataManagement = 0 (do nothing)",
                                dataExpr[i].Buffer(), nodeSamplingTime[i], samplingTime[i]);
                    }
                }
            }
            if (ok) {
                ok = originalSignalInformation.MoveToAncestor(1u);
                if (!ok) { //Should never happen
                    REPORT_ERROR(ErrorManagement::ParametersError, "Cannot move to the the immediate ancestor");
                }
            }
        }
    }
    if (ok) {
        endNode = new bool[numberOfNodeNames];
        for (uint32 i = 0u; i < numberOfNodeNames; i++) {
            endNode[i] = false;
        }
    }
    numCycles = 0;
    return ok;
}

bool MDSReaderNS::Synchronise() {
    currentTime = startTime + numCycles * period;
//std::cout << "MDSReaderNS - Current time: " << currentTime << std::endl;    
    for (uint32 i = 0u; i < numberOfNodeNames; i++) {
        endNode[i] = !GetDataNode(i);
    }
    PublishTime();
    if(AllNodesEnd() && !signalsEndedNotified)
    {
	signalsEndedNotified = true;
//std::cout << "FINITO TUTTI" << std::endl;
	notifySignalsEnded();
    }
    else
    {
	if(!AllNodesEnd())
	{
	    signalsEndedNotified = false;
	}
    }
    numCycles++;
    return true;
}

void MDSReaderNS::PublishTime() {
    float64 auxFloat = currentTime * static_cast<float64>(1000000);
    if (type[numberOfNodeNames] == UnsignedInteger32Bit) {
        *reinterpret_cast<uint32 *>(&dataSourceMemory[offsets[numberOfNodeNames]]) = static_cast<uint32>(auxFloat);
    }
    else if (type[numberOfNodeNames] == SignedInteger32Bit) {
        //int32 auxInt = static_cast<int32>(auxFloat);
        //MemoryOperationsHelper::Copy(reinterpret_cast<void *>(&dataSourceMemory[offsets[numberOfNodeNames]]), static_cast<void *>(&auxInt), 4u);
        *reinterpret_cast<int32 *>(&dataSourceMemory[offsets[numberOfNodeNames]]) = static_cast<int32>(auxFloat);
    }
    else if (type[numberOfNodeNames] == UnsignedInteger64Bit) {
        uint32 auxIdx = offsets[numberOfNodeNames];
        uint64 *ptr = reinterpret_cast<uint64 *>(&(dataSourceMemory[auxIdx]));
        *ptr = static_cast<uint64>(auxFloat);
    }
    else if (type[numberOfNodeNames] == SignedInteger64Bit) {
        uint32 auxIdx = offsets[numberOfNodeNames];
        char8 *ptr2 = &dataSourceMemory[auxIdx];
        int64 *ptr = reinterpret_cast<int64 *>(ptr2);
        *ptr = static_cast<int64>(auxFloat);
    }
    else {

    }
    return;
}

bool MDSReaderNS::AllNodesEnd() const {
    bool ret = true;
    if (endNode != NULL_PTR(bool *)) {
        for (uint32 i = 0u; (i < numberOfNodeNames) && ret; i++) {
            ret = endNode[i];
        }
    }
    return ret;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: NOOP at StateChange, independently of the function parameters.*/
bool MDSReaderNS::PrepareNextState(const char8 * const currentStateName,
                                 const char8 * const nextStateName) {
    return true;
}

bool MDSReaderNS::AllocateMemory() {
    return true;
}

uint32 MDSReaderNS::GetNumberOfMemoryBuffers() {
    return 1u;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The signalAddress is independent of the bufferIdx.*/
bool MDSReaderNS::GetSignalMemoryBuffer(const uint32 signalIdx,
                                      const uint32 bufferIdx,
                                      void *&signalAddress) {
    bool ok = (dataSourceMemory != NULL_PTR(char8 *));
    if (!ok) {
        REPORT_ERROR(ErrorManagement::FatalError, "Memory not allocated");
    }
    if (ok) {
        /*lint -e{613} dataSourceMemory cannot be NULL here*/
        char8 *memPtr = &dataSourceMemory[offsets[signalIdx]];
        signalAddress = reinterpret_cast<void *&>(memPtr);
    }
    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The brokerName only depends on the direction*/
const char8 *MDSReaderNS::GetBrokerName(StructuredDataI &data,
                                      const SignalDirection direction) {
    const char8* brokerName = "";
    if (direction == InputSignals) {
        brokerName = "MemoryMapSynchronisedInputBroker";
    }
    return brokerName;
}

bool MDSReaderNS::GetInputBrokers(ReferenceContainer &inputBrokers,
                                const char8* const functionName,
                                void * const gamMemPtr) {
    bool ok;
    ReferenceT<MemoryMapSynchronisedInputBroker> broker("MemoryMapSynchronisedInputBroker");
    ok = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    if (ok) {
        ok = inputBrokers.Insert(broker);
    }
    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: InputBrokers are not supported. Function returns false irrespectively of the parameters.*/
bool MDSReaderNS::GetOutputBrokers(ReferenceContainer &outputBrokers,
                                 const char8* const functionName,
                                 void * const gamMemPtr) {
    return false;
}

bool MDSReaderNS::OpenTree() {
    bool ret = true;
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), shotNumber);
    }
    catch (const MDSplus::MdsException &exc) {
        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Fail opening tree %s with shotNumber %d:  %s", treeName.Buffer(), shotNumber, exc.what());
        ret = false;
    }
    return ret;
}





/**
 * -1--> end data (not found)
 * 0--> is not end of data but not found. Case signal trigger (the segment are not continuous)
 * 1--> time found in a segment
 */

bool MDSReaderNS::GetDataNode(const uint32 nodeNumber) {
  
//  std::cout << "GET DATA NODE " << nodeNumber << "  nElements: " << nElements[nodeNumber] << "Management: " << (int)dataManagement[nodeNumber] <<std::endl;  
  
    switch(dataManagement[nodeNumber])  {
	case 0: //Data "As Is"
	      if(lastSignalSample[nodeNumber] < numSignalSamples[nodeNumber])
	      {
		  for(uint32 i = 0; i < nElements[nodeNumber]; i++)
		  {
		      CopyValue(nodeNumber, i, signalData[nodeNumber][lastSignalSample[nodeNumber]*nElements[nodeNumber]+i]);
		  }
		  (lastSignalSample[nodeNumber])++;
		  return true;
	      }
	      else
	      {
		  for(uint32 i = 0; i < nElements[nodeNumber]; i++)
		      CopyValue(nodeNumber, i, 0.);
		  return false;
	      }
	case 1: //Interpolation
	{
	    uint32 startIdx;
//std::cout << "signal timebase 0: " << signalTimebase[nodeNumber][0] << " num signal samples" << numSignalSamples[nodeNumber] << "  current time: " << currentTime << std::endl;
	    if(signalTimebase[nodeNumber][0] > currentTime)
	    {
		for(uint32 i = 0; i < nElements[nodeNumber]; i++)
		{
		    CopyValue(nodeNumber, i, 0.);
		}
		return true;
	    }
	    if(signalTimebase[nodeNumber][numSignalSamples[nodeNumber]-1] < currentTime)
	    {
		for(uint32 i = 0; i < nElements[nodeNumber]; i++)
		{
		    CopyValue(nodeNumber, i, 0.);
		}
		return false;
	    }
	    for(startIdx = lastSignalSample[nodeNumber]; startIdx < numSignalSamples[nodeNumber]-1 && signalTimebase[nodeNumber][startIdx+1] < currentTime; startIdx++);
	    for(uint32 i = 0; i < nElements[nodeNumber]; i++)
	    {
		float64 interpValue = signalData[nodeNumber][startIdx*nElements[nodeNumber]+i] + (currentTime - signalTimebase[nodeNumber][startIdx])*
		(signalData[nodeNumber][(startIdx+1)*nElements[nodeNumber]+i] - signalData[nodeNumber][startIdx*nElements[nodeNumber]+i])/(signalTimebase[nodeNumber][startIdx+1] - signalTimebase[nodeNumber][startIdx]);
		CopyValue(nodeNumber, i, interpValue);
// std::cout << "startIdx: " << startIdx << "   Current time: " << currentTime << "Current Index: " << startIdx*nElements[nodeNumber]+i << 
// "Val1: "<< signalData[nodeNumber][startIdx*nElements[nodeNumber]+i] <<
// "Interp Val: " << interpValue <<std::endl;
	    }
	    lastSignalSample[nodeNumber] = startIdx;
	    return true;
	}
	case 2: //Closest Sample
	{
	    uint32 startIdx;
	    if(lastSignalSample[nodeNumber] == 0 && signalTimebase[nodeNumber][lastSignalSample[nodeNumber]] > currentTime)
	    {
		for(uint32 i = 0; i < nElements[nodeNumber]; i++)
		{
		    CopyValue(nodeNumber, i, 0.);
		}
		return true;
	    }
	    if(lastSignalSample[nodeNumber] >= numSignalSamples[nodeNumber] - 1 && signalTimebase[nodeNumber][lastSignalSample[nodeNumber]] < currentTime)
	    {
		for(uint32 i = 0; i < nElements[nodeNumber]; i++)
		{
		    CopyValue(nodeNumber, i, 0.);
		}
		return false;
	    }
	    for(startIdx = lastSignalSample[nodeNumber]; startIdx < numSignalSamples[nodeNumber]-1 && signalTimebase[nodeNumber][startIdx+1] <= currentTime; startIdx++);
	    if((currentTime - signalTimebase[nodeNumber][startIdx]) < (signalTimebase[nodeNumber][startIdx+1] - currentTime))
	    {
		for(uint32 i = 0; i < nElements[nodeNumber]; i++)
		{
		  CopyValue(nodeNumber, i, signalData[nodeNumber][startIdx*nElements[nodeNumber]+i]);
		}
	    }
	    else
	    {
		if(startIdx < numSignalSamples[nodeNumber]-1)
		{
		    for(uint32 i = 0; i < nElements[nodeNumber]; i++)
		    {
			CopyValue(nodeNumber, i, signalData[nodeNumber][(startIdx+1)*nElements[nodeNumber]+i]);
		    }
		}
		else
		{
		    for(uint32 i = 0; i < nElements[nodeNumber]; i++)
		    {
		      CopyValue(nodeNumber, i, signalData[nodeNumber][startIdx*nElements[nodeNumber]+i]);
		    }
		}
	    }
	    lastSignalSample[nodeNumber] = startIdx;
	    return true;
	}
	default: return false; //Never happens
    }
}
	
    
 
//lint -e{613} Possible use of null pointer. Not possible. If initialisation fails this function is not called.
void MDSReaderNS::CopyValue(const uint32 idxNumber, uint32 element, float64 value) {

    if (type[idxNumber] == UnsignedInteger8Bit) {
        CopyValueTemplate<uint8>(idxNumber, element, value);
    }
    else if (type[idxNumber] == SignedInteger8Bit) {
        CopyValueTemplate<int8>(idxNumber, element, value);
    }
    else if (type[idxNumber] == UnsignedInteger16Bit) {
        CopyValueTemplate<uint16>(idxNumber, element, value);
    }
    else if (type[idxNumber] == SignedInteger16Bit) {
        CopyValueTemplate<int16>(idxNumber, element, value);
    }
    else if (type[idxNumber] == UnsignedInteger32Bit) {
        CopyValueTemplate<uint32>(idxNumber, element, value);
    }
    else if (type[idxNumber] == SignedInteger32Bit) {
        CopyValueTemplate<int32>(idxNumber, element, value);
    }
    else if (type[idxNumber] == UnsignedInteger64Bit) {
        CopyValueTemplate<uint64>(idxNumber, element, value);
    }
    else if (type[idxNumber] == SignedInteger64Bit) {
        CopyValueTemplate<int64>(idxNumber, element, value);
    }
    else if (type[idxNumber] == Float32Bit) {
        CopyValueTemplate<float32>(idxNumber, element, value);
    }
    else if (type[idxNumber] == Float64Bit) {
        CopyValueTemplate<float64>(idxNumber, element, value);
    }
    else {

    }
    return;
}


bool MDSReaderNS::GetNodeDataAndSamplingTime(const uint32 idx, float64 * &data, uint32 &numElements, float64 * &timebase, uint32 &numSamples,
            float64 &tDiff) const
{
    int nDims;
    try {
        MDSplus::Data *nodeData = MDSplus::compile(dataExpr[idx].Buffer(), tree);
	int dataSamples;
	MDSplus::Data *evalData = nodeData->data();
//std::cout << "Data: " << nodeData << std::endl;
//std::cout << "EvalData: " << evalData << std::endl;
	MDSplus::deleteData(nodeData);
	int *shape = evalData->getShape(&nDims);
std::cout << "GET NODE DATA  " << idx << std::endl; 
	if(useColumnOrder[idx])
	{
std::cout << "USE COLUMN ORDER" << std::endl;
	  
	    numSamples = shape[nDims-1];
	    numElements = 1;
	    for(int i = 0; i < nDims-1; i++)
		numElements *= shape[i];

//std::cout << "NumElements: " << numElements << std::endl;
//std::cout << "NumSamples: " << numSamples << std::endl;
	    float64 *currData = evalData->getDoubleArray(&dataSamples);
	    data = new float64[dataSamples];
	    for(uint32 i = 0; i < numSamples; i++)
	    {
		for(uint32 j = 0; j < numElements; j++)
		{
		    data[i*numElements + j] = currData[j*numSamples+i];
		}
	    }
	    delete [] currData;
	}
	else
	{
	    numSamples = shape[0];
	    numElements = 1;
	    for(int i = 1; i < nDims; i++)
		numElements *= shape[i];
	    data = evalData->getDoubleArray(&dataSamples);
	}
         MDSplus::deleteData(evalData);
    }catch(MDSplus::MdsException &exc)
    {
	REPORT_ERROR(ErrorManagement::ParametersError, "Cannot read data for signal %s: %s", dataExpr[idx].Buffer(), exc.what());
	return false;
    }
    try {
        int dimSamples;
         MDSplus::Data *nodeTimebase = MDSplus::compile(timebaseExpr[idx].Buffer(), tree);
	 MDSplus::Data *dataTimebase = nodeTimebase->data();
        timebase = dataTimebase->getDoubleArray(&dimSamples);
//std::cout << "Timebase: " << dataTimebase << std::endl;
        MDSplus::deleteData(dataTimebase);
        MDSplus::deleteData(nodeTimebase);
        if((uint32)dimSamples < numSamples)
	    numSamples = dimSamples;
	tDiff = (timebase[dimSamples - 1] - timebase[0])/dimSamples;
	return true;
    }catch(MDSplus::MdsException &exc)
    {
	REPORT_ERROR(ErrorManagement::ParametersError, "Cannot read timebase for signal %s: %s", timebaseExpr[idx].Buffer(), exc.what());
	return false;
    }
}
    
void MDSReaderNS::notifySignalsEnded()
{
    for(uint32 i = 0; i < nOfMessages; i++)
    {
        if(signalsEndedMsg[i].IsValid())
	{
            if (!MessageI::SendMessage(signalsEndedMsg[i], this)) {
                StreamString destination = signalsEndedMsg[i]->GetDestination();
                StreamString function = signalsEndedMsg[i]->GetFunction();
                REPORT_ERROR(ErrorManagement::FatalError, "Could not send TreeOpenedOK message to %s [%s]", destination.Buffer(), function.Buffer());
            }
        }
    }
}




CLASS_REGISTER(MDSReaderNS, "1.0")
}
