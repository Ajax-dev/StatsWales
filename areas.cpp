


/*
  +---------------------------------------+
  | BETH YW? WELSH GOVERNMENT DATA PARSER |
  +---------------------------------------+

  AUTHOR: <963906>

  The file contains the Areas class implementation. Areas are the top
  level of the data structure in Beth Yw? for now.

  Areas is also responsible for importing data from a stream (using the
  various populate() functions) and creating the Area and Measure objects.

  This file contains numerous functions you must implement. Each function you
  must implement has a TODO block comment. 
*/

#include <stdexcept>
#include <iostream>
#include <string>
#include <stdexcept>
#include <tuple>
#include <sstream>
#include <unordered_set>
#include <vector>
#include <map>
#include <typeinfo>
#include <locale>
#include <algorithm>

#include "lib_json.hpp"

#include "datasets.h"
#include "areas.h"
#include "measure.h"

/*
  An alias for the imported JSON parsing library.
*/
using json = nlohmann::json;

/*
  TODO: Areas::Areas()

  Constructor for an Areas object.

  @example
    Areas data = Areas();
*/
Areas::Areas() {
//  throw std::logic_error("Areas::Areas() has not been implemented!");
}

/*
  TODO: Areas::setArea(localAuthorityCode, area)

  Add a particular Area to the Areas object.

  If an Area already exists with the same local authority code, overwrite all
  data contained within the existing Area with those in the new
  Area (i.e. they should be combined, but the new Area's data should take
  precedence, e.g. replace a name with the same language identifier).

  @param localAuthorityCode
    The local authority code of the Area

  @param area
    The Area object that will contain the Measure objects

  @return
    void

  @example
    Areas data = Areas();
    std::string localAuthorityCode = "W06000023";
    Area area(localAuthorityCode);
    data.setArea(localAuthorityCode, area);
*/
void Areas::setArea(const std::string localAuthorityCode, Area area) {
    if (areasContainer.empty()) {
        this -> areasContainer.insert({localAuthorityCode, area});
    } else {
        bool found = false;
        for (auto& keyValPair: areasContainer) {
            if (areasContainer.find(localAuthorityCode)->first == keyValPair.first) {
                found = true;
                // Calls helper function in area class to replace the values inside this area object
                keyValPair.second = area.combineAreas(area, keyValPair.second);
            }
        }
        if (!found) {
            this -> areasContainer.insert({localAuthorityCode, area});
        }
    }
}

/*
  TODO: Areas::getArea(localAuthorityCode)

  Retrieve an Area instance with a given local authority code.

  @param localAuthorityCode
    The local authority code to find the Area instance of

  @return
    An Area object

  @throws
    std::out_of_range if an Area with the set local authority code does not
    exist in this Areas instance

  @example
    Areas data = Areas();
    std::string localAuthorityCode = "W06000023";
    Area area(localAuthorityCode);
    data.setArea(localAuthorityCode, area);
    ...
    Area area2 = areas.getArea("W06000023");
*/
Area& Areas::getArea(std::string localAuthorityCode) {
    auto it = areasContainer.find(localAuthorityCode);

    if (it != areasContainer.end()) {
        return it -> second;
    } else {
        std::string errorMsg = "No area found matching " + localAuthorityCode;
        throw std::out_of_range(errorMsg);
    }
}


/*
  TODO: Areas::size()

  Retrieve the number of Areas within the container. This function should be 
  callable from a constant context, not modify the state of the instance, and
  must promise not throw an exception.

  @return
    The number of Area instances

  @example
    Areas data = Areas();
    std::string localAuthorityCode = "W06000023";
    Area area(localAuthorityCode);
    data.setArea(localAuthorityCode, area);
    
    auto size = areas.size(); // returns 1
*/

unsigned int Areas::size() const {
    return this -> areasContainer.size();
}


/*
  TODO: Areas::populateFromAuthorityCodeCSV(is, cols, areasFilter)

  This function specifically parses the compiled areas.csv file of local 
  authority codes, and their names in English and Welsh.

  This is a simple dataset that is a comma-separated values file (CSV), where
  the first row gives the name of the columns, and then each row is a set of
  data.

  For this coursework, you can assume that areas.csv will always have the same
  three columns in the same order.

  Once the data is parsed, you need to create the appropriate Area objects and
  insert them in to a Standard Library container within Areas.

  @param is
    The input stream from InputSource

  @param cols
    A map of the enum BethyYw::SourceColumnMapping (see datasets.h) to strings
    that give the column header in the CSV file

  @param areasFilter
    An umodifiable pointer to set of umodifiable strings for areas to import,
    or an empty set if all areas should be imported

  @return
    void

  @see
    See datasets.h for details of how the variable cols is organised

  @see
    See bethyw.cpp for details of how the variable areasFilter is created

  @example
    InputFile input("data/areas.csv");
    auto is = input.open();

    auto cols = InputFiles::AREAS.COLS;

    auto areasFilter = BethYw::parseAreasArg();

    Areas data = Areas();
    areas.populateFromAuthorityCodeCSV(is, cols, &areasFilter);

  @throws 
    std::runtime_error if a parsing error occurs (e.g. due to a malformed file)
    std::out_of_range if there are not enough columns in cols
*/
void Areas::populateFromAuthorityCodeCSV(
    std::istream &is,
    const BethYw::SourceColumnMapping &cols,
    const StringFilterSet * const areasFilter) {

    if (cols.size() != 3) {
        throw std::out_of_range("Wrong number of columns");
    }
    //Queues to hold all column values, queues are first in first out and are probably best for population
    std::vector<std::string> authCodes, nameEng, nameCym;


    // When the csv file has content in it, store the data in relevant queues
    std::vector<std::string> fileContent;
    std::vector<std::string> allContent;
    std::string thisLine;
    if (is.good()) {
        while(std::getline(is, thisLine)) {//Add all lines of the file to a vector
            fileContent.push_back(thisLine);
        }
        for (unsigned int i = 0; i < fileContent.size(); i++) {
            std::stringstream ss(fileContent[i]);
            while (ss.good()) {//Add all individual elements to a bigger vector
                std::string tempString;
                std::getline( ss, tempString, ',' );
                allContent.push_back(tempString);
            }
        }
        // Ignore column titles
        for (unsigned int i = 3; i < allContent.size(); i+=3) {
            authCodes.push_back(allContent[i]);
            nameEng.push_back(allContent[i+1]);
            nameCym.push_back(allContent[i+2]);
        }

        // Remove the column headings as we don't want these as area objects
        // If empty we can assume no args were called or 'all' was called and we want everything
        if (areasFilter == nullptr || areasFilter->empty()) {
            for (unsigned int i = 0; i < authCodes.size(); i++) {
                Area newArea(authCodes[i]);
                newArea.setName("eng", nameEng[i]);
                newArea.setName("cym", nameCym[i]);
                this->setArea(authCodes[i], newArea);
            }
        } else {
            for (auto it = areasFilter->begin(); it != areasFilter->end(); it++) {
                unsigned int indexOfReq = 0;

                for (auto const& authCode: authCodes) {
                    if (authCode == *it) { break; }
                    else { indexOfReq++; }
                }

                Area newArea(authCodes[indexOfReq]);
                newArea.setName("eng", nameEng[indexOfReq]);
                newArea.setName("cym", nameCym[indexOfReq]);
                this->setArea(newArea.getLocalAuthorityCode(), newArea);
            }
        }
    }
}

/*
  TODO: Areas::populateFromWelshStatsJSON(is,
                                          cols,
                                          areasFilter,
                                          measuresFilter,
                                          yearsFilter)

  Data from StatsWales is in the JSON format, and contains three
  top-level keys: odata.metadata, value, odata.nextLink. value contains the
  data we need. Rather than been hierarchical, it contains data as a
  continuous list (e.g. as you would find in a table). For each row in value,
  there is a mapping of various column headings and their respective vaues.

  Therefore, you need to go through the items in value (in a loop)
  using a JSON library. To help you, I've selected the nlohmann::json
  library that you must use for your coursework. Read up on how to use it here:
  https://github.com/nlohmann/json

  Example of using this library:
    - Reading/parsing in from a stream is very simply using the >> operator:
        json j;
        stream >> j;

    - Looping through parsed JSON is done with a simple for each loop. Inside
      the loop, you can access each using the array syntax, with the key/
      column name, e.g. data["Localauthority_ItemName_ENG"] gives you the
      local authority name:
        for (auto& el : j["value"].items()) {
           auto &data = el.value();
           std::string localAuthorityCode = data["Localauthority_ItemName_ENG"];
           // do stuff here...
        }

  In this function, you will have to parse the JSON datasets, extracting
  the local authority code, English name (the files only contain the English
  names), and each measure by year.

  If you encounter an Area that does not exist in the Areas container, you
  should create the Area object

  If areasFilter is a non-empty set only include areas matching the filter. If
  measuresFilter is a non-empty set only include measures matching the filter.
  If yearsFilter is not equal to <0,0>, only import years within the range
  specified by the tuple (inclusive).

  I've provided the column names for each JSON file that you need to parse
  as std::strings in datasets.h. This mapping should be passed through to the
  cols parameter of this function.

  Note that in the JSON format, years are stored as strings, but we need
  them as ints. When retrieving values from the JSON library, you will
  have to cast them to the right type.

  @param is
    The input stream from InputSource

  @param cols
    A map of the enum BethyYw::SourceColumnMapping (see datasets.h) to strings
    that give the column header in the CSV file

  @param areasFilter
    An umodifiable pointer to set of umodifiable strings of areas to import,
    or an empty set if all areas should be imported

  @param measuresFilter
    An umodifiable pointer to set of umodifiable strings of measures to import,
    or an empty set if all measures should be imported

  @param yearsFilter
    An umodifiable pointer to an umodifiable tuple of two unsigned integers,
    where if both values are 0, then all years should be imported, otherwise
    they should be treated as the range of years to be imported (inclusively)

  @return
    void

  @throws 
    std::runtime_error if a parsing error occurs (e.g. due to a malformed file)
    std::out_of_range if there are not enough columns in cols

  @see
    See datasets.h for details of how the variable cols is organised

  @see
    See bethyw.cpp for details of how the variable areasFilter is created

  @example
    InputFile input("data/popu1009.json");
    auto is = input.open();

    auto cols = InputFiles::DATASETS["popden"].COLS;

    auto areasFilter = BethYw::parseAreasArg();
    auto measuresFilter = BethYw::parseMeasuresArg();
    auto yearsFilter = BethYw::parseMeasuresArg();

    Areas data = Areas();
    areas.populateFromWelshStatsJSON(
      is,
      cols,
      &areasFilter,
      &measuresFilter,
      &yearsFilter);
*/
void Areas::populateFromWelshStatsJSON(std::istream& is,
                                       const BethYw::SourceColumnMapping& cols,
                                       const StringFilterSet * const areasFilter,
                                       const StringFilterSet * const measuresFilter,
                                       const YearFilterTuple * const yearsFilter) {
    // istream is is the file that we have input
    // cols is the columns of
    if (cols.size() == 6) {
        json j;
        is >> j;

        for (auto& el : j["value"].items()) {
            // Setting up temporary values to populate measure and area object with based on file
            auto &data = el.value();
            std::string code;
            std::string label;
            if (j["odata.metadata"] == "http://open.statswales.gov.wales/en-gb/dataset/$metadata#tran0152") {
                code = cols.find(BethYw::SourceColumn::SINGLE_MEASURE_CODE)->second;
                label = cols.find(BethYw::SourceColumn::SINGLE_MEASURE_NAME)->second;
            } else {
                code = data[cols.find(BethYw::SourceColumn::MEASURE_CODE)->second];
                label = data[cols.find(BethYw::SourceColumn::MEASURE_NAME)->second];
            }
            std::string authCode = data[cols.find(BethYw::SourceColumn::AUTH_CODE)->second];
            std::string engName = data[cols.find(BethYw::SourceColumn::AUTH_NAME_ENG)->second];
            std::string tempYear = data[cols.find(BethYw::SourceColumn::YEAR)->second];
            unsigned int year = std::stoi(tempYear);

            // do stuff here...
            double value = 0;
            std::string odata = j["odata.metadata"];
            if (odata == "http://open.statswales.gov.wales/en-gb/dataset/$metadata#envi0201") {
                std::string tempValStr = data[cols.find(BethYw::SourceColumn::VALUE)->second];
                value = std::stod(tempValStr);
            } else {
                value = data[cols.find(BethYw::SourceColumn::VALUE)->second];
            }
            authCode = toUpper(authCode);
            code = toLower(code);
            Area tempArea(authCode);
            tempArea.setName("eng", engName);

            Measure tempMeasure(code, label);
            tempMeasure.setValue(year, value);
            tempArea.setMeasure(code, tempMeasure);

            // Then add it to areas container
            /**
             * Checking for filters starting with the areas filter, followed by measure, then years,
             * as all of the filter checks are basically the same I will only explain areas,
             * checks from start to end and looks for the value in the filter being equal to the current authority code
             * if it is found then it sets the areas argument to true to move on to measure.
             */
            bool areaBool = false;
            for (auto it = areasFilter->begin(); it != areasFilter->end(); it++) {
                if (*it == authCode) {
                    areaBool = true;
                }
            }
            if (areasFilter->empty()) {
                areaBool = true;
            }

            bool measureBool = false;
            for (auto it = measuresFilter->begin(); it != measuresFilter->end(); it++) {
                std::string lowerMeasureCode = toLower(code);
                std::string tempM = toLower(*it);
                if (lowerMeasureCode == tempM) {
                    measureBool = true;
                }
            }
            if (measuresFilter->empty()) {
                measureBool = true;
            }

            bool yearBool = false;
            if (std::get<0>(*yearsFilter) == 0 || std::get<1>(*yearsFilter) == 0) {
                yearBool = true;
            } else if (yearsFilter == nullptr) {
                yearBool = true;
            }else {
                if (((year >= std::get<0>(*yearsFilter)) && (year <= std::get<1>(*yearsFilter))) && (yearsFilter !=
                        nullptr)) {
                    yearBool = true;
                }
            }

            if (areaBool && measureBool && yearBool) {
                setArea(authCode, tempArea);
            }
        }
    } else {
        throw std::out_of_range("There are not enough columns in cols");
    }
}

/**
 * As in measure, I realise this should've been made a global function but ended up being crunch for time
 * @param s the string to set to lower
 * @return the string in lowercase
 */
std::string Areas::toLower(std::string s) {
    std::locale loc;
    std::string lower = "";
    for(unsigned int i = 0; i < s.length(); i++) {
        lower += std::tolower(s[i], loc);
    }
    return lower;
}

/**
 * As above but for uppercase, in hindsight, lambda expressions may have been more useful
 */
std::string Areas::toUpper(std::string s) {
    std::locale loc;
    std::string upper = "";
    for(unsigned int i = 0; i < s.length(); i++) {
        upper += std::toupper(s[i], loc);
    }
    return upper;
}

/*
  TODO: Areas::populateFromAuthorityByYearCSV(is,
                                              cols,
                                              areasFilter,
                                              yearFilter)

  This function imports CSV files that contain a single measure. The 
  CSV file consists of columns containing the authority code and years.
  Each row contains an authority code and values for each year (or no value
  if the data doesn't exist).

  Note that these files do not include the names for areas, instead you 
  have to rely on the names already populated through 
  Areas::populateFromAuthorityCodeCSV();

  The datasets that will be parsed by this function are
   - complete-popu1009-area.csv
   - complete-popu1009-pop.csv
   - complete-popu1009-opden.csv

  @param is
    The input stream from InputSource

  @param cols
    A map of the enum BethyYw::SourceColumnMapping (see datasets.h) to strings
    that give the column header in the CSV file

  @param areasFilter
    An umodifiable pointer to set of umodifiable strings for areas to import,
    or an empty set if all areas should be imported

  @param measuresFilter
    An umodifiable pointer to set of strings for measures to import, or an empty 
    set if all measures should be imported

  @param yearsFilter
    An umodifiable pointer to an umodifiable tuple of two unsigned integers,
    where if both values are 0, then all years should be imported, otherwise
    they should be treated as a the range of years to be imported

  @return
    void

  @see
    See datasets.h for details of how the variable cols is organised

  @see
    See bethyw.cpp for details of how the variable areasFilter is created

  @example
    InputFile input("data/complete-popu1009-pop.csv");
    auto is = input.open();

    auto cols = InputFiles::DATASETS["complete-pop"].COLS;

    auto areasFilter = BethYw::parseAreasArg();
    auto yearsFilter = BethYw::parseYearsArg();

    Areas data = Areas();
    areas.populateFromAuthorityCodeCSV(is, cols, &areasFilter, &yearsFilter);

  @throws 
    std::runtime_error if a parsing error occurs (e.g. due to a malformed file)
    std::out_of_range if there are not enough columns in cols
*/
void Areas::populateFromAuthorityByYearCSV(std::istream& is,
                                           const BethYw::SourceColumnMapping& cols,
                                           const StringFilterSet * const areasFilter,
                                           const StringFilterSet * const measuresFilter,
                                           const YearFilterTuple * const yearFilter){

    if (cols.size() != 3) {
        throw std::out_of_range("Wrong number of columns");
    }
    //Queues to hold all column values, queues are first in first out and are probably best for population
    std::vector<std::string> authCodes;
    std::vector<int> yearsVec;
    // Each line of the file will be stored as auth code, and then the following values

    std::map<std::string, std::map<int, double>> authYearValMap;


    // When the csv file has content in it, store the data in relevant queues
    std::vector<std::string> fileContent;
    std::vector<std::string> allContent;
    std::string thisLine;
    if (is.good()) {
        while (std::getline(is, thisLine)) {
            fileContent.push_back(thisLine);
        }
        for (unsigned int i = 0; i < fileContent.size(); i++) {
            std::stringstream ss(fileContent[i]);
            while (ss.good()) {
                std::string tempString;
                std::getline(ss, tempString, ',');
                allContent.push_back(tempString);
            }
        }

        //Populate years vector
        for (int i = 1; i < 12; i++) {
            int tempYear = std::stoi(allContent[i]);
            yearsVec.push_back(tempYear);
        }
        // Ignore column titles
        // Remove the years are outside of the range
        int year1 = std::get<0>(*yearFilter);
        int year2 = std::get<1>(*yearFilter);
        for (unsigned int i = 12; i < allContent.size(); i += 12) {
            std::map<int, double> tempSavedVals;
            int mapIterator = 1;
            for (auto it = yearsVec.begin(); it != yearsVec.end(); it++) {
                int tempYear = *it;
                double tempVal = std::stod(allContent[i+mapIterator]);
                if ((yearFilter != nullptr) && (year1 != 0 && year2 != 0)) {
                    if (tempYear >= year1 && tempYear <= year2) {
                        tempSavedVals.insert({tempYear, tempVal});
                    }
                } else {
                    tempSavedVals.insert({tempYear, tempVal});
                }
                mapIterator++;
            }
            if (areasFilter->size() != 0 && areasFilter != nullptr) {
                if (areasFilter->find(allContent[i]) != areasFilter->end()) {
                    authYearValMap.insert({allContent[i], tempSavedVals});
                }
            } else {
                authYearValMap.insert({allContent[i], tempSavedVals});
            }
        }
        // Remove the column headings as we don't want these as area objects
        // Remove the authority code areas that aren't wanted

        // Find the relevant code and name values

        std::string measureCode = cols.find(BethYw::SourceColumn::SINGLE_MEASURE_CODE)->second;
        std::string measureName = cols.find(BethYw::SourceColumn::SINGLE_MEASURE_NAME)->second;
        Measure tempMeasure(measureCode, measureName);
        // Finally create all areas, 1 by 1 and populate their measure values
        for (auto &codeMeasureMap: authYearValMap) {//Look at all areas 1 by 1 and populate
            Area tempArea(codeMeasureMap.first);
            // now iterate through the measure map and set values
            std::map<int, double> tempYearMap = codeMeasureMap.second;
            for (auto &yearValMap: tempYearMap) {//Set all measures in a tempArea
                tempMeasure.setValue(yearValMap.first, yearValMap.second);
            }
            tempArea.setMeasure(codeMeasureMap.first, tempMeasure);

            if (measuresFilter->size() != 0 && measuresFilter != nullptr) {
                if (measuresFilter->find(measureCode) != measuresFilter->end()) {//If measure code is found, measure is to be included
                    this->setArea(codeMeasureMap.first, tempArea);
                }
            } else {//Include all measures
                this->setArea(codeMeasureMap.first, tempArea);
            }
        }
    } else {
        std::cout << "File isn't good" << std::endl;
    }

}


/*
  TODO: Areas::populate(is, type, cols)

  Parse data from an standard input stream `is`, that has data of a particular
  `type`, and with a given column mapping in `cols`.

  This function should look at the `type` and hand off to one of the three 
  functions populate………() functions.

  The function must check if the stream is in working order and has content.

  @param is
    The input stream from InputSource

  @param type
    A value from the BethYw::SourceDataType enum which states the underlying
    data file structure

  @param cols
    A map of the enum BethyYw::SourceColumnMapping (see datasets.h) to strings
    that give the column header in the CSV file

  @return
    void

  @throws 
    std::runtime_error if a parsing error occurs (e.g. due to a malformed file),
    the stream is not open/valid/has any contents, or an unexpected type
    is passed in.
    std::out_of_range if there are not enough columns in cols

  @see
    See datasets.h for details of the values variable type can have

  @see
    See datasets.h for details of how the variable cols is organised

  @see
    See bethyw.cpp for details of how the variable areasFilter is created

  @example
    InputFile input("data/popu1009.json");
    auto is = input.open();

    auto cols = InputFiles::DATASETS["popden"].COLS;

    Areas data = Areas();
    areas.populate(
      is,
      DataType::WelshStatsJSON,
      cols);
*/
void Areas::populate(std::istream &is,
                     const BethYw::SourceDataType &type,
                     const BethYw::SourceColumnMapping &cols) {
    if (type == BethYw::AuthorityCodeCSV) {
        populateFromAuthorityCodeCSV(is, cols);
    } else {
    throw std::runtime_error("Areas::populate: Unexpected data type");
  }
}

/*
  TODO: Areas::populate(is,
                        type,
                        cols,
                        areasFilter,
                        measuresFilter,
                        yearsFilter)

  Parse data from an standard input stream, that is of a particular type,
  and with a given column mapping, filtering for specific areas, measures,
  and years, and fill the container.

  This function should look at the `type` and hand off to one of the three 
  functions you've implemented above.

  The function must check if the stream is in working order and has content.

  This overloaded function includes pointers to the three filters for areas,
  measures, and years.

  @param is
    The input stream from InputSource

  @param type
    A value from the BethYw::SourceDataType enum which states the underlying
    data file structure

  @param cols
    A map of the enum BethyYw::SourceColumnMapping (see datasets.h) to strings
    that give the column header in the CSV file

  @param areasFilter
    An umodifiable pointer to set of umodifiable strings for areas to import,
    or an empty set if all areas should be imported

  @param measuresFilter
    An umodifiable pointer to set of umodifiable strings for measures to import,
    or an empty set if all measures should be imported

  @param yearsFilter
    An umodifiable pointer to an umodifiable tuple of two unsigned integers,
    where if both values are 0, then all years should be imported, otherwise
    they should be treated as a the range of years to be imported

  @return
    void

  @throws 
    std::runtime_error if a parsing error occurs (e.g. due to a malformed file),
    the stream is not open/valid/has any contents, or an unexpected type
    is passed in.
    std::out_of_range if there are not enough columns in cols

  @see
    See datasets.h for details of the values variable type can have

  @see
    See datasets.h for details of how the variable cols is organised

  @see
    See bethyw.cpp for details of how the variables areasFilter, measuresFilter,
    and yearsFilter are created

  @example
    InputFile input("data/popu1009.json");
    auto is = input.open();

    auto cols = InputFiles::DATASETS["popden"].COLS;

    auto areasFilter = BethYw::parseAreasArg();
    auto measuresFilter = BethYw::parseMeasuresArg();
    auto yearsFilter = BethYw::parseMeasuresArg();

    Areas data = Areas();
    areas.populate(
      is,
      DataType::WelshStatsJSON,
      cols,
      &areasFilter,
      &measuresFilter,
      &yearsFilter);
*/
void Areas::populate(
    std::istream &is,
    const BethYw::SourceDataType &type,
    const BethYw::SourceColumnMapping &cols,
    const StringFilterSet * const areasFilter,
    const StringFilterSet * const measuresFilter,
    const YearFilterTuple * const yearsFilter)
     {
  if (type == BethYw::AuthorityCodeCSV) {
    populateFromAuthorityCodeCSV(is, cols, areasFilter);
  } else if (type == BethYw::WelshStatsJSON) {
      populateFromWelshStatsJSON(is, cols, areasFilter, measuresFilter, yearsFilter);
  } else if (type == BethYw::AuthorityByYearCSV) {
      populateFromAuthorityByYearCSV(is, cols, areasFilter, measuresFilter, yearsFilter);
  } else {
    throw std::runtime_error("Areas::populate: Unexpected data type");
  }
}

/*
  TODO: Areas::toJSON()

  Convert this Areas object, and all its containing Area instances, and
  the Measure instances within those, to values.

  Use the sample JSON library as above to create this. Construct a blank
  JSON object:
    json j;

  Convert this json object to a string:
    j.dump();

  You then need to loop through your areas, measures, and years/values
  adding this data to the JSON object.

  Read the documentation for how to convert your outcome code to JSON:
    https://github.com/nlohmann/json#arbitrary-types-conversions
  
  The JSON should be formatted as such:
    {
    "<localAuthorityCode1>" : {
                              "names": { "<languageCode1>": "<languageName1>",
                                         "<languageCode2>": "<languageName2>" 
                                         …
                                         "<languageCodeN>": "<languageNameN>" }, 
                              "measures" : { "<year1>": <value1>,
                                             "<year2>": <value2>,
                                             …
                                            "<yearN>": <valueN> }
                               },
    "<localAuthorityCode2>" : {
                              "names": { "<languageCode1>": "<languageName1>",
                                         "<languageCode2>": "<languageName2>" 
                                         …
                                         "<languageCodeN>": "<languageNameN>" }, 
                              "measures" : { "<year1>": <value1>,
                                             "<year2>": <value2>,
                                             …
                                            "<yearN>": <valueN> }
                               }
    ...
    "<localAuthorityCodeN>" : {
                              "names": { "<languageCode1>": "<languageName1>",
                                         "<languageCode2>": "<languageName2>" 
                                         …
                                         "<languageCodeN>": "<languageNameN>" }, 
                              "measures" : { "<year1>": <value1>,
                                             "<year2>": <value2>,
                                             …
                                            "<yearN>": <valueN> }
                               }
    }

  An empty JSON is "{}" (without the quotes), which you must return if your
  Areas object is empty.
  
  @return
    std::string of JSON

  @example
    InputFile input("data/popu1009.json");
    auto is = input.open();

    auto cols = InputFiles::DATASETS["popden"].COLS;

    auto areasFilter = BethYw::parseAreasArg();
    auto measuresFilter = BethYw::parseMeasuresArg();
    auto yearsFilter = BethYw::parseMeasuresArg();

    Areas data = Areas();
    std::cout << data.toJSON();
*/
std::string Areas::toJSON() const {
  json j;
  if (this->areasContainer.size() == 0) {
      return "{}";
  } else {
      for (auto it = areasContainer.begin(); it != areasContainer.end(); it++) {
          // it -> first is local auth code, inside format names followed by measures, for loop to set up json
          // names and measures objects
          Area tempArea = it->second;
          //names convert to json
          json jNames;
          //Make a temporary map which is the area's values
          std::map<std::string, std::string> tempNameMap = tempArea.getNamesMap();
          //Convert this map with json lib
          json j_map(tempNameMap);
          jNames = tempNameMap;

          //measures conv to json
          std::vector<Measure> measureVec = tempArea.getMeasuresVector();
          json jMeasures;
          for (std::vector<Measure>::iterator m = measureVec.begin(); m != measureVec.end(); m++) {
              Measure tempMeasure = *m;
              std::string mName = tempMeasure.getCodename();
              std::map<std::string,double> tempMeasureMap;
              for (auto& kvp: tempMeasure.getDataMap()) {
                  std::string yearString = std::to_string(kvp.first);
                  double val = kvp.second;
                  tempMeasureMap.insert({yearString, val});
              }
              json j_map(tempMeasureMap);
              //Append the json created map to the relevant measure name
              jMeasures[mName] = tempMeasureMap;
          }

          j[it->first]["names"] = jNames;
          j[it->first]["measures"] = jMeasures;
      }
  }
  std::string jsonStr = j.dump();
  return jsonStr;
}

/*
  TODO: operator<<(os, areas)

  Overload the << operator to print all of the imported data.

  Output should be formatted like the following to pass the tests. Areas should
  be printed, ordered alphabetically by their local authority code. Measures 
  within each Area should be ordered alphabetically by their codename.

  See the coursework specification for more information, although for reference
  here is a quick example of how output should be formatted:

    <English name of area 1> / <Welsh name of area 1> (<authority code 1>)
    <Measure 1 name> (<Measure 1 code>)
     <year 1>  <year 2> <year 3> ...  <year n>  Average    Diff.   % Diff.
    <value 1>  <year 2> <year 3> ... <value n> <mean 1> <diff 1> <diffp 1>

    <Measure 2 name> (<Measure 2 code>)
     <year 1>  <year 2> <year 3> ...  <year n>  Average    Diff.   % Diff.
    <value 1>  <year 2> <year 3> ... <value n> <mean 2> <diff 2> <diffp 2>

    <Measure 3 name> (<Measure 3 code>)
     <year 1>  <year 2> <year 3> ...  <year n>  Average    Diff.   % Diff.
    <value 1>  <year 2> <year 3> ... <value n> <mean 3> <diff 3> <diffp 3>

    ...

    <Measure x name> (<Measure x code>)
     <year 1>  <year 2> <year 3> ...  <year n>  Average    Diff.   % Diff.
    <value 1>  <year 2> <year 3> ... <value n> <mean x> <diff x> <diffp x>


    <English name of area 2> / <Welsh name of area 2> (<authority code 2>)
    <Measure 1 name> (<Measure 1 code>)
     <year 1>  <year 2> <year 3> ...  <year n>  Average    Diff.   % Diff.
    <value 1>  <year 2> <year 3> ... <value n> <mean 1> <diff 1> <diffp 1>

    <Measure 2 name> (<Measure 2 code>)
     <year 1>  <year 2> <year 3> ...  <year n>  Average    Diff.   % Diff.
    <value 1>  <year 2> <year 3> ... <value n> <mean 2> <diff 2> <diffp 2>

    <Measure 3 name> (<Measure 3 code>)
     <year 1>  <year 2> <year 3> ...  <year n>  Average    Diff.   % Diff.
    <value 1>  <year 2> <year 3> ... <value n> <mean 3> <diff 3> <diffp 3>

    ...

    <Measure x name> (<Measure x code>)
     <year 1>  <year 2> <year 3> ...  <year n>
    <value 1>  <year 2> <year 3> ... <value n> <mean x> <diff x> <diffp x>

    ...

    <English name of area y> / <Welsh name of area y> (<authority code y>)
    <Measure 1 name> (<Measure 1 codename>)
     <year 1>  <year 2> <year 3> ...  <year n>
    <value 1>  <year 2> <year 3> ... <value n> <mean 1> <diff 1> <diffp 1>

    <Measure 2 name> (<Measure 2 codename>)
     <year 1>  <year 2> <year 3> ...  <year n>
    <value 1>  <year 2> <year 3> ... <value n> <mean 2> <diff 2> <diffp 2>

    <Measure 3 name> (<Measure 3 codename>)
     <year 1>  <year 2> <year 3> ...  <year n>
    <value 1>  <year 2> <year 3> ... <value n> <mean 3> <diff 3> <diffp 3>

    ...

    <Measure x name> (<Measure x codename>)
     <year 1>  <year 2> <year 3> ...  <year n>  Average    Diff.   % Diff.
    <value 1>  <year 2> <year 3> ... <value n> <mean x> <diff x> <diffp x>

  With real data, your output should start like this for the command
  bethyw --dir <dir> -p popden -y 1991-1993 (truncated for readability):

    Isle of Anglesey / Ynys Môn (W06000001)
    Land area (area) 
          1991       1992       1993    Average    Diff.  % Diff. 
    711.680100 711.680100 711.680100 711.680100 0.000000 0.000000 

    Population density (dens) 
         1991      1992      1993   Average    Diff.  % Diff. 
    97.126504 97.486216 98.038430 97.550383 0.911926 0.938905 

    Population (pop) 
            1991         1992         1993      Average      Diff.  % Diff. 
    69123.000000 69379.000000 69772.000000 69424.666667 649.000000 0.938906 


    Gwynedd / Gwynedd (W06000002)
    Land area (Area)
    ...

  @param os
    The output stream to write to

  @param areas
    The Areas object to write to the output stream

  @return
    Reference to the output stream

  @example
    Areas areas();
    std::cout << areas << std::end;
*/
std::ostream &operator<<(std::ostream &os, const Areas &areas) {
    if (areas.size() != 0) {
        std::vector<Area> areasCpy;
        for (auto it = areas.areasContainer.begin(); it != areas.areasContainer.end(); it++) {
            areasCpy.push_back(it->second);
        }
        std::sort(areasCpy.begin(), areasCpy.end());
        for (auto it = areasCpy.begin(); it != areasCpy.end(); it++) {
//            std::cout << "Printing " << *it;
            os << *it;
        }
    } else {
        os << "No areas to print\n";
    }
    return os;
}


