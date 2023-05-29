#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

// Fungsi objektif untuk mencari nilai fitness
int fungsi_objektif(int a, int b, int c, int d) {
    return abs((a + 4*b + 2*c + 3*d) - 30);
}

// Inisialisasi populasi
std::vector<std::vector<int>> 
inisialisasi_populasi(int jumlah_populasi) {
    std::vector<std::vector<int>> populasi;
    srand(time(0)); // Seed the random number generator
    for (int i = 0; i < jumlah_populasi; i++) {
        int a = rand() % 31; // Random value between 0 and 30 for variable a
        int b = rand() % 11; // Random value between 0 and 10 for variable b
        int c = rand() % 11; // Random value between 0 and 10 for variable c
        int d = rand() % 11; // Random value between 0 and 10 for variable d
        std::vector<int> chromosome = {a, b, c, d};
        populasi.push_back(chromosome);
    }
    return populasi;
}

// Evaluasi populasi
std::vector<int> evaluasi_populasi(std::vector<std::vector<int>>& populasi) {
    std::vector<int> fitness_values;
    for (const auto& chromosome : populasi) {
        int fitness = 1 / (1 + fungsi_objektif(chromosome[0], chromosome[1], chromosome[2], chromosome[3]));
        fitness_values.push_back(fitness);
    }
    return fitness_values;
}

// Seleksi menggunakan metode roulette wheel
std::vector<std::vector<int>> seleksi(std::vector<std::vector<int>>& populasi, const std::vector<int>& fitness_values) {
    int total_fitness = 0;
    for (int fitness : fitness_values) {
        total_fitness += fitness;
    }
    std::vector<double> probabilities;
    for (int fitness : fitness_values) {
        double probability = (double)fitness / total_fitness;
        probabilities.push_back(probability);
    }
    std::vector<double> cumulative_probabilities;
    double cumulative_probability = 0;
    for (double probability : probabilities) {
        cumulative_probability += probability;
        cumulative_probabilities.push_back(cumulative_probability);
    }
    std::vector<std::vector<int>> selected;
    srand(time(0)); // Seed the random number generator
    for (int i = 0; i < populasi.size(); i++) {
        double r = (double)rand() / RAND_MAX; // Random value between 0 and 1
        for (int j = 0; j < cumulative_probabilities.size(); j++) {
            if ((j == 0 && r < cumulative_probabilities[j]) || (cumulative_probabilities[j - 1] < r && r < cumulative_probabilities[j])) {
                selected.push_back(populasi[j]);
                break;
            }
        }
    }
    return selected;
}

// Crossover menggunakan one-cut point
std::vector<std::vector<int>> crossover(const std::vector<std::vector<int>>& parents, double crossover_rate) {
    std::vector<std::vector<int>> offspring;
    srand(time(0)); // Seed the random number generator
    for (int i = 0; i < parents.size(); i += 2) {
        if ((double)rand() / RAND_MAX < crossover_rate) {
            int cut_point = rand() % parents[i].size(); // Random cut point
            std::vector<int> child1, child2;
            for (int j = 0; j < cut_point; j++) {
                child1.push_back(parents[i][j]);
                child2.push_back(parents[i + 1][j]);
            }
            for (int j = cut_point; j < parents[i].size(); j++) {
                child1.push_back(parents[i + 1][j]);
                child2.push_back(parents[i][j]);
            }
            offspring.push_back(child1);
            offspring.push_back(child2);
        } else {
            offspring.push_back(parents[i]);
            offspring.push_back(parents[i + 1]);
        }
    }
    return offspring;
}

// Mutasi dengan mengganti satu gen secara acak
void mutasi(std::vector<std::vector<int>>& populasi, double mutation_rate) {
    srand(time(0)); // Seed the random number generator
    for (auto& chromosome : populasi) {
        if ((double)rand() / RAND_MAX < mutation_rate) {
            int index = rand() % chromosome.size(); // Random index
            chromosome[index] = rand() % (index == 0 ? 31 : 11); // Random value between 0 and 30 for variable a, or between 0 and 10 for variables b, c, d
        }
    }
}

// Algoritma genetik
std::vector<std::vector<int>> algoritma_genetik(int jumlah_generasi, int jumlah_populasi, double crossover_rate, double mutation_rate) {
    std::vector<std::vector<int>> populasi = inisialisasi_populasi(jumlah_populasi);

    for (int generasi = 0; generasi < jumlah_generasi; generasi++) {
        std::vector<int> fitness_values = evaluasi_populasi(populasi);
        std::vector<std::vector<int>> selected = seleksi(populasi, fitness_values);
        std::vector<std::vector<int>> offspring = crossover(selected, crossover_rate);
        mutasi(offspring, mutation_rate);

        populasi = offspring;
    }

    return populasi;
}

int main() {
    int jumlah_generasi = 100;
    int jumlah_populasi = 6;
    double crossover_rate = 0.8;
    double mutation_rate = 0.1;

    std::vector<std::vector<int> hasil = algoritma_genetik(jumlah_generasi, jumlah_populasi, crossover_rate, mutation_rate);

    // Menampilkan hasil
    for (const auto & chromosome : hasil) {
        int a = chromosome[0];
        int b = chromosome[1];
        int c = chromosome[2];
        int d = chromosome[3];
        if (a + 4*b + 2*c + 3*d == 30) {
            std::cout << "Solusi ditemukan: a = " << a << ", b = " << b << ", c = " << c << ", d = " << d << std::endl;
        }
    }

    return 0;
}

