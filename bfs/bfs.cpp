#include <queue>
#include "node.hpp"
#include <iostream>
#include <thread>

static const int num_threads = 50;
static std::mutex mtx;

void worker(int tid, std::queue<node *> *queue, int *count, int height){
    int layer;
    node *current_node, *new_node;
    std::queue<node *> nodes = *queue;
    while (true){
        
        // Pop the next node from the queue
        mtx.lock();
        if (nodes.empty()){
            mtx.unlock(); 
            break;
        }

        current_node = nodes.front();
        nodes.pop();


        // std::cout << current_node << std::endl;

        // We can increment a counter map using current_node->layer
        count[current_node->layer] += 1;
        mtx.unlock();

        // Ensure the child nodes are at a layer less than or equal the height
        layer = current_node->layer + 1;
        if (layer > height){
            //delete current_node;
            continue;
        }

        new_node = current_node->get_left();
        if (new_node != nullptr){
            mtx.lock();
            nodes.push(new_node);
            mtx.unlock();
        }

        new_node = current_node->get_right();
        if (new_node != nullptr){
            mtx.lock();
            nodes.push(new_node);
            mtx.unlock();
        }

        // delete current_node;
    }

    mtx.lock();
    std::cout << "thread " << tid << " terminated \n";
    mtx.unlock();
}

int main(){
    int height = 20;

    // Create a queue with an initial context
    std::queue<node *> *nodes = new std::queue<node *>();
    nodes->push(new node(new std::vector<context *>{
        new recurring_gap(3, 1),
        new recurring_gap(4, 50),
    }, -1, 0));


    int *count = new int[height + 1];
    for (int i = 0; i < height + 1; i++){
        count[i] = 0;
    }

    std::thread threads[num_threads];
    for (int i = 0; i < num_threads; i++){
        threads[i] = std::thread(worker, i, nodes, count, height);
    }

    std::cout << num_threads << " threads running ...\n";
    
    for (int i = 0; i < num_threads; i++){
        threads[i].join();
    }   

    for (int i = 0; i < height + 1; i ++){
        std::cout << i << ": " << count[i] << std::endl;
    }
    return 0;
}