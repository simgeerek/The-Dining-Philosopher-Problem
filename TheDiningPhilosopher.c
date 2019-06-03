#include <pthread.h> 
#include <semaphore.h> 
#include <stdio.h> 

int state[5]; // Initially state[i]==THINKING for all i.
int philosopher[5] = { 0, 1, 2, 3, 4 }; //number of philosophers
sem_t mutex; // Initially set to 1.
sem_t s[5]; // Initially s[i] is set to 0 for all i.
#define THINKING 0 // 0 represents thinking
#define HUNGRY 1 //1 represents hungry
#define EATING 2 // 2 represents eating


int left(int i) {// Philosopher to the left of i.
   return (i + 4) % 5;
}
int right(int i) {// Philosopher to the right of i.
   return (i + 1) % 5;
}


void test(int i) { 
  // if i is hungry and philosophers of the i's right and left sides is not eating, i can eat
  if (state[i] == HUNGRY &&
     state[left(i)] != EATING &&
     state[right(i)] != EATING) {
     state[i] = EATING;
     sleep(2);
     printf("Philosopher %d takes chopsticks %d and %d\n",  i + 1, left(i) + 1, i + 1); 
  
        printf("Philosopher %d is Eating\n", i + 1); 
     sem_post(&s[i]);
}
}


//i take the chopstick 
void takechopstick(int i) {
   sem_wait(&mutex);
   state[i] = 1; // hungry state
   printf("Philosopher %d is Hungry\n", i + 1); 
   test(i);
   sem_post(&mutex);
   sem_wait(&s[i]);
   sleep(2);
}

//i put the chopsticks
void putchopstick(int i) {
   sem_wait(&mutex);
   state[i] = 0;
   printf("Philosopher %d putting chopsticks %d and %d down\n",  i + 1, left(i) + 1, i + 1); 
    printf("Philosopher %d is thinking\n", i + 1); 
   test(left(i));
   test(right(i));
   sem_post(&mutex);
}


void* philospher(void* i) 
{ 
  while (1) { 
   int* x=i; 
   sleep(2); 
   takechopstick(*x); 
   sleep(1);      
   putchopstick(*x); 
    } 
}


int main() 
{ 
    int i; 
    pthread_t thread_id[5]; 
    // initialize the semaphores 
    sem_init(&mutex, 0, 1); 
  
    for (i = 0; i < 5; i++){
            sem_init(&s[i], 0, 0); 
    }
    for (i = 0; i < 5; i++) { 
       // create philosopher processes 
        pthread_create(&thread_id[i], NULL, 
                       philospher, &philosopher[i]); 
  
        printf("Philosopher %d is thinking\n", i + 1); 
    } 
  
    for (i = 0; i < 5; i++) {
    pthread_join(thread_id[i], NULL); 
    }
} 
