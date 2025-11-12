#include <iostream>
#include <vector>
#include <queue>

struct Node {

    int level;
    double value;
    double weight;
    double upper_bound;

    bool operator<(Node &other){
        return upper_bound < other.upper_bound;
    };
};

double calculate_bound(Node root, std::vector<double> values, std::vector<double> weights, const double capacity){
    double bound = root.value;
    double weight = root.weight;

    for(int i=root.level + 1; i< values.size(); i++){
        if(weight + weights[i] > capacity){
            double left_capacity = capacity - weight;
            double value_per_unit = (values[i] / weights[i]);
            double relaxed_value = value_per_unit * left_capacity;
            return bound + relaxed_value;
        }

        bound+=values[i];
        weight+=weights[i];
    }

    return bound;
}

const int capacity = 7;


int main(){
    std::vector<double> weights = {2.0,3.0,4.0,5.0};
    std::vector<double> values  = {40.0,50.0,65.0,75.0};

    Node root = {-1, 0, 0};
    root.upper_bound = calculate_bound(root, values, weights, 7);

    std::cout << root.upper_bound << std::endl;
    std::priority_queue<Node> pq;

    pq.push(root);

    int level = 0;

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
        int next_weight = current.weight + values[next_level];
        int next_value = current.value + values[next_level];

        Node with = {next_level, next_value, next_weight};
        with.upper_bound = calculate_bound(with, values, weights, capacity);

        if(with.upper_bound > max_profit){ // Prune because this node will be worse than what we have.
            pq.push(with);
        }

        next_level++;
        
        Node without = {next_level, current.value, current.weight};
        without.upper_bound = calculate_bound(without, values, weights, capacity);

        if(without.upper_bound > max_profit) {  // Prune because this node will be worse than what we have.
            pq.push(without);
        }
    }

    std::cout << max_profit << std::endl;

    return 0;
}