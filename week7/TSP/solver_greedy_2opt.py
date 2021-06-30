#!/usr/bin/env python3

import sys
import math

from common import print_tour, read_input, format_tour


def distance(city1, city2):
    return math.sqrt((city1[0] - city2[0]) ** 2 + (city1[1] - city2[1]) ** 2)


# greedyアルゴリズム
def solve(cities):
    N = len(cities)

    dist = [[0] * N for i in range(N)]
    for i in range(N):
        for j in range(i, N):
            dist[i][j] = dist[j][i] = distance(cities[i], cities[j])

    current_city = 0
    unvisited_cities = set(range(1, N))
    tour = [current_city]

    while unvisited_cities:
        next_city = min(unvisited_cities,
                        key=lambda city: dist[current_city][city])
        unvisited_cities.remove(next_city)
        tour.append(next_city)
        current_city = next_city

    opt_tour = opt_2(tour, dist)
    return opt_tour


# 2-opt法
def opt_2(tour, dist):
    size = len(tour)
    while True:
        count = 0
        for i in range(size - 2):
            i1 = i + 1
            for j in range(i + 2, size):
                if j == size - 1:
                    j1 = 0
                else:
                    j1 = j + 1
                if i != 0 or j1 != 0:
                    l1 = dist[tour[i]][tour[i1]]
                    l2 = dist[tour[j]][tour[j1]]
                    l3 = dist[tour[i]][tour[j]]
                    l4 = dist[tour[i1]][tour[j1]]
                    if l1 + l2 > l3 + l4: # 繋ぎ変える
                        new_tour = tour[i1:j+1]
                        tour[i1:j+1] = new_tour[::-1]
                        count += 1
        if count == 0: break
    return tour


if __name__ == '__main__':
    assert len(sys.argv) > 1
    tour = solve(read_input(sys.argv[1]))
    print_tour(tour)
    with open(f'output_{sys.argv[1][6]}.csv', 'w') as f:
                f.write(format_tour(tour) + '\n')
