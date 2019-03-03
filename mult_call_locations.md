### Synopsis
Users can add multiple methods to call the weather with their location. Store the call method with the size of the location and call method. This could be done by adding the locations call methods into a linked list or into a struct with all the call methods. 

### Description
#### Part I:
* Inside the JSON city config, their could be the call method ID (?id={ID}), geographical coordinates, (?lat={LAT}&lon={LON}, and by the name (?q={CITY NAME} or ?q={CITY NAME},{COUNTRY CODE}), where "?id=" is the call method and "{ID}" is the location.
* This can get tricky due to the geographical coordinates and the name is in two parts, (call method by name can be in one part). i.e. "?lat=" and "&lon=" is the call method and "{LAT}" and "{LON}" is the location.

#### Part II:
 * The user may also add a format to retreive the JSON file, units (&units={units}), and language (&lang={lang}). 
