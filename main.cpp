#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>

using namespace std;

struct City
{
    string name, countryCode;
    int population;

    City(string nameIN, string countryCodeIN, int populationIN): name(std::move(nameIN)),
           countryCode(countryCodeIN), population(populationIN) {}
};

class CityCacheList
{
    public:
        virtual ~CityCacheList() = default;
        virtual void insertCity2Cache(City* cityIN) = 0;
        virtual City* findCity(string& cityNameIN, string& countryCodeIN) = 0;
        virtual void displayCache() = 0;
};

class FIFOCache : public CityCacheList
{
    private:
        vector<City*> cities;
    public:
        FIFOCache()
        {
            for (int i = 0; i < 10; i++)
            {
                cities.push_back(nullptr);
            }
        }

        void insertCity2Cache(City* cityIN)
        {
            cities.erase(cities.begin());
            cities.push_back(cityIN);
        }

        City* findCity(string& cityNameIN, string& countryCodeIN)
        {
            /*
             *For loop will always loop through 10 indexes even if the index is nullptr and the loop is not dependent on
             *the array size, therefore being O(1) and not O(N)
             */
            for (int i = 0; i < 10; i++)
            {
                if (cities.at(i) != nullptr)
                {
                    if (cities.at(i)->countryCode == countryCodeIN && cities.at(i)->name == cityNameIN)
                    {
                        return cities.at(i);
                    }
                }
            }
            return nullptr;
        }

        void displayCache()
        {
            for (int i = 0; i < 10; i++)
            {
                if (cities.at(i) != nullptr)
                {
                    cout << cities.at(i)->name << " " << endl;
                }
                else
                {
                    cout << " nullptr" << endl;
                }
            }
        }
};

class RandomCacheMethod : public CityCacheList
{
private:
    int size = 0;
    vector<City*> cities;
public:
    RandomCacheMethod()
    {
        for (int i = 0; i < 10; i++)
        {
            cities.push_back(nullptr);
        }
    }

    void insertCity2Cache(City* cityIN)
    {
        if (size >= 10)
        {
            cities.at(rand() % 10) = cityIN;
            return;
        }
        else
        {
            cities.erase(cities.begin());
            cities.push_back(cityIN);
            size++;
        }
    }

    City* findCity(string& cityNameIN, string& countryCodeIN)
    {
        /*
         *For loop will always loop through 10 indexes even if the index is nullptr and the loop is not dependent on
         *the array size, therefore being O(1) and not O(N)
         */
        for (int i = 0; i < 10; i++)
        {
            if (cities.at(i) != nullptr)
            {
                if (cities.at(i)->countryCode == countryCodeIN && cities.at(i)->name == cityNameIN)
                {
                    return cities.at(i);
                }
            }
        }
        return nullptr;
    }

    void displayCache()
    {
        for (int i = 0; i < 10; i++)
        {
            if (cities.at(i) != nullptr)
            {
                cout << cities.at(i)->name << " " << endl;
            }
            else
            {
                cout << " nullptr" << endl;
            }
        }
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
                    cout << "City Found: " << lineData.at(0) << " " << lineData.at(1) << " "<< lineData.at(2) << endl;
                    cacheListIN.insertCity2Cache(new City(lineData.at(1), lineData.at(0), stoi(lineData.at(2))));
                    return;
                }

            }
            cout << "City was not found in csv file." << endl;
            file.close();
       }
};

void preloadCities(CityCacheList* cacheList)
{
    cacheList->insertCity2Cache(new City("andorra la vella", "ad", 20430));
    cacheList->insertCity2Cache(new City("canillo", "ad", 3292));
    cacheList->insertCity2Cache(new City("encamp", "ad", 11224));
    cacheList->insertCity2Cache(new City("la massana", "ad", 7211));
    cacheList->insertCity2Cache(new City("les escaldes", "ad", 15854));
    cacheList->insertCity2Cache(new City("ordino", "ad", 2553));
    cacheList->insertCity2Cache(new City("sant julia de loria", "ad", 8020));
    cacheList->insertCity2Cache(new City("abu dhabi", "ae", 603687));
    cacheList->insertCity2Cache(new City("dubai", "ae", 1137376));
    cacheList->insertCity2Cache(new City("sharjah", "ae", 543942));
}

int main()
{
    CityCacheList* cacheList = new RandomCacheMethod();

    preloadCities(cacheList);

    string cityName, countryCode;
    while (true)
    {
        cout << "1. Display cache." << endl;
        cout << "2. Search for city." << endl;
        cout << "3. exit." << endl;

        string choiceHold;
        getline(cin, choiceHold);
        City* hold;
        try
        {
            switch (stoi(choiceHold))
            {
                hold = nullptr;
                case 1:
                cacheList->displayCache();
                break;
                case 2:
                cout << "City name?: ";
                getline(cin, cityName);
                cout << "City countryCode?: ";
                getline(cin, countryCode);
                hold = cacheList->findCity(cityName, countryCode);
                if (hold != nullptr)
                {
                    cout << "City found in the cache: " << hold->name << " " << hold->countryCode << " " << hold->population << endl;
                    break;
                }
                else
                {
                    cout << "City not found in cache, looking in csv file." << endl;
                    CSVReader::findCity(cityName, countryCode, *cacheList);
                }
                break;
                case 3:
                return 0;
                default:
                cout << "Invalid choice." << endl;
            }
            cout << endl;
        }
        catch(...)
        {
            cout << "Error with your input try again." << endl << endl;
        }
    }
}