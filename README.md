# Coursework ReadMe

First of all, all tests **pass locally** all 1506 assertions.
**Compiler** I used was GNU with wsl, and then used build.sh on this for everything.
Did not manage to complete task 8, plan to come back to this later for fun and will do it then.

For the Output tests: locally:
- output 1 passes with no issue and returns `No dataset matches key: invalidataset`
- output 2 passes with no issue and returns
```
Error importing dataset:
InputFile::open: Failed to open file invalidir/areas.csv
```
- output 3 fails due to a check I run on areas argument to see if it is valid for the regex of the areas, this returns `Invalid input for area argument`
- output 4,5,6,7,8,9 passes without issue, the output is the same as the txt file
- output 10 is the same output but with a blank line at the end

For the Output tests: csAutoGrader:
- As of March 28, *16:27* all tests but 5 passed and only outputs 1 and 2 passed, however this was due to me error testing and custom outputs.
- Currently waiting on results of *22:38*


## Questions/Issues

With your outputs on the pdf I'm a bit confused between
`bethyw -d popden -a W06000011,W06000010 -y 1990-1993` your outputs for difference as percentage appear to divide by the largest value of the 2.
However with `bethyw -d popden,trains -a W06000011 -m rail,pop -y 2015-2018` for the % diff you are dividing by the smallest value, I did not know how to recreate this so have opted for division by the largest value for percentage difference.

In your `parseAuthorityByYearCSV` you show the brackets with no measure filter but in params have a measure filter, I believe this was a mistake and have included measure filtering regardless.

With output 3, you've asked for a {} output, but when parsing areas arg you've asked us to check for it being a valid area, `doesnotexist` doesn't meet the criteria of an area code so instead of an empty set of braces my error is thrown.