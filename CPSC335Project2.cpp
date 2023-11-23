#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

using namespace std;

struct Stock {
    int quantity;
    int value;
};

struct Result {
    int totalSpentExhaustive;
    int totalSpentDynamic;
};

// exhaustive search
void findMaxSpentExhaustive(int index, int budget, vector<Stock>& stocks, int& maxSpent, int currentSpent, int& maxStocks, int currentStocks) {
    if (index == stocks.size()) {
        if (currentStocks > maxStocks || (currentStocks == maxStocks && currentSpent < maxSpent)) {
            maxSpent = currentSpent;
            maxStocks = currentStocks;
        }
        return;
    }
    findMaxSpentExhaustive(index + 1, budget, stocks, maxSpent, currentSpent, maxStocks, currentStocks);
    if (budget >= stocks[index].value) {
        int purchasable = min(stocks[index].quantity, budget / stocks[index].value);
        int spent = purchasable * stocks[index].value;
        findMaxSpentExhaustive(index + 1, budget - spent, stocks, maxSpent, currentSpent + spent, maxStocks, currentStocks + purchasable);
    }
}

// dynamic approach
void findMaxSpentDynamic(vector<Stock>& stocks, int budget, int& totalSpent) {
    int numStocks = stocks.size();
    vector<vector<int>> spent(numStocks + 1, vector<int>(budget + 1, 0));

    for (int i = 1; i <= numStocks; i++) {
        for (int j = 1; j <= budget; j++) {
            spent[i][j] = spent[i - 1][j];
            int maxQuantity = min(stocks[i - 1].quantity, j / stocks[i - 1].value);
            for (int k = 1; k <= maxQuantity; k++) {
                int cost = k * stocks[i - 1].value;
                if (j >= cost) {
                    spent[i][j] = max(spent[i][j], spent[i - 1][j - cost] + cost);
                }
            }
        }
    }

    totalSpent = spent[numStocks][budget];
}

int main() {
    ifstream file("input_project2.txt");
    ofstream outFile("output_project2.txt");
    int numStocks, budget;
    vector<Result> results;

    if (file.is_open()) {
        while (true) {
            file >> numStocks;
            if (file.fail()) break;

            file.ignore(numeric_limits<streamsize>::max(), '\n');
            vector<Stock> stocks;
            string line;
            getline(file, line);
            stringstream ss(line);

            char ch;
            int quantity, value;
            while (ss >> ch >> ch >> quantity >> ch >> value >> ch) {
                stocks.push_back({ quantity, value });
            }

            file >> budget;

            Result result;
            result.totalSpentExhaustive = 0;
            int maxStocks = 0;
            findMaxSpentExhaustive(0, budget, stocks, result.totalSpentExhaustive, 0, maxStocks, 0);
            findMaxSpentDynamic(stocks, budget, result.totalSpentDynamic);
            results.push_back(result);
        }
        file.close();
    } else {
        cerr << "Unable to open file";
        return 1;
    }

    // Print the results
    outFile << "Exhaustive Search \n";
    for (const auto& res : results) {
        outFile << "Output: " << res.totalSpentExhaustive << endl;
    }
    outFile << "\nDynamic Approach \n";
    for (const auto& res : results) {
        outFile << "Output: " << res.totalSpentDynamic << endl;
    }

    outFile.close();

    return 0;
}
