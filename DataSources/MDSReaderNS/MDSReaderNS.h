#ifndef DATASOURCES_MDSREADERNS_MDSREADERNS_H_
#define DATASOURCES_MDSREADERNS_MDSREADERNS_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*lint -u__cplusplus This is required as otherwise lint will get confused after including this header file.*/
#include "mdsobjects.h"
/*lint -D__cplusplus*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DataSourceI.h"
#include "MessageI.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief MDSReaderNS is a data source which allows to read data from segmented and non segmented nodes a MDSplus tree.
 * @details MDSReaderNS is an input data source which takes data from MDSPlus nodes (as many as desired) and publishes it on a real time application.
 *
 * The MDSReaderNS can either interpolate, decimate or take the raw data, as it is, from the tree depending on the parameter called "DataManagement" which is given
 * in the configuration file. This data source is intended mainly for the following two use cases:
 * <ul>
 * <li>Simulation, where the inputs are read from a pulse file.</li>
 * <li>Reference waveforms, normally defined in non segmented nodes as a signal defining few X and Y points and assuming interpolated values in between.</li>
 * </ul>
 * The timebase of MDSReaderNS data source is specified by Frequency and StartTime values. The last declared signal is the output time expressed in 
 * microseconds (supported types are int32, uint32, int64, uint64). At every cycle the the oputput time is updated as 1E6 * (StartTime + cycleCount/frequency)
 * DataManagement can take the following values:
 * <ul>
 * <li>0 --> MDSReaderNS takes the data from the tree as it is (raw). In this configuration, the frequency/numberOfElements must be the same than the node sampling frequency.</li>
 * <li>1 --> MDSReaderNS interpolates the signal taking as a reference the two nearest data values. If the frequency/numberOfElements is smaller than the sample frequency
 * of the MDSplus node the data source interpolates the signal. If the frequency/numberOfElements larger than the node sample frequency the signals is decimated.</li>
 * <li>2 --> MDSReaderNS holds the value following the criteria of the nearest value given specific time. I.e the node data is (t1, d1) = (1, 1) and (t2, d2) = (2, 5) and the currentTime is t = 1.6 the
 * nearest data to the given time is 5.</li>
 * </ul>
 *
 * The MDSReaderNS can handle as many nodes as desired. Each node can have their on data type, maximum number of segments, elements per segment and sampling time. When the
 * end of a node is reached the data of the corresponding node is filled with 0 and the data source continuous running until all nodes reach the end.
 *
 * The supported types for the nodes are:
 * <ul>
 * <li>uint8</li>
 * <li>int8</li>
 * <li>uint16</li>
 * <li>int16</li>
 * <li>uint32</li>
 * <li>int32</li>
 * <li>uint64</li>
 * <li>int64</li>
 * <li>float32</li>
 * <li>float64</li>
 * </ul>
 *
 * Nodes are assumed to be signals, i.e. they bring also timebase information. The timebases can be different from nodes to nodes, except for the raw case (0) in DataManaegment
 * where they are assumed to represent a signal at exactly the same frequency of the actual runtime timebase. 
 * The supported type for the timebase are:
 * <ul>
 * <li>uint32</li>
 * <li>int32</li>
 * <li>uint64</li>
 * <li>int64</li>
 * </ul>
 *
 *The configuration syntax is (names and signal quantity are only given as an example):
 *<pre>
 * +MDSReaderNS_0 = {
 *     Class = MDSReaderNS
 *     TreeName = "test_tree" //Compulsory. Name of the MDSplus tree.
 *     ShotNumber = 1 //Compulsory. 0 --> last shot number (to use 0 shotid.sys must exist)
 *     Frequency = 1000 // in Hz. Is the cycle time of the real time application. 
 *     StartTime = 0 // in s. Time of the first iteration.
 *
 *     Signals = {
 *         S_uint8 = {
 *             DataExpr = "S_uint8" // expression for data
 * 	       TimeExpr = "dim_of(S_uint8)" //expression for timebase
 *             NumberOfElements = 32 //Dimension of the data sample. Must be consistent with the dimensionality of the MDSplus  data item. 
 * 	       Type = uint8   //Output Data type
 *             DataManagement = 0 //could be 0, 1 or 2
 *         }
 *         ....
 *         ....
 *         ....
 *     }
 * }
 * </pre>
 */
class MDSReaderNS: public DataSourceI {
//TODO Add the macro DLL_API to the class declaration (i.e. class DLL_API MDSReaderNS)
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief default constructor
     */
MDSReaderNS    ();

    /**
     * @brief default destructor.
     */
    virtual ~MDSReaderNS();

    /**
     * @brief Copy data from the tree nodes to the dataSourceMemory
     * @details When a node does not have more data to retrieve the dataSourceMemory is filled with 0.
     * @return true if all nodes read return false.
     */
    virtual bool Synchronise();

    /**
     * @brief Reads, checks and initialises the DataSource parameters
     * @details Load from a configuration file the DataSource parameters.
     * If no errors occurs the following operations are performed:
     * <ul>
     * <li>Reads tree name </li>
     * <li>Reads the shot number </li>
     * <li>Opens the tree with the shot number </li>
     * <li>Reads the real-time thread Frequency parameter.</li>
     * </ul>
     * @param[in] data is the configuration file.
     * @return true if all parameters can be read and the values are valid
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Read, checks and initialises the Signals
     * @details If no errors occurs the following operations are performed:
     * <ul>
     * <li>Reads nodes name (could be 1 or several nodes)</li>
     * <li>Opens nodes</li>
     * <li>Gets the node type</li>
     * <li>Verifies the node type</li>
     * <li>Gets number of elements per node (or signal).
     * <li>Gets the the size of the type in bytes</li>
     * <li>Allocates memory
     * <li>Read signals and their timebases in memory
     * </ul>
     * @param[in] data is the configuration file.
     * @return true if all parameters can be read and the values are valid
     */

    virtual bool SetConfiguredDatabase(StructuredDataI & data);

    /**
     * @brief Do nothing
     * @return true
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);

    /**
     * @brief Do nothing
     * @return true
     */
    virtual bool AllocateMemory();

    /**
     * @return 1u
     */
    virtual uint32 GetNumberOfMemoryBuffers();

    /**
     * @brief Gets the signal memory buffer for the specified signal.
     * @param[in] signalIdx indicates the index of the signal to be obtained.
     * @param[in] bufferIdx indicate the index of the buffer to be obtained. Since only one buffer is allowed this parameter is always 0
     * @param[out] signalAddress is where the address of the desired signal is copied.
     */
    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
            const uint32 bufferIdx,
            void *&signalAddress);

    /**
     * @brief See DataSourceI::GetBrokerName.
     * @details Only InputSignals are supported if TimebaseMode is 0 (FixedFrequency).
     * @return MemoryMapSynchronisedInputBroker
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

    /**
     * @brief See DataSourceI::GetInputBrokers.
     * @details adds a MemoryMapSynchronisedInputBroker instance to the inputBrokers.
     */
    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
            const char8* const functionName,
            void * const gamMemPtr);

    /**
     * @brief See DataSourceI::GetOutputBrokers.
     * @return false 
     */
    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
            const char8* const functionName,
            void * const gamMemPtr);
private:
    /**
     * @brief Open MDS tree
     * @details Open the treeName and copy the pointer of the object to tree variables.
     */
    bool OpenTree();


    /**
     * @brief Gets the data node for a real time cycle.
     * @brief First determine the topology of the chunk of data to be read (i.e if there is enough data in the node, if the data has holes)
     * end then decides how to copy the data.
     * @param[in] nodeNumber node number to be copied to the dataSourceMemory.
     * @return true if node data can be copied. false if is the end of the node
     */
    bool GetDataNode(const uint32 nodeNumber);

    /**
     * @brief copy the time internally generated to the dataSourceMemory.
     */
    void PublishTime();

    /**
     * @brief Calculates values, timebase, numSamples and the (average) period
     * @return true on succeed.
     */
    bool GetNodeDataAndSamplingTime(const uint32 idx, float64 * &data, uint32 &numElements, float64 * &timebase, uint32 &numSamples,
            float64 &tDiff) const;

    /**
     * @brief Copy the same value as many times as indicated.
     * @details this function decides the type of data to copy and then calls the MDSReaderNS::CopyTheSameValue()
     * @param[in] idxNumber is the node number from which the data must be copied.
     * @param[in] numberOfTimes how many samples must be copied.
     * @param[in] samplesOffset indicates how many samples has already copied.
     */
    void CopyValue(const uint32 idxNumber, uint32 element, float64 value);

    /**
     * @brief Template functions which actually performs the copy
     * @param[in] idxNumber is the node number from which the data must be copied.
     * @param[in] numberOfTimes how many samples must be copied.
     * @param[in] samplesOffset indicates how many samples has already copied.
     */
    template<typename T>
    void CopyValueTemplate(uint32 idxNumber, uint32 element, float64 value);

    /**
     * @brief First fills a hole and then copy data from the node
     * @param[in] nodeNumber node number from where copy data.
     * @param[in] minSegment indicates the segment from where start to be copied.
     * @param[in] numberOfDiscontinuities indicates the number of times that the algorithm must be applied
     */

    /**
     * @brief Convert the the MDSplus type into MARTe type.
     */
    TypeDescriptor ConvertMDStypeToMARTeType(StreamString mdsType) const;

    bool AllNodesEnd() const;

    
    void notifySignalsEnded();
    /**
     * The name of the MDSplus tree to be opened.
     */
    StreamString treeName;

    /**
     * The MDSplus tree to be opened.
     */
    MDSplus::Tree *tree;

    /**
     * The data expressions to be managed.
     */
    StreamString *dataExpr;


    /**
     * The timebase expressions to be managed.
     */
    StreamString *timebaseExpr;


    /**
     * The number of nodes to be managed.
     */
    uint32 numberOfNodeNames;

    /**
     * The number of input signals.
     */
    uint32 nOfInputSignals;

    /**
     * The number of input on each function.
     */
    uint32 nOfInputSignalsPerFunction;


    uint32 *byteSizeSignals;

    /**
     * In SetConfiguredDatabase() the information is modified. I.e the node name is not copied because is unknown parameter for MARTe.
     */
    ConfigurationDatabase originalSignalInformation;

    /**
     * Indicates the pulse number to open. If it is not specified -1 by default.
     */
    int32 shotNumber;

    /**
     * Signal type expected to be read. It is optional parameter on the configuration file. If exist it is check against signalType.
     */
    TypeDescriptor *type;

    /**
     * Number of bytes of each signal to be read.
     */
    uint32 *bytesType;

    /**
     * Number of elements that should be read each MARTe cycle. It is read from the configuration file
     */
    uint32 *numberOfElements;

    /**
     * Data source memory.
     */
    char8 *dataSourceMemory;

    /**
     * Signals offsets in the memory.
     */
    uint32 *offsets;

    /**
     * Number of Elements for each signal.
     */
    uint32 *nElements;

     /**
     * Time in seconds. It indicates the time of each sample. At the beginning of each cycle currentTime = time.
     */
    float64 currentTime;

     /**
     * Total number of cyccles.
     */
    uint64 numCycles;

    /**
     * Time increment between Synchronisations. It is he inverse of frequency;
     */
    float64 period;

    /**
     * Real-time application frequency.
     */
    float64 frequency;


    /**
     * Start Time. Default 0
     */
    float64 startTime;


    /**
     * Management of the data. indicates what to do with the data.
     * 0 --> nothing
     * 1 --> linear interpolation
     * 2 --> hold last value.
     *
     */
    uint8 *dataManagement;

    /**
     * Management of the  hole (when there is no data stored).
     * 0 --> add 0
     * 1 --> hold last value
     */
     float64 *samplingTime;

 
    bool *endNode;
    float64 *nodeSamplingTime;
///GABRIELE    
    int timebaseMode; //Not used now
    float64 **signalData;
    float64 **signalTimebase;
    uint32 *numSignalSamples;
    /**
     * hold the last signalSample considered in output generation where the t was found. It is used for optimization since the time could not go back.
     */
    uint32 *lastSignalSample;
    /**
     * The messages to send when signals have terminated .
     */
    ReferenceT<Message> *signalsEndedMsg;
    uint32 nOfMessages;
    bool signalsEndedNotified;
    bool *useColumnOrder;

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
template<typename T>
void MDSReaderNS::CopyValueTemplate(uint32 idxNumber, uint32 element, float64 value) {
    T *ptr = reinterpret_cast<T *>(&dataSourceMemory[offsets[idxNumber]]);
    ptr[element] = value;

//std::cout << "COPY Offset: " <<  offsets[idxNumber] << std::endl; 
    
    
}



}



#endif /* DATASOURCES_MDSREADERNS_MDSREADERNS_H_ */
