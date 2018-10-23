/*
Weather Client

You are going to develop a program that gets the statistics about weather in the current city
using information from a certain server. The goal is to calculate statistics using the data from weather server.

To communicate with the weather server you have to implement interface IWeatherServer,
which provides the raw string from the real server for the requested day and time.

The real server (i.e. "weather.com") gets the requests in this format:
"<date>;<time>", for example:
"31.08.2018;03:00"

The server answers on requests with string like this:
"20;181;5.1"
This string contains the weather for the requested time and means next:
"<air_temperature_in_celsius>;<wind_direction_in_degrees>:<wind_speed>".
Wind direction value may be in range from 0 to 359 inclusively, temperature may be negative.

The task:
1. Implement fake server, because interacting with real network is inacceptable within the unit tests.
To do this, you need to use real server responses. Fortunately, you've collected some results for the several dates from the weather server.
Each line means "<request>" : "<response>":

"31.08.2018;03:00" : "20;181;5.1"
"31.08.2018;09:00" : "23;204;4.9"
"31.08.2018;15:00" : "33;193;4.3"
"31.08.2018;21:00" : "26;179;4.5"

"01.09.2018;03:00" : "19;176;4.2"
"01.09.2018;09:00" : "22;131;4.1"
"01.09.2018;15:00" : "31;109;4.0"
"01.09.2018;21:00" : "24;127;4.1"

"02.09.2018;03:00" : "21;158;3.8"
"02.09.2018;09:00" : "25;201;3.5"
"02.09.2018;15:00" : "34;258;3.7"
"02.09.2018;21:00" : "27;299;4.0"

IMPORTANT:
* Server returns empty string if request is invalid.
* Real server stores weather only for times 03:00, 09:00, 15:00 and 21:00 for every date. Do not use other hours in a day.

2. Implement IWeatherClient using fake server.
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>

struct Weather
{
    short temperature = 0;
    unsigned short windDirection = 0;
    double windSpeed = 0;
    bool operator==(const Weather& right) const
    {
        return temperature == right.temperature &&
               windDirection == right.windDirection &&
               std::abs(windSpeed - right.windSpeed) < 0.01;
    }
};

class IWeatherServer
{
public:
    virtual ~IWeatherServer() { }
    // Returns raw response with weather for the given day and time in request
    virtual std::string GetWeather(const std::string& request) = 0;
};

// Implement this interface
class IWeatherClient
{
public:
    virtual ~IWeatherClient() { }
    virtual double GetAverageTemperature(IWeatherServer& server, const std::string& date) = 0;
    virtual double GetMinimumTemperature(IWeatherServer& server, const std::string& date) = 0;
    virtual double GetMaximumTemperature(IWeatherServer& server, const std::string& date) = 0;
    virtual double GetAverageWindDirection(IWeatherServer& server, const std::string& date) = 0;
    virtual double GetMaximumWindSpeed(IWeatherServer& server, const std::string& date) = 0;
};

/* Test list:
 * helper functions
 * 0. Parse weather responce
 * 0.1. Parse temp
 * 0.2. Parse wind direction
 * 0.3. Parse wind speed
 * 1. get data for single date-time
 * 2. get day hours (3, 9, 15, 21)
 * 3. collect day data
 *
 * public functions
 * 1. get average temperature for one date
 * 2. get average temperature for another date (acceptance)
 *
 * 3. get min temperature for one date
 * 4. get min temperature for another date (acceptance)
 *
 * 5. get max temperature for one date
 * 6. get max temperature for another date (acceptance)
 *
 * 7. get average wind for one date
 * 8. get average wind for another date (acceptance)
 *
 * 7. get max wind speed for one date
 * 8. get max wind speed another date (acceptance)
 */

class FakeWeatherServer : IWeatherServer
{
public:
    virtual ~FakeWeatherServer() { }
    virtual std::string GetWeather(const std::string& request){
        std::string responce = "";
        try
        {
            responce = responces.at(request);
        }
        catch (const std::exception& /*ex*/)
        {
            //invalid request
        }
        return responce;
    }
private:
    std::map<std::string, std::string> responces = {
        {"31.08.2018;03:00", "20;181;5.1"},
        {"31.08.2018;09:00", "23;204;4.9"},
        {"31.08.2018;15:00", "33;193;4.3"},
        {"31.08.2018;21:00", "26;179;4.5"},
        {"01.09.2018;03:00", "19;176;4.2"},
        {"01.09.2018;09:00", "22;131;4.1"},
        {"01.09.2018;15:00", "31;109;4.0"},
        {"01.09.2018;21:00", "24;127;4.1"},
        {"02.09.2018;03:00", "21;158;3.8"},
        {"02.09.2018;09:00", "25;201;3.5"},
        {"02.09.2018;15:00", "34;258;3.7"},
        {"02.09.2018;21:00", "27;299;4.0"}
    };
};

namespace details
{

    void GetTokensFromString(const std::string& source,
                             const char delim,
                             std::vector<std::string>& tokens)
    {
        size_t start = 0;
        size_t end = 0;
        while(end < source.length())
        {
            end = source.find(delim, start);
            if (end == std::string::npos)
            {
                end = source.length();
            }
            tokens.push_back(source.substr(start, end - start));
            start = end + 1;
        }
    }

    void ParseWeatherString(const std::string& str, Weather& weather)
    {
        std::vector<std::string> tokens;
        GetTokensFromString(str, ';', tokens);

        if (tokens.size() > 0)
        {
            weather.temperature = std::stoi(tokens[0]);
        }

        if (tokens.size() > 1)
        {
            weather.windDirection = std::stoi(tokens[1]);
        }

        if (tokens.size() > 2)
        {
            weather.windSpeed = std::stod(tokens[2]);
        }
    }
}

TEST(WeatherClient, ParseWeatherTemperature)
{
    Weather weather;
    details::ParseWeatherString("20", weather);
    EXPECT_EQ(20, weather.temperature);
}

TEST(WeatherClient, ParseWeatherTemperature2)
{
    Weather weather;
    details::ParseWeatherString("35", weather);
    EXPECT_EQ(35, weather.temperature);
}

TEST(WeatherClient, ParseWeatherWindDirection)
{
    Weather weather;
    details::ParseWeatherString("35;100", weather);
    EXPECT_EQ(100, weather.windDirection);
}

TEST(WeatherClient, ParseWeatherWindSpeed)
{
    Weather weather;
    details::ParseWeatherString("35;100;15.5", weather);
    EXPECT_EQ(15.5, weather.windSpeed);
}

TEST(WeatherClient, ParseWeatherStringAcceptance)
{
    Weather weather;
    details::ParseWeatherString("-3;250;5.5", weather);
    Weather expected;
    expected.temperature = -3;
    expected.windDirection = 250;
    expected.windSpeed = 5.5;
    EXPECT_EQ(expected, weather);
}
