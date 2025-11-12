#include <iostream>
#include <vector>
#include <queue>
#include "colors.h"
#include <string>

using namespace std;

struct Node {

    int level;
    double value;
    double weight;
    double upper_bound;

    bool operator<(const Node &other) const {
        return upper_bound < other.upper_bound;
    };
};

double calculate_bound(Node root, vector<double> values, vector<double> weights, double capacity){
    double bound = root.value;
    double weight = root.weight;

    for(int i=root.level + 1; i< values.size(); i++){
        if(weight + weights[i] > capacity){
            double left_capacity = capacity - weight;
            double value_per_unit = (values[i] / weights[i]);
            double relaxed_value = value_per_unit * left_capacity;
            bound += relaxed_value;
            break;
        }

        bound+=values[i];
        weight+=weights[i];
    }

    return bound;
}



double branch_and_bound_knapsak(vector<double> weights, vector<double> values, int capacity){

    Node root = {-1, 0.0, 0.0, 0.0};
    root.upper_bound = calculate_bound(root, values, weights, capacity);

    priority_queue<Node> pq;

    pq.push(root);

    double max_profit = 0;
    while(!pq.empty()){

        Node current = pq.top();
        pq.pop();

        if(current.weight > capacity){ // Unfeasible solution
            continue;
        }

        if(current.value > max_profit){
            max_profit = current.value;
        }

        int next_level = current.level + 1;
        const double next_weight = current.weight + weights[next_level];
        const double next_value = current.value + values[next_level];

        Node with = {next_level, next_value, next_weight, 0.0};
        with.upper_bound = calculate_bound(with, values, weights, capacity);

        if(with.upper_bound > max_profit){ // Prune because this node will be worse than what we have.
            pq.push(with);
        }

        Node without = {next_level, current.value, current.weight, 0.0};
        without.upper_bound = calculate_bound(without, values, weights, capacity);

        if(without.upper_bound > max_profit) {  // Prune because this node will be worse than what we have.
            pq.push(without);
        }
    }

    return max_profit;
}

int main(){
    vector<vector<double>> weights = {
        {2.0, 3.0, 4.0, 5.0},            // caso 1
        {3.0, 4.0, 6.0, 5.0},            // caso 2
        {2.0, 2.0, 3.0, 4.0, 5.0},       // caso 3
        {1.0, 2.0, 3.0, 8.0, 7.0, 4.0},  // caso 4
        {5.0, 4.0, 6.0, 3.0, 2.0}        // caso 5
    };

    vector<vector<double>> values = {
        {40.0, 50.0, 65.0, 75.0},        // caso 1
        {30.0, 50.0, 80.0, 60.0},        // caso 2
        {10.0, 20.0, 30.0, 40.0, 50.0},  // caso 3
        {10.0, 40.0, 30.0, 50.0, 60.0, 35.0}, // caso 4
        {25.0, 20.0, 40.0, 15.0, 10.0}   // caso 5
    };

    vector<double> capacities = {7, 10, 8, 15, 9};

    vector<double> expected_results = {115, 140, 90, 150, 65};
    vector<double> results = {};

    for(size_t i=0; i<weights.size(); i++){
        results.push_back(branch_and_bound_knapsak(weights[i], values[i], capacities[i]));
        cout << results[i] << endl;
    }

    for(size_t i=0; i<weights.size(); i++){
        if(results[i] == expected_results[i]){
            string text = "Test "+to_string(i+1)+" has passed";
            print_green(text);
        } else {
            string text = "Test "+to_string(i+1)+" has failed";
            string expected = "Expected: "+ to_string(expected_results[i]) + " Received: " + to_string(results[i]);
            
            print_red(text);
            print_gray(expected);
        }
    }

    return 0;
}