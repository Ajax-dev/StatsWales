


/*
  +---------------------------------------+
  | BETH YW? WELSH GOVERNMENT DATA PARSER |
  +---------------------------------------+

  AUTHOR: <963906>

  This file contains the implementation for the Area class. Area is a relatively
  simple class that contains a local authority code, a container of names in
  different languages (perhaps stored in an associative container?) and a series
  of Measure objects (also in some form of container).

  This file contains numerous functions you must implement. Each function you
  must implement has a TODO block comment. 
*/

#include <stdexcept>
#include <string>
#include <locale>
#include <ostream>
#include <vector>
#include <algorithm>
#include "area.h"

/*
  TODO: Area::Area(localAuthorityCode)

  Construct an Area with a given local authority code.

  @param localAuthorityCode
    The local authority code of the Area

  @example
    Area("W06000023");
*/
Area::Area(const std::string& localAuthorityCode) {
//  throw std::logic_error("Area::Area() has not been implemented!");
    this -> areaCode = localAuthorityCode;
}

/*
  TODO: Area::getLocalAuthorityCode()

  Retrieve the local authority code for this Area. This function should be 
  callable from a constant context and not modify the state of the instance.
  
  @return
    The Area's local authority code

  @example
    Area area("W06000023");
    ...
    auto authCode = area.getLocalAuthorityCode();
*/

std::string Area::getLocalAuthorityCode() const {
    return this -> areaCode;
}


/*
  TODO: Area::getName(lang)

  Get a name for the Area in a specific language.  This function should be 
  callable from a constant context and not modify the state of the instance.

  @param lang
    A three-leter language code in ISO 639-3 format, e.g. cym or eng

  @return
    The name for the area in the given language

  @throws
    std::out_of_range if lang does not correspond to a language of a name stored
    inside the Area instance

  @example
    Area area("W06000023");
    std::string langCode  = "eng";
    std::string langValue = "Powys";
    area.setName(langCode, langValue);
    ...
    auto name = area.getName(langCode);
*/
std::string Area::getName(std::string lang) const {
    if(!isValidLangCode(lang)) {
        throw std::out_of_range("Area::getName: Language code must be three alphabetical letters only");
    } else {
            auto it = names.find(lang);
            if (it != names.end()) {
                return it->second;
            } else {
                std::string errorMsg = "Area: " + lang + " doesn't exist in this";
                throw std::out_of_range(errorMsg);
            }
    }
}

/**
 * Checking if the lang code is valid, only alphabetical characters and 3 characters long
 * @param lang the lang code
 * @return a bool, true if valid, else false
 */
bool Area::isValidLangCode(std::string lang) const {
    if (lang.length() != 3 || lang.find_first_not_of("abcdefghijklmonpqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") != std::string::npos) {
        return false;
    } else {
        return true;
    }
}

/**
 * Self explanatory lower case method
 * @param s - the string to put to lowercase
 * @return the string in lowercase
 */
std::string Area::toLower(std::string s) {
    std::locale loc;
    std::string lower = "";
    for(unsigned int i = 0; i < s.length(); i++) {
        lower += std::tolower(s[i], loc);
    }
    return lower;
}

/**
 * Returns the map of lang codes and names
 * @return the map
 */
std::map<std::string,std::string> Area::getNamesMap() const {
    return names;
}

/**
 * Get the vector of measures
 * @return the vector for the area
 */
std::vector<Measure> Area::getMeasuresVector() const {
    return measures;
}

/*
  TODO: Area::setName(lang, name)

  Set a name for the Area in a specific language.

  @param lang
    A three-letter (alphabetical) language code in ISO 639-3 format,
    e.g. cym or eng, which should be converted to lowercase

  @param name
    The name of the Area in `lang`

  @throws
    std::invalid_argument if lang is not a three letter alphabetic code

  @example
    Area area("W06000023");
    std::string langCodeEnglish  = "eng";
    std::string langValueEnglish = "Powys";
    area.setName(langCodeEnglish, langValueEnglish);

    std::string langCodeWelsh  = "cym";
    std::string langValueWelsh = "Powys";
    area.setName(langCodeWelsh, langValueWelsh);
*/
void Area::setName(const std::string& lang, const std::string& name) {
    if(!isValidLangCode(lang)) {
        throw std::invalid_argument("Area::setName: Language code must be three alphabetical letters only");
    }
    else {
        std::string langCode = toLower(lang);
        auto it = names.find(langCode);
        if (it == names.end()) {
            names.insert(std::pair<std::string, std::string>(langCode, name));
        } else {
            it->second = name;
        }
    }
}


/*
  TODO: Area::getMeasure(key)

  Retrieve a Measure object, given its codename. This function should be case
  insensitive when searching for a measure.

  @param key
    The codename for the measure you want to retrieve

  @return
    A Measure object

  @throws
    std::out_of_range if there is no measure with the given code, throwing
    the message:
    No measure found matching <codename>

  @example
    Area area("W06000023");
    Measure measure("Pop", "Population");
    area.setMeasure("Pop", measure);
    ...
    auto measure2 = area.getMeasure("pop");
*/
Measure& Area::getMeasure(std::string codename) {
    std::string newCode = toLower(codename);
    // Measures object is a list of measures
    // Need to iterate through the measures set and compare the measure object first to codename
    // Instead of returning a declared object, most likely return a pointer to the iteration in measures that gives the object
    for (Measure &m: this-> measures) {
        if (newCode == m.getCodename()) {
            return m;
        }
    }
    std::string errorMsg = "No measure found matching " + codename;
    throw std::out_of_range(errorMsg);
}


/*
  TODO: Area::setMeasure(codename, measure)

  Add a particular Measure to this Area object. Note that the Measure's
  codename should be converted to lowercase.

  If a Measure already exists with the same codename in this Area, overwrite any
  values contained within the existing Measure with those in the new Measure
  passed into this function. The resulting Measure stored inside the Area
  instance should be a combination of the two Measures instances.

  @param codename
    The codename for the Measure

  @param measure
    The Measure object

  @return
    void

  @example
    Area area("W06000023");

    std::string codename = "Pop";
    std::string label = "Population";
    Measure measure(codename, label);

    double value = 12345678.9;
    measure.setValue(1999, value);

    area.setMeasure(codename, measure);
*/
void Area::setMeasure(const std::string& codename, const Measure& measure) {
    std::string newCode = toLower(codename);

//    auto itYear = measure.getDataMap().find(measure.getKey());
    if (measures.size() != 0) {
        bool found = false;
        for (Measure &m: measures) {
            if (newCode == toLower(m.getCodename())) {
                found = true;
                m.setLabel(measure.getLabel());
                for (auto& yearValPair: measure.getDataMap()) {
                    m.setValue(yearValPair.first, yearValPair.second);
                }
            }
        }
        if (!found){
            this -> measures.push_back(measure);
        }
    } else {
        this -> measures.push_back(measure);
    }
}

/*
  TODO: Area::size()

  Retrieve the number of Measures we have for this Area. This function should be 
  callable from a constant context, not modify the state of the instance, and
  must promise not throw an exception.

  @return
    The size of the Area (i.e., the number of Measures)

  @example
    Area area("W06000023");
    std::string langCode  = "eng";
    std::string langValue = "Powys";
    area.setName(langCode, langValue);

    std::string codename = "Pop";
    std::string label = "Population";
    Measure measure(codename, label);

    area.setMeasure(code, measure);
    auto size = area.size();
*/
unsigned int Area::size() const {
    return this -> measures.size();
}


/**
 * New method to help out with the AREAS class setArea function where combining without full erasure needs to happen
 * This allows for data persistence via looping through the new area to be inserted, and if any
 ** -> Names match in the old area, then the lang code remains but the name is replaced
 **** -> If any measure objects are the same they are replaced, all others are left
 * @param areaNew the new Area object to be added
 * @param areaOrig the old area object to be overwritten
 * @return a combined area object
 */
Area Area::combineAreas(Area& areaNew, Area& areaOrig) {
    Area combined = areaOrig;

    // Replaces any same lang code names with the new name
    if (areaNew.names.size() != 0) {
        for (auto it = areaNew.names.begin(); it != areaNew.names.end(); it++) {
            combined.setName(it->first, it->second);
        }
    }

    // Replace any measure objects that are the same, otherwise add
    for (Measure& m: areaNew.measures) {
        combined.setMeasure(m.getCodename(), m);
    }

    return combined;
}

/*
  TODO: operator<<(os, area)

  Overload the stream output operator as a free/global function.

  Output the name of the Area in English and Welsh, followed by the local
  authority code. Then output all the measures for the area (see the coursework
  worksheet for specific formatting).

  If the Area only has only one name, output this. If the area has no names,
  output the name "Unnamed".

  Measures should be ordered by their Measure codename. If there are no measures
  output the line "<no measures>" after you have output the area names.

  See the coursework specification for more examples.

  @param os
    The output stream to write to

  @param area
    Area to write to the output stream

  @return
    Reference to the output stream

  @example
    Area area("W06000023");
    area.setName("eng", "Powys");
    std::cout << area << std::endl;
*/
std::ostream &operator<<(std::ostream &os, const Area &area) {
    if (area.names.size() != 0) {
        auto engIt = area.names.find("eng");
        auto cymIt = area.names.find("cym");
        if (engIt != area.names.end() && cymIt != area.names.end()) {
            os << area.getName("eng") << " / " << area.getName("cym") << "(" << area.getLocalAuthorityCode() << ")\n";
        } else if (engIt == area.names.end() && cymIt != area.names.end()) {
            os << area.getName("cym") << "(" << area.getLocalAuthorityCode() << ")\n";
        } else if (engIt != area.names.end() && cymIt == area.names.end()) {
            os << area.getName("eng") << "(" << area.getLocalAuthorityCode() << ")\n";
        }
    } else {
        os << "Unnamed (" << area.getLocalAuthorityCode() << ")\n";
    }
    if (area.measures.size() == 0) {
        os << "<No measures>\n";
    } else {
        std::vector<Measure> areasOutput = area.measures;
        std::sort(areasOutput.begin(), areasOutput.end());
        for (auto it = areasOutput.begin(); it != areasOutput.end(); it++) {
            os << *it;
        }
    }
    return os;
}


/*
  TODO: operator==(lhs, rhs)

  Overload the == operator for two Area objects as a global/free function. Two
  Area objects are only equal when their local authority code, all names, and
  all data are equal.

  @param lhs
    An Area object

  @param rhs
    A second Area object

  @return
    true if both Area instances have the same local authority code, names
    and data; false otherwise.

  @example
    Area area1("MYCODE1");
    Area area2("MYCODE1");

    bool eq = area1 == area2;
*/
bool operator==(const Area &lhs, const Area &rhs) {
    bool authCodes = false;
    bool names = false;
    bool data = false;


    if (lhs.areaCode == rhs.areaCode) {
        authCodes = true;
    }

    // I think we're only matching if the data is the same and in the same order
    if ((lhs.names == rhs.names) || (rhs.names.empty() && lhs.names.empty())) {
        names = true;
    }

    if ((lhs.measures == rhs.measures) || (lhs.measures.empty() && rhs.measures.empty()) ) {
        data = true;
    }
    return (authCodes && names && data);
}

/*
  Method to overload the < operator to allow of authority code wise sorting of areas

  @param &lhs
    The first area to compare against as an Area object

  @param &rhs
    The second area to compare against as an Area object

  @return
    A boolean if the area code is less than the second

  @throws
    Nothing
 */
bool operator<(const Area &lhs, const Area &rhs) {

    return lhs.getLocalAuthorityCode() < rhs.getLocalAuthorityCode();
}
