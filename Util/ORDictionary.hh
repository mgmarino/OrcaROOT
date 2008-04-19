// ORDictionary.hh

#ifndef _ORDictionary_hh_
#define _ORDictionary_hh_

#include <string>
#include <map>
#include <vector>

//! Dictionary type base class
/*!
  Provides a base class for a dictionary type data
  structure.
 */
class ORVDictValue 
{
  public:
    //! Allowable types
    enum EValType { kDict, kString, kReal, kInt, kBool, kArray };
    ORVDictValue() {}
    virtual ~ORVDictValue() {}

    virtual EValType GetValueType() const = 0;
    virtual bool IsA(EValType valType) const 
      { return valType == GetValueType(); }

    //! Returns a string of the value.
    /*!
        For example, for the number 1, it would
        return a string "1".  This is useful for 
        generating a string output of a dictionary.
     */ 
    virtual std::string GetStringOfValue() const = 0;

    //! overload this for dict, array
    virtual size_t GetNValues() const { return 1; } 
};

//! True dictionary class
/*!
    Stores ORVDictValue data types accoring to a string
    key.
 */
class ORDictionary : public ORVDictValue
{
  public:
    ORDictionary(std::string name = "") 
      { SetName(name);}

    //! Copy constructor to handle making a new dictionary from another.
    ORDictionary(const ORDictionary& dict);
    virtual ~ORDictionary();

    virtual EValType GetValueType() const { return kDict; }

    virtual const std::string& GetName() const { return fName; }
    virtual const ORVDictValue* LookUp(std::string key, char delimiter = ':') const;
    virtual ORVDictValue* LookUp(std::string key, char delimiter = ':');
    virtual void LoadEntry(std::string key, ORVDictValue* value) { fDictMap[key] = value; }
    virtual void SetName(std::string name) { fName = name; }
    virtual std::string GetStringOfValue() const {return "";}
    virtual size_t GetNValues() const {return fDictMap.size();}
    
    // The following functions are useful for iterating over the contents of
    // the dictionary:
    virtual const std::map<const std::string, ORVDictValue*>* 
      GetDictMap() const { return &fDictMap; }

  protected:
    std::string fName;
    std::map<const std::string, ORVDictValue*> fDictMap;
};

//! Dictionary value for string
class ORDictValueS : public ORVDictValue
{
  public:
    ORDictValueS(std::string s) { SetS(s); };
    virtual ~ORDictValueS() {}

    virtual EValType GetValueType() const { return kString; }

    virtual void SetS(std::string s) { fS = s; };
    virtual const std::string& GetS() const { return fS; }
    virtual std::string GetStringOfValue() const {return fS;}

  protected:
    std::string fS;
};

//! Dictionary value for double
class ORDictValueR : public ORVDictValue
{
  public:
    ORDictValueR(double r) { SetR(r); };
    virtual ~ORDictValueR() {}

    virtual EValType GetValueType() const { return kReal; }

    virtual void SetR(double r) { fR = r; };
    virtual const double& GetR() const { return fR; }
    virtual std::string GetStringOfValue() const; 

  protected:
    double fR;
};

//! Dictionary value for integer
class ORDictValueI : public ORVDictValue
{
  public:
    ORDictValueI(int i) { SetI(i); };
    virtual ~ORDictValueI() {}

    virtual EValType GetValueType() const { return kInt; }

    virtual void SetI(int i) { fI = i; };
    virtual const int& GetI() const { return fI; }
    virtual std::string GetStringOfValue() const;

  protected:
    int fI;
};

//! Dictionary value for bool
class ORDictValueB : public ORVDictValue
{
  public:
    ORDictValueB(bool b) { SetB(b); };
    virtual ~ORDictValueB() {}

    virtual EValType GetValueType() const { return kBool; }

    virtual void SetB(bool b) { fB = b; };
    virtual const bool& GetB() const { return fB; }
    virtual std::string GetStringOfValue() const {return (fB) ? "true" : "false";}

  protected:
    bool fB;
};

//! Dictionary value for array
/*!
    Provides an array full of the same type of ORVDictValues.
 */
class ORDictValueA : public ORVDictValue
{
  public:
    ORDictValueA(std::string name = "") { SetName(name); }
    ORDictValueA(const ORDictValueA& dictA);
    virtual ~ORDictValueA();

    virtual EValType GetValueType() const { return kArray; }

    virtual const std::string& GetName() const { return fName; }
    virtual void LoadValue(ORVDictValue* value) { fDictVals.push_back(value); }
    virtual void SetName(std::string name) { fName = name; }
    virtual std::string GetStringOfValue() const {return "";}
    virtual size_t GetNValues() const { return fDictVals.size(); }
    virtual const ORVDictValue* At(int i) const { return fDictVals[i]; }

  protected:
    std::string fName;
    std::vector<ORVDictValue*> fDictVals;
};


#endif
