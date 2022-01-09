#ifndef MEASURE_H_
#define MEASURE_H_

/*
  +---------------------------------------+
  | BETH YW? WELSH GOVERNMENT DATA PARSER |
  +---------------------------------------+

  AUTHOR: <963906>

  This file contains the decalaration of the Measure class.

  TODO: Read the block comments with TODO in measure.cpp to know which 
  functions and member variables you need to declare in this class.
 */

#include <string>
#include <map>

/*
  The Measure class contains a measure code, label, and a container for readings
  from across a number of years.

  TODO: Based on your implementation, there may be additional constructors
  or functions you implement here, and perhaps additional operators you may wish
  to overload.
*/
class Measure {
public:
    Measure(std::string code, const std::string &label);

    //setters
    void setLabel(const std::string& label);
    void setValue(const int& year,const double& value);

    //getters
    double getValue(int key);
    double getAverage() const;
    std::string getCodename() const;
    std::string getLabel() const;
    unsigned int size() const;
    std::map<int, double> getDataMap() const;
    double getDifference() const;
    double getDifferenceAsPercentage() const;
    int getKey() const;

    //helpers
    std::string toLower(std::string s);

    //friends, overloads
    friend bool operator==(const Measure &lhs, const Measure &rhs);
    friend bool operator<(const Measure &lhs, const Measure &rhs);
    friend std::ostream &operator<<(std::ostream &os, const Measure &measure);

protected:
    std::string name;
    std::string codename;
    int key;
    std::map<int, double> data;
};

#endif // MEASURE_H_