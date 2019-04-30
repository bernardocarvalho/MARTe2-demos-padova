/**
 * @file SimpleUDPSender.h
 * @brief Header file for class SimpleUDPSender
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

 * @details This header file contains the declaration of the class SimpleUDPSender
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EXAMPLES_CORE_SIMPLEUDPSENDER_H_
#define EXAMPLES_CORE_SIMPLEUDPSENDER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "DataSourceI.h"
#include "UDPSocket.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe2Tutorial {
/**
 * @brief An example of a DataSource which streams UDP packets.
 *
 * The configuration syntax is (names and types are only given as an example):
 *
 * +UDPSender = {
 *     Class = SimpleUDPSender
 *     Port = 8888
 *     Address = localhost
 *     Signals = {
 *         Signal1 = {
 *             Type = uint32
 *         }
 *         Signal2 = {
 *             Type = float64
 *         }
 *     }
 * }
 */
class SimpleUDPSender : public MARTe::DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. NOOP.
     */
    SimpleUDPSender();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~SimpleUDPSender();

    /**
     * @brief Reads the Port and the Address from the configuration file.
     * @param[in] data see DataSourceI::Initialise. 
     * @return true if the Port and the Address are set.
     */
    virtual bool Initialise(MARTe::StructuredDataI & data);

    /**
     * @brief Sends the UDP packet.
     */
    virtual bool Synchronise();

    /**
     * @brief Allocates the packet memory to be sent.
     * @return true.
     */
    virtual bool AllocateMemory();

    /**
     * @brief Returns the address of the signal in the packet memory.
     */
    virtual bool GetSignalMemoryBuffer(const MARTe::uint32 signalIdx, const MARTe::uint32 bufferIdx, void *&signalAddress);

    /**
     * @brief Returns MemoryMapSynchronisedOutputBroker.
     */
    virtual const MARTe::char8 *GetBrokerName(MARTe::StructuredDataI &data, const MARTe::SignalDirection direction);

    /**
     * @brief NOOP
     */
    virtual bool PrepareNextState(const MARTe::char8 * const currentStateName, const MARTe::char8 * const nextStateName);

    /**
     * @brief Computes the signal sizes and location in the payload.
     */
    virtual bool SetConfiguredDatabase(MARTe::StructuredDataI & data);

private:

    /**
     * The total memory size.
     */
    MARTe::uint32 totalMemorySize;

    /**
     * The payload to be sent.
     */
    MARTe::char8 *payload;
  
    /**
     * The location of the signals in the payload.
     */
    MARTe::uint32 *offsets; 

    /**
     * UDPSocket
     */
   MARTe::UDPSocket udpSocket;

};
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EXAMPLES_CORE_SIMPLEUDPSENDER_H_ */

