#ifndef AREA_H_
#define AREA_H_

/*
  +---------------------------------------+
  | BETH YW? WELSH GOVERNMENT DATA PARSER |
  +---------------------------------------+

  AUTHOR: <963906>

  This file contains the Area class declaration. Area objects contain all the
  Measure objects for a given local area, along with names for that area and a
  unique authority code.

  TODO: Read the block comments with TODO in area.cpp to know which 
  functions and member variables you need to declare in this class.
 */

#include <string>
#include <vector>
#include <map>

#include "measure.h"

/*
  An Area object consists of a unique authority code, a container for names
  for the area in any number of different languages, and a container for the
  Measures objects.

  TODO: Based on your implementation, there may be additional constructors
  or functions you implement here, and perhaps additional operators you may wish
  to overload.
*/
class Area {

public:
    Area(const std::string& localAuthorityCode);

    //setters
    void setName(const std::string& lang, const std::string& name);
    void setMeasure(const std::string& codename, const Measure& measure);

    //getters
    Measure& getMeasure(std::string codename);
    std::string getLocalAuthorityCode() const;
    std::string getName(std::string lang) const;
    std::map<std::string,std::string> getNamesMap() const;
    std::vector<Measure> getMeasuresVector() const;
    unsigned int size() const;

    //helpers
    std::string toLower(std::string s);
    bool isValidLangCode(std::string lang) const;
    Area combineAreas(Area& areaNew, Area& areaOrig);

    //friends, overloads, json conv
    friend bool operator==(const Area &lhs, const Area &rhs);
    friend std::ostream &operator<<(std::ostream &os, const Area &area);
    friend bool operator<(const Area &lhs, const Area &rhs);

protected:
    std::string areaCode;
    std::vector<Measure> measures;
    std::map<std::string, std::string> names;

};

#endif // AREA_H_