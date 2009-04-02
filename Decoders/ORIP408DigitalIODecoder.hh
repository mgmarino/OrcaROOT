// ORIP408DigitalIODecoder.hh
/*
*/
#ifndef _ORIP408DigitalIODecoder_hh_
#define _ORIP408DigitalIODecoder_hh_

#include "ORVBasicTreeDecoder.hh"
#include "ORVIndustryPakDecoder.hh"

//! Class for decoding the IP 408 Digital IO board.
/*!i 
 *  The 408 Digital IO essentially acts like a set of switches.
 *  This record records those values (+ write/read mask) coming from
 *  the card.
 */
class ORIP408DigitalIODecoder: public ORVBasicTreeDecoder, public ORVIndustryPakDecoder
{
  public:
    ORIP408DigitalIODecoder() {}
    virtual ~ORIP408DigitalIODecoder() {}
    
    //! Returns Write Mask  
    virtual UInt_t WriteMaskOf( UInt_t* record )
      { return ( record[3] ); }
    
    //! Returns Read Mask  
    virtual UInt_t ReadMaskOf( UInt_t* record )
      { return ( record[4] ); }
    
    //! Returns Write Value  
    virtual UInt_t WriteValueOf( UInt_t* record )
      { return ( record[5] ); }
    
    //! Returns Read Value  
    virtual UInt_t ReadValueOf( UInt_t* record )
      { return ( record[6] ); }

    /*!
       Parameters are as follows:

        - Crate
        - Card
        - IP Slot
        - Time
        - Write Mask
        - Read Mask
        - Write Value
        - Read Value
     */
    virtual size_t GetNPars() { return 8; }
    virtual std::string GetParName( size_t iPar );
    virtual UInt_t GetPar(UInt_t* record, size_t iPar, size_t /*iRow*/);
    virtual std::string GetDataObjectPath() 
      { return "IP408:IP408Values"; }  
    
};

#endif
