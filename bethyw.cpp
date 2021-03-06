


/*
  +---------------------------------------+
  | BETH YW? WELSH GOVERNMENT DATA PARSER |
  +---------------------------------------+

  AUTHOR: <963906>

  This file contains all the helper functions for initialising and running
  Beth Yw? In languages such as Java, this would be a class, but we really
  don't need a class here. Classes are for modelling data, and so forth, but
  here the code is pretty much a sequential block of code (BethYw::run())
  calling a series of helper functions.

  TODO: This file contains numerous functions you must implement. Each one
  is denoted with a TODO in the block comment. Note that some code has been
  provided in some functions to get you started, but you should read through
  this code and make sure it is safe and complete. You may need to remove or
  modify the provided code to get your program to work fully. You may implement
  additional functions not specified.
*/

#include <iostream>
#include <string>
#include <tuple>
#include <unordered_set>
#include <vector>
#include <sstream>

#include "lib_cxxopts.hpp"

#include "areas.h"
#include "datasets.h"
#include "bethyw.h"
#include "input.h"

/*
  Run Beth Yw?, parsing the command line arguments, importing the data,
  and outputting the requested data to the standard output/error.

  Hint: cxxopts.parse() throws exceptions you'll need to catch. Read the cxxopts
  documentation for more information.

  @param argc
    Number of program arguments

  @param argv
    Program arguments

  @return
    Exit code
*/

int BethYw::run(int argc, char *argv[]) {
  auto cxxopts = BethYw::cxxoptsSetup();
  auto args = cxxopts.parse(argc, argv);

  // Print the help usage if requested
  if (args.count("help")) {
    std::cerr << cxxopts.help() << std::endl;
    return 0;
  }


  try {
      // Parse data directory argument
      std::string dir = args["dir"].as<std::string>() + DIR_SEP;

      // Parse other arguments and import data
      auto datasetsToImport = BethYw::parseDatasetsArg(args);
      auto areasFilter = BethYw::parseAreasArg(args);
      auto measuresFilter = BethYw::parseMeasuresArg(args);
      auto yearsFilter = BethYw::parseYearsArg(args);

      Areas data = Areas();


      BethYw::loadAreas(data, dir, areasFilter);

      BethYw::loadDatasets(data,
                           dir,
                           datasetsToImport,
                           areasFilter,
                           measuresFilter,
                           yearsFilter);

      if (args.count("json")) {
          // The output as JSON
          std::cout << data.toJSON() << std::endl;
      } else {
          // The output as tables
          std::cout << data << std::endl;
      }

  } catch (std::exception const &e) {
      std::cerr << e.what() << std::endl;
  }
  return 0;
}


/*
  This function sets up and returns a valid cxxopts object. You do not need to
  modify this function.

  @return
     A constructed cxxopts object

  @example
    auto cxxopts = BethYw::cxxoptsSetup();
    auto args = cxxopts.parse(argc, argv);
*/
cxxopts::Options BethYw::cxxoptsSetup() {
  cxxopts::Options cxxopts(
        "bethyw",
        "Student ID: " + STUDENT_NUMBER + "\n\n"
        "This program is designed to parse official Welsh Government"
        " statistics data files.\n");
    
  cxxopts.add_options()(
      "dir",
      "Directory for input data passed in as files",
      cxxopts::value<std::string>()->default_value("datasets"))(

      "d,datasets",
      "The dataset(s) to import and analyse as a comma-separated list of codes "
      "(omit or set to 'all' to import and analyse all datasets)",
      cxxopts::value<std::vector<std::string>>()->default_value("all"))(

      "a,areas",
      "The areas(s) to import and analyse as a comma-separated list of "
      "authority codes (omit or set to 'all' to import and analyse all areas)",
      cxxopts::value<std::vector<std::string>>()->default_value("all"))(

      "m,measures",
      "Select a subset of measures from the dataset(s) "
      "(omit or set to 'all' to import and analyse all measures)",
      cxxopts::value<std::vector<std::string>>()->default_value("all"))(

      "y,years",
      "Focus on a particular year (YYYY) or "
      "inclusive range of years (YYYY-ZZZZ)",
      cxxopts::value<std::string>()->default_value("0"))(

      "j,json",
      "Print the output as JSON instead of tables.")(

      "h,help",
      "Print usage.");

  return cxxopts;
}

/*
  TODO: BethYw::parseDatasetsArg(args)

  Parse the datasets argument passed into the command line. 

  The datasets argument is optional, and if it is not included, all datasets 
  should be imported. If it is included, it should be a comma-separated list of 
  datasets to import. If the argument contains the value "all"
  (case-insensitive), all datasets should be imported.

  This function validates the passed in dataset names against the codes in
  DATASETS array in the InputFiles namespace in datasets.h. If an invalid code
  is entered, throw a std::invalid_argument with the message:
  No dataset matches key: <input code>
  where <input name> is the name supplied by the user through the argument.

  @param args
    Parsed program arguments

  @return
    A std::vector of BethYw::InputFileSource instances to import

  @throws
    std::invalid_argument if the argument contains an invalid dataset with
    message: No dataset matches key <input code>

  @example
    auto cxxopts = BethYw::cxxoptsSetup();
    auto args = cxxopts.parse(argc, argv);

    auto datasetsToImport = BethYw::parseDatasetsArg(args);
 */
std::vector<BethYw::InputFileSource> BethYw::parseDatasetsArg(
    cxxopts::ParseResult& args) {
  // This function is incomplete, but to get you started...
  // You may want to delete much of these // comments too!

  // Retrieve all valid datasets, see datasets.h
  size_t numDatasets = InputFiles::NUM_DATASETS;
  auto &allDatasets = InputFiles::DATASETS;

  // Create the container for the return type
  std::vector<InputFileSource> datasetsToImport;

  // You can get the std::vector of arguments from cxxopts like this.
  // Note that this function will throw an exception if datasets is not set as 
  // an argument. Check the documentation! Read it and understand it.
  std::vector<std::string> inputDatasets = args["datasets"].as < std::vector < std::string >> ();

  // You now need to compare the strings in this vector to the keys in
  // allDatasets above. Populate datasetsToImport with the values
  // from allDatasets above and then return a vector

  // You'll want to ignore/remove the following lines of code, they simply
  // import all datasets (for now) as an example to get you started
  if (inputDatasets.empty() == true) {
      for (unsigned int i = 0; i < numDatasets; i++) {
          datasetsToImport.push_back(allDatasets[i]);
      }
  } else {
      unsigned int numUnmatched;
      for (unsigned int i = 0; i < inputDatasets.size(); i++) {
          numUnmatched = 0;
          if (inputDatasets[i] == "all") {
            datasetsToImport.clear();
          }
          std::string wrongData = "";
          for (unsigned int j = 0; j < numDatasets; j++) {
              if (inputDatasets[i] == allDatasets[j].CODE) {
                  datasetsToImport.push_back(allDatasets[j]);
              } else if (inputDatasets[i] == "all") {
                  datasetsToImport.push_back(allDatasets[j]);
              } else {
                  wrongData = inputDatasets[i];
                  numUnmatched++;
              } if (numUnmatched == numDatasets) {
                  std::string errorMsg = "No dataset matches key: " + wrongData;
                  throw std::invalid_argument(errorMsg);
              }
          }
      }


  }


  return datasetsToImport;
}

/*
  TODO: BethYw::parseAreasArg(args)
  
  Parses the areas command line argument, which is optional. If it doesn't 
  exist or exists and contains "all" as value (any case), all areas should be
  imported, i.e., the filter should be an empty set.

  Unlike datasets we can't check the validity of the values as it depends
  on each individual file imported (which hasn't happened until runtime).
  Therefore, we simply fetch the list of areas and later pass it to the
  Areas::populate() function.

  The filtering of inputs should be case insensitive.

  @param args
    Parsed program arguments

  @return 
    An std::unordered_set of std::strings corresponding to specific areas
    to import, or an empty set if all areas should be imported.

  @throws
    std::invalid_argument if the argument contains an invalid areas value with
    message: Invalid input for area argument
*/
std::unordered_set<std::string> BethYw::parseAreasArg(
    cxxopts::ParseResult& args) {
  // The unordered set you will return
  std::unordered_set<std::string> areas;

  // Retrieve the areas argument like so:
  std::vector<std::string> temp = args["areas"].as<std::vector<std::string>>();

  try {
      if (temp.empty() == true) {
          areas.clear();
      } else {
          bool allEntered;
          for (unsigned int i = 0; i < temp.size(); i++) {
              if (temp[i] == "all") {
                  allEntered = true;
              } else if (BethYw::isValidArea(temp[i]) != true) {
                  throw std::invalid_argument("Invalid input for area argument");
              } else {
                  areas.insert(temp[i]);
              }
          }

          if (allEntered == true) {
              areas.clear();
          }

      }
      return areas;

  } catch (const std::invalid_argument &exception) {
      throw exception;
  }
}
/*
  Self explanatory method checking if area code is valid and aligns with Welsh stats

  @param area
    area code as a string

  @return
    A boolean if the area is value or nothing

  @throws
    Nothing
 */
bool BethYw::isValidArea(std::string& area) {
    if (area.length() != 9) {
        return false;
    } else {
        for (unsigned int i = 1; i < area.length(); i++) {
            if (area.find("W") == std::string::npos) {
                return false;
            } else {
                if (isdigit(area[i] == false)) {
                    return false;
                }
            }
        }
        return true;
    }
}

/*
  TODO: BethYw::parseMeasuresArg(args)

  Parse the measures command line argument, which is optional. If it doesn't 
  exist or exists and contains "all" as value (any case), all measures should
  be imported.

  Unlike datasets we can't check the validity of the values as it depends
  on each individual file imported (which hasn't happened until runtime).
  Therefore, we simply fetch the list of measures and later pass it to the
  Areas::populate() function.

  The filtering of inputs should be case insensitive.

  @param args
    Parsed program arguments

  @return 
    An std::unordered_set of std::strings corresponding to specific measures
    to import, or an empty set if all measures should be imported.

  @throws
    std::invalid_argument if the argument contains an invalid measures value
    with the message: Invalid input for measures argument
*/

std::unordered_set<std::string> BethYw::parseMeasuresArg(
        cxxopts::ParseResult& args) {

    // The unordered set you will return
    std::unordered_set<std::string> measures;

    // Retrieve the areas argument like so:
    std::vector<std::string> temp = args["measures"].as < std::vector < std::string >> ();


    if (temp.empty()) {
        measures.clear();
    } else {
        bool allEntered;
        for (unsigned int i = 0; i < temp.size(); i++) {
            std::string measure = temp[i];
            for(unsigned int i = 0; i < measure.length(); i++) {
                if (measure.find_first_not_of("abcdefghijklmnopqrstuvwxys") != std::string::npos) {
                    throw new std::invalid_argument("Invalid input for measures argument");
                }
            }
            if (temp[i] == "all") {
                allEntered = true;
            } else {
                measures.insert(temp[i]);
            }
        }
        if (allEntered == true) {
            measures.clear();
        }
    }

    return measures;
}

/*
  TODO: BethYw::parseYearsArg(args)

  Parse the years command line argument. Years is either a four digit year 
  value, or two four digit year values separated by a hyphen (i.e. either 
  YYYY or YYYY-ZZZZ).

  This should be parsed as two integers and inserted into a std::tuple,
  representing the start and end year (inclusive). If one or both values are 0,
  then there is no filter to be applied. If no year argument is given return
  <0,0> (i.e. to import all years). You will have to search
  the web for how to construct std::tuple objects! 

  @param args
    Parsed program arguments

  @return
    A std::tuple containing two unsigned ints

  @throws
    std::invalid_argument if the argument contains an invalid years value with
    the message: Invalid input for years argument
*/
std::tuple<unsigned int, unsigned int> BethYw::parseYearsArg(cxxopts::ParseResult& args) {
    std::tuple<unsigned int, unsigned int> years;

    std::string yearsString = args["years"].as<std::string>();

    int hyphen = yearsString.find("-");


    try {
        // If the find method doesn't find the value it returns -1, we can assume there's either no numbers or only 1 year
        if (hyphen == -1) {
            if (yearsString.length() == 0) {
                years = std::make_tuple(0, 0);
            } else {
                // Checking if the string of length 4 is actually a number
                bool isNum = BethYw::yearIsNumber(yearsString);
                if (isNum) {
                    int theYear = std::stoi(yearsString);
                    years = std::make_tuple(theYear, theYear);
                } else {
                    throw std::invalid_argument("Invalid input for years argument");
                }
            }
        } else {
            // This is when there is a hyphen and it is within the bounds
            std::istringstream yearsStream;
            yearsStream.str(yearsString);
            std::vector<std::string> yearsGiven;
            std::string tempYear;
            while (std::getline(yearsStream, tempYear, yearsString.at(hyphen))) {
                yearsGiven.push_back(tempYear);
            }
            int firstYear;
            int secondYear;
            if (yearIsNumber(yearsGiven.at(0)) && yearIsNumber(yearsGiven.at(1))) {
                firstYear = std::stoi(yearsGiven.at(0));
                secondYear = std::stoi(yearsGiven.at(1));
                years = std::make_tuple(firstYear, secondYear);
            } else {
                throw std::invalid_argument("Invalid input for years argument");
            }
        }
    } catch (const std::invalid_argument &exception) {
        throw exception;
    }

    return years;
}

/*
  Self explanatory method checking if year is a number

  @param year
    year as a string

  @return
    A boolean if the year is number or not

  @throws
    Nothing
 */
bool BethYw::yearIsNumber(std::string& year)
{
    for (unsigned int i = 0; i < year.length(); i++) {
        if (isdigit(year[i]) == false) {
            return false;
        }
    }
    return true;
}

/*
  TODO: BethYw::loadAreas(areas, dir, areasFilter)

  Load the areas.csv file from the directory `dir`. Parse the file and
  create the appropriate Area objects inside the Areas object passed to
  the function in the `areas` argument.

  areas.csv is guaranteed to be formatted as:
    Local authority code,Name (eng),Name (cym)

  Hint: To implement this function. First you will need create an InputFile 
  object with the filename of the areas file, open it, and then pass reference 
  to the stream to the Areas::populate() function.

  Hint 2: you can retrieve the specific filename for a dataset, e.g. for the 
  areas.csv file, from the InputFileSource's FILE member variable

  @param areas
    An Areas instance that should be modified (i.e. the populate() function
    in the instance should be called)

  @param dir
    Directory where the areas.csv file is

  @param areasFilter
    An unordered set of areas to filter, or empty to import all areas

  @return
    void

  @example
    Areas areas();

    BethYw::loadAreas(areas, "data", BethYw::parseAreasArg(args));
*/
void BethYw::loadAreas(Areas &areas,std::string dir,std::unordered_set<std::string> areasFilter) {
        InputFile loadAreaFile(dir + InputFiles::AREAS.FILE);
        std::istream &loaded = loadAreaFile.open();

        SourceColumnMapping areaCols = InputFiles::AREAS.COLS;
        areas.populate(loaded, SourceDataType::AuthorityCodeCSV, areaCols, &areasFilter);
}


/*
  TODO: BethYw::loadDatasets(areas,
                             dir,
                             datasetsToImport,
                             areasFilter,
                             measuresFilter,
                             yearsFilter)

  Import datasets from `datasetsToImport` as files in `dir` into areas, and
  filtering them with the `areasFilter`, `measuresFilter`, and `yearsFilter`.

  The actual filtering will be done by the Areas::populate() function, thus 
  you need to merely pass pointers on to these flters.

  This function should promise not to throw an exception. If there is an
  error/exception thrown in any function called by thus function, catch it and
  output 'Error importing dataset:', followed by a new line and then the output
  of the what() function on the exception.

  @param areas
    An Areas instance that should be modified (i.e. datasets loaded into it)

  @param dir
    The directory where the datasets are

  @param datasetsToImport
    A vector of InputFileSource objects

  @param areasFilter
    An unordered set of areas (as authority codes encoded in std::strings)
    to filter, or empty to import all areas

  @param measuresFilter
    An unordered set of measures (as measure codes encoded in std::strings)
    to filter, or empty to import all measures

  @param yearsFilter
    An two-pair tuple of unsigned ints corresponding to the range of years 
    to import, which should both be 0 to import all years.

  @return
    void

  @example
    Areas areas();

    BethYw::loadDatasets(
      areas,
      "data",
      BethYw::parseDatasetsArgument(args),
      BethYw::parseAreasArg(args),
      BethYw::parseMeasuresArg(args),
      BethYw::parseYearsArg(args));
*/
void BethYw::loadDatasets(Areas &areas,
                          std::string dir,
                          std::vector<BethYw::InputFileSource> datasetsToImport,
                          std::unordered_set<std::string> areasFilter,
                          std::unordered_set<std::string> measuresFilter,
                          std::tuple<unsigned int, unsigned int> yearsFilter
) {
        for (auto it = datasetsToImport.begin(); it != datasetsToImport.end(); it++) {
            InputFile tempFile(dir + it->FILE);
            SourceColumnMapping tempColumns = it->COLS;
            SourceDataType tempType = it->PARSER;
            std::istream &loaded = tempFile.open();
            areas.populate(loaded, tempType, tempColumns, &areasFilter, &measuresFilter, &yearsFilter);
//            std::cout << areas << std::endl;
        }
}



