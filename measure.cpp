


/*
  +---------------------------------------+
  | BETH YW? WELSH GOVERNMENT DATA PARSER |
  +---------------------------------------+

  AUTHOR: <963906>

  This file contains the implementation of the Measure class. Measure is a
  very simple class that needs to contain a few member variables for its name,
  codename, and a Standard Library container for data. The data you need to 
  store is values, organised by year. I'd recommend storing the values as 
  doubles.

  This file contains numerous functions you must implement. Each function you
  must implement has a TODO block comment. 
*/

#include <stdexcept>
#include <string>
#include <locale>
#include <ostream>
#include <iomanip>

#include "measure.h"

/*
  TODO: Measure::Measure(codename, label);

  Construct a single Measure, that has values across many years.

  All StatsWales JSON files have a codename for measures. You should convert 
  all codenames to lowercase.

  @param codename
    The codename for the measure

  @param label
    Human-readable (i.e. nice/explanatory) label for the measure

  @example
    std::string codename = "Pop";
    std::string label = "Population";
    Measure measure(codename, label);
*/
Measure::Measure(std::string codename, const std::string &label) {
  this -> codename = toLower(codename);
  this -> name = label;
}
std::string Measure::toLower(std::string s) {
    std::locale loc;
    std::string lower = "";
    for(unsigned int i = 0; i < s.length(); i++) {
        lower += std::tolower(s[i], loc);
    }
    return lower;
}

/*
  TODO: Measure::getCodename()

  Retrieve the code for the Measure. This function should be callable from a 
  constant context and must promise to not modify the state of the instance or 
  throw an exception.

  @return
    The codename for the Measure

  @example
    std::string codename = "Pop";
    std::string label = "Population";
    Measure measure(codename, label);

    measure.setValue(1999, 12345678.9);
    ...
    auto codename2 = measure.getCodename();
*/
std::string Measure::getCodename() const {
    return this -> codename;
}

/**
 * Gets the year-val data map
 * @return the map
 */
std::map<int, double> Measure::getDataMap() const {
    return data;
}


/*
  TODO: Measure::getLabel()

  Retrieve the human-friendly label for the Measure. This function should be 
  callable from a constant context and must promise to not modify the state of 
  the instance and to not throw an exception.

  @return
    The human-friendly label for the Measure

  @example
    std::string codename = "Pop";
    std::string label = "Population";
    Measure measure(codename, label);

    measure.setValue(1999, 12345678.9);
    ...
    auto label = measure.getLabel();
*/
std::string Measure::getLabel() const {
    return this -> name;
}


/*
  TODO: Measure::setLabel(label)

  Change the label for the Measure.

  @param label
    The new label for the Measure

  @example
    Measure measure("pop", "Population");
    measure.setValue(1999, 12345678.9);
    ...
    measure.setLabel("New Population");
*/
void Measure::setLabel(const std::string& label) {
    this -> name = label;
}

/*
  TODO: Measure::getValue(key)

  Retrieve a Measure's value for a given year.

  @param key
    The year to find the value for

  @return
    The value stored for the given year

  @throws
    std::out_of_range if year does not exist in Measure with the message
    No value found for year <year>

  @return
    The value

  @example
    std::string codename = "Pop";
    std::string label = "Population";
    Measure measure(codename, label);

    measure.setValue(1999, 12345678.9);
    ...
    auto value = measure.getValue(1999); // returns 12345678.9
*/
double Measure::getValue(int key) {
    auto itYear = this->data.find(key);

    if (itYear != data.end()) {
        return itYear->second;
    } else {
        std::string errorMessage = "No value found for year " + std::to_string(key);
        throw std::out_of_range(errorMessage);
    }
}


/*
  TODO: Measure::setValue(key, value)

  Add a particular year's value to the Measure object. If a value already
  exists for the year, replace it.

  @param key
    The year to insert a value at

  @param value
    The value for the given year

  @return
    void

  @example
    std::string codename = "Pop";
    std::string label = "Population";
    Measure measure(codename, label);

    measure.setValue(1999, 12345678.9);
*/
void Measure::setValue(const int& year,const double& value) {
    if (data.size() != 0) {
        bool found = false;
        for (auto& yearValPair: data) {
            if (year == yearValPair.first) {
                found = true;
                yearValPair.second = value;
            }
        }
        if (!found){
            this -> data.insert({year,value});
        }
    } else {
        this -> data.insert({year,value});
    }
}

int Measure::getKey() const {
    return this -> key;
}

/*
  TODO: Measure::size()

  Retrieve the number of years data we have for this measure. This function
  should be callable from a constant context and must promise to not change
  the state of the instance or throw an exception.

  @return
    The size of the measure

  @example
    std::string codename = "Pop";
    std::string label = "Population";
    Measure measure(codename, label);

    measure.setValue(1999, 12345678.9);
    auto size = measure.size(); // returns 1
*/
unsigned int Measure::size() const {
    return this->data.size();
}


/*
  TODO: Measure::getDifference()

  Calculate the difference between the first and last year imported. This
  function should be callable from a constant context and must promise to not
  change the state of the instance or throw an exception.

  @return
    The difference/change in value from the first to the last year, or 0 if it
    cannot be calculated

  @example
    Measure measure("pop", "Population");
    measure.setValue(1999, 12345678.9);
    measure.setValue(2001, 12345679.9);
    auto diff = measure.getDifference(); // returns 1.0
*/
double Measure::getDifference() const {
    if (this->size() <= 1) {
        return 0;
    } else {
        auto firstValIt = data.begin();
        double firstVal = firstValIt->second;
        auto secondValIt = --data.end();
        double lastVal = secondValIt->second;

        double sum = lastVal - firstVal;
        return sum;
    }
}


/*
  TODO: Measure::getDifferenceAsPercentage()

  Calculate the difference between the first and last year imported as a 
  percentage. This function should be callable from a constant context and
  must promise to not change the state of the instance or throw an exception.

  @return
    The difference/change in value from the first to the last year as a decminal
    value, or 0 if it cannot be calculated

  @example
    Measure measure("pop", "Population");
    measure.setValue(1990, 12345678.9);
    measure.setValue(2010, 12345679.9);
    auto diff = measure.getDifferenceAsPercentage();
*/
double Measure::getDifferenceAsPercentage() const {
    auto firstValIt = data.begin();
    auto secondValIt = --data.end();
    double largestVal;
    if ((firstValIt -> second) > (secondValIt->second)) {
        largestVal = firstValIt -> second;
    } else if ((firstValIt -> second) < (secondValIt->second)) {
        largestVal = secondValIt -> second;
    } else {
        largestVal = 0;
    }

    if (largestVal != 0) {
        double valToReturn = (getDifference() / largestVal) * 100;
        return valToReturn;
    } else {
        return 0;
    }
}


/*
  TODO: Measure::getAverage()

  Calculate the average/mean value for all the values. This function should be
  callable from a constant context and must promise to not change the state of 
  the instance or throw an exception.

  @return
    The average value for all the years, or 0 if it cannot be calculated

  @example
    Measure measure("pop", "Population");
    measure.setValue(1999, 12345678.9);
    measure.setValue(2001, 12345679.9);
    auto diff = measure.getAverage(); // returns 12345678.4
*/
double Measure::getAverage() const {
    double sum = 0;
    double numVals = 0;
    for (auto it = data.begin(); it != data.end(); it++) {
        sum += it -> second;
        numVals++;
    }
    return sum/numVals;
}

/*
  TODO: operator<<(os, measure)

  Overload the << operator to print all of the Measure's imported data.

  We align the year and value outputs by padding the outputs with spaces,
  i.e. the year and values should be right-aligned to each other so they
  can be read as a table of numerical values.

  Years should be printed in chronological order. Three additional columns
  should be included at the end of the output, correspodning to the average
  value across the years, the difference between the first and last year,
  and the percentage difference between the first and last year.

  If there is no data in this measure, print the name and code, and 
  on the next line print: <no data>

  See the coursework specification for more information.

  @param os
    The output stream to write to

  @param measure
    The Measure to write to the output stream

  @return
    Reference to the output stream

  @example
    std::string codename = "Pop";
    std::string label = "Population";
    Measure measure(codename, label);

    measure.setValue(1999, 12345678.9);
    std::cout << measure << std::end;
*/
std::ostream &operator<<(std::ostream &os, const Measure &measure) {
    const int tabVal = 14;
    const int precision = 6;
    os << measure.getLabel() << " (" << measure.getCodename() << ")\n";
    if (measure.size() != 0) {
        for (auto& it: measure.data) {
            os << std::setw(tabVal) << it.first;
        }
        os << std::setw(tabVal) << "Average";
        os << std::setw(tabVal) << "Diff.";
        os << std::setw(tabVal) << "%Diff" << "\n";
        os << std::fixed << std::setprecision(precision);
        for (auto& it: measure.data) {
            os << std::setw(tabVal) << it.second;
        }
        os << std::setw(tabVal) << measure.getAverage();
        os << std::setw(tabVal) << measure.getDifference();
        os << std::setw(tabVal) << measure.getDifferenceAsPercentage() << "\n";
    } else {
        os << "no data to read";
    }
    return os;
}

/*
  TODO: operator==(lhs, rhs)

  Overload the == operator for two Measure objects. Two Measure objects
  are only equal when their codename, label and data are all equal.

  @param lhs
    A Measure object

  @param rhs
    A second Measure object

  @return
    true if both Measure objects have the same codename, label and data; false
    otherwise
*/
bool operator==(const Measure &lhs, const Measure &rhs) {
    bool authCodes = false;
    bool names = false;
    bool data = true;

    //Convert both codenames to lowercase for comparison
    std::locale loc;
    std::string lowerLeft = "";
    std::string lowerRight = "";
    for(unsigned int i = 0; i < lhs.codename.length(); i++) {
        lowerLeft += std::tolower(lhs.codename[i], loc);
    }
    for (unsigned int i = 0; i < rhs.codename.length(); i++) {
        lowerRight += std::tolower(rhs.codename[i], loc);
    }
    if (lowerLeft == lowerRight) {
        authCodes = true;
    }

    if (lhs.name == rhs.name) {
        names = true;
    }

    if (lhs.data.size() == rhs.data.size()) {
        auto lhsIt = lhs.data.begin();
        auto rhsIt = rhs.data.begin();
        int counter = 0;
        for (unsigned int i = 0; i < lhs.data.size(); i++) {
            if (lhsIt == rhsIt) {
            } else {
                data = false;
            }
            counter++;
            lhsIt = lhs.data.begin().operator++();
            rhsIt = rhs.data.begin().operator++();
        }
    } else {
        data = false;
    }
    return (authCodes && names && data);
}

// overload the operator<
bool operator<(const Measure &lhs, const Measure &rhs) {

    return lhs.getCodename() < rhs.getCodename();
}
