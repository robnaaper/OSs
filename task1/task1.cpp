#include <iostream>
#include <pthread.h>
#include <string>

const int NUM_THREADS = 4;
std::string src;
std::string dest;

void* copy_string(void* arg) {
    int thread_id = *((int*)arg);
    int start = thread_id * (src.length() / NUM_THREADS);
    int end = start + (src.length() / NUM_THREADS);
    
    for (int i = start; i < end; i++) {
        dest[i] = src[i];
    }
    
    pthread_exit(NULL);
}

int main() {
    std::cout << "Write a string: ";
    std::getline(std::cin, src);
    
    dest = src;
    
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, copy_string, (void*)&thread_ids[i]);
    }
    
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    std::cout << "Copied string: " << dest << std::endl;
    
    return 0;
}