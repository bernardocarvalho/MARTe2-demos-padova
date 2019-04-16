/**
 * @file FileWriter.h
 * @brief Header file for class FileWriter
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

 * @details This header file contains the declaration of the class FileWriter
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STREAMOUT_H_
#define STREAMOUT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DataSourceI.h"
#include "ProcessorType.h"
#include "MemoryMapSynchronisedOutputBroker.h"
#include "MessageI.h"
#include "RegisteredMethodsMessageFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief A DataSourceI interface which allows to store signals in a file.
 *
 * @details Data written into this data source is temporarily stored in a circular buffer and
 * asynchronously flushed to a file either as text (csv) or binary. This circular buffer can either be
 * continuously stored or stored only when a given event occurs (see StoreOnTrigger below).
 *
 * All the signals are stored in a single file.
 * If the format is csv the first line will be a comment with each signal name, type and number of elements.
 * e.g."#Trigger (uint8)[1];Time (uint32)[1];SignalUInt8 (uint8)[1];SignalUInt16 (uint16)[4]", where ; is the CSVSeparator.
 * A new line will be added every time all the signal samples are written.
 *
 * If the format is binary an header with the following information is created: the first 4 bytes
 * contain the number of signals. Then, for each signal, the signal type will be encoded in two bytes, followed
 *  by 32 bytes to encode the signal name, followed by 4 bytes which store the number of elements of a given signal.
 *  Following the header the signal samples are consecutively stored in binary format.
 *
 * */
class StreamOut: public DataSourceI, public MessageI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor.
     * @details Initialises all the optional parameters as described in the class description.
     * Registers the RPC FlushFile, CloseFile and OpenFile callback functions.
     */
    StreamOut();

    /**
     * @brief Destructor.
     * @details Flushes the file and frees the circular buffer.
     */
    virtual ~StreamOut();

    /**
     * @brief See DataSourceI::AllocateMemory. NOOP.
     * @return true.
     */
    virtual bool AllocateMemory();

    /**
     * @brief See DataSourceI::GetNumberOfMemoryBuffers.
     * @return 1.
     */
    virtual uint32 GetNumberOfMemoryBuffers();

    /**
     * @brief See DataSourceI::GetSignalMemoryBuffer.
     * @pre
     *   SetConfiguredDatabase
     */
    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
            const uint32 bufferIdx,
            void *&signalAddress);

    /**
     * @brief See DataSourceI::GetBrokerName.
     * @details Only OutputSignals are supported.
     * @return MemoryMapAsyncOutputBroker if storeOnTrigger == 0, MemoryMapAsyncTriggerOutputBroker otherwise.
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

    /**
     * @brief See DataSourceI::GetInputBrokers.
     * @return false.
     */
    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
            const char8* const functionName,
            void * const gamMemPtr);

    /**
     * @brief See DataSourceI::GetOutputBrokers.
     * @details If storeOnTrigger == 0 it adds a MemoryMapAsyncOutputBroker instance to
     *  the inputBrokers, otherwise it adds a MemoryMapAsyncTriggerOutputBroker instance to the outputBrokers.
     * @pre
     *   GetNumberOfFunctions() == 1u
     */
    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
            const char8* const functionName,
            void * const gamMemPtr);

    /**
     * @brief Writes the buffer data into the specified file in the specified format.
     * @return true if the data can be successfully written into the file.
     */
    virtual bool Synchronise();

    /**
     * @brief See DataSourceI::PrepareNextState. NOOP.
     * @return true.
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);

    /**
     * @brief Loads and verifies the configuration parameters detailed in the class description.
     * @return true if all the mandatory parameters are correctly specified and if the specified optional parameters have valid values.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Final verification of all the parameters and opening of the file.
     * @details This method verifies that all the parameters (e.g. number of samples) requested by the GAMs interacting with this DataSource
     *  are valid and consistent with the parameters set during the initialisation phase.
     * In particular the following conditions shall be met:
     * - If relevant, the Trigger signal shall have type uint8
     * - The number of samples of all the signals is one.
     * - At least one signal (apart from the eventual Trigger signal) is set.
     * @return true if all the parameters are valid and if the file can be successfully opened.
     */
    virtual bool SetConfiguredDatabase(StructuredDataI & data);

    /**
     * @brief Flushes the file.
     * @return true if the file can be successfully flushed.
     */
    virtual bool IsSupportedBroker(const SignalDirection direction, const uint32 functionIdx, 
				   const uint32 functionSignalIdx, const char8* const brokerClassName);

    /**
     * @brief Gets the number of buffers in the circular buffer.
     * @return the number of buffers in the circular buffer.
     */
    uint32 GetNumberOfBuffers() const;

    /**
     * @brief Gets the number of post configured buffers in the circular buffer.
     * @return the number of post configured buffers in the circular buffer.
     */
    ErrorManagement::ErrorType sayHello(const int32 param);
    
    ReferenceT<RegisteredMethodsMessageFilter> filter;

     /**
     * The message to send if the Tree is successfully opened.
     */
    ReferenceT<Message> outStepMsg;
   
private:

    /**
     * Offset of each signal in the dataSourceMemory
     */
    uint32 *offsets;

    /**
     * Memory holding all the signals that are to be stored, for each cycle, in the output file.
     */
    char8 *dataSourceMemory;


    /**
     * The asynchronous triggered broker that provides the interface between the GAMs and the output file.
     */
    MemoryMapSynchronisedOutputBroker *broker;
    uint32 nOfSignals; 
    uint64 counter;
    uint32 eventDivision; //If not defined, it is set to 1
    uint32 bufSamples;
    uint32 bufIdx;
    uint32 numChannels;
    float32 **streamBuffers;
    uint32 shotNumber;
    uint32 timeIdx; //Index of time input signal
};
}


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* STREAMOUT_H_ */
	
