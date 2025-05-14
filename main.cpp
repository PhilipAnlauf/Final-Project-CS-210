#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <utility>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

struct City
{
    string name, countryCode;
    int population;
    int uses = 0;

    City(string nameIN, string countryCodeIN, const int populationIN): name(std::move(nameIN)),
           countryCode(move(countryCodeIN)), population(populationIN) {}
};

struct TrieNode
{
    unordered_map<char, TrieNode*> children;
    City* city = nullptr;
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

        void insertCity2Cache(City* cityIN) override
        {
            cities.erase(cities.begin());
            cities.push_back(cityIN);
        }

        City* findCity(string& cityNameIN, string& countryCodeIN) override
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

        void displayCache() override
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

    void insertCity2Cache(City* cityIN) override
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

    City* findCity(string& cityNameIN, string& countryCodeIN) override
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

    void displayCache() override
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

class LeastUsedCache : public CityCacheList
{
    private:
        int size = 0;
        vector<City*> cities;
    public:
        LeastUsedCache()
        {
            for (int i = 0; i < 10; i++)
            {
                cities.push_back(nullptr);
            }
        }

        void insertCity2Cache(City* cityIN) override
        {
            if (size >= 10)
            {
                int smallestIndex = 0;
                for (int i = 0; i < 10; i++)
                {
                    if (cities.at(i)->uses < cities.at(smallestIndex)->uses)
                    {
                        smallestIndex = i;
                    }
                }
                cities.at(smallestIndex) = cityIN;
            }
            else
            {
                cities.erase(cities.begin());
                cities.push_back(cityIN);
                size++;
            }
        }

        City* findCity(string& cityNameIN, string& countryCodeIN) override
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
                        cities.at(i)->uses++;
                        return cities.at(i);
                    }
                }
            }
            return nullptr;
        }

        void displayCache() override
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

class TrieTree
{
    private:
        TrieNode* root;
    public:
        TrieTree()
        {
            root = new TrieNode();
        }

        void insert(City* cityIN) const
        {
            TrieNode* node = root;
            string key = cityIN->countryCode + cityIN->name;

            for (char ch : key)
            {
                if (!node->children.count(ch))
                {
                    node->children[ch] = new TrieNode();
                }
                node = node->children[ch];
            }
            node->city = cityIN;
        }


        void lookup(const string& cityNameIN, const string& countryCodeIN) const
        {
            TrieNode* node = root;
            const string key = countryCodeIN + cityNameIN;

            for (char ch : key)
            {
                if (!node->children.count(ch))
                {
                    cout << "City not found in the Trie." << endl;
                    return;
                }
                node = node->children[ch];
            }

            City* foundCity = node->city;
            if (foundCity != nullptr)
            {
                //cout << "City found in the Trie: " << foundCity->name << " " << foundCity->countryCode << " "
                //     << foundCity->population << endl;
            }
            else
            {
                //cout << "City not found in the Trie." << endl;
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

        static void createTrie(TrieTree& trieIN)
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

                trieIN.insert(new City(lineData.at(1), lineData.at(0), stoi(lineData.at(2))));
            }
            file.close();
        }
};


//This is for testing so I don't have to input 10 cities every time I want to test
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
    vector<string> botChosenCitiesFirstHalf;
    ifstream file("world_cities.csv");

    CityCacheList* cacheList;
    TrieTree* trieTree = new TrieTree();
    CSVReader::createTrie(*trieTree);


    //==================================
    //Bot choosing cities
    //==================================
    system("./bot");
    vector<string> botChosenCities;
    ifstream botChoices("botCities.txt");

    string line;
    while (getline(botChoices, line))
    {
        botChosenCities.push_back(line);
    }
    //=================================================================
    //Going over first strategy
    auto time = 0.0;
    FIFOCache* cacheList1 = new FIFOCache();

    for (int i = 0 ; i < 1000 ; i ++)
    {
        auto start = chrono::high_resolution_clock::now();
        City* tempCity;

        //Getting data from first two
        stringstream ss(botChosenCities[i]);
        string part;
        vector<string> firstTwo;

        int count = 0;
        while (getline(ss, part, ',') && count < 2) {
            firstTwo.push_back(part);
            count++;
        }

        tempCity = cacheList1->findCity(firstTwo[1], firstTwo[0]);
        if (tempCity != nullptr)
        {
            //cout << "City found in the cache: " << hold->name << " " << hold->countryCode << " " << hold->population << endl;
            break;
        }
        else
        {
            //cout << "City not found in cache, looking in csv file." << endl;
            //cout << "Searching for " << firstTwo[1] << " and " << firstTwo[0] << endl;
            trieTree->lookup(firstTwo[1], firstTwo[0]);
        }
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        time += duration.count();
    }
    cout << "Time taken for First in first out method: " << time << " microseconds "<< endl;
    //=================================================================
    //Going over second strategy
    auto time2 = 0.0;
    LeastUsedCache* cacheList2 = new LeastUsedCache();

    for (int i = 0 ; i < 1000 ; i ++)
    {
        auto start = chrono::high_resolution_clock::now();
        City* tempCity;

        //Getting data from first two
        stringstream ss(botChosenCities[i]);
        string part;
        vector<string> firstTwo;

        int count = 0;
        while (getline(ss, part, ',') && count < 2) {
            firstTwo.push_back(part);
            count++;
        }

        tempCity = cacheList2->findCity(firstTwo[1], firstTwo[0]);
        if (tempCity != nullptr)
        {
            //cout << "City found in the cache: " << hold->name << " " << hold->countryCode << " " << hold->population << endl;
            break;
        }
        else
        {
            //cout << "City not found in cache, looking in csv file." << endl;
            //cout << "Searching for " << firstTwo[1] << " and " << firstTwo[0] << endl;
            trieTree->lookup(firstTwo[1], firstTwo[0]);
        }
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        time2 += duration.count();
    }
    cout << "Time taken for least used method: " << time2 << " microseconds "<< endl;
    //=================================================================
    //Going over third strategy
    auto time3 = 0.0;
    RandomCacheMethod* cacheList3 = new RandomCacheMethod();

    for (int i = 0 ; i < 1000 ; i ++)
    {
        auto start = chrono::high_resolution_clock::now();
        City* tempCity;

        //Getting data from first two
        stringstream ss(botChosenCities[i]);
        string part;
        vector<string> firstTwo;

        int count = 0;
        while (getline(ss, part, ',') && count < 2) {
            firstTwo.push_back(part);
            count++;
        }

        tempCity = cacheList3->findCity(firstTwo[1], firstTwo[0]);
        if (tempCity != nullptr)
        {
            //cout << "City found in the cache: " << hold->name << " " << hold->countryCode << " " << hold->population << endl;
            break;
        }
        else
        {
            //cout << "City not found in cache, looking in csv file." << endl;
            //cout << "Searching for " << firstTwo[1] << " and " << firstTwo[0] << endl;
            trieTree->lookup(firstTwo[1], firstTwo[0]);
        }
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        time3 += duration.count();
    }
    cout << "Time taken for random choice method: " << time3 << " microseconds "<< endl;


    //================================================================

    // bool isDone = false;
    // while (!isDone)
    // {
    //     cout << "Choose a cache method:" << endl;
    //     cout << "1. First in first out method." << endl;
    //     cout << "2. Least used method." << endl;
    //     cout << "3. Random method." << endl;
    //
    //     string choiceHold;
    //     getline(cin, choiceHold);
    //     try
    //     {
    //         switch (stoi(choiceHold))
    //         {
    //             case 1:
    //                 cacheList = new FIFOCache();
    //                 isDone = true;
    //             break;
    //             case 2:
    //                 cacheList = new LeastUsedCache();
    //                 isDone = true;
    //             break;
    //             case 3:
    //                 cacheList = new RandomCacheMethod();
    //                 isDone = true;
    //                 break;
    //             default:
    //                 cout << "Invalid choice." << endl;
    //         }
    //         cout << endl;
    //     }
    //     catch(...)
    //     {
    //         cout << "Error with your input try again." << endl << endl;
    //     }
    // }
    //
    // preloadCities(cacheList);
    //
    // string cityName, countryCode;
    // while (true)
    // {
    //     cout << "1. Display cache." << endl;
    //     cout << "2. Search for city." << endl;
    //     cout << "3. exit." << endl;
    //
    //     string choiceHold;
    //     getline(cin, choiceHold);
    //     try
    //     {
    //         City* hold;
    //         switch (stoi(choiceHold))
    //         {
    //             case 1:
    //             cacheList->displayCache();
    //             break;
    //             case 2:
    //             cout << "City name?: ";
    //             getline(cin, cityName);
    //             cout << "City countryCode?: ";
    //             getline(cin, countryCode);
    //             hold = cacheList->findCity(cityName, countryCode);
    //             if (hold != nullptr)
    //             {
    //                 cout << "City found in the cache: " << hold->name << " " << hold->countryCode << " " << hold->population << endl;
    //                 break;
    //             }
    //             else
    //             {
    //                 cout << "City not found in cache, looking in csv file." << endl;
    //                 trieTree->lookup(cityName, countryCode);
    //             }
    //             break;
    //             case 3:
    //                 return 0;
    //             return 0;
    //             default:
    //             cout << "Invalid choice." << endl;
    //         }
    //         cout << endl;
    //     }
    //     catch(...)
    //     {
    //         cout << "Error with your input try again." << endl << endl;
    //     }
    // }
}