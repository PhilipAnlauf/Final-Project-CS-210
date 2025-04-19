#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <vector>
#include <bitset>
#include <chrono>

using namespace std;

struct City
{
    string name, countryCode;
    int population;

    City(string nameIN, string countryCodeIN, int populationIN): name(std::move(nameIN)),
           countryCode(countryCodeIN), population(populationIN) {}
};

int getHashIndex(string& cityNameIN, int& cityPopIN, int arraySize)
{
    int asciiIntSum = 0;
    for(const char character : cityNameIN)
    {
        asciiIntSum += stoi(to_string(static_cast<int>(character)));
    }
    return (asciiIntSum+cityPopIN) % arraySize;
}

class CityCacheList
{
    private:
        City* cityCache[10] = {nullptr};
    public:
        void insertCity2Cache(City* newCity)
        {
            const int hashIndex = getHashIndex(newCity->name, newCity->population, (sizeof(cityCache) / sizeof(cityCache[0])));

            if (hashIndex == -1) { return; }
            if(cityCache[hashIndex] == nullptr) { cityCache[hashIndex] = newCity; return;}

            //TODO: implement Collision handling
            else
            {
                 //Need to create new array * 2 size
                // assign old cities to new list with new has
                // assign new array as cityCache
            }

            City* temp = cityCache[hashIndex];

        }

};

class CSVReader
{
    public:
        static void findCity(const string& cityNameIN, const string& countryCodeIN, CityCacheList& cacheListIN)
        {
            ifstream file("world_cities.csv");
            string line, word;

            if (!file.is_open())
            {
                cerr << "Error: Could not open file" << endl;
                return;
            }

            while (getline(file, line))
            {
                stringstream ss(line);
                vector<string> lineData;
                while (getline(ss, word, ','))
                {
                    lineData.push_back(word);
                }

                if (lineData.at(0) == countryCodeIN && lineData.at(1) == cityNameIN)
                {
                    cout << "City Found: ";
                    cacheListIN.insertCity2Cache(new City(lineData.at(0), lineData.at(1), stoi(lineData.at(2))));
                    return;
                }

            }
            file.close();
            return;
        }
};

int main()
{
    CityCacheList cacheList;

    string cityName, countryCode;
    while (true)
    {
        cout << "1. INVALID CHOICE." << endl;
        cout << "2. Search for city." << endl;
        cout << "3. INVALID CHOICE." << endl;
        cout << "4. exit." << endl;

        string choiceHold;
        getline(cin, choiceHold);
        switch (stoi(choiceHold))
        {
            case 1:

            break;
            case 2:
                cout << "City name?: ";
                getline(cin, cityName);
                cout << "City countryCode?: ";
                getline(cin, countryCode);
                CSVReader::findCity(cityName, countryCode, cacheList);
            break;
            case 3:

            break;
            case 4:
                return 0;
            default:
                cout << "Invalid choice." << endl;
        }
        cout << endl;
    }
}