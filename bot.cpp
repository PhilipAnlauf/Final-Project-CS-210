#include <fstream>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    ifstream file("world_cities.csv");
    ofstream out("botCities.txt");

    if (!file.is_open())
    {
        cout << "File could not be opened" << endl;
        return 0;
    }

    vector<string> cities;
    string line;

    while (getline(file, line))
    {
        cities.push_back(line);
    }

    for (const string& city : cities)
    {
        out << city << endl;
    }

    cout << "Cities written to botCities.txt" << endl;
    return 0;
}
