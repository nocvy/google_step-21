#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>

using namespace std;

/* 分割する */
pair<double, double> split(const string& input, char delimiter)
{
    istringstream stream(input);
    string field;
    getline(stream, field, delimiter);
    double x = stod(field);
    getline(stream, field, delimiter);
    double y = stod(field);
    return make_pair(x, y);
}

/* inputのcsvファイルを読み込む */
void read_cities(int file_idx, vector<pair<double, double> > &cities) {
    string filename = "input_" + to_string(file_idx) + ".csv";
    ifstream ifs(filename);

    string line;
    getline(ifs, line);

    while (getline(ifs, line)) {
        pair<double, double> pos = split(line, ',');
        cities.push_back(pos);
    }
    return;
}

/* outputのcsvファイルに書き込む */
void output_route_to_file(int file_idx, const vector<int> &route) {
    string filename = "output_" + to_string(file_idx) + ".csv";
    ofstream outputfile(filename);
    outputfile << "index" << endl;
    for (int i = 0; i < route.size() - 1; i++) {
        outputfile << route[i] << endl;
    }
    outputfile.close();
}

/* 距離を計算する */
double calc_dist(pair<double, double> c1, pair<double, double> c2) {
    double dx = c2.first - c1.first;
    double dy = c2.second - c1.second;
    return sqrt(dx * dx + dy * dy);
}

/* 最も近い頂点を検索し訪問する */
/*! @brief search closest vertex and visit.
 @param[in, out] vertex vertex to search
 @param[in] cities
 @param[in, out] visited array to determine if the vertex has been visited
 @return minimum distance between the vertex and next vertex
*/
double visit_closest(int &vertex, const vector<pair<double, double> > &cities, vector<bool> &visited) {
    int closest;
    double min_dist = numeric_limits<double>::max();
    bool all_visited = true;
    for (int i = 0; i < cities.size(); i++) {
        if (!visited[i]) {
            all_visited = false;
            double dist = calc_dist(cities[vertex], cities[i]);
            if (min_dist > dist) {
                closest = i;
                min_dist = dist;
            }
        }
    }
    if (all_visited) return -1;
    vertex = closest;
    visited[closest] = true;
    return min_dist;
}

/*! @brief greedy search path until all vertices has visited.
 @param[in] start the start point of search
 @param[in] cities 
 @param[out] shortest_route the order of vertices to visit
 @param[out] shortest_length the shortest distance to visit all vertices
*/
void greedy(int start, const vector<pair<double, double> > &cities, vector<int> &shortest_route, double& shortest_length) {
    double length = 0;

    vector<bool> visited(cities.size(), false);
    visited[start] = true;

    vector<int> route;
    route.push_back(start);
    int vertex = start;

    while(true) {
        double dist = visit_closest(vertex, cities, visited);
        if (dist == -1) break;
        route.push_back(vertex);
        length += dist;
    }
    length += calc_dist(cities[start], cities[vertex]);
    if (length < shortest_length) {
        shortest_length = length;
        shortest_route = route;
    }
}


/*! @brief calculate how effective if route swapped */
double effect_swap(pair<double, double> &p1, pair<double, double> &p2, pair<double, double> &p3, pair<double, double> &p4) {
    double prev_length = calc_dist(p1, p2) + calc_dist(p3, p4);
    double swapped_length = calc_dist(p1, p3) + calc_dist(p2, p4);
    return prev_length - swapped_length;
}

/*! @brief search all pairs of vertices and swap them if it would be more efficient.
 @param[in, out] route the order of vertices to visit
 @param[in, out] length the distance to visit all vertices
 @param[in] cities 
 @return weather swapped or not
*/
bool two_opt(vector<int> &route, double &length, const vector<pair<double, double> > &cities) {
    bool swaped = false;
    for (int i = 0; i < route.size() - 2; i++) {
        for (int j = i + 1; j < route.size() - 1; j++) {
            pair<double, double> c1 = cities[route[i]];
            pair<double, double> c2 = cities[route[i+1]];
            pair<double, double> c3 = cities[route[j]];
            pair<double, double> c4 = cities[route[j+1]];
            double effect = effect_swap(c1, c2, c3, c4);
            if (effect > 0 ) {
                int a = i+1;
                int b = j;
                while (a < b) {
                    swap(route[a++], route[b--]);
                }
                length -= effect; 
                swaped = true;
            }
        }
    }
    return swaped;
}

int main()
{
    for (int i = 0; i < 8; i++) { // input files
        vector<pair<double, double> > cities;
        read_cities(i, cities);

        double shortest_length = numeric_limits<double>::max();
        vector<int> shortest_route;

        for (int i = 0; i < cities.size(); i++) {
            greedy(i, cities, shortest_route, shortest_length);
        }
        while (two_opt(shortest_route, shortest_length, cities)) continue;

        shortest_route.push_back(shortest_route[0]);
        output_route_to_file(i, shortest_route);
        
        cout << "input_" << i << " : " << shortest_length << endl;
    }
}