#include <iostream>
#include <pthread.h>

pthread_mutex_t printer_mutex = PTHREAD_MUTEX_INITIALIZER;
bool printer_available = true;

void* use_printer(void* arg) {
    int value = *((int*)arg);
    
    pthread_mutex_lock(&printer_mutex);
    
    while (!printer_available) {
        pthread_cond_wait(&printer_cond, &printer_mutex);
    }
    
    printer_available = false;
    
    std::cout << "Value: " << value << std::endl;
    
    printer_available = true;
    pthread_cond_broadcast(&printer_cond);
    pthread_mutex_unlock(&printer_mutex);
    
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[5];
    int values[5] = {1, 2, 3, 4, 5};
    
    for (int i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, use_printer, (void*)&values[i]);
    }
    
    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }
    
    return 0;
}