#!/usr/bin/env python3

import sys
import math
import random

from common import print_tour, read_input, format_tour


def distance(city1, city2):
    return math.sqrt((city1[0] - city2[0]) ** 2 + (city1[1] - city2[1]) ** 2)

#辺の合計の長さを求める
def total_path_length(city_path, dist, N):
    total_path = 0
    for i in range(N-1):
        total_path += dist[city_path[i]][city_path[i+1]]
    total_path += dist[city_path[N-1]][city_path[0]]
    return total_path


def solve(cities):
    N = len(cities)

    dist = [[0] * N for i in range(N)]
    for i in range(N):
        for j in range(i, N):
            dist[i][j] = dist[j][i] = distance(cities[i], cities[j])

    # スタート位置0を最初の基準とする
    g_tour = greedy(cities, N, dist, 0)
    t_tour = opt_2(g_tour, dist)
    m_tour = move_subsequence(t_tour, dist, 7)
    for i in reversed(range(1, 7)):
        m_tour = move_subsequence(m_tour, dist, i)

    m_path = total_path_length(m_tour, dist, N)
    print(m_path)

    return m_tour

# greedy法(貪欲法)
def greedy(cities, N, dist, start):
    current_city = start
    unvisited_cities = set(range(0, N))
    unvisited_cities.remove(start)
    tour = [current_city]

    while unvisited_cities:
        next_city = min(unvisited_cities,
                        key=lambda city: dist[current_city][city])
        unvisited_cities.remove(next_city)
        tour.append(next_city)
        current_city = next_city

    return tour

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

# or-opt法
def move_subsequence(tour, dist, subsequence_length):
    N = len(tour)
    while True:
        count = 0  # Number of times tour was changed
        for a_index in range(N-1):
            b_index = (a_index+subsequence_length+1) % N
            e_indexes = [(a_index+1+i) % N for i in range(subsequence_length)]
            for c_index in range(N-1):
                d_index = (c_index+1) % N

                # When C,D are not included in e_indexes
                if d_index not in e_indexes and d_index != b_index:
                    A = tour[a_index]
                    B = tour[b_index]
                    C = tour[c_index]
                    D = tour[d_index]
                    E = [tour[index] for index in e_indexes]
                    if dist[A][E[0]]+dist[E[-1]][B]+dist[C][D] > dist[A][B]+dist[C][E[-1]]+dist[E[0]][D]:
                        # Replace the cities of E with -1. The indexes of C and D are not changed
                        for i in range(subsequence_length):
                            tour[(a_index+1+i) % N] = -1
                        # Reverse E and put E between C and D
                        for i in range(subsequence_length):
                            tour.insert(d_index, E[i])
                        # Remove the cities of E by removing -1
                        for i in range(subsequence_length):
                            tour = [city for city in tour if city != -1]
                        count += 1
        if count == 0:
            break
    return tour

if __name__ == '__main__':
    assert len(sys.argv) > 1
    tour = solve(read_input(sys.argv[1]))
    with open(f'output_{sys.argv[1][6]}.csv', 'w') as f:
                f.write(format_tour(tour) + '\n')